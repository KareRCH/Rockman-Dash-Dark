#include "Component/VIBufferComp.h"

CVIBufferComp::CVIBufferComp(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
{
}

CVIBufferComp::CVIBufferComp(const CVIBufferComp& rhs)
	: Base(rhs)
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
