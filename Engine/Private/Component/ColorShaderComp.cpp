#include "Component/ColorShaderComp.h"

CColorShaderComp::CColorShaderComp(ID3D11Device* pGraphicDev)
    : Base(pGraphicDev)
{
    
}

CColorShaderComp::CColorShaderComp(const CColorShaderComp& rhs)
    : Base(rhs)
{
}

HRESULT CColorShaderComp::Initialize()
{
    return S_OK;
}

HRESULT CColorShaderComp::Initialize(HWND hWnd)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    Initialize_Shader(hWnd, L"VS_Test.vs", L"PS_Test.hlsl");

    return S_OK;
}

_int CColorShaderComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CColorShaderComp::Render(ID3D11DeviceContext* pDeviceContext)
{
    _matrix world, view, proj;
    proj = view = world = XMMatrixIdentity();

    if (Set_ShaderParameter(pDeviceContext, world, view, proj) == E_FAIL)
    {
        return;
    }

    Render_Shader(pDeviceContext, m_iIndexCount);
}

CColorShaderComp* CColorShaderComp::Create(ID3D11Device* pGraphicDev)
{
    ThisClass* pInstance = new ThisClass(pGraphicDev);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ColoShaderComp Create Failed");
        
        return nullptr;
    }

    return pInstance;
}

CPrimitiveComponent* CColorShaderComp::Clone()
{
    return new ThisClass(*this);
}

void CColorShaderComp::Free()
{
    SUPER::Free();
}

HRESULT CColorShaderComp::Initialize_Shader(HWND hWnd, const _tchar* vsFileName, const _tchar* psFileName)
{
    ID3D10Blob* errorMessage = nullptr;

    // 정점 셰이더 컴파일
    ID3D10Blob* pVertexShaderBuf = nullptr;
    if (FAILED(D3DCompileFromFile(vsFileName, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &pVertexShaderBuf, &errorMessage)))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, vsFileName);
        }
        else
        {
            MessageBox(hWnd, vsFileName, L"파일 없음", MB_OK);
        }

        return E_FAIL;
    }

    // 픽셀 셰이더 컴파일
    ID3D10Blob* pPixelShaderBuf = nullptr;
    if (FAILED(D3DCompileFromFile(psFileName, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &pPixelShaderBuf, &errorMessage)))
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hWnd, psFileName);
        }
        else
        {
            MessageBox(hWnd, vsFileName, L"파일 없음", MB_OK);
        }

        return E_FAIL;
    }

    // 버퍼로부터 정점 셰이더를 생성
    FAILED_CHECK_RETURN(m_pDevice->CreateVertexShader(pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(),
        NULL, &m_pVertexShader), E_FAIL);

    // 버퍼로부터 픽셀 셰이더를 생성
    FAILED_CHECK_RETURN(m_pDevice->CreatePixelShader(pPixelShaderBuf->GetBufferPointer(), pPixelShaderBuf->GetBufferSize(),
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
    tPolygonLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    tPolygonLayout[1].InputSlot = 0;
    tPolygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    tPolygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    tPolygonLayout[1].InstanceDataStepRate = 0;

    _uint iNumElements = sizeof(tPolygonLayout) / sizeof(tPolygonLayout[0]);

    if (FAILED(m_pDevice->CreateInputLayout(tPolygonLayout, iNumElements,
        pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(), &m_pLayout)))
    {
        return E_FAIL;
    }

    Safe_Release(pVertexShaderBuf);
    Safe_Release(pPixelShaderBuf);

    D3D11_BUFFER_DESC tMatBufferDesc;
    tMatBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    tMatBufferDesc.ByteWidth = sizeof(MATRIX_BUFFER_T);
    tMatBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    tMatBufferDesc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    tMatBufferDesc.MiscFlags = 0;
    tMatBufferDesc.StructureByteStride = 0;

    if (FAILED(m_pDevice->CreateBuffer(&tMatBufferDesc, NULL, &m_pMatrixBuffer)))
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CColorShaderComp::Set_ShaderParameter(ID3D11DeviceContext* pDeviceContext, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj)
{
    // 전치 행렬로 바꾸어주어야함, 다렉 row우선, HLSL은 col우선 연산이라 그렇다고 한다.
    matWorld = XMMatrixTranspose(matWorld);
    matView = XMMatrixTranspose(matView);
    matProj = XMMatrixTranspose(matProj);

    // 상수 버퍼의 내용을 쓸 수 있도록 잠금
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (FAILED(pDeviceContext->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        return E_FAIL;
    }

    // 상수 버퍼의 데이터에 대한 포인터를 가져옴
    MATRIX_BUFFER_T* pDataPtr = Cast<MATRIX_BUFFER_T*>(mappedResource.pData);

    // 상수 버퍼에 행렬 복사
    pDataPtr->matWorld = matWorld;
    pDataPtr->matView = matView;
    pDataPtr->matProj = matProj;

    // 상수 버퍼의 잠금 풀기
    pDeviceContext->Unmap(m_pMatrixBuffer, 0);

    // 정점 셰이더에서의 상수 버퍼의 위치를 설정
    _uint iBufferNumber = 0;

    pDeviceContext->VSSetConstantBuffers(iBufferNumber, 1, &m_pMatrixBuffer);

    return S_OK;
}

void CColorShaderComp::Render_Shader(ID3D11DeviceContext* pDeviceContext, _int iIndexCount)
{
    pDeviceContext->IASetInputLayout(m_pLayout);

    pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
    pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

    pDeviceContext->DrawIndexed(iIndexCount, 0, 0);
}
