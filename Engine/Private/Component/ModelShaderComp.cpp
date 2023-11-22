#include "Component/ModelShaderComp.h"

#include "System/GameInstance.h"

CModelShaderComp::CModelShaderComp(const CModelShaderComp& rhs)
    : Base(rhs)
{
}

HRESULT CModelShaderComp::Initialize(void* Arg)
{
    return S_OK;
}

HRESULT CModelShaderComp::Initialize(HWND hWnd)
{
    FAILED_CHECK_RETURN(Initialize(), E_FAIL);

    FAILED_CHECK_RETURN(Initialize_Shader(hWnd, L"VS_ModelTest", L"PS_ModelTest"), E_FAIL);

    return S_OK;
}

void CModelShaderComp::Priority_Tick(const _float& fTimeDelta)
{
}

_int CModelShaderComp::Tick(const _float& fTimeDelta)
{
    return 0;
}

void CModelShaderComp::Render(const MATRIX_BUFFER_T& tMatrixBuf, const CAMERA_BUFFER_T tCameraBuf, 
    const BONE_COMMON_BUFFER_T tBoneBuf, const LIGHT_BUFFER_T& tLightBuf)
{
    if (Set_ShaderParameter(tMatrixBuf, tCameraBuf, tBoneBuf, tLightBuf) == E_FAIL)
    {
        return;
    }

    Render_Shader(m_iIndexCount);
}

CModelShaderComp* CModelShaderComp::Create(HWND hWnd)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ModelShaderComp Create Failed");

        return nullptr;
    }

    return pInstance;
}

CComponent* CModelShaderComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        Engine::Safe_Release(pInstance);

        MSG_BOX("ModelShaderComp Copy Failed");

        return nullptr;
    }

    return Cast<CComponent*>(pInstance);
}

void CModelShaderComp::Free()
{
    SUPER::Free();

    
}

HRESULT CModelShaderComp::Initialize_Shader(HWND hWnd, const wstring& strVertexShaderKey, const wstring& strPixelShaderKey)
{
    // 셰이더 매니저로 부터 정점 셰이더 얻어오기
    ComPtr<ID3DBlob> pVertexShaderBuf = GameInstance()->Get_ShaderByte(EShaderType::Vertex, strVertexShaderKey);
    m_pVertexShader = GameInstance()->Get_ShaderBuffer<EShaderType::Vertex>(strVertexShaderKey);
    if (nullptr == m_pVertexShader || nullptr == pVertexShaderBuf)
    {
        MessageBox(hWnd, strVertexShaderKey.c_str(), L"셰이더 없음", MB_OK);
        return E_FAIL;
    }


    // 셰이더 매니저로 부터 픽셀 셰이더 얻어오기
    ComPtr<ID3DBlob> pPixelShaderBuf = GameInstance()->Get_ShaderByte(EShaderType::Pixel, strPixelShaderKey);
    m_pPixelShader = GameInstance()->Get_ShaderBuffer<EShaderType::Pixel>(strPixelShaderKey);
    if (nullptr == m_pPixelShader || nullptr == pPixelShaderBuf)
    {
        MessageBox(hWnd, strPixelShaderKey.c_str(), L"셰이더 없음", MB_OK);
        return E_FAIL;
    }


    /***********************************
    * 정점 레이아웃 구성 (시맨틱 전달자)
    ************************************/
    FAILED_CHECK_RETURN(D3D11Device()->CreateInputLayout(VERTEX_MODEL_SKIN_T::InputLayout, VERTEX_MODEL_SKIN_T::iMaxIndex,
        pVertexShaderBuf->GetBufferPointer(), pVertexShaderBuf->GetBufferSize(), &m_pLayout), E_FAIL);
    

    /***************
    * 텍스처 샘플러
    ***************/
    FAILED_CHECK_RETURN(D3D11Device()->CreateSamplerState(&SAMPLER_COMMON_DESC::Desc, &m_pSamplereState), E_FAIL);


    /***********
    * 행렬 버퍼
    ************/
    // 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성
    FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&MATRIX_BUFFER_T::BufferDesc, NULL, &m_pMatrixBuffer), E_FAIL);


    /*************
    * 카메라 버퍼
    **************/
    FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&CAMERA_BUFFER_T::BufferDesc, NULL, &m_pCameraBuffer), E_FAIL);


    /*********
    * 빛 버퍼
    ***********/
    // 픽셀 쎄이더에 있는 광원 동적 상수 버퍼의 설명을 설정
    // D3D11_BIND_CONSTANT_BUFFER를 사용시 ByteWidth가 16의 배수여야함. 아닐시 생성 실패
    FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&LIGHT_BUFFER_T::BufferDesc, NULL, &m_pLightBuffer), E_FAIL);


    /*********
    * 뼈 버퍼
    ***********/
    // 픽셀 쎄이더에 있는 광원 동적 상수 버퍼의 설명을 설정
    // D3D11_BIND_CONSTANT_BUFFER를 사용시 ByteWidth가 16의 배수여야함. 아닐시 생성 실패
    FAILED_CHECK_RETURN(D3D11Device()->CreateBuffer(&BONE_COMMON_BUFFER_T::BufferDesc, NULL, &m_pBoneBuffer), E_FAIL);


    return S_OK;
}

