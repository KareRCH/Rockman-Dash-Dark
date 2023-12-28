#include "Component/CommonModelComp.h"

#include "System/Data/ModelData.h"
#include "Component/MeshComp.h"
#include "Component/MaterialComponent.h"
#include "Component/SkeletalComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/EffectComponent.h"

CCommonModelComp::CCommonModelComp()
{
	NULL_CHECK(m_pEffectComp = CEffectComponent::Create());
}

CCommonModelComp::CCommonModelComp(const CCommonModelComp& rhs)
    : Base(rhs)
	, m_pEffectComp(rhs.m_pEffectComp)
	, m_pAnimationComp(rhs.m_pAnimationComp)
	, m_pMeshComps(rhs.m_pMeshComps)
	, m_pMaterialComps(rhs.m_pMaterialComps)
{
	NULL_CHECK(m_pSkeletalComp = Cast<CSkeletalComponent*>(rhs.m_pSkeletalComp->Clone()));
}

HRESULT CCommonModelComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

    return S_OK;
}

HRESULT CCommonModelComp::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

    return S_OK;
}

void CCommonModelComp::Priority_Tick(const _float& fTimeDelta)
{
	SUPER::Priority_Tick(fTimeDelta);

}

void CCommonModelComp::Tick(const _float& fTimeDelta)
{
	SUPER::Tick(fTimeDelta);

}

void CCommonModelComp::Late_Tick(const _float& fTimeDelta)
{
	SUPER::Late_Tick(fTimeDelta);

	if (m_eModelType == TYPE_ANIM)
	{
		if (nullptr != m_pSkeletalComp)
			m_pSkeletalComp->Invalidate_BoneTransforms();
	}
}

HRESULT CCommonModelComp::Render()
{
	// ���⼭ ���̴��� ��� ������ �Ѱ��ִ� ������ �Ѵ�.
	switch (m_eModelType)
	{
	case TYPE_NONANIM:
		if (FAILED(Render_NoAnimModel()))
			return E_FAIL;
		break;
	case TYPE_ANIM:
		if (FAILED(Render_AnimModel()))
			return E_FAIL;
		break;
	}

    return S_OK;
}

CCommonModelComp* CCommonModelComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CommonModelComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CCommonModelComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(Arg)))
	{
		MSG_BOX("CommonModelComp Copy Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CCommonModelComp::Free()
{
	SUPER::Free();

	for (auto& MeshComp : m_pMeshComps)
		Safe_Release(MeshComp);

	for (auto& MaterComp : m_pMaterialComps)
		Safe_Release(MaterComp);

	Safe_Release(m_pSkeletalComp);
	Safe_Release(m_pAnimationComp);
	Safe_Release(m_pEffectComp);
}

HRESULT CCommonModelComp::Render_AnimModel()
{
	LIGHT_BUFFER_T lightBuffer = {
		_float4(0.2f, 0.2f, 0.2f, 1.f),
		_float4(0.2f, 0.2f, 0.2f, 1.f),
		_float3(0.f, -0.2f, 1.f),
		_float(50.f), _float4(1.f, 0.2f, 0.2f, 1.f)
	};


	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		_float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
		_vector vTemp = {};

		m_pEffectComp->Bind_Matrix("g_matWorld", &matTemp);
		m_pEffectComp->Bind_Matrix("g_matView", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		m_pEffectComp->Bind_Matrix("g_matProj", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));
		m_pMeshComps[i]->Bind_BoneMatricesToEffect(m_pEffectComp, "g_matBones", *m_pSkeletalComp->Get_BoneGroup());

		m_pEffectComp->Bind_RawValue("g_vCamPosition", VPCast(&(vTemp = PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One))), sizeof(_float4));

		_uint iMatIndex = m_pMeshComps[i]->Get_MeshMaterialIndex();
		m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, "g_texDiffuse", aiTextureType_DIFFUSE);

		// �׸��� ����
		m_pEffectComp->Begin(0);

		// ���۸� ��ġ�� ���ε�
		m_pMeshComps[i]->Bind_Buffer();

		// ���ε��� ����, �ε��� �׸���
		m_pMeshComps[i]->Render_Buffer();
	}

	return S_OK;
}

