#include "Level/Level_Logo.h"

#include "Level/Level_Loading.h"

#include "GameObject/LoadingScreen.h"
#include "Component/PlaneModelComp.h"
#include "Component/RectBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"
#include "GameObject/TitleScreen.h"
#include "GameObject/UI_FadeIn.h"

CLevel_Logo::CLevel_Logo()
{
	Set_Name(TEXT("Logo"));
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Objects()))
		return E_FAIL;

    return S_OK;
}

void CLevel_Logo::Tick(const _float& fTimeDelta)
{
	if (m_pGI->IsKey_Pressed(DIK_RETURN))
	{
		if (FAILED(m_pGI->Open_Level(LEVEL_LOADING, 
			CLevel_Loading::Create(LEVEL_PARSED, GI()->Get_TextureMainPath() + TEXT("Levels/GamePlay.alevel")), false)))
			return;
	}
}

HRESULT CLevel_Logo::Render()
{
    return S_OK;
}

CLevel_Logo* CLevel_Logo::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
	SUPER::Free();
}

HRESULT CLevel_Logo::Ready_Objects()
{
	m_pGI->Set_LevelTag(Get_Name());

	m_pGI->Play_BGM(TEXT("RockmanDash2"), TEXT("01. Title Screen.mp3"), 1.f);

	if (FAILED(m_pGI->Add_GameObject(CTitleScreen::Create())))
		return E_FAIL;

	if (FAILED(m_pGI->Add_GameObject(CUI_FadeIn::Create())))
		return E_FAIL;

	_uint iID = 0;
	TLIGHT_DESC			LightDesc{};
	LightDesc.eType = TLIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vPosition = { 0.f, 1.f, 0.f, 1.f };
	LightDesc.fRange = 30.f;
	LightDesc.vDiffuse = _float4(0.6f, 0.6f, 0.6f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGI->Add_Light(LightDesc, iID)))
		return E_FAIL;

	return S_OK;
}
