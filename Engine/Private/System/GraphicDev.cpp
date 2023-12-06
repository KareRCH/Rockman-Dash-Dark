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
    if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, 
        m_pDevice.GetAddressOf(), &FeatureLV, m_pDeviceContext.GetAddressOf())))
        return E_FAIL;

    m_iScreenWidth = tInit.iScreenWidth;
    m_iScreenHeight = tInit.iScreenHeight;
    m_iNumRenderTargets = MaxRenderTarget;
    m_vecRTV.reserve(MaxRenderTarget);

    FAILED_CHECK_RETURN(Ready_SwapChain(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_BackBufferRenderTargetView(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_DepthStencilRenderTargetView(tInit), E_FAIL);

    FAILED_CHECK_RETURN(Ready_Viewport(tInit), E_FAIL);

    /* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 셋팅한다. */
    /* 장치는 최대 8개의 렌더타겟을 동시에 들고 있을 수 있다. */
    Regist_RenderTarget(0);
    Regist_RenderTarget(1);
    Bind_RenderTargetsOnDevice();

	return S_OK;
}

HRESULT CGraphicDev::Clear_BackBuffer_View(_float4 vClearColor)
{
    _float color[4] = { vClearColor.x, vClearColor.y, vClearColor.z, vClearColor.w };

    // 백버퍼 지우기
    for (auto& item : m_vecRTV)
    {
        auto pRTV = item;
        m_pDeviceContext->ClearRenderTargetView(pRTV, color);
    }

    return S_OK;
}

HRESULT CGraphicDev::Clear_DepthStencil_View()
{
    // 깊이 버퍼 지우기
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

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
        MSG_BOX("CGraphicDev Create Failed");
        Engine::Safe_Release(pInstance);

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
#endif // _DEBUG
}

HRESULT CGraphicDev::Ready_SwapChain(const FDEVICE_INIT& tInit)
{
#ifdef _DEBUG
    m_pDevice->QueryInterface(__uuidof(ID3D11Debug), ReCast<void**>(m_pDebug.GetAddressOf()));

    //m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
    ComPtr<IDXGIDevice> pDevice = nullptr;
    m_pDevice->QueryInterface(__uuidof(IDXGIDevice), ReCast<void**>(pDevice.GetAddressOf()));

    _uint adapterIndex = 0;

    ComPtr<IDXGIAdapter> pAdapter = nullptr;
    pDevice->GetParent(__uuidof(IDXGIAdapter), ReCast<void**>(pAdapter.GetAddressOf()));

    ComPtr<IDXGIFactory> pFactory = nullptr;
    pAdapter->GetParent(__uuidof(IDXGIFactory), ReCast<void**>(pFactory.GetAddressOf()));

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
    if (FAILED(pFactory->CreateSwapChain(m_pDevice.Get(), &SwapChainDesc, &m_pSwapChain)))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphicDev::Ready_BackBufferRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    Create_RenderTargets();

    return S_OK;
}

HRESULT CGraphicDev::Ready_DepthStencilRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    D3D11_TEXTURE2D_DESC	TextureDesc = {};
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

    // 깊이 스텐실 텍스처 생성
    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf())))
        return E_FAIL;

    /*******************************
    * 깊이 스텐실 상태 정의
    ******************************/
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;

    // Z버퍼용
    if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, m_pDepthStencilState.GetAddressOf())))
        return E_FAIL;

    // Z버퍼 안쓰는 물체용
    depthStencilDesc.DepthEnable = false;
    if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, m_pDepthDisabledStencilState.GetAddressOf())))
        return E_FAIL;

    // 깊이 스텐실 상태를 설정한다.
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);


    /***********************
    * 깊이 스텐실 뷰 정의
    ************************/
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // 깊이 스텐실 뷰 생성
    if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, m_pDepthStencilView.GetAddressOf())))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphicDev::Ready_RasterizeState(const FDEVICE_INIT& tInit)
{
    if (m_pDevice == nullptr)
        return E_FAIL;

    // 래스터라이즈 설정
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.f;

    if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, m_pRasterState.GetAddressOf())))
        return E_FAIL;

    rasterDesc.CullMode = D3D11_CULL_NONE;
    if (FAILED(m_pDevice->CreateRasterizerState(&rasterDesc, m_pRasterCullNoneState.GetAddressOf())))
        return E_FAIL;

    m_pDeviceContext->RSSetState(m_pRasterState.Get());

    return S_OK;
}

