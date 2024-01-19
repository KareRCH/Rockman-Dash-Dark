#include "GameObject/SkyBox.h"

#include "Component/BoxBufferComp.h"


CSkyBox::CSkyBox()
{
    Set_Name(L"SkyBox");
    Set_RenderGroup(ERenderGroup::Priority);
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
    : Base(rhs)
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    // 컴포넌트의 기본을 추가
    if (FAILED(Add_Component(TEXT("ModelComp"), m_pModelComp = CCylinderModelComp::Create())))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkyBox::Initialize_Prototype(FSerialData& InputData)
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

        switch (iComponentID)
        {
        case ECast(EComponentID::CylinderModel):
            NULL_CHECK_RETURN(m_pModelComp = CCylinderModelComp::Create(ProtoData), E_FAIL);
            if (FAILED(Add_Component(ConvertToWstring(strName), m_pModelComp)))
                return E_FAIL;
            break;
        }
    }

    return S_OK;
}

HRESULT CSkyBox::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize(Arg)))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkyBox::Initialize(FSerialData& InputData)
{


    return S_OK;
}

void CSkyBox::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CSkyBox::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CSkyBox::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

    Transform().Set_Position(PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One));

    m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CSkyBox::Render()
{
    SUPER::Render();

    m_pModelComp->Render();

    return S_OK;
}

CSkyBox* CSkyBox::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CSkyBox Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CSkyBox* CSkyBox::Create(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype(InputData)))
    {
        MSG_BOX("CSkyBox Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkyBox::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CSkyBox Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

CGameObject* CSkyBox::Clone(FSerialData& InputData)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize(InputData)))
    {
        MSG_BOX("CSkyBox Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CSkyBox::Free()
{
    SUPER::Free();
}

FSerialData CSkyBox::SerializeData_Prototype()
{
    FSerialData Data = SUPER::SerializeData_Prototype();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

FSerialData CSkyBox::SerializeData()
{
    FSerialData Data = SUPER::SerializeData();

    Data.Add_Member("ClassID", g_ClassID);

    return Data;
}

HRESULT CSkyBox::Initialize_Component()
{
    

    return S_OK;
}

HRESULT CSkyBox::Initialize_Component(FSerialData& InputData)
{
    _uint iNumPrototype = 0;
    iNumPrototype = InputData.Get_ArraySize("Components");
    for (_uint i = 0; i < iNumPrototype; i++)
    {
        FSerialData InputProto;
        InputData.Get_ObjectFromArray("Components", i, InputProto);

        _uint iComponentID = 0;
        if (FAILED(InputProto.Get_Data("ComponentID", iComponentID)))
            return E_FAIL;

        string strProtoName = "";
        if (FAILED(InputProto.Get_Data("ProtoName", strProtoName)))
            return E_FAIL;

        string strName = "";
        if (FAILED(InputProto.Get_Data("Name", strName)))
            return E_FAIL;

        switch (iComponentID)
        {
        case ECast(EComponentID::CylinderModel):
            FAILED_CHECK_RETURN(Add_Component(ConvertToWstring(strName),
                m_pModelComp = DynCast<CCylinderModelComp*>(GI()->Clone_PrototypeComp(ConvertToWstring(strProtoName), InputProto))), E_FAIL);
            break;
        }
    }

    return S_OK;
}
