#include "BaseClass/Layer.h"

#include "Component/PrimitiveComponent.h"

CLayer::CLayer()
	: m_fPriority(0.f)
{

}

CLayer* CLayer::Create(_float fPriority)
{
	CLayer* pLayer = new CLayer;

	if (FAILED(pLayer->Ready_Layer(fPriority)))
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

HRESULT CLayer::Ready_Layer(_float fPriority)
{
	m_fPriority = fPriority;

	return S_OK;
}

_int CLayer::Update_Layer(const _float& fTimeDelta)
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
		else
			++iter;
	}

	// �켱�� ������ ���ش�.
	for (_uint i = 0; i < Cast_EnumDef(EUPDATE_T::RENDER); i++)
	{
		m_arrvecPriorityObject[i].reserve(m_mapObject.size());

		// vector�� ��ü �߰�
		for (auto& item : m_mapObject)
			m_arrvecPriorityObject[i].push_back(item.second);

		// �켱�� ��� ����
		sort(m_arrvecPriorityObject[i].begin(), m_arrvecPriorityObject[i].end(),
			[&i](CGameObject* const pDst, CGameObject* const pSrc) {
				return (pDst->Get_Priority(i) > pSrc->Get_Priority(i));
			});
	}

	// �� ���̾ ���� ���� ������Ʈ�� ������Ʈ �Ѵ�.
	_int	iResult = 0;
	for (auto& gameObj : m_arrvecPriorityObject[Cast_EnumDef(EUPDATE_T::UPDATE)])
	{
		iResult = gameObj->Tick(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	// ���� ���� Ŭ����
	m_arrvecPriorityObject[Cast_EnumDef(EUPDATE_T::UPDATE)].clear();

	return iResult;
}

void CLayer::LateUpdate_Layer()
{
	for (auto& gameObj : m_arrvecPriorityObject[Cast_EnumDef(EUPDATE_T::LATE)])
	{
		gameObj->LateTick();
	}

	// ���� ���� Ŭ����
	m_arrvecPriorityObject[Cast_EnumDef(EUPDATE_T::LATE)].clear();
}

CPrimitiveComponent* CLayer::Get_Component(COMPONENTID eID, const _tchar* pObjTag, const _tchar* pComponentTag)
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
		_uint i = 0U;
		while (true)
		{
			wstringstream ss;
			ss << i;
			wstring strNew = strObjectTag + ss.str();
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
		m_mapObject.emplace(strObjectTag.c_str(), pGameObject);

	return S_OK;
}

// �� ���� ����
HRESULT CLayer::Add_GameObject(const wstring pObjTag, CGameObject* pGameObject)
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

CGameObject* CLayer::Get_GameObject(const wstring pObjTag)
{
	auto iter = m_mapObject.find(pObjTag);
	if (iter != m_mapObject.end())
		return (*iter).second;

	return nullptr;
}
