#include "Component/VIBufferComp.h"

CVIBufferComp::CVIBufferComp(const DX11DEVICE_T tDevice)
	: Base(tDevice)
{
}

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
	, m_pVtxBuffer(rhs.m_pVtxBuffer), m_pIndexBuffer(rhs.m_pIndexBuffer)
{
}

_int CVIBufferComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CVIBufferComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pVtxBuffer);
	Safe_Release(m_pIndexBuffer);
}