HRESULT CCommonModelComp::Render_NoAnimModel()
{
	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		_float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
		_vector vTemp = {};

		m_pEffectComp->Bind_Matrix("g_matWorld", &matTemp);
		m_pEffectComp->Bind_Matrix("g_matView", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		m_pEffectComp->Bind_Matrix("g_matProj", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));

		m_pEffectComp->Bind_RawValue("g_vCamPosition", VPCast(&(vTemp = PipelineComp().Get_CamPositionVector(ECamType::Persp, ECamNum::One))), sizeof(_float4));

		_uint iMatIndex = m_pMeshComps[i]->Get_MeshMaterialIndex();
		m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, "g_texDiffuse", aiTextureType_DIFFUSE);

		// �׸��� ����
		m_pEffectComp->Begin(0);

		// ���۸� ��ġ�� ���ε�
		m_pMeshComps[i]->Bind_Buffer();

		// ���ε��� ����, �ε��� �׸���
		m_pMeshComps[i]->Render_Buffer();
	}

	return S_OK;
}

HRESULT CCommonModelComp::Link_ToModelMgr()
{
	if (nullptr == m_pGI)
		return E_FAIL;

	// �� �Ŵ����� ���� ���������� �ʰ�, ���� �ν��Ͻ��� ����Ǵ����� Ȯ���Ѵ�.

	return S_OK;
}

HRESULT CCommonModelComp::Bind_Model(TYPE eType, EModelGroupIndex eGroupIndex, const wstring& strModelFilePath)
{
	if (FAILED(Link_ToModelMgr()))
		return E_FAIL;

	// �ε� ���� �̹� �ִ� �ε� �����ߴ� S_OK�� ��ȯ. �����ϸ� ������ ���� ��.
	if (FAILED(m_pGI->Load_Model(eGroupIndex, strModelFilePath)))
		return E_FAIL;

	m_eModelType = eType;

	const FModelData* pModelData = m_pGI->Find_ModelData(eGroupIndex, strModelFilePath);
	// �ε� �����̸� ������ ã����, Ȥ�� �� ���׸� ���� ���� �ڵ�
	// �ִٸ� �����ϰ� ���� ������Ʈ���� �Ŵ����κ��� �����͸� ���� �� �ִٴ� �����̴�.
	if (nullptr == pModelData)
		return E_FAIL;

	m_iNumMeshes = pModelData->pMeshGroup->Get_NumMeshes();
	m_pMeshComps.reserve(m_iNumMeshes);
	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		m_pMeshComps.push_back(CMeshComp::Create());
		m_pMeshComps[i]->Load_Mesh(eGroupIndex, strModelFilePath, i);
	}

	m_iNumMaterials = pModelData->pMaterialGroup->Get_NumMaterials();
	m_pMaterialComps.reserve(m_iNumMaterials);
	for (_uint i = 0; i < m_iNumMaterials; i++)
	{
		m_pMaterialComps.push_back(CMaterialComponent::Create());
		m_pMaterialComps[i]->Load_Materials(eGroupIndex, strModelFilePath, i);
	}

	// �ִϸ��̼� Ÿ���̸� ���� �ִϸ��̼��� �ε��Ѵ�.
	if (eType == TYPE_ANIM)
	{
		NULL_CHECK_RETURN(m_pSkeletalComp = CSkeletalComponent::Create(), E_FAIL);
		NULL_CHECK_RETURN(m_pAnimationComp = CAnimationComponent::Create(), E_FAIL);

		m_pSkeletalComp->Load_Skeletal(eGroupIndex, strModelFilePath);
		m_pAnimationComp->Bind_BoneGroup(m_pSkeletalComp->Get_BoneGroup());
		m_pAnimationComp->Load_Animations(eGroupIndex, strModelFilePath);
	}

	return S_OK;
}

void CCommonModelComp::Add_MaskTime(_uint iIndex, _float fAddTrackPos)
{
	if (nullptr == m_pAnimationComp)
		return;

	//m_pAnimationComp->Apply_MaskTime(iIndex, fAddTrackPos);
}

void CCommonModelComp::Apply_Pos()
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Apply_FinalMask();
}

void CCommonModelComp::Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse)
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Set_Animation(iAnimIndex, fSpeedMultiply, bIsLoop, bReverse);
}

void CCommonModelComp::Add_AnimTime(const _float& fTimeDelta)
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Add_AnimTime(fTimeDelta);
}

void CCommonModelComp::Invalidate_Animation()
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Invalidate_Animation();
}

void CCommonModelComp::Invalidate_BoneTransforms()
{
}

HRESULT CCommonModelComp::Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	if (m_pEffectComp == nullptr)
		return E_FAIL;

	return m_pEffectComp->Bind_Effect(strEffectKey, pElements, iNumElements);
}

HRESULT CCommonModelComp::Unbind_Effect()
{
	if (m_pEffectComp == nullptr)
		return E_FAIL;

	return m_pEffectComp->Unbind_Effect();
}
