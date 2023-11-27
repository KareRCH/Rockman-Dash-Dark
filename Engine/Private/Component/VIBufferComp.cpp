#include "Component/VIBufferComp.h"

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
	, m_pVtxBuffer(rhs.m_pVtxBuffer), m_pIndexBuffer(rhs.m_pIndexBuffer)
	, m_pDeviceComp(rhs.m_pDeviceComp)
{
	Safe_AddRef(m_pDeviceComp);
}

HRESULT CVIBufferComp::Initialize_Prototype(void* Arg)
{
	m_pDeviceComp = CD3D11DeviceComp::Create();

	return S_OK;
}

HRESULT CVIBufferComp::Initialize(void* Arg)
{
	return S_OK;
}

void CVIBufferComp::Free()
{

}
