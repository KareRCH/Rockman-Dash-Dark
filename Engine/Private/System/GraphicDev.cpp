#include "System/GraphicDev.h"

CGraphicDev::CGraphicDev()
{
}

HRESULT CGraphicDev::Initialize(_int iScreenWidth, _int iScreenHeight, _bool bVsync, HWND hWnd, _bool bFullScreen,
    _float fScreenDepth, _float fScreenNear)
{
    m_bVsync_Enabled = bVsync;

    // �׷��� �������̽� ���丮 ����
    IDXGIFactory* factory = nullptr;
    FAILED_CHECK_RETURN(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory), E_FAIL);

    // ���丮 ��ü�� ����� ù��° �׷��� ī�� �������̽� ����͸� �����Ѵ�.
    IDXGIAdapter* adapter = nullptr;
    FAILED_CHECK_RETURN(factory->EnumAdapters(0, &adapter), E_FAIL);

    // ���(�����)�� ���� ù��° ����͸� ����
    IDXGIOutput* adapterOutput = nullptr;
    FAILED_CHECK_RETURN(adapter->EnumOutputs(0, &adapterOutput), E_FAIL);

    // ��� (�����)�� ���� DXGI_FORMAT_R8G8B8A8_UNORM ǥ�� ���Ŀ� �´� ��� ���� ������
    _uint iNumModes = 0;
    FAILED_CHECK_RETURN(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED,
                        &iNumModes, NULL), E_FAIL);

    // ������ ��� ����Ϳ� �׷���ī�� ������ ������ ����Ʈ ����
    DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[iNumModes];
    if (!displayModeList)
        return E_FAIL;

    // ���÷��� ��忡 ���� ����Ʈ�� ä���.
    FAILED_CHECK_RETURN(adapterOutput->GetDisplayModeList(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, 
                        &iNumModes, displayModeList), E_FAIL);


    // �̸�� ���÷��� ��忡 ���� ȭ�� �ʺ�/���̿� �´� ���÷��̸�� ã��
    // ������ ���� ã���� ������� ���ΰ�ħ ������ �и�� ���� ���� ����
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

    // ����ī���� ����ü�� ��´�.
    DXGI_ADAPTER_DESC adapterDesc;
    FAILED_CHECK_RETURN(adapter->GetDesc(&adapterDesc), E_FAIL);


    // ����ī�� �޸� �뷮 ������ �ް�����Ʈ ������ ����
    m_iVideoCardMemory = (_int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);


    // ����ī���� �̸��� ����
    size_t iStringLength = 0;
    if (wcstombs_s(&iStringLength, m_cVideoCardDescription, 128, adapterDesc.Description, 128) != 0)
    {
        return E_FAIL;
    }

    // ���÷��� ��� ����Ʈ ����
    Safe_Delete_Array(displayModeList);

    // ��� ����͸� ����
    Safe_Release(adapterOutput);

    // ����� ����
    Safe_Release(adapter);

    // ���丮 ��ü ����
    Safe_Release(factory);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
    swapChainDesc.BufferCount = 1;          // ����� ����
    swapChainDesc.BufferDesc.Width = 0;     // ����� �ʺ� ����
    swapChainDesc.BufferDesc.Height = 0;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;   // 32��Ʈ ���ǽ� ����
    // ������� ���ΰ�ħ ���� ����
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
    // ������� ���뵵 ����
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    // �������� ���� ������ �ڵ� ����
    swapChainDesc.OutputWindow = hWnd;

    // ��Ƽ���ø��� ����
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    // ��üȭ��
    swapChainDesc.Windowed = !bFullScreen;

    // ��� �� ����۸� ��쵵�� ����
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // �߰� �ɼ� �÷��� ��� X
    swapChainDesc.Flags = 0;

    // ��ó������ DirectX 11�� ����
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;


    // ���� ü��, Direct3D ��ġ �� Direct3D ��ġ ���ؽ�Ʈ�� �����.
    FAILED_CHECK_RETURN(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
        D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pDevice, NULL, &m_pDeviceContext), E_FAIL);

    // ����� �����͸� ���´�.
    ID3D11Texture2D* backBufferPtr = nullptr;
    FAILED_CHECK_RETURN(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr), E_FAIL);

    FAILED_CHECK_RETURN(m_pDevice->CreateRenderTargetView(backBufferPtr, NULL, &m_pRenderTargetView), E_FAIL);

    Safe_Release(backBufferPtr);


    // ���� ���� ����ü�� �ʱ�ȭ�Ѵ�.
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    // ���� ���� ����ü�� �ۼ��Ѵ�.
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

    // ������ ���̹��� ����ü�� ����Ͽ� ���� ���� �ؽ��ĸ� �����Ѵ�.
    FAILED_CHECK_RETURN(m_pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pDethStencilBuffer), E_FAIL);
    

    // ���ٽ� ���� ����ü�� �ʱ�ȭ�Ѵ�.
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    // �ȼ� ���� ���ٽ� ����
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // �ȼ� �ĸ� ���ٽ� ����
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // ���� ���ٽ� ���¸� ����
    FAILED_CHECK_RETURN(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState), E_FAIL);

    // ���� ���ٽ� ���� ����ü�� �ʱ�ȭ
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    // ���� ���ٽ� �並 ����
    FAILED_CHECK_RETURN(m_pDevice->CreateDepthStencilView(m_pDethStencilBuffer, &depthStencilViewDesc, &m_pDepthStencilView), E_FAIL);

    // ������ ��� ��� ���� ���ٽ� ���۸� ��� ���� ������ ���ο� ���ε�
    m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

    //-------------------------------------

    // �׷����� ��� ��� ���� ���ٽ� ���۸� ��� ���� ������ ���ο� ���ε�
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

    // ��� �ۼ��� ����ü���� ������ ������ ���¸� �����.
    FAILED_CHECK_RETURN(m_pDevice->CreateRasterizerState(&rasterDesc, &m_pRasterState), E_FAIL);


    m_pDeviceContext->RSSetState(m_pRasterState);

    D3D11_VIEWPORT viewport;
    viewport.Width = (_float)iScreenWidth;
    viewport.Height = (_float)iScreenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.f;
    viewport.TopLeftX = 0.f;
    viewport.TopLeftY = 0.f;

    // ����Ʈ�� ����
    m_pDeviceContext->RSSetViewports(1, &viewport);

    // ���� ����� ����
    _float fFieldOfView = 3.141592654f / 4.f;
    _float fScreenAspect = (_float)iScreenWidth / (_float)iScreenHeight;

    // 3D �������� ���� ���� ����� �����.
    m_matProjection = XMMatrixPerspectiveFovLH(fFieldOfView, fScreenAspect, fScreenNear, fScreenDepth);

    // ���� ����� �׵� ��ķ� �ʱ�ȭ
    m_matWorld = XMMatrixIdentity();

    // 2D �������� ���� ���� ���� ����� �����.
    m_matOrtho = XMMatrixOrthographicLH((_float)iScreenWidth, (_float)iScreenHeight, fScreenNear, fScreenDepth);

	return S_OK;
}

HRESULT CGraphicDev::Render_Begin(_float fRed, _float fGreen, _float fBlue, _float fAlpha)
{
    _float color[4] = { fRed, fGreen, fBlue, fAlpha };

    // ����� �����
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, color);

    // ���� ���� �����
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

	return S_OK;
}

HRESULT CGraphicDev::Render_End()
{
    // ������ �Ϸ�Ǿ� ȭ�鿡 �� ���۸� ǥ��
    if (m_bVsync_Enabled)
    {
        // ȭ�� ���� ��ħ ���� ����
        m_pSwapChain->Present(1, 0);
    }
    else
    {
        // ������ ������ ���
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
