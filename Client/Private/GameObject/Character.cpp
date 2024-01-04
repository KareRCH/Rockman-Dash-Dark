#include "GameObject/Character.h"

CCharacter::CCharacter()
{
}

CCharacter::CCharacter(const CCharacter& rhs)
	: Base(rhs)
{
}

HRESULT CCharacter::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CCharacter::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);
}

void CCharacter::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);
}

void CCharacter::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);
}

HRESULT CCharacter::Render()
{
	return S_OK;
}

void CCharacter::Free()
{
	SUPER::Free();
}

HRESULT CCharacter::Initialize_Component()
{
	return S_OK;
}