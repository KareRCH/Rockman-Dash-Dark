#include "Component/ParticlePointBufferComp.h"

CParticlePointBufferComp::CParticlePointBufferComp()
{

}

CParticlePointBufferComp::CParticlePointBufferComp(const CParticlePointBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CParticlePointBufferComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticlePointBufferComp::Initialize_Prototype(_uint iNumInstance)
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	if (FAILED(Create_VIBuffers(iNumInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CParticlePointBufferComp::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

	return S_OK;
}

CParticlePointBufferComp* CParticlePointBufferComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CParticlePointBufferComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CParticlePointBufferComp* CParticlePointBufferComp::Create(_uint iNumInstance)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(iNumInstance)))
    {
        MSG_BOX("CParticlePointBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CParticlePointBufferComp::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CParticlePointBufferComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CComponent*>(pInstance);
}

void CParticlePointBufferComp::Free()
{
    SUPER::Free();


}

HRESULT CParticlePointBufferComp::Create_VIBuffers(_uint iNumInstance)
{
	m_iNumVertexBuffers = 2;
	m_iNumVertices = 1;
	m_iVertexStride = sizeof(VTXPOINT);
	m_iInstanceStride = sizeof(VTXINSTANCE);
	m_iIndexCountPerInstance = 1;
	m_iNumInstance = iNumInstance;

	m_pSpeeds = new _float[m_iNumInstance];
	m_pLifeTimes = new _float[m_iNumInstance];

	m_iNumIndices = iNumInstance;
	m_iIndexStride = 2;
	m_eIndexFormat = m_iIndexStride == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;




#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	VTXPOINT* pVertices = new VTXPOINT;

	pVertices->vPosition = _float3(0.0f, 0.0f, 0.f);
	pVertices->vPSize = _float2(1.0f, 1.f);

	m_SubResourceData.pSysMem = pVertices;

	/* pVertices에 할당하여 채워놨던 정점들의 정보를 ID3D11Buffer로 할당한 공간에 복사하여 채워넣는다. */
	if (FAILED(__super::Create_Buffer(m_pVB.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
#pragma endregion




#pragma region INDEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT /*D3D11_USAGE_DYNAMIC*/;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(m_pIB.GetAddressOf())))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
#pragma endregion

	return S_OK;
}
