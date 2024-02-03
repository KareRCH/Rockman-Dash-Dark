#include "System/Data/Light.h"

#include "Component/EffectComponent.h"
#include "Component/RectBufferComp.h"

HRESULT CLight::Initialize(const TLIGHT_DESC& LightDesc, const _uint iID)
{
	m_LightDesc = LightDesc;
	m_iID = iID;

    return S_OK;
}

HRESULT CLight::Render(CEffectComponent* pEffect, CVIBufferComp* pVIBuffer)
{
	_uint		iPassIndex = { 0 };

	if (TLIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		pEffect->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4));

		iPassIndex = 1;
	}
	else
	{
		pEffect->Bind_RawValue("g_vLightPos", &m_LightDesc.vPosition, sizeof(_float4));
		pEffect->Bind_RawValue("g_fLightRange", &m_LightDesc.fRange, sizeof(_float));

		iPassIndex = 2;
	}

	if (FAILED(pEffect->Bind_RawValue("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pEffect->Bind_RawValue("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pEffect->Bind_RawValue("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4))))
		return E_FAIL;

	pEffect->Begin(iPassIndex);

	pVIBuffer->Bind_Buffer();

	return pVIBuffer->Render_Buffer();
}

CLight* CLight::Create(const TLIGHT_DESC& LightDesc, const _uint iID)
{
	CLight* pInstance = new CLight();

	if (FAILED(pInstance->Initialize(LightDesc, iID)))
	{
		MSG_BOX("Failed to Created : CLight");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLight::Free()
{

}
