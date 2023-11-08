#include "BaseClass/Scene.h"



CScene::CScene(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CScene::Initialize()
{
	return S_OK;
}

void CScene::Priority_Tick(const _float& fTimeDelta)
{
}

_int CScene::Tick(const _float& fTimeDelta)
{
	// �켱�� ������ ���ش�.
	{
		m_vecPriorityLayer.reserve(m_mapLayer.size());

		// vector�� ��ü �߰�
		for (auto& item : m_mapLayer)
			m_vecPriorityLayer.push_back(item.second);

		// �켱�� ��� ����
		sort(m_vecPriorityLayer.begin(), m_vecPriorityLayer.end(),
			[](CLayer* const pDst, CLayer* const pSrc) {
				return pDst->Get_Priority() > pSrc->Get_Priority();
			});
	}

	_int	iResult = 0;
	for (auto& iter : m_vecPriorityLayer)
	{
		iResult = iter->Tick(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CScene::Late_Tick(const _float& fTimeDelta)
{
	for (auto& iter : m_vecPriorityLayer)
		iter->Late_Tick(fTimeDelta);

	// �켱�� ���� �ʱ�ȭ
	m_vecPriorityLayer.clear();
}

void CScene::Render()
{
	// _DEBUG ��
}

void CScene::Free()
{
	// ���̾� �����̳� ���� ����
	for (auto iter = m_mapLayer.begin(); iter != m_mapLayer.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapLayer.erase(iter);
	}

	// ���۷��� ī��Ʈ ���̱�
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
}

HRESULT CScene::InitializeLate_Scene()
{
	return S_OK;
}

CPrimitiveComponent* CScene::Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag)
{
	// �� -> ���̾� -> ������Ʈ -> ������Ʈ
	//auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	auto iter = m_mapLayer.find(pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

CGameObject* CScene::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
{
	CLayer* pLayer = m_mapLayer[pLayerTag];
	NULL_CHECK_RETURN(pLayer, nullptr);

	CGameObject* pObj = pLayer->Get_GameObject(pObjTag);

	return pObj;
}

void CScene::Add_GameObject(const wstring& pLayerTag, CGameObject* pObj)
{
	auto iter = m_mapLayer.find(pLayerTag);
	if (iter == m_mapLayer.end())
	{
		OutputDebugString(L"���̾� ����!");
		Safe_Release(pObj);
		return;
	}

	(*iter).second->Add_GameObject(pObj);
}

void CScene::Add_GameObject(const wstring& pLayerTag, const wstring& pObjTag, CGameObject* pObj)
{
	auto iter = m_mapLayer.find(pLayerTag);
	if (iter == m_mapLayer.end())
	{
		OutputDebugString(L"���̾� ����!");
		Safe_Release(pObj);
		return;
	}

	(*iter).second->Add_GameObject(pObjTag, pObj);
}

HRESULT CScene::Add_Layer(const wstring& pLayerTag, CLayer* pLayer)
{
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CScene::Delete_LayerAll()
{
	for (auto iter = m_mapLayer.begin(); iter != m_mapLayer.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapLayer.erase(iter);
	}
	

	return S_OK;
}
