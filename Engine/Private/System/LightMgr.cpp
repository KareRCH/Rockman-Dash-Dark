#include "System/LightMgr.h"

#include "System/Data/Light.h"
#include "Component/EffectComponent.h"
#include "Component/RectBufferComp.h"

HRESULT CLightMgr::Initialize()
{
    return S_OK;
}

HRESULT CLightMgr::Render(CEffectComponent* pEffect, CRectBufferComp* pVIBuffer)
{
	for (auto& pLight : m_Lights)
		pLight->Render(pEffect, pVIBuffer);

	return S_OK;
}

CLightMgr* CLightMgr::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLightMgr");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightMgr::Free()
{
	for (auto& pLight : m_Lights)
		Safe_Release(pLight);

	m_Lights.clear();
}

HRESULT CLightMgr::Add_Light(const TLIGHT_DESC& LightDesc)
{
	CLight* pLight = CLight::Create(LightDesc);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);

	return S_OK;
}
