#include "GameObject/UI_Player.h"

CUI_Player::CUI_Player()
{
}

CUI_Player::CUI_Player(const CUI_Player& rhs)
{
}

HRESULT CUI_Player::Initialize_Prototype()
{
    TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ
    TurnOn_State(EGObjectState::RenderZBuffer);     // ZBuffer 사용

    Set_RenderGroup(ERenderGroup::UI);

    return S_OK;
}

HRESULT CUI_Player::Initialize(void* Arg)
{
    return S_OK;
}

void CUI_Player::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CUI_Player::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CUI_Player::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);

}

HRESULT CUI_Player::Render()
{
    return S_OK;
}

CUI_Player* CUI_Player::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("CUI_Player Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CUI_Player::Free()
{
    SUPER::Free();

}

HRESULT CUI_Player::Initialize_Component()
{
    return S_OK;
}
