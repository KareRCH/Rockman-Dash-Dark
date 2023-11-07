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
    ID3D11RenderTargetView* pRTVs[1] = {
        m_pRenderTargetView,

    };

    m_pDeviceContext->OMSetRenderTargets(1, pRTVs,
        m_pDepthStencilView);

    D3D11_VIEWPORT			ViewPortDesc;
    ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
    ViewPortDesc.TopLeftX = 0;
    ViewPortDesc.TopLeftY = 0;
    ViewPortDesc.Width = (_float)tInit.iScreenWidth;
    ViewPortDesc.Height = (_float)tInit.iScreenHeight;
    ViewPortDesc.MinDepth = 0.f;
    ViewPortDesc.MaxDepth = 1.f;

    m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CGraphicDev::Clear_BackBuffer_View(_float4 vClearColor)
{
    _float color[4] = { vClearColor.x, vClearColor.y, vClearColor.z, vClearColor.w };

    // 백버퍼 지우기
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

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
#ifdef _DEBUG
    //m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

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

    if (m_pSwapChain)
    {
        m_pSwapChain->SetFullscreenState(false, NULL);
    }
    Safe_Release(m_pRasterState);
    Safe_Release(m_pDepthStencilView);
    Safe_Release(m_pDepthStencilState);
    Safe_Release(m_pDethStencilBuffer);
    Safe_Release(m_pRenderTargetView);
    Perfect_Release(m_pSwapChain);
    Perfect_Release(m_pDeviceContext);
    Perfect_Release(m_pDevice);

}

HRESULT CGraphicDev::Ready_SwapChain(const FDEVICE_INIT& tInit)
{
#ifdef _DEBUG
    m_pDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&m_pDebug);

    //m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
#endif
    IDXGIDevice* pDevice = nullptr;
    m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

    _uint adapterIndex = 0;

    IDXGIAdapter* pAdapter = nullptr;
    pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

    IDXGIFactory* pFactory = nullptr;
    pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

    pFactory->EnumAdapters(adapterIndex, &pAdapter);

    /*IDXGIDebug* pGxDebug = nullptr;
    pDevice->QueryInterface(__uuidof(IDXGIDebug), (void**)&pGxDebug);

    pGxDebug->ReportLiveObjects()*/
        

    /* 스왑체인을 생성한다. = 텍스쳐를 생성하는 행위 + 스왑하는 형태  */
    DXGI_SWAP_CHAIN_DESC		SwapChain;
    ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

    /*텍스쳐(백버퍼)를 생성하는 행위*/
    SwapChain.BufferDesc.Width = tInit.iScreenWidth;
    SwapChain.BufferDesc.Height = tInit.iScreenHeight;


    SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChain.BufferCount = 1;

    /*스왑하는 형태*/
    SwapChain.BufferDesc.RefreshRate.Numerator = 60;
    SwapChain.BufferDesc.RefreshRate.Denominator = 1;
    SwapChain.SampleDesc.Quality = 0;
    SwapChain.SampleDesc.Count = 1;

    SwapChain.OutputWindow = tInit.hWnd;
    SwapChain.Windowed = !tInit.bFullScreen;
    SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    /* 백버퍼라는 텍스쳐를 생성했다. */
    if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, &m_pSwapChain)))
        return E_FAIL;

    Safe_Release(pFactory);
    Safe_Release(pAdapter);
    Safe_Release(pDevice);

    

    return S_OK;
}

HRESULT CGraphicDev::Ready_BackBufferRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    /* 내가 앞으로 사용하기위한 용도의 텍스쳐를 생성하기위한 베이스 데이터를 가지고 있는 객체이다. */
    /* 내가 앞으로 사용하기위한 용도의 텍스쳐 : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
    ID3D11Texture2D* pBackBufferTexture = nullptr;

    /* 스왑체인이 들고있던 텍스처를 가져와봐. */
    if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
        return E_FAIL;

    if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pRenderTargetView)))
        return E_FAIL;

    Safe_Release(pBackBufferTexture);

    return S_OK;
}

HRESULT CGraphicDev::Ready_DepthStencilRenderTargetView(const FDEVICE_INIT& tInit)
{
    if (nullptr == m_pDevice)
        return E_FAIL;

    ID3D11Texture2D* pDepthStencilTexture = nullptr;

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

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr,
        &pDepthStencilTexture)))
        return E_FAIL;

    /* RenderTarget */
    /* ShaderResource */
    /* DepthStencil */

    if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr,
        &m_pDepthStencilView)))
        return E_FAIL;

    Safe_Release(pDepthStencilTexture);

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
