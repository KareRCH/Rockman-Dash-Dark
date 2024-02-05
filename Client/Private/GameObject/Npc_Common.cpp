#include "GameObject/Npc_Common.h"

CNpc_Common::CNpc_Common()
{
}

CNpc_Common::CNpc_Common(const CNpc_Common& rhs)
    : Base(rhs)
{
}

HRESULT CNpc_Common::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNpc_Common::Initialize_Prototype(FSerialData& InputData)
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

HRESULT CNpc_Common::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNpc_Common::Initialize(FSerialData& InputData)
{
    if (FAILED(__super::Initialize(InputData)))
        return E_FAIL;

    return S_OK;
}

void CNpc_Common::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CNpc_Common::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CNpc_Common::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CNpc_Common::Render()
{

    return S_OK;
}

void CNpc_Common::Free()
{
    SUPER::Free();

}

HRESULT CNpc_Common::Initialize_Component()
{


    return S_OK;
}

void CNpc_Common::OnCollision(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollision(pDst, pContact);

}

void CNpc_Common::OnCollisionEntered(CGameObject* pDst, const FContact* pContact)
{
    SUPER::OnCollisionEntered(pDst, pContact);

}

void CNpc_Common::OnCollisionExited(CGameObject* pDst)
{
    SUPER::OnCollisionExited(pDst);

}