HRESULT CModelShaderComp::Set_ShaderParameter(MATRIX_BUFFER_T tMatrixBuf, CAMERA_BUFFER_T tCameraBuf, 
    BONE_COMMON_BUFFER_T tBoneBuf, LIGHT_BUFFER_T tLightBuf)
{
    // 전치 행렬로 바꾸어주어야함, 다렉 row우선, HLSL은 col우선 연산이라 그렇다고 한다.
    tMatrixBuf.matWorld = XMMatrixTranspose(tMatrixBuf.matWorld);
    tMatrixBuf.matView = XMMatrixTranspose(tMatrixBuf.matView);
    tMatrixBuf.matProj = XMMatrixTranspose(tMatrixBuf.matProj);

    // 상수 버퍼의 내용을 쓸 수 있도록 잠금
    D3D11_MAPPED_SUBRESOURCE mappedResource;


    /***********
    * 행렬 버퍼
    ************/
    FAILED_CHECK_RETURN(D3D11Context()->Map(m_pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);

    // 상수 버퍼의 데이터에 대한 포인터를 가져옴
    MATRIX_BUFFER_T* pDataPtr = Cast<MATRIX_BUFFER_T*>(mappedResource.pData);

    // 상수 버퍼에 행렬 복사
    pDataPtr->matWorld = tMatrixBuf.matWorld;
    pDataPtr->matView = tMatrixBuf.matView;
    pDataPtr->matProj = tMatrixBuf.matProj;

    // 상수 버퍼의 잠금 풀기
    D3D11Context()->Unmap(m_pMatrixBuffer, 0);

    // 정점 셰이더에서의 상수 버퍼의 위치를 설정
    _uint iBufferNumber = 0;

    D3D11Context()->VSSetConstantBuffers(iBufferNumber, 1, &m_pMatrixBuffer);

    
    /*************
    * 카메라 버퍼
    **************/
    FAILED_CHECK_RETURN(D3D11Context()->Map(m_pCameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);
    CAMERA_BUFFER_T* dataPtr3 = Cast<CAMERA_BUFFER_T*>(mappedResource.pData);

    dataPtr3->vPosition = tCameraBuf.vPosition;
    dataPtr3->fPadding = 0.f;

    D3D11Context()->Unmap(m_pCameraBuffer, 0);

    iBufferNumber = 1;

    D3D11Context()->VSSetConstantBuffers(iBufferNumber, 1, &m_pCameraBuffer);

    D3D11Context()->PSSetShaderResources(0, 1, m_pTexture.GetAddressOf());


    /*************
    * 뼈 버퍼
    **************/
    FAILED_CHECK_RETURN(D3D11Context()->Map(m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);
    BONE_COMMON_BUFFER_T* dataPtr4 = Cast<BONE_COMMON_BUFFER_T*>(mappedResource.pData);

    for (size_t i = 0; i < 128; i++)
    {
        dataPtr4->matTransform[i] = tBoneBuf.matTransform[i];
    }
    
    D3D11Context()->Unmap(m_pCameraBuffer, 0);

    iBufferNumber = 2;

    D3D11Context()->VSSetConstantBuffers(iBufferNumber, 1, &m_pBoneBuffer);;


    /*********
    * 빛 버퍼
    ***********/
    FAILED_CHECK_RETURN(D3D11Context()->Map(m_pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource), E_FAIL);

    LIGHT_BUFFER_T* dataPtr2 = Cast<LIGHT_BUFFER_T*>(mappedResource.pData);

    dataPtr2->vAmbientColor = tLightBuf.vAmbientColor;
    dataPtr2->vDiffuseColor = tLightBuf.vDiffuseColor;
    dataPtr2->vLightDirection = tLightBuf.vLightDirection;
    dataPtr2->fSpecularPower = tLightBuf.fSpecularPower;
    dataPtr2->vSpecularColor = tLightBuf.vSpecularColor;

    D3D11Context()->Unmap(m_pLightBuffer, 0);

    iBufferNumber = 0;

    // 픽셀 셰이더에서 셰이더 텍스처 리소스 설정
    D3D11Context()->PSSetConstantBuffers(iBufferNumber, 1, &m_pLightBuffer);

    return S_OK;
}

void CModelShaderComp::Render_Shader(_int iIndexCount)
{
    D3D11Context()->IASetInputLayout(m_pLayout);

    D3D11Context()->VSSetShader(m_pVertexShader.Get(), NULL, 0);
    D3D11Context()->PSSetShader(m_pPixelShader.Get(), NULL, 0);

    D3D11Context()->PSSetSamplers(0, 1, m_pSamplereState.GetAddressOf());

    D3D11Context()->DrawIndexed(iIndexCount, 0, 0);
}
