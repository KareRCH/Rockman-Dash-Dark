#include "Component/TriBufferComp.h"

CTriBufferComp::CTriBufferComp(ID3D11Device* pGraphicDev)
	: Base(pGraphicDev)
{
}

CTriBufferComp::CTriBufferComp(const CTriBufferComp& rhs)
	: Base(rhs)
{
}

HRESULT CTriBufferComp::Initialize()
{
	return S_OK;
}

_int CTriBufferComp::Tick(const _float& fTimeDelta)
{
	return 0;
}

void CTriBufferComp::LateTick()
{
}

void CTriBufferComp::Render(ID3D11DeviceContext* pDeviceContext)
{
}

CPrimitiveComponent* CTriBufferComp::Clone()
{
	return new ThisClass(*this);
}

void CTriBufferComp::Free()
{
	SUPER::Free();
}
