#include "Component/VIBufferComp.h"

#include "System/GameInstance.h"

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
	, m_pDeviceComp(rhs.m_pDeviceComp)
	, m_pVtxBuffer(rhs.m_pVtxBuffer), m_pIndexBuffer(rhs.m_pIndexBuffer)
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
