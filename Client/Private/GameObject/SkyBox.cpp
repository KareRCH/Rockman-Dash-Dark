#include "GameObject/SkyBox.h"

#include "Component/BoxBufferComp.h"
#include "Component/BoxModelComp.h"

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
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용
    TurnOn_State(EGObjectState::RenderDeferred);    // 디퍼드 셰이딩 사용, ZBuffer 미사용시 무시


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

HRESULT CSkyBox::Initialize_Component()
{
    // 기본형 컴포넌트에 대한 정의
    if (FAILED(Add_Component(TEXT("ModelComp"), m_pModelComp = CBoxModelComp::Create())))
        return E_FAIL;

    return S_OK;
}
