#include "System/ObjectMgr.h"

#include "System/GameInstance.h"

CObjectMgr::CObjectMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
	, m_pScene_Current(nullptr), m_pScene_Reserve(nullptr)
{
}

HRESULT CObjectMgr::Initialize(const EMANAGE_SCENE eType)
{
	m_eSceneProcess = eType;

	return S_OK;
}

void CObjectMgr::Priority_Tick(const _float& fTimeDelta)
{
	// 씬 없으면 예외처리
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Priority_Tick(fTimeDelta);
}

_int CObjectMgr::Tick(const _float& fTimeDelta)
{
	if (m_pScene_Reserve)
	{
		switch (m_eSceneProcess)
		{
		case Engine::EMANAGE_SCENE::SINGLE:
		{
			Safe_Release(m_pScene_Current);
			m_pScene_Current = m_pScene_Reserve;
			m_pScene_Reserve = nullptr;
			break;
		}
		case Engine::EMANAGE_SCENE::MULTI:
		{
			m_pScene_Current = m_pScene_Reserve;
			m_pScene_Reserve = nullptr;
			break;
		}
		}

		//GameInstance()->Clear_RenderGroup();
		GameInstance()->Play_PhysicsSimulation(0);
	}

	// 씬 없으면 예외처리
	if (nullptr == m_pScene_Current)
		return -1;

	return m_pScene_Current->Tick(fTimeDelta);
}

void CObjectMgr::Late_Tick(const _float& fTimeDelta)
{
	// 씬 없으면 예외처리
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Late_Tick(fTimeDelta);
}

void CObjectMgr::Render()
{
	//GameInstance()->Render_GameObject(pGraphicDev);

	// 씬 없으면 예외처리
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Render();		// 디버깅용
}

CObjectMgr* CObjectMgr::Create(const DX11DEVICE_T tDevice, const EMANAGE_SCENE eType)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize(eType)))
	{
		MSG_BOX("Management Create Failed");
		Engine::Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CObjectMgr::Free()
{
	switch (m_eSceneProcess)
	{
	case EMANAGE_SCENE::SINGLE:
		Safe_Release(m_pScene_Current);
		break;
	case EMANAGE_SCENE::MULTI:
		for (auto& item : m_mapScene)
			Safe_Release(item.second);
		m_mapScene.clear();
		break;
	}
}


//-------------------------------------------------------



CPrimitiveComponent* Engine::CObjectMgr::Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag)
{
	// 매니지먼트 -> 씬 -> 레이어 -> 게임 오브젝트 -> 컴포넌트
	if (nullptr == m_pScene_Current)
		return nullptr;

	return m_pScene_Current->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* CObjectMgr::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
{
	// 매니지먼트 -> 씬 -> 레이어 -> 게임 오브젝트
	if (nullptr == m_pScene_Current)
		return nullptr;

	return m_pScene_Current->Get_GameObject(pLayerTag, pObjTag);
}

void CObjectMgr::Add_GameObject(const wstring& pLayerTag, CGameObject* const pObj)
{
	m_pScene_Current->Add_GameObject(pLayerTag, pObj);
}

void CObjectMgr::Add_GameObject(const wstring& pLayerTag, const wstring& pObjTag, CGameObject* const pObj)
{
	m_pScene_Current->Add_GameObject(pLayerTag, pObjTag, pObj);
}

void CObjectMgr::Add_Layer(const wstring& pLayerTag, CLayer* const pLayer)
{
	m_pScene_Current->Add_Layer(pLayerTag, pLayer);
}



HRESULT CObjectMgr::Set_Scene(CLevel* pScene)
{
	if (m_eSceneProcess != EMANAGE_SCENE::SINGLE)
		return E_FAIL;

	// 씬 해제 후 새로운 씬을 로드
	m_pScene_Reserve = pScene;
	//GameInstance()->Stop_PhysicsSimulation(0);

	return S_OK;
}

HRESULT CObjectMgr::Set_Scene(wstring strSceneName)
{
	auto iter = m_mapScene.find(strSceneName);
	if (iter == m_mapScene.end())
		return E_FAIL;

	switch (m_eSceneProcess)
	{
	case EMANAGE_SCENE::SINGLE:
		// 씬 해제 후 새로운 씬을 로드
		m_pScene_Reserve = (*iter).second;

		break;
	case EMANAGE_SCENE::MULTI:
		// 기존 씬 자체는 유지한다. 씬을 로드하러 올 때 그 상태 그대로 로드한다.
		// + 나중에 씬이 멈췄음을 인지할 수 있는 이벤트를 만들어 넣어주어야 한다.
		// 이를 통해 씬과 관련없는 매니저와 같은 곳에서 로드되지 않은 자원을 쓰는 객체에 대해 통제할 수 있다.
		// 특히 물리엔진
		m_pScene_Reserve = (*iter).second;

		break;
	}
	//Engine::Stop_PhysicsSimulation(0);

	return S_OK;
}

HRESULT CObjectMgr::Add_Scene(CLevel* pScene, wstring strSceneName)
{
	m_mapScene.emplace(strSceneName, pScene);

	return S_OK;
}

HRESULT CObjectMgr::Clear_CurrentScene()
{
	m_pScene_Current->Delete_LayerAll();

	return S_OK;
}

HRESULT CObjectMgr::Clear_SceneAll()
{
	for (auto& item : m_mapScene)
		item.second->Delete_LayerAll();

	return S_OK;
}

HRESULT CObjectMgr::Delete_SceneAll()
{
	for (auto& item : m_mapScene)
		Safe_Release(item.second);

	return S_OK;
}

HRESULT CObjectMgr::Delete_CurrentScene()
{
	Safe_Release(m_pScene_Current);

	return S_OK;
}
