#include "Component/InstancingModelComp.h"

#include "Component/ParticlePointBufferComp.h"
#include "Component/EffectComponent.h"
#include "Component/TextureComponent.h"


CInstancingModelComp::CInstancingModelComp()
{
	NULL_CHECK(m_pVIBufferComp = CParticlePointBufferComp::Create());
	NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
	NULL_CHECK(m_pTextureComp = CTextureComponent::Create());
}

CInstancingModelComp::CInstancingModelComp(const CInstancingModelComp& rhs)
{
	NULL_CHECK(m_pVIBufferComp = Cast<CParticlePointBufferComp*>(rhs.m_pVIBufferComp->Clone()));
	NULL_CHECK(m_pEffectComp = Cast<CEffectComponent*>(rhs.m_pEffectComp->Clone()));
	NULL_CHECK(m_pTextureComp = Cast<CTextureComponent*>(rhs.m_pTextureComp->Clone()));
}

HRESULT CInstancingModelComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancingModelComp::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancingModelComp::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CInstancingModelComp::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;

	return S_OK;
}

void CInstancingModelComp::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CInstancingModelComp::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	m_pVIBufferComp->Update_Buffer(fTimeDelta);
}

void CInstancingModelComp::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

}

HRESULT CInstancingModelComp::Render()
{


	return S_OK;
}

CInstancingModelComp* CInstancingModelComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CInstancingModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CInstancingModelComp* CInstancingModelComp::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CInstancingModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CInstancingModelComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("CInstancingModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

CComponent* CInstancingModelComp::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CInstancingModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CInstancingModelComp::Free()
{
	SUPER::Free();

	Safe_Release(m_pEffectComp);
	Safe_Release(m_pTextureComp);
	Safe_Release(m_pVIBufferComp);
}

FSerialData CInstancingModelComp::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	return Data;
}

FSerialData CInstancingModelComp::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	return Data;
}

HRESULT CInstancingModelComp::Create_InstanceVIBuffers(_uint iNumInstance, CInstancingBufferComp::INSTANCING_DESC Desc)
{
	// VIBufferComp의 VI와 Instance버퍼를 생성한다.
	if (nullptr == m_pVIBufferComp)
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Create_VIBuffers(iNumInstance)))
		return E_FAIL;

	if (FAILED(m_pVIBufferComp->Create_InstanceBuffer(Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancingModelComp::Load_Texture(const wstring& strPath, _uint iNumTextures)
{
	if (nullptr == m_pTextureComp)
		return E_FAIL;

	m_pTextureComp->Bind_Texture(strPath, iNumTextures);

	return S_OK;
}

HRESULT CInstancingModelComp::Bind_TextureToEffect(const _char* pConstantName, _uint iIndex)
{
	if (FAILED(m_pTextureComp->Bind_SRVToEffect(m_pEffectComp, pConstantName, iIndex)))
		return E_FAIL;

	return S_OK;
}

HRESULT CInstancingModelComp::BindAndRender_Buffer(_uint iPass)
{
	if (FAILED(m_pEffectComp->Begin(iPass)))
		return E_FAIL;
	if (FAILED(m_pVIBufferComp->Bind_Buffer()))
		return E_FAIL;
	if (FAILED(m_pVIBufferComp->Render_Buffer()))
		return E_FAIL;

	return S_OK;
}
