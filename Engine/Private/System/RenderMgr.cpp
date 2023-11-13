#include "System/RenderMgr.h"

CRenderMgr::CRenderMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_hReadyResult(E_FAIL)
{
	for (_uint i = 0; i < Cast_EnumDef(EGCAMERA_INDEX::SIZE); i++)
	{
		m_matPersView[i] = XMMatrixIdentity();
		m_matPersProj[i] = XMMatrixIdentity();
		m_matOrthoView[i] = XMMatrixIdentity();
		m_matOrthoProj[i] = XMMatrixIdentity();
	}
}

HRESULT CRenderMgr::Initialize(const _uint iWidth, const _uint iHeight)
{
	// 뷰포트 사용가능 디폴트 8개
	constexpr _uint iViewportCount = Cast_EnumDef(EVIEWPORT::SIZE);
	m_vecViewport.reserve(iViewportCount);
	for (size_t i = 0; i < iViewportCount; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0.f, 0.f, (_float)iWidth, (_float)iHeight, 0.f, 1.f };
		m_vecViewport.push_back(UiViewPort);
	}

	// 뷰포트 사용가능 디폴트 8개
	constexpr _uint iViewportRTCount = Cast_EnumDef(EVIEWPORT_RT::SIZE);
	m_vecViewport_RT.reserve(iViewportRTCount);
	for (size_t i = 0; i < iViewportRTCount; i++)
	{
		D3D11_VIEWPORT UiViewPort = { 0.f, 0.f, (_float)iWidth, (_float)iHeight, 0.f, 1.f };
		m_vecViewport_RT.push_back(UiViewPort);
	}

	//D3DXMatrixOrthoLH(&m_matOrthoProject, static_cast<float>(iWidth), static_cast<float>(iHeight), 0.f, 100.f);

	return m_hReadyResult = S_OK;
}

void CRenderMgr::Render()
{
	// 렌더처리를 하는 종류에 따라 따로 모아서 처리한다.
	Render_Perspective();
	Render_Orthogonal();

	// 항상 처리 후 다음 프레임을 위해 초기화시킨다.
	Clear_RenderGroup();
}

CRenderMgr* CRenderMgr::Create(const DX11DEVICE_T tDevice, const _uint iWidth, const _uint iHeight)
{
	ThisClass* pInstance = new ThisClass(tDevice);

	if (FAILED(pInstance->Initialize()))
	{
		Engine::Safe_Release(pInstance);

		MSG_BOX("RenderMgr Create Failed");

		return nullptr;
	}

	return pInstance;
}

void CRenderMgr::Free()
{
	Clear_RenderGroup();
}

void CRenderMgr::Add_RenderGroup(ERENDER_T eType, CGameObject* pGameObject)
{
	if ((0U > Cast<_uint>(eType) || ERENDER_T::SIZE <= eType) 
		|| nullptr == pGameObject)
		return;

	m_RenderGroup[Cast_EnumDef(eType)].push_back(pGameObject);
}

void CRenderMgr::Clear_RenderGroup()
{
	for (_uint i = 0; i < Cast_EnumDef(ERENDER_T::SIZE); ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void CRenderMgr::Render_Perspective()
{
	for (auto& iter : m_RenderGroup[Cast_EnumDef(ERENDER_T::PERSPECTIVE)])
		iter->Render();
}

void CRenderMgr::Render_Orthogonal()
{
	for (auto& iter : m_RenderGroup[Cast_EnumDef(ERENDER_T::ORTHOGONAL)])
		iter->Render();
}

void CRenderMgr::Set_PerspectiveViewMatrix(const _uint iCam, const _matrix& matPersView)
{
	if (0U > iCam || Cast_EnumDef(ERENDER_T::SIZE) <= iCam)
		return;

	m_matPersView[iCam] = matPersView;
}

void CRenderMgr::Set_PerspectiveProjMatrix(const _uint iCam, const _matrix& matPersProj)
{
	if (0U > iCam || Cast_EnumDef(ERENDER_T::SIZE) <= iCam)
		return;

	m_matPersProj[iCam] = matPersProj;
}

void CRenderMgr::Set_OrthogonalViewMatrix(const _uint iCam, const _matrix& matOrthoView)
{
	if (0U > iCam || Cast_EnumDef(ERENDER_T::SIZE) <= iCam)
		return;

	m_matOrthoView[iCam] = matOrthoView;
}

void CRenderMgr::Set_OrthogonalProjMatrix(const _uint iCam, const _matrix& matPersProj)
{
	if (0U > iCam || Cast_EnumDef(ERENDER_T::SIZE) <= iCam)
		return;

	m_matOrthoProj[iCam] = matPersProj;
}

