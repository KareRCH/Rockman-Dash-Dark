#include "GameObject/Character_Common.h"

CCharacter_Common::CCharacter_Common()
{
	FAILED_CHECK_RETURN(Add_Component(TEXT("TeamAgentComp"), 
		m_pTeamAgentComp = CTeamAgentComp::Create()), );
}

CCharacter_Common::CCharacter_Common(const CCharacter_Common& rhs)
	: Base(rhs)
{
	FAILED_CHECK_RETURN(Add_Component(TEXT("TeamAgentComp"), 
		m_pTeamAgentComp = DynCast<CTeamAgentComp*>(rhs.m_pTeamAgentComp->Clone())), );
}

HRESULT CCharacter_Common::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCharacter_Common::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

void CCharacter_Common::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CCharacter_Common::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

}

void CCharacter_Common::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CCharacter_Common::Render()
{

	return S_OK;
}

void CCharacter_Common::Free()
{
	SUPER::Free();

}

HRESULT CCharacter_Common::Initialize_Component()
{
	if (nullptr == m_pTeamAgentComp)
		return E_FAIL;



	return S_OK;
}

void CCharacter_Common::OnCollision(CGameObject* pDst, const FContact* pContact)
{

}

void CCharacter_Common::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{

}

void CCharacter_Common::OnCollisionExited(CGameObject* pDst)
{

}
