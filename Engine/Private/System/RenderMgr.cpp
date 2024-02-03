#include "System/RenderMgr.h"

#include "System/GameInstance.h"
#include "Component/EffectComponent.h"
#include "Component/RectBufferComp.h"

_uint		g_iSizeX = 8192;
_uint		g_iSizeY = 4608;

CRenderMgr::CRenderMgr(const DX11DEVICE_T tDevice)
	: m_pDevice(tDevice.pDevice), m_pDeviceContext(tDevice.pDeviceContext), m_hReadyResult(E_FAIL)
{
	NULL_CHECK(m_pGI = GI());
	Safe_AddRef(m_pGI);
	NULL_CHECK(m_pPipelineComp = DynCast<CPipelineComp*>(GI()->Reference_PrototypeComp(TEXT("CamViewComp"))));
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
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Diffuse"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Normal"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Depth */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Depth"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, -1.f, 1.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Shade"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Specular"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_LightDepth */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_LightDepth"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Final */
	if (FAILED(GI()->Add_RenderTarget(TEXT("Target_Final"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;



#pragma region 블룸 타겟
	if (FAILED(m_pGI->Add_RenderTarget(TEXT("Target_Effect"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect"))))
		return E_FAIL;

	if (FAILED(m_pGI->Add_RenderTarget(TEXT("Target_Blur_X"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pGI->Add_RenderTarget(TEXT("Target_Blur_Y"), m_vecViewport[0].Width, m_vecViewport[0].Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_Blur_Y"), TEXT("Target_Blur_Y"))))
		return E_FAIL;
#pragma endregion

	


	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_LightDepth"))))
		return E_FAIL;
	if (FAILED(m_pGI->Add_MRT(TEXT("MRT_Final"), TEXT("Target_Final"))))
		return E_FAIL;
	

	m_pVIBuffer = CRectBufferComp::Create();
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pEffect[ECast(EEffect::Deferred)] = CEffectComponent::Create();
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Effect(TEXT("Runtime/FX_Deferred.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements)))
		return E_FAIL;
	m_pEffect[ECast(EEffect::PostProcess)] = CEffectComponent::Create();
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Effect(TEXT("Runtime/FX_Fog.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements)))
		return E_FAIL;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(m_vecViewport[0].Width, m_vecViewport[0].Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_vecViewport[0].Width, m_vecViewport[0].Height, 0.f, 1.f));


#pragma region 스텐실 뷰
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr,
		&pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr,
		m_pLightDepthDSV.GetAddressOf())))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);
#pragma endregion




#ifdef _DEBUG
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Diffuse"), 50.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Normal"), 50.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Depth"), 50.f, 250.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Shade"), 150.f, 50.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Specular"), 150.f, 150.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Final"), 150.f, 250.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_LightDepth"), 1130.f, 150.f, 300.f, 300.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Effect"), 50.f, 350.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Blur_X"), 50.f, 450.f, 100.f, 100.f)))
		return E_FAIL;
	if (FAILED(m_pGI->Ready_RenderTarget_Debug(TEXT("Target_Blur_Y"), 150.f, 450.f, 100.f, 100.f)))
		return E_FAIL;
#endif

	return m_hReadyResult = S_OK;
}

HRESULT CRenderMgr::Render()
{
	// 렌더처리를 하는 종류에 따라 따로 모아서 처리한다.
	if (FAILED(Render_Priority()))
		return E_FAIL;
	if (FAILED(Render_Shadow()))
		return E_FAIL;
	if (FAILED(Render_NonBlend()))
		return E_FAIL;
	if (FAILED(Render_LightAcc()))
		return E_FAIL;
	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_Bloom()))
		return E_FAIL;

	if (FAILED(Render_Fog()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;
	if (FAILED(Render_PostProcess()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // _DEBUG

	// 항상 처리 후 다음 프레임을 위해 초기화시킨다.
	Clear_RenderGroup();

	return S_OK;
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

	for (_uint i = 0; i < ECast(EEffect::Size); i++)
	{
		Safe_Release(m_pEffect[i]);
	}
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pPipelineComp);
	Safe_Release(m_pGI);
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

HRESULT CRenderMgr::Render_Priority()
{
	//GameInstance()->TurnOff_ZBuffer();

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::Priority)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	return S_OK;
}

HRESULT CRenderMgr::Render_Shadow()
{
	if (FAILED(GI()->Begin_MRT(TEXT("MRT_Shadow"), m_pLightDepthDSV.Get())))
		return E_FAIL;

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = g_iSizeX;
	ViewPortDesc.Height = g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	GI()->Get_GraphicContext()->RSSetViewports(1, &ViewPortDesc);

	for (auto& pGameObject : m_RenderGroup[ECast(ERenderGroup::Shadow)])
	{
		if (nullptr != pGameObject)
		{
			pGameObject->Render_Shadow();
			Safe_Release(pGameObject);
		}
	}

	if (FAILED(GI()->End_MRT()))
		return E_FAIL;

	GI()->Get_GraphicContext()->RSSetViewports(1, &m_vecViewport[0]);

	return S_OK;
}

HRESULT CRenderMgr::Render_NonLight()
{
	if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_Effect"))))
		return E_FAIL;

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::NonLight)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	if (FAILED(m_pGI->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderMgr::Render_NonBlend()
{
	/* 기존에 셋팅되어있던 백버퍼를 빼내고 Diffuse와 Normal을 장치에 바인딩한다. */
	if (FAILED(GI()->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::NonBlend)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	/* 백버퍼를 원래 위치로 다시 장치에 바인딩한다. */
	if (FAILED(GI()->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderMgr::Render_Blend()
{
	if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_Effect"))))
		return E_FAIL;

	_vector vCamPos = PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One);
	m_RenderGroup[ECast(ERenderGroup::Blend)].sort(
		[&vCamPos](CGameObject* pDst, CGameObject* pSrc) {
			_vector vDstPos = vCamPos - pDst->Transform().Get_PositionVector();
			_float fDstLength = XMVectorGetX(XMVector3Length(vDstPos));

			_vector vSrcPos = vCamPos - pSrc->Transform().Get_PositionVector();
			_float fSrcLength = XMVectorGetX(XMVector3Length(vSrcPos));

			return fDstLength > fSrcLength;
		}
	);

	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::Blend)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	if (FAILED(m_pGI->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderMgr::Render_UI()
{
	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::UI)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	return S_OK;
}

HRESULT CRenderMgr::Render_PostProcess()
{
	for (auto& pObj : m_RenderGroup[ECast(ERenderGroup::PostProcess)])
	{
		pObj->Render();
		Safe_Release(pObj);
	}

	return S_OK;
}

HRESULT CRenderMgr::Render_LightAcc()
{
	/* Shade */
	/* 여러개 빛의 연산 결과를 저장해 준다. */
	if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4 matTemp = {};
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ViewMatrixInv",
		&(matTemp = PipelineComp().Get_CamInvFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ProjMatrixInv",
		&(matTemp = PipelineComp().Get_CamInvFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
		return E_FAIL;

	_float4 vCamPos = {};
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_RawValue("g_vCamPosition",
		&(vCamPos = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One)), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Normal"), m_pEffect[ECast(EEffect::Deferred)], "g_NormalTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pEffect[ECast(EEffect::Deferred)], "g_DepthTexture")))
		return E_FAIL;

	m_pGI->Render_Lights(m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);

	if (FAILED(m_pGI->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderMgr::Render_Deferred()
{
	if (m_bIsPostProcess)
	{
		if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_Final"))))
			return E_FAIL;
	}

	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4		ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixLookAtLH(XMVectorSet(150.f, 200.f, 150.f, 1.f), XMVectorSet(150.f, 0.f, 150.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
	XMStoreFloat4x4(&ProjMatrix, XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.1f, 1000.f));

	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_LightViewMatrix", &ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_LightProjMatrix", &ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Diffuse"), m_pEffect[ECast(EEffect::Deferred)], "g_DiffuseTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Shade"), m_pEffect[ECast(EEffect::Deferred)], "g_ShadeTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Specular"), m_pEffect[ECast(EEffect::Deferred)], "g_SpecularTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pEffect[ECast(EEffect::Deferred)], "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_LightDepth"), m_pEffect[ECast(EEffect::Deferred)], "g_LightDepthTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_Y"), m_pEffect[ECast(EEffect::Deferred)], "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Effect"), m_pEffect[ECast(EEffect::Deferred)], "g_EffectTexture")))
		return E_FAIL;



	m_pEffect[ECast(EEffect::Deferred)]->Begin(3);

	m_pVIBuffer->Bind_Buffer();

	m_pVIBuffer->Render_Buffer();

	if (m_bIsPostProcess)
	{
		if (FAILED(m_pGI->End_MRT()))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderMgr::Render_Bloom()
{
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

#pragma region X 블러
	if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_Blur_X"))))
		return E_FAIL;

	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Effect"), m_pEffect[ECast(EEffect::Deferred)], "g_EffectTexture")))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffer();

	m_pEffect[ECast(EEffect::Deferred)]->Begin(4);

	m_pVIBuffer->Render_Buffer();

	if (FAILED(m_pGI->End_MRT()))
		return E_FAIL;
#pragma endregion


#pragma region Y 블러
	if (FAILED(m_pGI->Begin_MRT(TEXT("MRT_Blur_Y"))))
		return E_FAIL;

	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_X"), m_pEffect[ECast(EEffect::Deferred)], "g_EffectTexture")))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffer();

	m_pEffect[ECast(EEffect::Deferred)]->Begin(5);

	m_pVIBuffer->Render_Buffer();

	if (FAILED(m_pGI->End_MRT()))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CRenderMgr::Render_Fog()
{
	if (!m_bIsPostProcess || !m_bIsFogShader)
		return S_OK;

	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_RawValue("g_fStart", &m_fFogStart, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_RawValue("g_fRange", &m_fFogRange, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_RawValue("g_fDensity", &m_fFogDensity, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_RawValue("g_vColor", &m_vFogColor, sizeof(_float4))))
		return E_FAIL;

	_float4x4 matTemp = {};
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Matrix("g_ViewMatrixInv",
		&(matTemp = PipelineComp().Get_CamInvFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)))))
		return E_FAIL;
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_Matrix("g_ProjMatrixInv",
		&(matTemp = PipelineComp().Get_CamInvFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)))))
		return E_FAIL;

	_float4 vCamPos = {};
	if (FAILED(m_pEffect[ECast(EEffect::PostProcess)]->Bind_RawValue("g_vCamPosition",
		&(vCamPos = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One)), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(GI()->Bind_RenderTarget_ShaderResource(TEXT("Target_Depth"), m_pEffect[ECast(EEffect::PostProcess)], "g_DepthTexture")))
		return E_FAIL;
	if (FAILED(GI()->Bind_RenderTarget_ShaderResource(TEXT("Target_Final"), m_pEffect[ECast(EEffect::PostProcess)], "g_FinalTexture")))
		return E_FAIL;
	if (FAILED(GI()->Bind_RenderTarget_ShaderResource(TEXT("Target_Blur_Y"), m_pEffect[ECast(EEffect::PostProcess)], "g_BlurTexture")))
		return E_FAIL;
	if (FAILED(m_pGI->Bind_RenderTarget_ShaderResource(TEXT("Target_Effect"), m_pEffect[ECast(EEffect::PostProcess)], "g_EffectTexture")))
		return E_FAIL;

	m_pEffect[ECast(EEffect::PostProcess)]->Begin(ECast(m_eFogType));

	m_pVIBuffer->Bind_Buffer();

	m_pVIBuffer->Render_Buffer();

	ID3D11ShaderResourceView* pSRV = { nullptr };
	m_pDeviceContext->PSSetShaderResources(0, 1, &pSRV);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderMgr::Render_Debug()
{
	m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix);
	m_pEffect[ECast(EEffect::Deferred)]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix);

	// 디버그용 렌더 이벤트를 등록하고 실행한뒤 제거한다.
	if (m_bIsDebugDraw)
		m_DebugEvent.Broadcast();
	m_DebugEvent.Clear();

	if (!m_bIsDeferred)
		return S_OK;

	m_pGI->Render_Debug_RTVs(TEXT("MRT_GameObjects"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_LightAcc"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_Shadow"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_Final"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_Effect"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_Blur_X"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);
	m_pGI->Render_Debug_RTVs(TEXT("MRT_Blur_Y"), m_pEffect[ECast(EEffect::Deferred)], m_pVIBuffer);

	return S_OK;
}
#endif

void CRenderMgr::Set_ViewportSize(_uint iResizeWidth, _uint iResizeHeight)
{
	for (_uint i = 0; i < m_iNumViewPorts; i++)
	{
		m_vecViewport[i].Width = Cast<_float>(iResizeWidth);
		m_vecViewport[i].Height = Cast<_float>(iResizeHeight);
	}

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(m_vecViewport[0].Width, m_vecViewport[0].Height, 1.f));
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(m_vecViewport[0].Width, m_vecViewport[0].Height, 0.f, 1.f));
}

