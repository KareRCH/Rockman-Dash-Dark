#include "Component/VIBufferComp.h"

#include "System/GameInstance.h"

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
	, m_pDeviceComp(rhs.m_pDeviceComp)
	, m_pVB(rhs.m_pVB), m_pIB(rhs.m_pIB)
{
	Safe_AddRef(m_pDeviceComp);
}

HRESULT CVIBufferComp::Initialize_Prototype(void* Arg)
{
	m_pDeviceComp = Cast<CD3D11DeviceComp*>(GI()->Reference_PrototypeComp(L"GraphicDevComp"));

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
