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
	// �� ������ ����ó��
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

	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return -1;

	return m_pScene_Current->Tick(fTimeDelta);
}

void CObjectMgr::Late_Tick(const _float& fTimeDelta)
{
	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Late_Tick(fTimeDelta);
}

void CObjectMgr::Render()
{
	//GameInstance()->Render_GameObject(pGraphicDev);

	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Render();		// ������
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
	// �Ŵ�����Ʈ -> �� -> ���̾� -> ���� ������Ʈ -> ������Ʈ
	if (nullptr == m_pScene_Current)
		return nullptr;

	return m_pScene_Current->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* CObjectMgr::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
{
	// �Ŵ�����Ʈ -> �� -> ���̾� -> ���� ������Ʈ
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

	// �� ���� �� ���ο� ���� �ε�
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
		// �� ���� �� ���ο� ���� �ε�
		m_pScene_Reserve = (*iter).second;

		break;
	case EMANAGE_SCENE::MULTI:
		// ���� �� ��ü�� �����Ѵ�. ���� �ε��Ϸ� �� �� �� ���� �״�� �ε��Ѵ�.
		// + ���߿� ���� �������� ������ �� �ִ� �̺�Ʈ�� ����� �־��־�� �Ѵ�.
		// �̸� ���� ���� ���þ��� �Ŵ����� ���� ������ �ε���� ���� �ڿ��� ���� ��ü�� ���� ������ �� �ִ�.
		// Ư�� ��������
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
