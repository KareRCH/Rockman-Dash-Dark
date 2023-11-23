#include "System/ObjectMgr.h"

#include "BaseClass/GameObject.h"
#include "System/GameInstance.h"
#include "System/RenderMgr.h"

HRESULT CObjectMgr::Initialize(_uint iNumLevels)
{
	m_vecGameObjects.reserve(100000);	// �⺻ ũ��� 10��

	return S_OK;
}

void CObjectMgr::Priority_Tick(const _float& fTimeDelta)
{
	RegistToTick_GameObjects();

	for (auto pObj : m_listTickObjects)
	{
		pObj->Priority_Tick(fTimeDelta);
	}
}

_int CObjectMgr::Tick(const _float& fTimeDelta)
{
	for (auto pObj : m_listTickObjects)
	{
		pObj->Tick(fTimeDelta);
	}

	return 0;
}

void CObjectMgr::Late_Tick(const _float& fTimeDelta)
{
	for (auto pObj : m_listTickObjects)
	{
		pObj->Late_Tick(fTimeDelta);
	}
}

CObjectMgr* CObjectMgr::Create(_uint iNumLevels)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Management Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectMgr::Free()
{
	// �ν��Ͻ� ����
	for (auto pObj : m_vecGameObjects)
		Safe_Release(pObj);
	m_vecGameObjects.clear();

	// ������Ÿ�� ����
	for (auto& Pair : m_mapPrototypes)
		Safe_Release(Pair.second);
	m_mapPrototypes.clear();
}

HRESULT CObjectMgr::Add_Prototype(const wstring& strPrototypeKey, CGameObject* pPrototype)
{
	if (nullptr == pPrototype ||
		nullptr != Find_Prototype(strPrototypeKey))
		return E_FAIL;

	m_mapPrototypes.emplace(strPrototypeKey, pPrototype);

	return S_OK;
}

HRESULT CObjectMgr::Add_CloneObject(const wstring& strPrototypeKey, void* pArg)
{
	/* ������ ã��. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeKey);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* ������ �����Ͽ� ���� ���ӳ��� ����� �纻 ��ü�� �����س���.  */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObjectMgr::Find_Prototype(const wstring& strPrototypeKey)
{
	auto iter = m_mapPrototypes.find(strPrototypeKey);
	if (iter == m_mapPrototypes.end())
		return nullptr;

	return (*iter).second;
}

void CObjectMgr::Clear_Prototypes(const wstring& strContainTag)
{
	// �з����� ������Ÿ���� �޸� �����ϴ� �۾�
	for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
	{
		CGameObject** ppObj = &(*iter).second;
		if ((*ppObj)->Has_Tag(EGObjTag::Level, strContainTag))
		{
			// �±� ������ �ٸ� �±װ� ������ �������� �ʴ´�. (�ٸ� �������� ��������)
			(*ppObj)->Delete_Tag(EGObjTag::Level, strContainTag);
			if ((*ppObj)->Tag_Size(EGObjTag::Level) == 0U)
			{
				Safe_Release(*ppObj);
				iter = m_mapPrototypes.erase(iter);
			}
			else
				++iter;
		}
		else
			++iter;
	}
}

HRESULT CObjectMgr::Add_GameObject(CGameObject* pObj)
{
	m_vecGameObjects.push_back(pObj);
	pObj->m_iID = m_iGiveObjectID++;

	return S_OK;
}

CGameObject* CObjectMgr::Find_GameObject(_uint iFindID)
{
	_uint iMaxIndex = Cast<_uint>(m_vecGameObjects.size() - 1);
	_uint iMinID = m_vecGameObjects.front()->Get_ID();
	_uint iMaxID = m_vecGameObjects.back()->Get_ID();

	if (iFindID > iMaxID)
		return nullptr;

	_float fRatio = (Cast<_float>(iFindID) / (Cast<_float>(iMaxID) - Cast<_float>(iMinID)));
	_uint iPivotIndex = iMaxIndex * fRatio;

	auto iter = m_vecGameObjects.begin() + iPivotIndex;

	_uint iPrevIndex = iPivotIndex;
	while (true)
	{
		// ������ �ȵ� ���¸� ������.
		if ((*iter) == nullptr)
			return nullptr;

		// ���� ID�� ������� ã�� ���ϰ� ������.
		if (iPrevIndex == iPivotIndex)
			return nullptr;

		_uint iID = (*iter)->Get_ID();
		if (iID == iFindID)
			return (*iter);
		else
		{
			if (iID < iFindID)
			{
				fRatio = Cast<_float>(iFindID - iMinID) / (Cast<_float>(iID - iMinID));
				iPivotIndex -= iPivotIndex * fRatio;
			}
			else if (iID > iFindID)
			{
				fRatio = Cast<_float>(iID - iFindID) / (Cast<_float>(iMaxID - iID));
				iPivotIndex += (iMaxIndex - iPivotIndex) * fRatio;
			}
		}

		iPrevIndex = iPivotIndex;
	}

	return nullptr;
}

void CObjectMgr::Clear_GameObject(const wstring& strLayerTag)
{
	_bool bChanged = false;
	for (_uint i = 0; i < m_vecGameObjects.size(); i++)
	{
		CGameObject** ppObj = &m_vecGameObjects[i];
		if ((*ppObj)->Has_Tag(EGObjTag::Level, strLayerTag))
		{
			// �±� ������ �ٸ� �±װ� ������ �������� �ʴ´�. (�ٸ� �������� ��������)
			(*ppObj)->Delete_Tag(EGObjTag::Level, strLayerTag);
			if ((*ppObj)->Tag_Size(EGObjTag::Level) == 0U)
			{
				Safe_Release(*ppObj);
				bChanged = true;
			}
		}
	}

	if (bChanged)
		Straighten_GameObjects();
}

void CObjectMgr::Straighten_GameObjects()
{
	vector<CGameObject*> vecNewVector;
	vecNewVector.reserve(m_vecGameObjects.capacity());

	for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
	{
		if (m_vecGameObjects[i] != nullptr)
		{
			vecNewVector.push_back(m_vecGameObjects[i]);
		}
	}

	m_vecGameObjects.swap(vecNewVector);
}

void CObjectMgr::RegistToTick_GameObjects()
{
	// ������Ʈ ����, ���� ����, ������Ʈ ���¸� ���� �������� �߰��ϴ� ����� ������.
	m_listTickObjects.clear();

	_bool bChanged = false;
	for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
	{
		CGameObject* pObj = m_vecGameObjects[i];
		if (pObj->IsDead())
		{
			// ���۷��� ī���Ͱ� 1�̻��̾��� �� �߰����� ���� �۾��� �ʿ��ϴ�.
			_uint iRefCount = Safe_Release(pObj);
			m_vecGameObjects[i] = nullptr;
			bChanged = true;
		}
		// �ƴҽ� �������� �߰��ϰ� ���� ��ҷ�
		else
		{
			// ���� ���°� ���������� ���ǿ� ���� �׷쿡 �߰��Ѵ�.
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
		}
	}

	if (bChanged)
		Straighten_GameObjects();

	// ����Ʈ�� ����
	copy(m_vecGameObjects.begin(), m_vecGameObjects.end(), back_inserter(m_listTickObjects));

	// Tick �켱�� ��� ����
	_uint iIndex = Cast_EnumDef(EGObjTickPriority::Tick);
	m_listTickObjects.sort(
		[&iIndex](CGameObject* const pDst, CGameObject* const pSrc) {
			return (pDst->Get_Priority(iIndex) > pSrc->Get_Priority(iIndex));
		});
}
