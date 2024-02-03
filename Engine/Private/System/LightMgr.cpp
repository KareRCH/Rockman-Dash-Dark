#include "System/LightMgr.h"

#include "System/Data/Light.h"
#include "Component/EffectComponent.h"
#include "Component/RectBufferComp.h"

_uint CLightMgr::g_iID = 0;

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

HRESULT CLightMgr::Add_Light(const TLIGHT_DESC& LightDesc, _uint& iReturnID, CLight** GetLight)
{
	CLight* pLight = CLight::Create(LightDesc, g_iID);
	if (nullptr == pLight)
		return E_FAIL;

	m_Lights.push_back(pLight);
	iReturnID = g_iID++;
	if (GetLight != nullptr)
		*GetLight = pLight;

	return S_OK;
}

HRESULT CLightMgr::Remove_Light(const _uint iID)
{
	auto iter = lower_bound(m_Lights.begin(), m_Lights.end(), iID,
		[](const CLight* pLight, const _uint iID) {
			return pLight->Get_ID() < iID;
		});
	if (iter == m_Lights.end())
		return E_FAIL;

	Safe_Release(*iter);
	m_Lights.erase(iter);

	return S_OK;
}
