#include "System/ObjectMgr.h"

#include "BaseClass/GameObject.h"
#include "System/GameInstance.h"
#include "System/RenderMgr.h"

HRESULT CObjectMgr::Initialize(_uint iNumLevels)
{
	m_vecGameObjects.reserve(100000);	// 기본 크기는 10만

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
	// 인스턴스 해제
	for (auto pObj : m_vecGameObjects)
		Safe_Release(pObj);
	m_vecGameObjects.clear();

	// 프로토타입 해제
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
	/* 원형을 찾고. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeKey);
	if (nullptr == pPrototype)
		return E_FAIL;

	/* 원형을 복제하여 실제 게임내에 사용할 사본 객체를 생성해낸다.  */
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
	// 분류별로 프로토타입을 메모리 해제하는 작업
	for (auto iter = m_mapPrototypes.begin(); iter != m_mapPrototypes.end();)
	{
		CGameObject** ppObj = &(*iter).second;
		if ((*ppObj)->Has_Tag(EGObjTag::Level, strContainTag))
		{
			// 태그 삭제후 다른 태그가 있으면 삭제하지 않는다. (다른 레벨에도 속해있음)
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
		// 정리가 안된 상태면 끝낸다.
		if ((*iter) == nullptr)
			return nullptr;

		// 이전 ID와 같은경우 찾지 못하고 나간다.
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
			// 태그 삭제후 다른 태그가 있으면 삭제하지 않는다. (다른 레벨에도 속해있음)
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
	// 오브젝트 삭제, 순서 조절, 오브젝트 상태를 통한 렌더러에 추가하는 기능을 가진다.
	m_listTickObjects.clear();

	_bool bChanged = false;
	for (_uint i = 0; i < m_vecGameObjects.size(); ++i)
	{
		CGameObject* pObj = m_vecGameObjects[i];
		if (pObj->IsDead())
		{
			// 레퍼런스 카운터가 1이상이었을 때 추가적인 해제 작업이 필요하다.
			_uint iRefCount = Safe_Release(pObj);
			m_vecGameObjects[i] = nullptr;
			bChanged = true;
		}
		// 아닐시 렌더러에 추가하고 다음 요소로
		else
		{
			// 렌더 상태가 켜져있으면 조건에 따라 그룹에 추가한다.
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
				// ZBuffer가 필요없는 그룹
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

	// 리스트에 저장
	copy(m_vecGameObjects.begin(), m_vecGameObjects.end(), back_inserter(m_listTickObjects));

	// Tick 우선도 기반 정렬
	_uint iIndex = Cast_EnumDef(EGObjTickPriority::Tick);
	m_listTickObjects.sort(
		[&iIndex](CGameObject* const pDst, CGameObject* const pSrc) {
			return (pDst->Get_Priority(iIndex) > pSrc->Get_Priority(iIndex));
		});
}
