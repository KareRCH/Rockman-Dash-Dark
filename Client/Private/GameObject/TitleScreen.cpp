#include "GameObject/TitleScreen.h"

#include "Component/PlaneModelComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

CTitleScreen::CTitleScreen()
{
    Set_Name(L"TitleScreen");
}

CTitleScreen::CTitleScreen(const CTitleScreen& rhs)
    : Base(rhs)
{
}

HRESULT CTitleScreen::Initialize_Prototype()
{
    FAILED_CHECK_RETURN(Initialize_Component(), E_FAIL);

    return S_OK;
}

HRESULT CTitleScreen::Initialize(void* Arg)
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

void CTitleScreen::Priority_Tick(const _float& fTimeDelta)
{
    SUPER::Priority_Tick(fTimeDelta);

}

void CTitleScreen::Tick(const _float& fTimeDelta)
{
    SUPER::Tick(fTimeDelta);

}

void CTitleScreen::Late_Tick(const _float& fTimeDelta)
{
    SUPER::Late_Tick(fTimeDelta);


}

HRESULT CTitleScreen::Render()
{
    SUPER::Render();

    m_pBackgroundImage->Render();
    m_pTitleImage->Render();

    return S_OK;
}

void CTitleScreen::BeginPlay()
{
    SUPER::BeginPlay();

    Set_RenderGroup(ERenderGroup::UI);              // UI 그룹에서 렌더링, 나중에 그려지며 깊이 버퍼를 쓰지 않음
}

CTitleScreen* CTitleScreen::Create()
{
    ThisClass* pInstance = new ThisClass();

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("LoadingScreen Create Failed");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTitleScreen::Clone(void* Arg)
{
    ThisClass* pInstance = new ThisClass(*this);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("LoadingScreen Create Failed");
        Safe_Release(pInstance);
    }

    return Cast<CGameObject*>(pInstance);
}

void CTitleScreen::Free()
{
    SUPER::Free();
}

FSerialData CTitleScreen::SerializeData()
{
    return FSerialData();
}

HRESULT CTitleScreen::Initialize_Component()
{
    // 프로토타입용 컴포넌트 초기화
    if (FAILED(Add_Component(L"Title", m_pTitleImage = CPlaneModelComp::Create())))
        return E_FAIL;

    HRESULT hr = 0;
    m_pTitleImage->Set_CurrentTextureIndex(0);
    m_pTitleImage->Set_Mode(CPlaneModelComp::ORTHO);
    m_pTitleImage->Transform().Set_Position(0.f, 100.f, 1.f);
    m_pTitleImage->Transform().Set_Scale(1280.f * 0.6f, 720.f * 0.6f, 1.f);
    hr = m_pTitleImage->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/Images/RockmanDash2_Logo.png"));
    hr = m_pTitleImage->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pTitleImage->Reset_ActivePass();
    m_pTitleImage->Set_ActivePass(1);
    m_pTitleImage->Set_Alpha(1.f);


    if (FAILED(Add_Component(L"Background", m_pBackgroundImage = CPlaneModelComp::Create())))
        return E_FAIL;

    m_pBackgroundImage->Set_CurrentTextureIndex(0);
    m_pBackgroundImage->Set_Mode(CPlaneModelComp::ORTHO);
    m_pBackgroundImage->Transform().Set_Position(0.f, 0.f, 1.f);
    m_pBackgroundImage->Transform().Set_Scale(1280.f, 720.f, 1.f);
    hr = m_pBackgroundImage->TextureComp()->Bind_Texture(TEXT("Textures/RockmanDash2/Images/TitleBackground.png"));
    hr = m_pBackgroundImage->EffectComp()->Bind_Effect(TEXT("Runtime/FX_VtxPosTexAlpha.hlsl"), SHADER_VTX_TEXCOORD::Elements, SHADER_VTX_TEXCOORD::iNumElements);
    m_pBackgroundImage->Reset_ActivePass();
    m_pBackgroundImage->Set_ActivePass(1);
    m_pBackgroundImage->Set_Alpha(1.f);

    return S_OK;
}
