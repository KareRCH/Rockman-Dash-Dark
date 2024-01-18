#include "GameObject/Character_Common.h"

CCharacter_Common::CCharacter_Common()
{
	FAILED_CHECK_RETURN(Add_Component(TEXT("TeamAgentComp"), 
		m_pTeamAgentComp = CTeamAgentComp::Create()), );
}

CCharacter_Common::CCharacter_Common(const CCharacter_Common& rhs)
	: Base(rhs)
	, m_fHP(rhs.m_fHP)
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

HRESULT CCharacter_Common::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData ProtoData;
        InputData.Get_ObjectFromArray("Components", i, ProtoData);

        _uint iComponentID = 0;
        if (FAILED(ProtoData.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strName = "";
        if (FAILED(ProtoData.Get_Data("Name", strName)))
            return E_FAIL;

        string strProtoName = "";
        if (FAILED(ProtoData.Get_Data("ProtoName", strProtoName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::TeamAgent):
            TeamAgentComp().Set_ProtoName(ConvertToWstring(strProtoName));
            break;
        }
    }

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

HRESULT CCharacter_Common::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
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
