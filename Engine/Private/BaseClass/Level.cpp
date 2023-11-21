#include "BaseClass/Level.h"



CLevel::CLevel(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext)
{
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

void CLevel::Priority_Tick(const _float& fTimeDelta)
{
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

	for (auto& iter : m_vecPriorityLayer)
		iter->Priority_Tick(fTimeDelta);
}

_int CLevel::Tick(const _float& fTimeDelta)
{
	// �켱�� ������ ���ش�.
	_int	iResult = 0;
	for (auto& iter : m_vecPriorityLayer)
	{
		iResult = iter->Tick(fTimeDelta);

		if (iResult & 0x80000000)
			return iResult;
	}

	return iResult;
}

void CLevel::Late_Tick(const _float& fTimeDelta)
{
	for (auto& iter : m_vecPriorityLayer)
		iter->Late_Tick(fTimeDelta);

	// �켱�� ���� �ʱ�ȭ
	m_vecPriorityLayer.clear();
}

HRESULT CLevel::Render()
{
	// _DEBUG ��
}

void CLevel::Free()
{
	// ���̾� �����̳� ���� ����
	for (auto iter = m_mapLayer.begin(); iter != m_mapLayer.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapLayer.erase(iter);
	}
}

HRESULT CLevel::InitializeLate_Scene()
{
	return S_OK;
}

CPrimitiveComponent* CLevel::Get_Component(COMPONENTID eID, const wstring& pLayerTag, const wstring& pObjTag, const wstring& pComponentTag)
{
	// �� -> ���̾� -> ������Ʈ -> ������Ʈ
	//auto	iter = find_if(m_mapLayer.begin(), m_mapLayer.end(), CTag_Finder(pLayerTag));
	auto iter = m_mapLayer.find(pLayerTag);

	if (iter == m_mapLayer.end())
		return nullptr;

	return iter->second->Get_Component(eID, pObjTag, pComponentTag);
}

CGameObject* CLevel::Get_GameObject(const wstring& pLayerTag, const wstring& pObjTag)
{
	CLayer* pLayer = m_mapLayer[pLayerTag];
	NULL_CHECK_RETURN(pLayer, nullptr);

	CGameObject* pObj = pLayer->Get_GameObject(pObjTag);

	return pObj;
}

void CLevel::Add_GameObject(const wstring& pLayerTag, CGameObject* pObj)
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

void CLevel::Add_GameObject(const wstring& pLayerTag, const wstring& pObjTag, CGameObject* pObj)
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

HRESULT CLevel::Add_Layer(const wstring& pLayerTag, CLayer* pLayer)
{
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	m_mapLayer.emplace(pLayerTag, pLayer);

	return S_OK;
}

HRESULT CLevel::Delete_LayerAll()
{
	for (auto iter = m_mapLayer.begin(); iter != m_mapLayer.end();)
	{
		Safe_Release((*iter).second);
		iter = m_mapLayer.erase(iter);
	}
	

	return S_OK;
}
