#include "System/GraphicDev.h"

#include <d3d11sdklayers.h>
#include <dxgidebug.h>

CGraphicDev::CGraphicDev()
{
}

HRESULT CGraphicDev::Initialize(const FDEVICE_INIT& tInit)
{
    _uint		iFlag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
    iFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL			FeatureLV;

    /* 그래픽 장치를 초기화한다. */
    /* ID3D11Device, ID3D11DeviceContext 바로 생성한다. */
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pDeviceContext)))
        return E_FAIL;

    FAILED_CHECK_RETURN(Ready_SwapChain(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_BackBufferRenderTargetView(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_DepthStencilRenderTargetView(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_Viewport(tInit), E_FAIL);

    /* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 셋팅한다. */
    /* 장치는 최대 8개의 렌더타겟을 동시에 들고 있을 수 있다. */
    m_vecRTV.reserve(6);
    m_vecRTV.clear();
    m_vecRTV.push_back(m_pRTV_SwapChain);
    m_vecRTV.push_back(m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Albedo)]);
    m_vecRTV.push_back(m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Normal)]);
    m_vecRTV.push_back(m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Metallic)]);
    m_vecRTV.push_back(m_pRTV_PBR[Cast_EnumDef(ERenderTarget_PBR::Roughness)]);
    m_vecRTV.push_back(m_pRTV_PBR[Cast_EnumDef(ERenderTarget_Common::Emission)]);

    ID3D11RenderTargetView* pRTV[] = { m_vecRTV[0].Get(), m_vecRTV[1].Get(), m_vecRTV[2].Get(), m_vecRTV[3].Get(), m_vecRTV[4].Get() };

    //m_pDeviceContext->OMSetRenderTargets(m_vecRTV.size(), m_vecRTV[0].GetAddressOf(), m_pDepthStencilView);
    m_pDeviceContext->OMSetRenderTargets(2, pRTV, m_pDepthStencilView);

	return S_OK;
}

HRESULT CGraphicDev::Clear_BackBuffer_View(_float4 vClearColor)
{
    _float color[4] = { vClearColor.x, vClearColor.y, vClearColor.z, vClearColor.w };

    // 백버퍼 지우기
    for (auto& item : m_vecRTV)
    {
        auto pRTV = item.Get();
        m_pDeviceContext->ClearRenderTargetView(pRTV, color);
    }

    return S_OK;
}

HRESULT CGraphicDev::Clear_DepthStencil_View()
{
    // 깊이 버퍼 지우기
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    return S_OK;
}

HRESULT CGraphicDev::Present()
{
    // 렌더링 완료되어 화면에 백 버퍼를 표시
    if (m_bVsync_Enabled)
    {
        // 화면 새로 고침 비율 고정
        m_pSwapChain->Present(1, 0);
    }
    else
    {
        // 가능한 빠르게 출력
        m_pSwapChain->Present(0, 0);
    }

    return S_OK;
}

CGraphicDev* CGraphicDev::Create(const FDEVICE_INIT& tInit)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(tInit)))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("CGraphicDev Create Failed");

        return nullptr;
    }

    return pInstance;
}

void CGraphicDev::Free()
{
    if (m_pSwapChain)
    {
        m_pSwapChain->SetFullscreenState(false, NULL);
    }
    Safe_Release(m_pRasterState);
    Safe_Release(m_pDepthStencilView);
    Safe_Release(m_pDepthStencilState);
    Safe_Release(m_pDethStencilBuffer);
    Safe_Release(m_pSwapChain);
    Safe_Release(m_pDeviceContext);
    Safe_Release(m_pDevice);

#ifdef _DEBUG
    m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

    //ID3D11InfoQueue* pInfoQueue = nullptr;
    //m_pDebug->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&pInfoQueue);

    //pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    //pInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);

    //// 메세지 필터를 설정합니다. (예시: 무시할 경고의 종류)
    //D3D11_MESSAGE_ID hide[] = {
    //    D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
    //    // 추가적인 메세지 ID들을 여기에 추가할 수 있습니다.
    //    //D3D11_MESSAGE_ID_
    //};

    //D3D11_INFO_QUEUE_FILTER filter = {};
    //filter.DenyList.NumIDs = _countof(hide);
    //filter.DenyList.pIDList = hide;

    //pInfoQueue->AddRetrievalFilterEntries(&filter);

    //Safe_Release(pInfoQueue);

    Safe_Release(m_pDebug);
#endif // _DEBUG
}

