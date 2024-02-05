#include "GameObject/LoadingScreen.h"

#include "Component/PlaneModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CLoadingScreen::CLoadingScreen()
{
    Set_Name(L"LoadingScreen");
}

CLoadingScreen::CLoadingScreen(const CLoadingScreen& rhs)
    : Base(rhs)
{
}

HRESULT CLoadingScreen::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CLoadingScreen::Initialize(void* Arg)
{
    TLoadingScreenDesc* pDesc = Cast<TLoadingScreenDesc*>(Arg);
    if (pDesc == nullptr)
        return E_FAIL;

    m_fX = pDesc->fX;
    m_fY = pDesc->fY;
    m_fSizeX = pDesc->fSizeX;
    m_fSizeY = pDesc->fSizeY;

    return S_OK;
}

void CLoadingScreen::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CLoadingScreen::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CLoadingScreen::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);


}

HRESULT CLoadingScreen::Render()
{
    SUPER::Render();

    m_pPlaneModelComp->Render();

    return S_OK;
}

void CLoadingScreen::BeginPlay()
{
    SUPER::BeginPlay();

    Set_RenderGroup(ERenderGroup::UI);              // UI 그룹에서 렌더링, 나중에 그려지며 깊이 버퍼를 쓰지 않음
}

CLoadingScreen* CLoadingScreen::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("LoadingScreen Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLoadingScreen::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("LoadingScreen Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CLoadingScreen::Free()
{
    SUPER::Free();
}

FSerialData CLoadingScreen::SerializeData()
{
    return FSerialData();
}

HRESULT CLoadingScreen::Initialize_Component()
{
    // 프로토타입용 컴포넌트 초기화
    if (FAILED(Add_Component(L"LoadingScreen", m_pPlaneModelComp = CPlaneModelComp::Create())))
        return E_FAIL;

    HRESULT hr = 0;
    m_pPlaneModelComp->Set_CurrentTextureIndex(0);
    m_pPlaneModelComp->Set_Mode(CPlaneModelComp::ORTHO);
    hr = m_pPlaneModelComp->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/Images/Splash.png"));
    hr = m_pPlaneModelComp->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pPlaneModelComp->Reset_ActivePass();
    m_pPlaneModelComp->Set_ActivePass(1);
    m_pPlaneModelComp->Set_Alpha(1.f);

    return S_OK;
}
