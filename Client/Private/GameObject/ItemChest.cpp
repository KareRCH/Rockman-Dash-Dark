#include "GameObject/ItemChest.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"


CItemChest::CItemChest()
{
	Set_Name(TEXT("ItemChest"));
	Set_RenderGroup(ERenderGroup::NonBlend);
}

CItemChest::CItemChest(const CItemChest& rhs)
{
}

HRESULT CItemChest::Initialize_Prototype()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItemChest::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);
	m_pModelComp->Transform().Set_Scale(_float3(0.3f, 0.3f, 0.3f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Chest/Chest.amodel");

	return S_OK;
}

HRESULT CItemChest::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CItemChest::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
		return E_FAIL;
	if (FAILED(Initialize_Component(InputData)))
		return E_FAIL;

	return S_OK;
}

void CItemChest::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CItemChest::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (GI()->IsKey_Pressed(DIK_R))
	{
		++m_iTest;
		if (m_iTest > 3)
			m_iTest = 0;

		m_pModelComp->Set_Animation(m_iTest, 1.f, true);
	}
}

void CItemChest::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CItemChest::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

#ifdef _DEBUG
	GI()->Add_DebugEvent(MakeDelegate(m_pColliderComp, &CColliderComponent::Render));
#endif

	return S_OK;
}

CItemChest* CItemChest::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CItemChest* CItemChest::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItemChest::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

CGameObject* CItemChest::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CItemChest::Free()
{
	SUPER::Free();
}

HRESULT CItemChest::Initialize_Component()
{
	

	return S_OK;
}

HRESULT CItemChest::Initialize_Component(FSerialData& InputData)
{
	return E_NOTIMPL;
}

FSerialData CItemChest::SerializeData_Prototype()
{
	return FSerialData();
}

FSerialData CItemChest::SerializeData()
{
	return FSerialData();
}
