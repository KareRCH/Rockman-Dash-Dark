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

    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    Set_RenderGroup(ERenderGroup::Priority);

    return S_OK;
}

HRESULT CSkyBox::Initialize(void* Arg)
{
    if (FAILED(__super::Initialize(Arg)))
        return E_FAIL;


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
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkyBox::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CBoxModelComp Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CSkyBox::Free()
{
    SUPER::Free();
}

FSerialData CSkyBox::SerializeData()
{
    return FSerialData();
}

HRESULT CSkyBox::Initialize_Component()
{
    // 컴포넌트의 기본을 추가
    if (FAILED(Add_Component(TEXT("ModelComp"), m_pModelComp = CCylinderModelComp::Create())))
        return E_FAIL;

    return S_OK;
}
