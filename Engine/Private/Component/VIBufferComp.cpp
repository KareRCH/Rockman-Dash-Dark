#include "Component/VIBufferComp.h"

#include "System/GameInstance.h"

CVIBufferComp::CVIBufferComp()
{
	NULL_CHECK(m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp")));
}

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
	, m_pDeviceComp(rhs.m_pDeviceComp)
	, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
{
	Safe_AddRef(m_pDeviceComp);
}

HRESULT CVIBufferComp::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CVIBufferComp::Initialize(void* Arg)
{
	return S_OK;
}

void CVIBufferComp::Free()
{
	Safe_Release(m_pDeviceComp);
}

HRESULT CVIBufferComp::Create_Buffer(_Inout_ ID3D11Buffer** ppBuffer)
{
	return D3D11Device()->CreateBuffer(&m_BufferDesc, &m_SubResourceData, ppBuffer);
}

void CVIBufferComp::Update_VBuffer(void* pData, _uint iCount)
{
	D3D11Context()->UpdateSubresource(m_pVB.Get(), 0, nullptr, pData, m_iVertexStride, iCount);
}

HRESULT CVIBufferComp::Copy_VBufferToArray(void* pArray, size_t iSize, size_t iSizePerIndex)
{
	if (nullptr == m_pVB)
		return E_FAIL;


	D3D11_BUFFER_DESC tDesc = {};
	m_pVB->GetDesc(&tDesc);
	tDesc.Usage = D3D11_USAGE_STAGING;
	tDesc.BindFlags = 0;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	
	ID3D11Buffer* pBuffer = { nullptr };
	if (FAILED(D3D11Device()->CreateBuffer(&tDesc, nullptr, &pBuffer)))
	{
		Safe_Release(pBuffer);
		return E_FAIL;
	}

	D3D11Context()->CopyResource(pBuffer, m_pVB.Get());

	// ������ �а� ���� �迭�� �����ϱ�
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (SUCCEEDED(D3D11Context()->Map(pBuffer, 0, D3D11_MAP_READ, 0, &mappedResource)))
	{
		// ���ε� ���ҽ����� ������ �б�
		rsize_t iDataSize = iSize * iSizePerIndex;
		memcpy_s(pArray, iDataSize, mappedResource.pData, iDataSize);

		// ���� ����
		D3D11Context()->Unmap(pBuffer, 0);
	}

	Safe_Release(pBuffer);

	return S_OK;
}

HRESULT CVIBufferComp::Copy_IBufferToArray(void* pArray, size_t iSize)
{
	if (nullptr == m_pIB)
		return E_FAIL;

	auto pContext = D3D11Context();

	D3D11_BUFFER_DESC Desc = {};
	m_pIB->GetDesc(&Desc);
	Desc.Usage = D3D11_USAGE_STAGING;
	Desc.BindFlags = 0;
	Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

	ID3D11Buffer* pBuffer = { nullptr };
	if (FAILED(D3D11Device()->CreateBuffer(&Desc, nullptr, &pBuffer)))
	{
		Safe_Release(pBuffer);
		return E_FAIL;
	}

	pContext->CopyResource(pBuffer, m_pIB.Get());

	// ������ �а� ���� �迭�� �����ϱ�
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (SUCCEEDED(pContext->Map(pBuffer, 0, D3D11_MAP_READ, 0, &mappedResource)))
	{
		// ���ε� ���ҽ����� ������ �б�
		memcpy_s(pArray, iSize, mappedResource.pData, iSize);

		// ���� ����
		pContext->Unmap(pBuffer, 0);
	}

	Safe_Release(pBuffer);

	return S_OK;
}

HRESULT CVIBufferComp::Bind_Buffer()
{
	if (nullptr == m_pVB || nullptr == m_pIB)
		return E_FAIL;

	ID3D11Buffer* pVBs[] = {
		m_pVB.Get()
	};

	_uint           iStrides[] = {
		m_iVertexStride
	};
	_uint           iOffsets[] = {
		0,
	};

	auto pContext = D3D11Context();

	/* � ���ؽ� ���۵��� �̿��ҰŴ�. */
	pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVBs, iStrides, iOffsets);

	/* � �ε��� ���۸� �̿��ҰŴ�. */
	pContext->IASetIndexBuffer(m_pIB.Get(), m_eIndexFormat, 0);

	/* ������ ������� �̾ �׸��Ŵ�. */
	pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

HRESULT CVIBufferComp::Render_Buffer()
{
	D3D11Context()->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}