HRESULT CGraphicDev::Ready_SwapChain(const FDEVICE_INIT& tInit)
{
#ifdef _DEBUG
    m_pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug);

    //m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
    ComPtr<IDXGIDevice> pDevice = nullptr;
    m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDevice.GetAddressOf());

    _uint adapterIndex = 0;

    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());

    ComPtr<IDXGIFactory> pFactory = nullptr;
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

    pFactory->EnumAdapters(adapterIndex, &pAdapter);

    /*IDXGIDebug* pGxDebug = nullptr;
    pDevice->QueryInterface(__uuidof(IDXGIDebug), (void**)&pGxDebug);

    pGxDebug->ReportLiveObjects()*/
    
    /* 스왑체인을 생성한다. = 텍스쳐를 생성하는 행위 + 스왑하는 형태  */
    DXGI_SWAP_CHAIN_DESC		SwapChainDesc;
    ZeroMemory(&SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

    /*텍스쳐(백버퍼)를 생성하는 행위*/
    SwapChainDesc.BufferDesc.Width = tInit.iScreenWidth;
    SwapChainDesc.BufferDesc.Height = tInit.iScreenHeight;


    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;

    /*스왑하는 형태*/
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.SampleDesc.Count = 1;

    SwapChainDesc.OutputWindow = tInit.hWnd;
    SwapChainDesc.Windowed = !tInit.bFullScreen;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    /* 백버퍼라는 텍스쳐를 생성했다. */
    if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChainDesc, &m_pSwapChain)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphicDev::Ready_BackBufferRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    /* 내가 앞으로 사용하기위한 용도의 텍스쳐를 생성하기위한 베이스 데이터를 가지고 있는 객체이다. */
    /* 내가 앞으로 사용하기위한 용도의 텍스쳐 : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
    ComPtr<ID3D11Texture2D> pBackBufferTexture = { nullptr };
    ComPtr<ID3D11Texture2D> pTexture = { nullptr };
    D3D11_TEXTURE2D_DESC textureDesc = {};
    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};

    /* 스왑체인이 들고있던 텍스처를 가져와봐. */
    if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), ReCast<void**>(pBackBufferTexture.GetAddressOf()))))
        return E_FAIL;

    ComPtr<ID3D11RenderTargetView> pRTV = { nullptr };
    if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture.Get(), nullptr, pRTV.GetAddressOf())))
        return E_FAIL;

    pBackBufferTexture->GetDesc(&textureDesc);
    pRTV->GetDesc(&rtvDesc);

    m_pTexture_SwapChain = pBackBufferTexture;
    pBackBufferTexture.Reset();
    m_pRTV_SwapChain = pRTV;
    pRTV.Reset();

    // PBR 렌더타깃 생성
    for (_uint i = 0; i < Cast_EnumDef(ERenderTarget_Legacy::Size); i++)
    {
        if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, pTexture.GetAddressOf())))
            return E_FAIL;

        if (FAILED(m_pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, pRTV.GetAddressOf())))
            return E_FAIL;

        m_pTexture_LGC[i] = pTexture;
        pTexture.Reset();
        m_pRTV_LGC[i] = pRTV;
        pRTV.Reset();
    }

    // PBR 렌더타깃 생성
    for (_uint i = 0; i < Cast_EnumDef(ERenderTarget_PBR::Size); i++)
    {
        if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, pTexture.GetAddressOf())))
            return E_FAIL;

        if (FAILED(m_pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, pRTV.GetAddressOf())))
            return E_FAIL;

        m_pTexture_PBR[i] = pTexture;
        pTexture.Reset();
        m_pRTV_PBR[i] = pRTV;
        pRTV.Reset();
    }

    // Emission, SSAO
    for (_uint i = 0; i < Cast_EnumDef(ERenderTarget_Common::Size); i++)
    {
        if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, pTexture.GetAddressOf())))
            return E_FAIL;

        if (FAILED(m_pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, pRTV.GetAddressOf())))
            return E_FAIL;

        m_pTexture_Common[i] = pTexture;
        pTexture.Reset();
        m_pRTV_Common[i] = pRTV;
        pRTV.Reset();
    }

    return S_OK;
}

HRESULT CGraphicDev::Ready_DepthStencilRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    ComPtr<ID3D11Texture2D> pDepthStencilTexture = nullptr;

    D3D11_TEXTURE2D_DESC	TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

    TextureDesc.Width = tInit.iScreenWidth;
    TextureDesc.Height = tInit.iScreenHeight;
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

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, pDepthStencilTexture.GetAddressOf())))
        return E_FAIL;

    /* RenderTarget */
    /* ShaderResource */
    /* DepthStencil */

    if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), nullptr, &m_pDepthStencilView)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphicDev::Ready_Viewport(const FDEVICE_INIT& tInit)
{
    D3D11_VIEWPORT viewport;
    viewport.TopLeftX = 0.f;
    viewport.TopLeftY = 0.f;
    viewport.Width = (_float)tInit.iScreenWidth;
    viewport.Height = (_float)tInit.iScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.f;

    // 뷰포트를 생성
    m_pDeviceContext->RSSetViewports(1, &viewport);

    // 투영 행렬을 설정
    _float fFieldOfView = XM_PI * 0.25f;
    _float fScreenAspect = (_float)tInit.iScreenWidth / (_float)tInit.iScreenHeight;

    // 3D 렌더링을 위한 투영 행렬을 만든다.
    m_matProjection = XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, tInit.fScreenNear, tInit.fScreenDepth);

    // 세계 행렬을 항등 행렬로 초기화
    m_matWorld = XMMatrixIdentity();

    // 2D 렌더링을 위한 직교 투영 행렬을 만든다.
    m_matOrtho = XMMatrixOrthographicLH((_float)tInit.iScreenWidth, (_float)tInit.iScreenHeight, tInit.fScreenNear, tInit.fScreenDepth);

    return S_OK;
}

const _matrix& CGraphicDev::GetProjectionMatrix()
{
    return m_matProjection;
}

const _matrix& CGraphicDev::GetWorldMatrix()
{
    return m_matWorld;
}

const _matrix& CGraphicDev::GetOrthoMatrix()
{
    return m_matOrtho;
}
