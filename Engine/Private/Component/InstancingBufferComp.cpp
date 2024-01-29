#include "Component/InstancingBufferComp.h"

CInstancingBufferComp::CInstancingBufferComp(const CInstancingBufferComp& rhs)
	: Base(rhs)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_iInstanceStride(rhs.m_iInstanceStride)
	, m_iIndexCountPerInstance(rhs.m_iIndexCountPerInstance)
	, m_RandomNumber(rhs.m_RandomNumber)
	, m_pSpeeds(rhs.m_pSpeeds)
	, m_pLifeTimes(rhs.m_pLifeTimes)
	, m_InstancingDesc(rhs.m_InstancingDesc)
{
}

HRESULT CInstancingBufferComp::Initialize_Prototype(void* Arg)
{
    return S_OK;
}

HRESULT CInstancingBufferComp::Initialize(void* Arg)
{
    return S_OK;
}

void CInstancingBufferComp::Free()
{
	SUPER::Free();

	if (!m_bIsCloned)
	{
		Safe_Delete_Array(m_pSpeeds);
		Safe_Delete_Array(m_pLifeTimes);
	}

	Safe_Release(m_pVBInstance);
}

HRESULT CInstancingBufferComp::Bind_Buffer()
{
	if (nullptr == m_pVB ||
		nullptr == m_pIB)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB.Get(),
		m_pVBInstance,
	};

	_uint				iStrides[] = {
		m_iVertexStride,
		m_iInstanceStride

	};

	_uint				iOffsets[] = {
		0,
		0,
	};

	auto pContext = D3D11Context();
	/* 어떤 버텍스 버퍼들을 이용할거다. */
	pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	/* 어떤 인덱스 버퍼를 이용할거다. */
	pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);

	/* 정점을 어떤식으로 이어서 그릴거다. */
	pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CInstancingBufferComp::Update_Buffer(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	D3D11_MAPPED_SUBRESOURCE			SubResource = {};

	auto pContext = D3D11Context();
	pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &SubResource);

	VTXINSTANCE* pVertices = ((VTXINSTANCE*)SubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float		fAlpha = max(m_pLifeTimes[i] - m_fTimeAcc, 0.f);

		pVertices[i].vColor.w = fAlpha;

		_vector		vDir = XMVector3Normalize(XMLoadFloat4(&pVertices[i].vPosition) - XMLoadFloat3(&m_InstancingDesc.vCenter));
		vDir = XMVectorSetW(vDir, 0.f);

		XMStoreFloat4(&pVertices[i].vPosition, XMLoadFloat4(&pVertices[i].vPosition) + vDir * m_pSpeeds[i].x * fTimeDelta);
	}

	pContext->Unmap(m_pVBInstance, 0);
}

HRESULT CInstancingBufferComp::Render_Buffer()
{
	D3D11Context()->DrawIndexedInstanced(m_iIndexCountPerInstance, m_iNumInstance, 0, 0, 0);

    return S_OK;
}
