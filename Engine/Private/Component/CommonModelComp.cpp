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
	// 여기서 셰이더에 모든 정보를 넘겨주는 행위를 한다.
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

		// 그리기 시작
		m_pEffectComp->Begin(0);

		// 버퍼를 장치에 바인드
		m_pMeshComps[i]->Bind_Buffer();

		// 바인딩된 정점, 인덱스 그리기
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

		// 그리기 시작
		m_pEffectComp->Begin(0);

		// 버퍼를 장치에 바인드
		m_pMeshComps[i]->Bind_Buffer();

		// 바인딩된 정점, 인덱스 그리기
		m_pMeshComps[i]->Render_Buffer();
	}

	return S_OK;
}

HRESULT CCommonModelComp::Link_ToModelMgr()
{
	if (nullptr == m_pGI)
		return E_FAIL;

	// 모델 매니저에 직접 접근하지는 않고, 게임 인스턴스에 연결되는지만 확인한다.

	return S_OK;
}

HRESULT CCommonModelComp::Bind_Model(TYPE eType, EModelGroupIndex eGroupIndex, const wstring& strModelFilePath)
{
	if (FAILED(Link_ToModelMgr()))
		return E_FAIL;

	// 로드 모델은 이미 있던 로드 성공했던 S_OK를 반환. 실패하면 파일이 없는 것.
	if (FAILED(m_pGI->Load_Model(eGroupIndex, strModelFilePath)))
		return E_FAIL;

	m_eModelType = eType;

	const FModelData* pModelData = m_pGI->Find_ModelData(eGroupIndex, strModelFilePath);
	// 로드 성공이면 무조건 찾지만, 혹시 모를 버그를 위해 안전 코드
	// 있다면 안전하게 각기 컴포넌트들이 매니저로부터 데이터를 얻어올 수 있다는 증거이다.
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

	// 애니메이션 타입이면 뼈와 애니메이션을 로드한다.
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
