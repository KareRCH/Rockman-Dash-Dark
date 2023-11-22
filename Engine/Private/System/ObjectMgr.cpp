#include "System/ObjectMgr.h"

#include "System/GameInstance.h"

CObjectMgr::CObjectMgr()
{
}

HRESULT CObjectMgr::Initialize(_uint iNumLevels)
{
	if (nullptr != m_pLayers)
		return E_FAIL;

	m_iNumLevels = iNumLevels;

	m_pLayers = new map_layer[iNumLevels];

	return S_OK;
}

void CObjectMgr::Priority_Tick(const _float& fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Priority_Tick(fTimeDelta);
		}
	}
}

_int CObjectMgr::Tick(const _float& fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Tick(fTimeDelta);
		}
	}

	return 0;
}

void CObjectMgr::Late_Tick(const _float& fTimeDelta)
{
	for (_uint i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Late_Tick(fTimeDelta);
		}
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
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}
	Safe_Delete_Array(m_pLayers);

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

HRESULT CObjectMgr::Add_CloneObject(_uint iLevelIndex, const wstring& strLayerKey, const wstring& strPrototypeKey, void* pArg)
{
	/* 원형을 찾고. */
	CGameObject* pPrototype = Find_Prototype(strPrototypeKey);

	if (nullptr == pPrototype)
		return E_FAIL;

	/* 원형을 복제하여 실제 게임내에 사용할 사본 객체를 생성해낸다.  */
	CGameObject* pGameObject = pPrototype->Clone(pArg);
	if (nullptr == pGameObject)
		return E_FAIL;

	/* 만들어낸 사본객체를 추가해야할 레이어를 찾자. */
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerKey);

	/* 아직 해당 이름을 가진 레이어가 없었다. */
	/* 이 이름을 가진 레이어에 최초로 추가하고 있는 상황이다. */
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create(0);
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLevelIndex].emplace(strLayerKey, pLayer);
	}
	/* 이미 이름을 가진 레이어가 있었어. */
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

CGameObject* CObjectMgr::Find_Prototype(const wstring& strPrototypeKey)
{
	return nullptr;
}

CLayer* CObjectMgr::Find_Layer(_uint iLevelIndex, const wstring& strLayerKey)
{
	return nullptr;
}


//-------------------------------------------------------



//CPrimitiveComponent* Engine::CObjectMgr::Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag)
//{
//	// 매니지먼트 -> 씬 -> 레이어 -> 게임 오브젝트 -> 컴포넌트
//	if (nullptr == m_pScene_Current)
//		return nullptr;
//
//	return m_pScene_Current->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
//}
//
//CGameObject* CObjectMgr::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
//{
//	// 매니지먼트 -> 씬 -> 레이어 -> 게임 오브젝트
//	if (nullptr == m_pScene_Current)
//		return nullptr;
//
//	return m_pScene_Current->Get_GameObject(pLayerTag, pObjTag);
//}
//
//void CObjectMgr::Add_GameObject(const wstring& pLayerTag, CGameObject* const pObj)
//{
//	m_pScene_Current->Add_GameObject(pLayerTag, pObj);
//}
//
//void CObjectMgr::Add_GameObject(const wstring& pLayerTag, const wstring& pObjTag, CGameObject* const pObj)
//{
//	m_pScene_Current->Add_GameObject(pLayerTag, pObjTag, pObj);
//}
//
//void CObjectMgr::Add_Layer(const wstring& pLayerTag, CLayer* const pLayer)
//{
//	m_pScene_Current->Add_Layer(pLayerTag, pLayer);
//}
//
