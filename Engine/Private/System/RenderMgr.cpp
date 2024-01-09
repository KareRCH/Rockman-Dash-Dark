#include "System/RenderMgr.h"

#include "System/GameInstance.h"

CRenderMgr::CRenderMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_hReadyResult(E_FAIL)
{
}

HRESULT CRenderMgr::Initialize(const _uint iWidth, const _uint iHeight)
{
	// 뷰포트 사용가능 디폴트 8개
	m_iNumViewPorts = ECast(EViewportIndex::Size);
	m_vecViewport.reserve(m_iNumViewPorts);
	for (size_t i = 0; i < m_iNumViewPorts; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0.f, 0.f, (_float)iWidth, (_float)iHeight, 0.f, 1.f };
		m_vecViewport.push_back(UiViewPort);
	}

	// 뷰포트 사용가능 디폴트 8개
	constexpr _uint iViewportRTCount = ECast(EViewportRT::Size);
	m_vecViewport_RT.reserve(iViewportRTCount);
	for (size_t i = 0; i < iViewportRTCount; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0.f, 0.f, (_float)iWidth, (_float)iHeight, 0.f, 1.f };
		m_vecViewport_RT.push_back(UiViewPort);
	}

	/* Target_Diffuse */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Diffuse"), 
		Cast<_uint>(m_vecViewport[0].Width), Cast<_uint>(m_vecViewport[0].Height), DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Normal"), 
		Cast<_uint>(m_vecViewport[0].Width), Cast<_uint>(m_vecViewport[0].Height), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Shade"), 
		Cast<_uint>(m_vecViewport[0].Width), Cast<_uint>(m_vecViewport[0].Height), DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(GI()->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(GI()->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(GI()->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;

	return m_hReadyResult = S_OK;
}

void CRenderMgr::Render()
{
	// 렌더처리를 하는 종류에 따라 따로 모아서 처리한다.
	Render_Priority();
	Render_Alpha();
	Render_NonAlpha();
	Render_UI();
	Render_PostProcess();

	// 항상 처리 후 다음 프레임을 위해 초기화시킨다.
	Clear_RenderGroup();
}

CRenderMgr* CRenderMgr::Create(const DX11DEVICE_T tDevice, const _uint iWidth, const _uint iHeight)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("RenderMgr Create Failed");
		Engine::Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderMgr::Free()
{
	Clear_RenderGroup();
}

void CRenderMgr::Add_RenderGroup(ERenderGroup eType, CGameObject* pGameObject)
{
	if (nullptr == pGameObject ||
		(0U > Cast<_uint>(eType) || ERenderGroup::Size <= eType))
		return;

	m_RenderGroup[ECast(eType)].push_back(pGameObject);
	Safe_AddRef(pGameObject);
}

void CRenderMgr::Clear_RenderGroup()
{
	for (_uint i = 0; i < ECast(ERenderGroup::Size); ++i)
		m_RenderGroup[i].clear();
}

void CRenderMgr::Render_Priority()
{
	GameInstance()->TurnOff_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::Priority)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}
}

void CRenderMgr::Render_Alpha()
{
	GameInstance()->TurnOn_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::Alpha)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}
}

void CRenderMgr::Render_NonAlpha()
{
	GameInstance()->TurnOn_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::Blend)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}
}

void CRenderMgr::Render_UI()
{
	GameInstance()->TurnOff_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::UI)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}
}

void CRenderMgr::Render_PostProcess()
{
	GameInstance()->TurnOff_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::PostProcess)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}
}