HRESULT CGraphicDev::Ready_Viewport(const FDEVICE_INIT& tInit)
{
    D3D11_VIEWPORT viewport1;
    viewport1.TopLeftX = 0.f;
    viewport1.TopLeftY = 0.f;
    viewport1.Width = (_float)tInit.iScreenWidth;
    viewport1.Height = (_float)tInit.iScreenHeight;
    viewport1.MinDepth = 0.0f;
    viewport1.MaxDepth = 1.f;

    D3D11_VIEWPORT viewport2;
    viewport2.TopLeftX = 0.f;
    viewport2.TopLeftY = 0.f;
    viewport2.Width = 320;
    viewport2.Height = viewport2.Width * ((_float)tInit.iScreenHeight / (_float)tInit.iScreenWidth);
    viewport2.MinDepth = 0.0f;
    viewport2.MaxDepth = 1.f;

    D3D11_VIEWPORT viewports[] = {
        viewport1,
        viewport2
    };
    
    // 뷰포트를 생성
    m_pDeviceContext->RSSetViewports(2, viewports);

    return S_OK;
}

HRESULT CGraphicDev::Resize_SwapChain(_uint iWidth, _uint iHeight)
{
    if (m_pSwapChain == nullptr)
        return E_FAIL;

    m_iScreenWidth = iWidth;
    m_iScreenHeight = iHeight;

    CleanUp_RenderTargets();
    m_pSwapChain->ResizeBuffers(0, m_iScreenWidth, m_iScreenHeight, DXGI_FORMAT_UNKNOWN, 0);
    Create_RenderTargets();
    Resize_DepthStencil();

    return S_OK;
}

void CGraphicDev::CleanUp_RenderTargets()
{
    m_vecRTV.clear();
    // PBR 렌더타깃 생성
    for (_uint i = 0; i < MaxRenderTarget; i++)
    {
        m_pTexture[i].Reset();
        m_pRTV[i].Reset();
    }
}

HRESULT CGraphicDev::Create_RenderTargets()
{
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

    m_pTexture[0] = pBackBufferTexture;
    pBackBufferTexture.Reset();
    m_pRTV[0] = pRTV;
    pRTV.Reset();

    // PBR 렌더타깃 생성
    for (_uint i = 1; i < MaxRenderTarget; i++)
    {
        if (FAILED(m_pDevice->CreateTexture2D(&textureDesc, nullptr, pTexture.GetAddressOf())))
            return E_FAIL;

        if (FAILED(m_pDevice->CreateRenderTargetView(pTexture.Get(), &rtvDesc, pRTV.GetAddressOf())))
            return E_FAIL;

        m_pTexture[i] = pTexture;
        pTexture.Reset();
        m_pRTV[i] = pRTV;
        pRTV.Reset();
    }

    return S_OK;
}

HRESULT CGraphicDev::Resize_DepthStencil()
{
    D3D11_TEXTURE2D_DESC	TextureDesc = {};
    m_pDepthStencilBuffer->GetDesc(&TextureDesc);
    m_pDepthStencilBuffer.Reset();
    TextureDesc.Width = m_iScreenWidth;
    TextureDesc.Height = m_iScreenHeight;

    // 깊이 스텐실 텍스처 생성
    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf())))
        return E_FAIL;

    /**************************
    * 깊이 스텐실 뷰 사이즈 변경
    ***************************/
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    m_pDepthStencilView->GetDesc(&depthStencilViewDesc);
    m_pDepthStencilView.Reset();
    if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, m_pDepthStencilView.GetAddressOf())))
        return E_FAIL;

    return S_OK;
}

HRESULT CGraphicDev::Regist_RenderTarget(_uint iRenderTargetIndex)
{
    if (iRenderTargetIndex < 0 || iRenderTargetIndex >= m_iNumRenderTargets)
        return E_FAIL;

    m_vecRTV.push_back(m_pRTV[iRenderTargetIndex].Get());

    return S_OK;
}

void CGraphicDev::Bind_RenderTargetsOnDevice()
{
    m_pDeviceContext->OMSetRenderTargets(Cast<_uint>(m_vecRTV.size()), m_vecRTV.data(), m_pDepthStencilView.Get());
}

void CGraphicDev::TurnOn_ZBuffer()
{
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
}

void CGraphicDev::TurnOff_ZBuffer()
{
    m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
}

void CGraphicDev::TurnOn_Cull()
{
    m_pDeviceContext->RSSetState(m_pRasterState.Get());
}

void CGraphicDev::TurnOff_Cull()
{
    m_pDeviceContext->RSSetState(m_pRasterCullNoneState.Get());
}

