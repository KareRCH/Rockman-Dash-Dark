#include "BaseClass/Level.h"

HRESULT CLevel::Initialize()
{
	return S_OK;
}

void CLevel::Priority_Tick(const _float& fTimeDelta)
{
	{
		m_vecPriorityLayer.reserve(m_mapLayer.size());

		// vector에 객체 추가
		for (auto& item : m_mapLayer)
			m_vecPriorityLayer.push_back(item.second);

		// 우선도 기반 정렬
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
	// 우선도 설정을 해준다.
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

	// 우선도 벡터 초기화
	m_vecPriorityLayer.clear();
}

HRESULT CLevel::Render()
{
	// _DEBUG 용
	return S_OK;
}

void CLevel::Free()
{
	// 레이어 컨테이너 비우고 삭제
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
	// 씬 -> 레이어 -> 오브젝트 -> 컴포넌트
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
		OutputDebugString(L"레이어 없음!");
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
		OutputDebugString(L"레이어 없음!");
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
