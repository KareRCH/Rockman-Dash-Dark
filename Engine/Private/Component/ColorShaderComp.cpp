#include "Component/ColorShaderComp.h"

CColorShaderComp::CColorShaderComp(const CColorShaderComp& rhs)
    : Base(rhs)
{
}

HRESULT CColorShaderComp::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CColorShaderComp::Initialize(void* Arg)
{
    return S_OK;
}

HRESULT CColorShaderComp::Initialize(HWND hWnd)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    FAILED_CHECK_RETURN(Initialize_Shader(hWnd, L"./Resource/Shader/VS_Test1.hlsl", L"./Resource/Shader/PS_Test1.hlsl"), E_FAIL);

    return S_OK;
}

void CColorShaderComp::Render(const _matrix& matWorld, const _matrix& matView, const _matrix& matProj)
{
    if (Set_ShaderParameter(matWorld, matView, matProj) == E_FAIL)
    {
        return;
    }

    Render_Shader(m_iIndexCount);
}

CColorShaderComp* CColorShaderComp::Create(HWND hWnd)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        MSG_BOX("ColorShaderComp Create Failed");
        Safe_Release(pInstance);
        
        return nullptr;
    }

    return pInstance;
}

CComponent* CColorShaderComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("ColorShaderComp Copy Failed");
        Safe_Release(pInstance);

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CColorShaderComp::Free()
{
    SUPER::Free();

}

HRESULT CColorShaderComp::Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName)
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
    FAILED_CHECK_RETURN(D3D11Device()->CreateVertexShader(pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(),
        NULL, &m_pVertexShader), E_FAIL);

    // ���۷κ��� �ȼ� ���̴��� ����
    FAILED_CHECK_RETURN(D3D11Device()->CreatePixelShader(pPixelShaderBuf->GetBufferPointer(), pPixelShaderBuf->GetBufferSize(),
        NULL, &m_pPixelShader), E_FAIL);

    D3D11_INPUT_ELEMENT_DESC tPolygonLayout[2];
    tPolygonLayout[0].SemanticName = "POSITION";
    tPolygonLayout[0].SemanticIndex = 0;
    tPolygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    tPolygonLayout[0].InputSlot = 0;
    tPolygonLayout[0].AlignedByteOffset = 0;
    tPolygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[0].InstanceDataStepRate = 0;


    tPolygonLayout[1].SemanticName = "COLOR";
    tPolygonLayout[1].SemanticIndex = 0;
    tPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    tPolygonLayout[1].InputSlot = 0;
    tPolygonLayout[1].AlignedByteOffset = 12;
    tPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[1].InstanceDataStepRate = 0;

    _uint iNumElements = sizeof(tPolygonLayout) / sizeof(tPolygonLayout[0]);

    FAILED_CHECK_RETURN(D3D11Device()->CreateInputLayout(tPolygonLayout, iNumElements,
        pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(), &m_pLayout), E_FAIL);

    Safe_Release(pVertexShaderBuf);
    Safe_Release(pPixelShaderBuf);

    D3D11_BUFFER_DESC tMatBufferDesc;
    tMatBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    tMatBufferDesc.ByteWidth = sizeof(MATRIX_BUFFER_T);
    tMatBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    tMatBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    tMatBufferDesc.MiscFlags = 0;
    tMatBufferDesc.StructureByteStride = 0;

    FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&tMatBufferDesc, NULL, &m_pMatrixBuffer), E_FAIL);

    return S_OK;
}

HRESULT CColorShaderComp::Set_ShaderParameter(XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj)
{
    // ��ġ ��ķ� �ٲپ��־����, �ٷ� row�켱, HLSL�� col�켱 �����̶� �׷��ٰ� �Ѵ�.
    matWorld = XMMatrixTranspose(matWorld);
    matView = XMMatrixTranspose(matView);
    matProj = XMMatrixTranspose(matProj);

    // ��� ������ ������ �� �� �ֵ��� ���
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    FAILED_CHECK_RETURN(D3D11Context()->Map(m_pMatrixBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);

    // ��� ������ �����Ϳ� ���� �����͸� ������
    MATRIX_BUFFER_T* pDataPtr = Cast<MATRIX_BUFFER_T*>(mappedResource.pData);

    // ��� ���ۿ� ��� ����
    pDataPtr->matWorld = matWorld;
    pDataPtr->matView = matView;
    pDataPtr->matProj = matProj;

    // ��� ������ ��� Ǯ��
    D3D11Context()->Unmap(m_pMatrixBuffer.Get(), 0);

    // ���� ���̴������� ��� ������ ��ġ�� ����
    _uint iBufferNumber = 0;

    D3D11Context()->VSSetConstantBuffers(iBufferNumber, 1, &m_pMatrixBuffer);

    return S_OK;
}

void CColorShaderComp::Render_Shader(_int iIndexCount)
{
    D3D11Context()->IASetInputLayout(m_pLayout.Get());

    D3D11Context()->VSSetShader(m_pVertexShader.Get(), NULL, 0);
    D3D11Context()->PSSetShader(m_pPixelShader.Get(), NULL, 0);

    D3D11Context()->DrawIndexed(iIndexCount, 0, 0);
}