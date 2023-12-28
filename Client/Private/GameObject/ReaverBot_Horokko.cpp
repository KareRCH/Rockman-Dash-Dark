#include "GameObject/ReaverBot_Horokko.h"

#include "Component/TriBufferComp.h"
#include "Component/ColorShaderComp.h"
#include "Component/SkinnedModelComp.h"
#include "Component/ModelShaderComp.h"
#include "Component/ModelBufferComp.h"
#include "Component/CommonModelComp.h"

CReaverBot_Horokko::CReaverBot_Horokko()
{
	Set_Name(TEXT("ItemChest"));
}

CReaverBot_Horokko::CReaverBot_Horokko(const CReaverBot_Horokko& rhs)
{
}

HRESULT CReaverBot_Horokko::Initialize_Prototype()
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize_Prototype(const _float3 vPos)
{
	if (FAILED(Initialize_Component()))
		return E_FAIL;

	TurnOn_State(EGObjectState::Render);            // 렌더링 유무, Tick은 작동함, 주의ㅋ

	Transform().Set_Position(vPos);
	m_pModelComp->Set_Animation(0, 1.f, true);

	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(void* Arg)
{
	return S_OK;
}

HRESULT CReaverBot_Horokko::Initialize(const _float3 vPos)
{
	Transform().Set_Position(vPos);

	return S_OK;
}

void CReaverBot_Horokko::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CReaverBot_Horokko::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

	if (GI()->IsKey_Pressed(DIK_R))
	{
		++m_iTest;
		if (m_iTest > 7)
			m_iTest = 0;

		m_pModelComp->Set_Animation(m_iTest, 1.f, true);
	}

	m_pModelComp->Add_AnimTime(fTimeDelta);
	m_pModelComp->Invalidate_Animation();
	m_pModelComp->Invalidate_BoneTransforms();
}

void CReaverBot_Horokko::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	m_pModelComp->Late_Tick(fTimeDelta);
}

HRESULT CReaverBot_Horokko::Render()
{
	SUPER::Render();

	m_pModelComp->Render();

	return S_OK;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CReaverBot_Horokko* CReaverBot_Horokko::Create(const _float3 vPos)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(vPos)))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CReaverBot_Horokko::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CUI_Player Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CGameObject*>(pInstance);
}

void CReaverBot_Horokko::Free()
{
	SUPER::Free();
}

HRESULT CReaverBot_Horokko::Initialize_Component()
{
	FAILED_CHECK_RETURN(Add_Component(L"Model", m_pModelComp = CCommonModelComp::Create()), E_FAIL);

	m_pModelComp->Transform().Set_Scale(_float3(0.1f, 0.1f, 0.1f));
	m_pModelComp->Bind_Effect(L"Runtime/FX_ModelTest.hlsl", SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
	m_pModelComp->Bind_Model(CCommonModelComp::TYPE_ANIM, EModelGroupIndex::Permanent, L"Model/Character/Reaverbots/Horokko/Horokko.amodel");

	return S_OK;
}
