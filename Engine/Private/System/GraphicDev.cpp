#include "System/GraphicDev.h"

CGraphicDev::CGraphicDev()
{
}

HRESULT CGraphicDev::Initialize(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
    _float fScreenDepth, _float fScreenNear)
{
    m_bVsync_Enabled = bVsync;

    // 그래픽 인터페이스 팩토리 생성
    IDXGIFactory* factory = nullptr;
    FAILED_CHECK_RETURN(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), E_FAIL);

    // 팩토리 객체를 사용해 첫번째 그래픽 카드 인터페이스 어댑터를 생성한다.
    IDXGIAdapter* adapter = nullptr;
    FAILED_CHECK_RETURN(factory->EnumAdapters(0, &adapter), E_FAIL);

    // 출력(모니터)에 대한 첫번째 어댑터를 지정
    IDXGIOutput* adapterOutput = nullptr;
    FAILED_CHECK_RETURN(adapter->EnumOutputs(0, &adapterOutput), E_FAIL);

    // 출력 (모니터)에 대한 DXGI_FORMAT_R8G8B8A8_UNORM 표시 형식에 맞는 모드 수를 가져옴
    _uint iNumModes = 0;
    FAILED_CHECK_RETURN(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
                        &iNumModes, NULL), E_FAIL);

    // 가능한 모든 모니터와 그래픽카드 조합을 저장할 리스트 생성
    DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[iNumModes];
    if (!displayModeList)
        return E_FAIL;

    // 디스플레이 모드에 대한 리스트를 채운다.
    FAILED_CHECK_RETURN(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, 
                        &iNumModes, displayModeList), E_FAIL);


    // 이모든 디스플레이 모드에 대해 화면 너비/높이에 맞는 디스플레이모드 찾기
    // 적합한 것을 찾으면 모니터의 새로고침 비율의 분모와 분자 값을 저장
    _uint iNumerator = 0;
    _uint iDenominator = 0;
    for (_uint i = 0; i < iNumModes; i++)
    {
        if (displayModeList[i].Width == (_uint)g_iWindowSizeX)
        {
            if (displayModeList[i].Height == (_uint)g_iWindowSizeY)
            {
                iNumerator = displayModeList[i].RefreshRate.Numerator;
                iDenominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // 비디오카드의 구조체를 얻는다.
    DXGI_ADAPTER_DESC adapterDesc;
    FAILED_CHECK_RETURN(adapter->GetDesc(&adapterDesc), E_FAIL);


    // 비디오카드 메모리 용량 단위를 메가바이트 단위로 저장
    m_iVideoCardMemory = (_int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);


    // 비디오카드의 이름을 저장
    size_t iStringLength = 0;
    if (wcstombs_s(&iStringLength, m_cVideoCardDescription, 128, adapterDesc.Description, 128) != 0)
    {
        return E_FAIL;
    }

    // 디스플레이 모드 리스트 해제
    Safe_Delete_Array(displayModeList);

    // 출력 어댑터를 해제
    Safe_Release(adapterOutput);

    // 어댑터 해제
    Safe_Release(adapter);

    // 팩토리 객체 해제
    Safe_Release(factory);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
    swapChainDesc.BufferCount = 1;          // 백버퍼 개수
    swapChainDesc.BufferDesc.Width = 0;     // 백버퍼 너비 높이
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // 32비트 서피스 설정
    // 백버퍼의 새로고침 비율 설정
    if (m_bVsync_Enabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = iNumerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = iDenominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }
    // 백버퍼의 사용용도 지정
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // 렌더링에 사용될 윈도우 핸들 지정
    swapChainDesc.OutputWindow = hWnd;

    // 멀티샘플링을 끈다
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // 전체화면
    swapChainDesc.Windowed = !bFullScreen;

    // 출력 후 백버퍼를 비우도록 지정
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // 추가 옵션 플래그 사용 X
    swapChainDesc.Flags = 0;

    // 피처레벨을 DirectX 11로 설정
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


    // 스왑 체인, Direct3D 장치 및 Direct3D 장치 컨텍스트를 만든다.
    FAILED_CHECK_RETURN(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext), E_FAIL);

    // 백버퍼 포인터를 얻어온다.
    ID3D11Texture2D* backBufferPtr = nullptr;
    FAILED_CHECK_RETURN(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr), E_FAIL);

    FAILED_CHECK_RETURN(m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView), E_FAIL);

    Safe_Release(backBufferPtr);


    // 깊이 버퍼 구조체를 초기화한다.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // 깊이 버퍼 구조체를 작성한다.
    depthBufferDesc.Width = iScreenWidth;
    depthBufferDesc.Height = iScreenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    // 설정된 깊이버퍼 구조체를 사용하여 깊이 버퍼 텍스쳐를 생성한다.
    FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDethStencilBuffer), E_FAIL);
    

    // 스텐실 상태 구조체를 초기화한다.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // 픽셀 정면 스텐실 설정
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 픽셀 후면 스텐실 설정
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // 깊이 스텐실 상태를 생성
    FAILED_CHECK_RETURN(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState), E_FAIL);

    // 깊이 스텐실 뷰의 구조체를 초기화
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // 깊이 스텐실 뷰를 생성
    FAILED_CHECK_RETURN(m_pDevice->CreateDepthStencilView(m_pDethStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView), E_FAIL);

    // 렌더링 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    //-------------------------------------

    // 그려지는 대상 뷰와 깊이 스텐실 버퍼를 출력 렌더 파이프 라인에 바인딩
    D3D11_RASTERIZER_DESC rasterDesc;
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    // 방금 작성한 구조체에서 래스터 라이저 상태를 만든다.
    FAILED_CHECK_RETURN(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState), E_FAIL);


    m_pDeviceContext->RSSetState(m_pRasterState);

    D3D11_VIEWPORT viewport;
    viewport.Width = (_float)iScreenWidth;
    viewport.Height = (_float)iScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.f;
    viewport.TopLeftX = 0.f;
    viewport.TopLeftY = 0.f;

    // 뷰포트를 생성
    m_pDeviceContext->RSSetViewports(1, &viewport);

    // 투영 행렬을 설정
    _float fFieldOfView = 3.141592654f / 4.f;
    _float fScreenAspect = (_float)iScreenWidth / (_float)iScreenHeight;

    // 3D 렌더링을 위한 투영 행렬을 만든다.
    m_matProjection = XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, fScreenNear, fScreenDepth);

    // 세계 행렬을 항등 행렬로 초기화
    m_matWorld = XMMatrixIdentity();

    // 2D 렌더링을 위한 직교 투영 행렬을 만든다.
    m_matOrtho = XMMatrixOrthographicLH((_float)iScreenWidth, (_float)iScreenHeight, fScreenNear, fScreenDepth);

	return S_OK;
}

HRESULT CGraphicDev::Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha)
{
    _float color[4] = { fRed, fGreen, fBlue, fAlpha };

    // 백버퍼 지우기
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // 깊이 버퍼 지우기
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicDev::Render_End()
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

CGraphicDev* CGraphicDev::Create(_int iScreenWidth, _int iScreenHeight, 
    _bool bVsync, HWND hWnd, _bool bFullScreen, _float fScreenDepth, _float fScreenNear)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(iScreenWidth, iScreenHeight, bVsync, hWnd, bFullScreen, fScreenDepth, fScreenNear)))
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
    Safe_Release(m_pRenderTargetView);
    Safe_Release(m_pDeviceContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pSwapChain);
}
