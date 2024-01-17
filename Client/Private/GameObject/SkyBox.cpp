#include "GameObject/SkyBox.h"

#include "Component/BoxBufferComp.h"


CSkyBox::CSkyBox()
{
    Set_Name(L"SkyBox");
}

CSkyBox::CSkyBox(const CSkyBox& rhs)
    : Base(rhs)
{
}

HRESULT CSkyBox::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    if (FAILED(Initialize_Component()))
        return E_FAIL;

    Set_RenderGroup(ERenderGroup::Priority);

    return S_OK;
}

HRESULT CSkyBox::Initialize_Prototype(FSerialData& InputData)
{
    return E_NOTIMPL;
}

HRESULT CSkyBox::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize(Arg)))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkyBox::Initialize(FSerialData& InputData)
{
    return E_NOTIMPL;
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
    // 컴포넌트의 기본을 추가
    if (FAILED(Add_Component(TEXT("ModelComp"), m_pModelComp = CCylinderModelComp::Create())))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkyBox::Initialize_Component(FSerialData& InputData)
{
    return E_NOTIMPL;
}
