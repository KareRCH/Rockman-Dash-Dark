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
}

CItemChest::CItemChest(const CItemChest& rhs)
{
}

HRESULT CItemChest::Initialize_Prototype()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	return S_OK;
}

HRESULT CItemChest::Initialize_Prototype(const _float3 vPos)
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	Transform().Set_Position(vPos);

	return S_OK;
}

HRESULT CItemChest::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CItemChest::Initialize(const _float3 vPos)
{
	Transform().Set_Position(vPos);

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

		m_pModelComp->Set_Animation(2, true);
	}

	m_pModelComp->Invalidate_Animation();
	m_pModelComp->Invalidate_BoneTransforms();
}

void CItemChest::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CItemChest::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

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

CItemChest* CItemChest::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
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

void CItemChest::Free()
{
	SUPER::Free();
}

HRESULT CItemChest::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Object/Chest/Chest.amodel");

	return S_OK;
}
