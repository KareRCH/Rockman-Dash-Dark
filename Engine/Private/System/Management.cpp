#include "System/Management.h"

#include "System/GameInstance.h"

CManagement::CManagement(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
	, m_pScene_Current(nullptr), m_pScene_Reserve(nullptr)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CManagement::Initialize(const EMANAGE_SCENE eType)
{
	m_eSceneProcess = eType;

	return S_OK;
}

void CManagement::PriorityTick()
{
	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->PriorityTick();
}

_int CManagement::Tick(const _float& fTimeDelta)
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

void CManagement::LateTick()
{
	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->LateTick();
}

void CManagement::Render()
{
	//GameInstance()->Render_GameObject(pGraphicDev);

	// �� ������ ����ó��
	if (nullptr == m_pScene_Current)
		return;

	m_pScene_Current->Render();		// ������
}

CManagement* CManagement::Create(const DX11DEVICE_T tDevice, const EMANAGE_SCENE eType)
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

void CManagement::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);

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



CPrimitiveComponent* Engine::CManagement::Get_Component(COMPONENTID eID, const _tchar* pLayerTag, const _tchar* pObjTag, const _tchar* pComponentTag)
{
	// �Ŵ�����Ʈ -> �� -> ���̾� -> ���� ������Ʈ -> ������Ʈ
	if (nullptr == m_pScene_Current)
		return nullptr;

	return m_pScene_Current->Get_Component(eID, pLayerTag, pObjTag, pComponentTag);
}

CGameObject* CManagement::Get_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag)
{
	// �Ŵ�����Ʈ -> �� -> ���̾� -> ���� ������Ʈ
	if (nullptr == m_pScene_Current)
		return nullptr;

	return m_pScene_Current->Get_GameObject(pLayerTag, pObjTag);
}

void CManagement::Add_GameObject(const _tchar* pLayerTag, CGameObject* const pObj)
{
	m_pScene_Current->Add_GameObject(pLayerTag, pObj);
}

void CManagement::Add_GameObject(const _tchar* pLayerTag, const _tchar* pObjTag, CGameObject* const pObj)
{
	m_pScene_Current->Add_GameObject(pLayerTag, pObjTag, pObj);
}

void CManagement::Add_Layer(const _tchar* pLayerTag, CLayer* const pLayer)
{
	m_pScene_Current->Add_Layer(pLayerTag, pLayer);
}



HRESULT CManagement::Set_Scene(CScene* pScene)
{
	if (m_eSceneProcess != EMANAGE_SCENE::SINGLE)
		return E_FAIL;

	// �� ���� �� ���ο� ���� �ε�
	m_pScene_Reserve = pScene;
	//GameInstance()->Stop_PhysicsSimulation(0);

	return S_OK;
}

HRESULT CManagement::Set_Scene(wstring strSceneName)
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

HRESULT CManagement::Add_Scene(CScene* pScene, wstring strSceneName)
{
	m_mapScene.emplace(strSceneName, pScene);

	return S_OK;
}

HRESULT CManagement::Clear_CurrentScene()
{
	m_pScene_Current->Delete_LayerAll();

	return S_OK;
}

HRESULT CManagement::Clear_SceneAll()
{
	for (auto& item : m_mapScene)
		item.second->Delete_LayerAll();

	return S_OK;
}

HRESULT CManagement::Delete_SceneAll()
{
	for (auto& item : m_mapScene)
		Safe_Release(item.second);

	return S_OK;
}

HRESULT CManagement::Delete_CurrentScene()
{
	Safe_Release(m_pScene_Current);

	return S_OK;
}
