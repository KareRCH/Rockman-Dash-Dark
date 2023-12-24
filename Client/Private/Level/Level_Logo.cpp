#include "Level/Level_Logo.h"

#include "Level/Level_Loading.h"

#include "GameObject/LoadingScreen.h"
#include "Component/PlaneModelComp.h"
#include "Component/RectBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"

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
		if (FAILED(m_pGI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(LEVEL_GAMEPLAY))))
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

	CLoadingScreen* pLoadingScreen = { nullptr };

	if (FAILED(m_pGI->Add_GameObject(pLoadingScreen = CLoadingScreen::Create())))
		return E_FAIL;

	CPlaneModelComp* pModel = pLoadingScreen->Get_Component<CPlaneModelComp>(TEXT("LoadingScreen"));
	pModel->Transform().Set_Scale(XMVectorSet(g_iWindowSizeX, g_iWindowSizeY, 1.f, 0.f));
	pModel->Transform().Set_Position(XMVectorSet(0.f, 0.f, 10.f, 1.f));

	return S_OK;
}
