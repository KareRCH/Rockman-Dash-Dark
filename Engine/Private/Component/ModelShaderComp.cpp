#include "Component/ModelShaderComp.h"

CModelShaderComp::CModelShaderComp(const DX11DEVICE_T tDevice)
    : Base(tDevice)
{
}

CModelShaderComp::CModelShaderComp(const CModelShaderComp& rhs)
    : Base(rhs)
{
}

HRESULT CModelShaderComp::Initialize()
{
    return S_OK;
}

HRESULT CModelShaderComp::Initialize(HWND hWnd)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    FAILED_CHECK_RETURN(Initialize_Shader(hWnd, L"./Resource/Shader/ModelTest.vs", L"./Resource/Shader/ModelTest.ps"), E_FAIL);

    return S_OK;
}

void CModelShaderComp::PriorityTick()
{
}

_int CModelShaderComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CModelShaderComp::Render(const MATRIX_BUFFER_T& tMatrixBuf, const LIGHT_BUFFER_T& tLightBuf)
{
    if (Set_ShaderParameter(tMatrixBuf, tLightBuf) == E_FAIL)
    {
        return;
    }

    Render_Shader(m_iIndexCount);
}

CModelShaderComp* CModelShaderComp::Create(const DX11DEVICE_T tDevice, HWND hWnd)
{
    ThisClass* pInstance = new ThisClass(tDevice);

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ModelShader Create Failed");

        return nullptr;
    }

    return pInstance;
}

CPrimitiveComponent* CModelShaderComp::Clone(void* Arg)
{
    return new ThisClass(*this);
}

void CModelShaderComp::Free()
{
    SUPER::Free();

    Safe_Release(m_pSamplereState);
    Safe_Release(m_pTexture);
}

HRESULT CModelShaderComp::Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName)
{
    ID3DBlob* errorMessage = nullptr;

    // ���� ���̴� ������
    ID3DBlob* pVertexShaderBuf = nullptr;
    if (FAILED(D3DCompileFromFile(vsFileName, NULL, NULL, "main", "vs_5_0", 0, 0,
        &pVertexShaderBuf, &errorMessage)))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, vsFileName);
        }
        else
        {
            MessageBox(hWnd, vsFileName, L"���� ����", MB_OK);
        }

        return E_FAIL;
    }

    // �ȼ� ���̴� ������
    ID3DBlob* pPixelShaderBuf = nullptr;
    if (FAILED(D3DCompileFromFile(psFileName, NULL, NULL, "main", "ps_5_0", 0, 0,
        &pPixelShaderBuf, &errorMessage)))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, psFileName);
        }
        else
        {
            MessageBox(hWnd, psFileName, L"���� ����", MB_OK);
        }

        return E_FAIL;
    }

    // ���۷κ��� ���� ���̴��� ����
    FAILED_CHECK_RETURN(m_pDevice->CreateVertexShader(pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(),
        NULL, &m_pVertexShader), E_FAIL);

    // ���۷κ��� �ȼ� ���̴��� ����
    FAILED_CHECK_RETURN(m_pDevice->CreatePixelShader(pPixelShaderBuf->GetBufferPointer(), pPixelShaderBuf->GetBufferSize(),
        NULL, &m_pPixelShader), E_FAIL);

    D3D11_INPUT_ELEMENT_DESC tPolygonLayout[3];
    tPolygonLayout[0].SemanticName = "POSITION";
    tPolygonLayout[0].SemanticIndex = 0;
    tPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    tPolygonLayout[0].InputSlot = 0;
    tPolygonLayout[0].AlignedByteOffset = 0;
    tPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[0].InstanceDataStepRate = 0;


    tPolygonLayout[1].SemanticName = "NORMAL";
    tPolygonLayout[1].SemanticIndex = 0;
    tPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    tPolygonLayout[1].InputSlot = 0;
    tPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    tPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[1].InstanceDataStepRate = 0;

    tPolygonLayout[2].SemanticName = "TEXCOORD";
    tPolygonLayout[2].SemanticIndex = 0;
    tPolygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    tPolygonLayout[2].InputSlot = 0;
    tPolygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    tPolygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[2].InstanceDataStepRate = 0;

    _uint iNumElements = sizeof(tPolygonLayout) / sizeof(tPolygonLayout[0]);

    FAILED_CHECK_RETURN(m_pDevice->CreateInputLayout(tPolygonLayout, iNumElements,
        pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(), &m_pLayout), E_FAIL);

    Safe_Release(pVertexShaderBuf);
    Safe_Release(pPixelShaderBuf);
    

    // �ؽ�ó ���÷� ���� ����ü ����
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    FAILED_CHECK_RETURN(m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplereState), E_FAIL);


    // ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ�
    D3D11_BUFFER_DESC tMatBufferDesc;
    tMatBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    tMatBufferDesc.ByteWidth = sizeof(MATRIX_BUFFER_T);
    tMatBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    tMatBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    tMatBufferDesc.MiscFlags = 0;
    tMatBufferDesc.StructureByteStride = 0;

    FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&tMatBufferDesc, NULL, &m_pMatrixBuffer), E_FAIL);


    // �ȼ� ���̴��� �ִ� ���� ���� ��� ������ ������ ����
    // D3D11_BIND_CONSTANT_BUFFER�� ���� ByteWidth�� 16�� ���������. �ƴҽ� ���� ����
    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LIGHT_BUFFER_T);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    FAILED_CHECK_RETURN(m_pDevice->CreateBuffer(&lightBufferDesc, NULL, &m_pLightBuffer), E_FAIL);

    return S_OK;
}

