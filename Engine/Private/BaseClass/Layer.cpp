#include "BaseClass/Layer.h"

#include "Component/PrimitiveComponent.h"
#include "System/GameInstance.h"
#include "System/RenderMgr.h"

CLayer::CLayer()
	: m_fPriority(0.f)
{

}

CLayer* CLayer::Create(_float fPriority)
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Initialize(fPriority)))
	{
		Safe_Release(pLayer);

		MSG_BOX("Layer Create Failed");
		return nullptr;
	}

	return pLayer;
}

void CLayer::Free()
{
	// �����̳� ���� �۾�
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapObject.erase(iter);
	}
}

HRESULT CLayer::Initialize(_float fPriority)
{
	m_fPriority = fPriority;

	return S_OK;
}

void CLayer::Priority_Tick(const _float& fTimeDelta)
{
	// Set Dead�� ������ ��ü�� ������Ų��.
	for (auto iter = m_mapObject.begin(); iter != m_mapObject.end();)
	{
		if ((*iter).second->IsDead())
		{
			// ���۷��� ī���Ͱ� 1�̻��̾��� �� �߰����� ���� �۾��� �ʿ��ϴ�.
			_uint iRefCount = Safe_Release((*iter).second);
			iter = m_mapObject.erase(iter);
		}
		// �ƴҽ� �������� �߰��ϰ� ���� iter��
		else
		{
			// ���� ���°� ���������� ���ǿ� ���� �׷쿡 �߰��Ѵ�.
			CGameObject* pObj = (*iter).second;
			if (pObj->IsState(EGObjectState::Render))
			{
				if (pObj->IsState(EGObjectState::RenderZBuffer))
				{
					if (pObj->IsState(EGObjectState::RenderDeferred))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::NonAlpha, pObj);
					}
					else
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::Alpha, pObj);
					}
				}
				// ZBuffer�� �ʿ���� �׷�
				else
				{
					if (pObj->IsState(EGObjectState::RenderPriority))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::Priority, pObj);
					}
					else if (pObj->IsState(EGObjectState::RenderPostProcess))
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::PostProcess, pObj);
					}
					else
					{
						GameInstance()->Add_RenderGroup(ERenderGroup::UI, pObj);
					}
				}
			}

			++iter;
		}
	}

	// �켱�� ������ ���ش�.
	for (auto& item : m_mapObject)
		m_listPriorityObject.push_back(item.second);

	// Tick �켱�� ��� ����
	_uint iIndex = Cast_EnumDef(EGObjTickPriority::Tick);

	m_listPriorityObject.sort(
		[&iIndex](CGameObject* const pDst, CGameObject* const pSrc) {
			return (pDst->Get_Priority(iIndex) > pSrc->Get_Priority(iIndex));
		});

	// ���� Tick ����
	for (auto& gameObj : m_listPriorityObject)
	{
		gameObj->Priority_Tick(fTimeDelta);
	}
}

_int CLayer::Tick(const _float& fTimeDelta)
{
	// �� ���̾ ���� ���� ������Ʈ�� ������Ʈ �Ѵ�.
	_int iResult = 0;
	for (auto& gameObj : m_listPriorityObject)
	{
		iResult = gameObj->Tick(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLayer::Late_Tick(const _float& fTimeDelta)
{
	// Late Tick ����
	for (auto& gameObj : m_listPriorityObject)
	{
		gameObj->Late_Tick(fTimeDelta);
	}

	// ���� ���� Ŭ����
	m_listPriorityObject.clear();
}

CPrimitiveComponent* CLayer::Get_Component(COMPONENTID eID, const wstring& pObjTag, const wstring& pComponentTag)
{
	// ���̾� -> ������Ʈ -> ������Ʈ
	//auto	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CTag_Finder(pObjTag));
	auto iter = m_mapObject.find(pObjTag);

	if (iter == m_mapObject.end())
		return nullptr;

	return nullptr;// (*iter).second->Get_Component(eID, pComponentTag);
}

// �ΰ��ӿ��� �߰��� �� (Update Ÿ�ֶ̹�)
HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	// �� ���̾ ���� ������Ʈ�� �����Ѵ�.
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	wstring strObjectTag = pGameObject->Get_Name();
	auto iter = m_mapObject.find(strObjectTag.c_str());
	// ���� �ߺ� �̸� ������ ���� �ٿ��� �ִ� ���
	if (iter != m_mapObject.end())
	{
		// ������ ���� �����
		_uint i = 0U;
		_tchar cLast = strObjectTag.back();
		_uint iMul = 1U;

		// ���ڸ� 1�� ����
		if (iswdigit(cLast))
			i = 1U;

		while (iswdigit(cLast))
		{
			i += _wtoi(&cLast) * iMul;
			iMul *= 10U;
			strObjectTag.pop_back();
			cLast = strObjectTag.back();
		}

		// ������ ���ڷ� �߰��Ѵ�.
		wstringstream ss;
		ss << i;
		wstring strNew = strObjectTag + ss.str();
		auto iterObject = m_mapObject.find(strNew);
		// ������ �߰��ϱ�
		if (iterObject == m_mapObject.end())
		{
			m_mapObject.emplace(strNew, pGameObject);
			pGameObject->Set_Name(strNew);
		}
	}
	else
		m_mapObject.emplace(strObjectTag.c_str(), pGameObject);

	return S_OK;
}

// �� ���� ����
HRESULT CLayer::Add_GameObject(const wstring& pObjTag, CGameObject* pGameObject)
{
	// �� ���̾ ���� ������Ʈ�� �����Ѵ�.
	NULL_CHECK_RETURN(pGameObject, E_FAIL);

	auto iter = m_mapObject.find(pObjTag);
	// ���� �ߺ� �̸� ������ ���� �ٿ��� �ִ� ���
	if (iter != m_mapObject.end())
	{
		_uint i = 0U;
		while (true)
		{
			wstringstream ss;
			ss << i;
			wstring strNew = pObjTag + ss.str();
			auto iterObject = m_mapObject.find(strNew);
			// ������ �߰��ϱ�
			if (iterObject == m_mapObject.end())
			{
				m_mapObject.emplace(strNew, pGameObject);
				pGameObject->Set_Name(strNew);
				break;
			}

			++i;
		}
		
	}
	else
	{
		m_mapObject.emplace(pObjTag, pGameObject);
		pGameObject->Set_Name(pObjTag);
	}
		

	return S_OK;
}

CGameObject* CLayer::Get_GameObject(const wstring& pObjTag)
{
	auto iter = m_mapObject.find(pObjTag);
	if (iter != m_mapObject.end())
		return (*iter).second;

	return nullptr;
}
