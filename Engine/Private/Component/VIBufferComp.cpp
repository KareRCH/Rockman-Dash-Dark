#include "Component/VIBufferComp.h"

CVIBufferComp::CVIBufferComp(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
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
}