HRESULT CModelShaderComp::Set_ShaderParameter(MATRIX_BUFFER_T tMatrixBuf, LIGHT_BUFFER_T tLightBuf)
{
    // ��ġ ��ķ� �ٲپ��־����, �ٷ� row�켱, HLSL�� col�켱 �����̶� �׷��ٰ� �Ѵ�.
    tMatrixBuf.matWorld = XMMatrixTranspose(tMatrixBuf.matWorld);
    tMatrixBuf.matView = XMMatrixTranspose(tMatrixBuf.matView);
    tMatrixBuf.matProj = XMMatrixTranspose(tMatrixBuf.matProj);

    // ��� ������ ������ �� �� �ֵ��� ���
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    FAILED_CHECK_RETURN(m_pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);

    // ��� ������ �����Ϳ� ���� �����͸� ������
    MATRIX_BUFFER_T* pDataPtr = Cast<MATRIX_BUFFER_T*>(mappedResource.pData);

    // ��� ���ۿ� ��� ����
    pDataPtr->matWorld = tMatrixBuf.matWorld;
    pDataPtr->matView = tMatrixBuf.matView;
    pDataPtr->matProj = tMatrixBuf.matProj;

    // ��� ������ ��� Ǯ��
    m_pDeviceContext->Unmap(m_pMatrixBuffer, 0);

    // ���� ���̴������� ��� ������ ��ġ�� ����
    _uint iBufferNumber = 0;

    m_pDeviceContext->VSSetConstantBuffers(iBufferNumber, 1, &m_pMatrixBuffer);

    m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

    //----------------------------------

    // lightBuffer ���� ���
    FAILED_CHECK_RETURN(m_pDeviceContext->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);

    LIGHT_BUFFER_T* dataPtr2 = Cast<LIGHT_BUFFER_T*>(mappedResource.pData);

    dataPtr2->vDiffuseColor = tLightBuf.vDiffuseColor;
    dataPtr2->vLightDirection = tLightBuf.vLightDirection;
    dataPtr2->fPadding = 0.0f;

    m_pDeviceContext->Unmap(m_pLightBuffer, 0);

    iBufferNumber = 0;

    // �ȼ� ���̴����� ���̴� �ؽ�ó ���ҽ� ����
    m_pDeviceContext->PSSetConstantBuffers(iBufferNumber, 1, &m_pLightBuffer);

    return S_OK;
}

void CModelShaderComp::Render_Shader(_int iIndexCount)
{
    m_pDeviceContext->IASetInputLayout(m_pLayout);

    m_pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

    m_pDeviceContext->PSSetSamplers(0, 1, &m_pSamplereState);

    m_pDeviceContext->DrawIndexed(iIndexCount, 0, 0);
}
