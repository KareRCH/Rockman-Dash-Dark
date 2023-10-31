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

CColorShaderComp* CColorShaderComp::Create()
{


    return nullptr;
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

        }
        else
        {
            MessageBox(hWnd, vsFileName, L"파일 없음", MB_OK);
        }

        return E_FAIL;
    }

    // 버퍼로부터 정점 셰이더를 생성
    FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexShader(pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(),
        NULL, &m_pVertexShader), E_FAIL);

    // 버퍼로부터 픽셀 셰이더를 생성
    FAILED_CHECK_RETURN(m_pGraphicDev->CreatePixelShader(pPixelShaderBuf->GetBufferPointer(), pPixelShaderBuf->GetBufferSize(),
        NULL, &m_pPixelShader), E_FAIL);

    return S_OK;
}

HRESULT CColorShaderComp::Set_ShaderParameter(ID3D11DeviceContext* pDeviceContext, XMMATRIX matWorld, XMMATRIX matView, XMMATRIX matProj)
{


    return S_OK;
}

void CColorShaderComp::Render_Shader(ID3D11DeviceContext* pDeviceContext, _int iIndexCount)
{
    pDeviceContext->IASetInputLayout(m_pLayout);

    pDeviceContext->VSSetShader(m_pVertexShader, NULL, 0);
    pDeviceContext->PSSetShader(m_pPixelShader, NULL, 0);

    pDeviceContext->DrawIndexed(iIndexCount, 0, 0);
}
