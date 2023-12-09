#include "Component/PrimitiveComponent.h"

#include "Component/D3D11DeviceComp.h"
#include "System/GameInstance.h"

CPrimitiveComponent::CPrimitiveComponent(const CPrimitiveComponent& rhs)
	: Base(rhs)
{
	
}

HRESULT CPrimitiveComponent::Initialize_Prototype(void* Arg)
{
	FAILED_CHECK_RETURN(__super::Initialize_Prototype(Arg), E_FAIL);


	return S_OK;
}

void CPrimitiveComponent::Free()
{
	SUPER::Free();

}