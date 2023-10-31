#include "System/RenderMgr.h"

CRenderMgr::CRenderMgr()
	: m_hReadyResult(E_FAIL)
{
	m_matPersView = XMMatrixIdentity();
	m_matOrthoView = XMMatrixIdentity();
}

void CRenderMgr::Free()
{
	Clear_RenderGroup();
}

HRESULT CRenderMgr::Initialize(const _uint iWidth, const _uint iHeight)
{
	// 뷰포트 사용가능 디폴트 8개
	_uint iViewportCount = static_cast<_uint>(EVIEWPORT::SIZE);
	m_vecViewport.reserve(iViewportCount);
	for (size_t i = 0; i < iViewportCount; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0, 0, iWidth, iHeight, 0.f, 1.f };
		m_vecViewport.push_back(UiViewPort);
	}
	
	// 뷰포트 사용가능 디폴트 8개
	_uint iViewportRTCount = static_cast<_uint>(EVIEWPORT_RT::SIZE);
	m_vecViewport_RT.reserve(iViewportRTCount);
	for (size_t i = 0; i < iViewportRTCount; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0, 0, iWidth, iHeight, 0.f, 1.f };
		m_vecViewport_RT.push_back(UiViewPort);
	}

	//D3DXMatrixOrthoLH(&m_matOrthoProject, static_cast<float>(iWidth), static_cast<float>(iHeight), 0.f, 100.f);

	return m_hReadyResult = S_OK;
}

void CRenderMgr::Add_RenderGroup(RENDERID eType, CGameObject* pGameObject)
{
	if ((0 > eType || RENDER_END <= eType) || nullptr == pGameObject)
		return;

	m_RenderGroup[eType].push_back(pGameObject);
	pGameObject->AddRef();
}

void CRenderMgr::Render(ID3D11DeviceContext* pGraphicDev)
{
	// 렌더처리를 하는 종류에 따라 따로 모아서 처리한다.
	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Alpha(pGraphicDev);
	Render_AlphaTest(pGraphicDev); // + 성희 추가
	Render_UI(pGraphicDev);

	// 항상 처리 후 다음 프레임을 위해 초기화시킨다.
	Clear_RenderGroup();
}

void CRenderMgr::Clear_RenderGroup()
{
	for (size_t i = 0; i < RENDER_END; ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void CRenderMgr::Render_Priority(ID3D11DeviceContext* pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render(pGraphicDev);
}

// + 성희 추가
void CRenderMgr::Render_AlphaTest(ID3D11DeviceContext* pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_ALPHATEST])
		iter->Render(pGraphicDev);
}

void CRenderMgr::Render_NonAlpha(ID3D11DeviceContext* pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render(pGraphicDev);
}

void CRenderMgr::Render_Alpha(ID3D11DeviceContext* pGraphicDev)
{
	for (auto& iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render(pGraphicDev);
}

void CRenderMgr::Render_UI(ID3D11DeviceContext* pGraphicDev)
{

	for (auto& iter : m_RenderGroup[RENDER_UI])
		iter->Render(pGraphicDev);

}

