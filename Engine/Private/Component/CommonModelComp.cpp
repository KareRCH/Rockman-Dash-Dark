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
	, m_pMeshComps(rhs.m_pMeshComps)
	, m_pMaterialComps(rhs.m_pMaterialComps)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_eModelType(rhs.m_eModelType)
	, m_ActiveMeshes(rhs.m_ActiveMeshes)
	, m_iNumActiveMeshes(rhs.m_iNumActiveMeshes)
{
	Safe_AddRef(m_pEffectComp);
	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		Safe_AddRef(m_pMeshComps[i]);
	}
	for (_uint i = 0; i < m_iNumMaterials; i++)
	{
		Safe_AddRef(m_pMaterialComps[i]);
	}
	
	if (m_eModelType == TYPE_ANIM)
	{
		NULL_CHECK(m_pSkeletalComp = Cast<CSkeletalComponent*>(rhs.m_pSkeletalComp->Clone()));
		NULL_CHECK(m_pAnimationComp = Cast<CAnimationComponent*>(rhs.m_pAnimationComp->Clone()));
		m_pAnimationComp->Bind_BoneGroup(m_pSkeletalComp->Get_BoneGroup());
	}
}

HRESULT CCommonModelComp::Initialize_Prototype(void* Arg)
{
	if (FAILED(__super::Initialize_Prototype(Arg)))
		return E_FAIL;

    return S_OK;
}

HRESULT CCommonModelComp::Initialize_Prototype(FSerialData& InputData)
{
	if (FAILED(__super::Initialize_Prototype(InputData)))
		return E_FAIL;

	_uint iModelType;
	if (FAILED(InputData.Get_Data("ModelType", iModelType)))
		return E_FAIL;
	m_eModelType = Cast<TYPE>(iModelType);

	string strModelPath;
	if (FAILED(InputData.Get_Data("ModelPath", strModelPath)))
		return E_FAIL;
	 
	if (FAILED(Bind_Model(m_eModelType, EModelGroupIndex::Permanent, ConvertToWstring(strModelPath))))
		return E_FAIL;

	string strEffectPath;
	if (FAILED(InputData.Get_Data("EffectPath", strEffectPath)))
		return E_FAIL;

	switch (m_eModelType)
	{
	case TYPE_ANIM:
		Bind_Effect(ConvertToWstring(strEffectPath), SHADER_VTX_SKINMODEL::Elements, SHADER_VTX_SKINMODEL::iNumElements);
		break;
	case TYPE_NONANIM:
		Bind_Effect(ConvertToWstring(strEffectPath), SHADER_VTX_MODEL::Elements, SHADER_VTX_MODEL::iNumElements);
		break;
	}

	return S_OK;
}

HRESULT CCommonModelComp::Initialize(void* Arg)
{
	if (FAILED(__super::Initialize(Arg)))
		return E_FAIL;

    return S_OK;
}

HRESULT CCommonModelComp::Initialize(FSerialData& InputData)
{
	if (FAILED(__super::Initialize(InputData)))
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

CCommonModelComp* CCommonModelComp::Create(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype(InputData)))
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

CComponent* CCommonModelComp::Clone(FSerialData& InputData)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize(InputData)))
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

FSerialData CCommonModelComp::SerializeData_Prototype()
{
	FSerialData Data = SUPER::SerializeData_Prototype();

	Data.Add_Member("ComponentID", g_ClassID);
	Data.Add_Member("ModelType", Cast<_uint>(m_eModelType));
	Data.Add_MemberString("ModelPath", ConvertToString(m_strModelPath));
	Data.Add_MemberString("EffectPath", ConvertToString(m_strEffectPath));

	return Data;
}

FSerialData CCommonModelComp::SerializeData()
{
	FSerialData Data = SUPER::SerializeData();

	Data.Add_Member("ComponentID", g_ClassID);
	Data.Add_Member("ModelType", Cast<_uint>(m_eModelType));
	Data.Add_MemberString("ModelPath", ConvertToString(m_strModelPath));
	Data.Add_MemberString("EffectPath", ConvertToString(m_strEffectPath));


	return Data;
}

HRESULT CCommonModelComp::Render_AnimModel()
{
	for (_uint i = 0; i < m_iNumActiveMeshes; ++i)
	{
		_uint iIndex = m_ActiveMeshes[i];

		_float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
		_float4 TempFloat4 = {};

		m_pEffectComp->Bind_Matrix("g_WorldMatrix", &matTemp);
		m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));
		m_pMeshComps[iIndex]->Bind_BoneMatricesToEffect(m_pEffectComp, "g_BoneMatrices", *m_pSkeletalComp->Get_BoneGroup());

		m_pEffectComp->Bind_RawValue("g_vCamPosition", VPCast(&(TempFloat4 = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One))), sizeof(_float4));

		_uint iMatIndex = m_pMeshComps[iIndex]->Get_MeshMaterialIndex();
		m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, "g_DiffuseTexture", aiTextureType_DIFFUSE);

		auto pOwnerObj = Get_OwnerObject();
		if (nullptr != pOwnerObj)
		{
			_int iTemp = -1;
			m_pEffectComp->Bind_RawValue("g_iObjectID", &(iTemp = pOwnerObj->Get_ID()), sizeof(_int));
		}

		for (_uint j = 0; j < m_iNumActivePasses; j++)
		{
			// �׸��� ����
			m_pEffectComp->Begin(m_vecActivePasses[j]);

			// ���۸� ��ġ�� ���ε�
			m_pMeshComps[iIndex]->Bind_Buffer();

			// ���ε��� ����, �ε��� �׸���
			m_pMeshComps[iIndex]->Render_Buffer();
		}
	}

	return S_OK;
}

HRESULT CCommonModelComp::Render_NoAnimModel()
{
	for (_uint i = 0; i < m_iNumActiveMeshes; ++i)
	{
		_uint iIndex = m_ActiveMeshes[i];

		_float4x4 matTemp = Calculate_TransformFloat4x4FromParent();
		_float4 TempFloat4 = {};

		m_pEffectComp->Bind_Matrix("g_WorldMatrix", &matTemp);
		m_pEffectComp->Bind_Matrix("g_ViewMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::View, ECamNum::One)));
		m_pEffectComp->Bind_Matrix("g_ProjMatrix", &(matTemp = PipelineComp().Get_CamFloat4x4(ECamType::Persp, ECamMatrix::Proj, ECamNum::One)));

		m_pEffectComp->Bind_RawValue("g_vCamPosition", VPCast(&(TempFloat4 = PipelineComp().Get_CamPositionFloat4(ECamType::Persp, ECamNum::One))), sizeof(_float4));

		_uint iMatIndex = m_pMeshComps[iIndex]->Get_MeshMaterialIndex();
		m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, "g_DiffuseTexture", aiTextureType_DIFFUSE);

		auto pOwnerObj = Get_OwnerObject();
		if (nullptr != pOwnerObj)
		{
			_int iTemp = -1;
			m_pEffectComp->Bind_RawValue("g_iObjectID", &(iTemp = pOwnerObj->Get_ID()), sizeof(_int));
		}

		for (_uint j = 0; j < m_iNumActivePasses; j++)
		{
			// �׸��� ����
			m_pEffectComp->Begin(m_vecActivePasses[j]);

			// ���۸� ��ġ�� ���ε�
			m_pMeshComps[iIndex]->Bind_Buffer();

			// ���ε��� ����, �ε��� �׸���
			m_pMeshComps[iIndex]->Render_Buffer();
		}
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
	m_strModelPath = strModelFilePath;

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

	Active_AllMeshes();

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

void CCommonModelComp::Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse, _float fTransitionSpeed)
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Set_Animation(iAnimIndex, fSpeedMultiply, bIsLoop, bReverse, fTransitionSpeed);
}

void CCommonModelComp::Set_AnimationMaintain(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse, _float fTransitionSpeed)
{
	if (nullptr == m_pAnimationComp)
		return;

	m_pAnimationComp->Set_AnimationMaintain(iAnimIndex, fSpeedMultiply, bIsLoop, bReverse, fTransitionSpeed);
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

	m_strEffectPath = strEffectKey;

	return m_pEffectComp->Bind_Effect(strEffectKey, pElements, iNumElements);
}

HRESULT CCommonModelComp::Unbind_Effect()
{
	if (m_pEffectComp == nullptr)
		return E_FAIL;

	return m_pEffectComp->Unbind_Effect();
}

void CCommonModelComp::Active_Mesh(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumMeshes)
		return;

	auto iter = find_if(m_ActiveMeshes.begin(), m_ActiveMeshes.end(),
		[&iIndex](const _uint iActive) {
			return iIndex == iActive;
		});
	if (iter != m_ActiveMeshes.end())
		return;

	m_ActiveMeshes.push_back(iIndex);
	++m_iNumActiveMeshes;
}

void CCommonModelComp::Active_AllMeshes()
{
	m_iNumActiveMeshes = 0;
	m_ActiveMeshes.clear();
	for (_uint i = 0; i < m_iNumMeshes; i++)
	{
		m_ActiveMeshes.push_back(i);
		++m_iNumActiveMeshes;
	}
}

void CCommonModelComp::Deactive_Mesh(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumMeshes)
		return;

	auto iter = find_if(m_ActiveMeshes.begin(), m_ActiveMeshes.end(),
		[&iIndex](const _uint iActive) {
			return iIndex == iActive;
		});
	if (iter == m_ActiveMeshes.end())
		return;

	m_ActiveMeshes.erase(iter);
	--m_iNumActiveMeshes;
}

void CCommonModelComp::Deactive_AllMeshes()
{
	m_iNumActiveMeshes = 0;
	m_ActiveMeshes.clear();
}

void CCommonModelComp::BindAndRender_Mesh(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_iNumMeshes)
		return;

	// ���۸� ��ġ�� ���ε�
	m_pMeshComps[iIndex]->Bind_Buffer();

	// ���ε��� ����, �ε��� �׸���
	m_pMeshComps[iIndex]->Render_Buffer();
}

void CCommonModelComp::BindAndRender_Meshes(_uint iPass, _bool bIsUseBones)
{
	if (iPass < 0)
		return;

	for (_uint i = 0; i < m_iNumActiveMeshes; ++i)
	{
		_uint iIndex = m_ActiveMeshes[i];

		if (bIsUseBones)
			m_pMeshComps[iIndex]->Bind_BoneMatricesToEffect(m_pEffectComp, "g_BoneMatrices", *m_pSkeletalComp->Get_BoneGroup());

		_uint iMatIndex = m_pMeshComps[iIndex]->Get_MeshMaterialIndex();
		m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, "g_DiffuseTexture", aiTextureType_DIFFUSE);

		m_pEffectComp->Begin(iPass);

		// ���۸� ��ġ�� ���ε�
		m_pMeshComps[iIndex]->Bind_Buffer();

		// ���ε��� ����, �ε��� �׸���
		m_pMeshComps[iIndex]->Render_Buffer();
	}

	
}

void CCommonModelComp::Bind_MeshMaterial(_uint iMeshIndex, aiTextureType eType, const _char* pConstName)
{
	if (iMeshIndex < 0 || iMeshIndex >= m_iNumMeshes)
		return;

	_uint iMatIndex = m_pMeshComps[iMeshIndex]->Get_MeshMaterialIndex();
	m_pMaterialComps[iMatIndex]->Bind_TextureToEffect(m_pEffectComp, pConstName, eType);
}

_float4x4 CCommonModelComp::Get_BoneTransformFloat4x4WithParents(_uint iIndex)
{
	if (!m_pSkeletalComp)
		return _float4x4();

	_matrix CompMatrix = Calculate_TransformMatrixFromParent();
	CompMatrix = m_pSkeletalComp->Get_BoneTransformMatrix(iIndex) * CompMatrix;

	_float4x4 ResultFloat4x4 = {};
	XMStoreFloat4x4(&ResultFloat4x4, CompMatrix);

	return ResultFloat4x4;
}

_matrix CCommonModelComp::Get_BoneTransformMatrixWithParents(_uint iIndex)
{
	if (!m_pSkeletalComp)
		return _matrix();

	_matrix CompMatrix = Calculate_TransformMatrixFromParent();
	CompMatrix = m_pSkeletalComp->Get_BoneTransformMatrix(iIndex) * CompMatrix;

	return CompMatrix;
}
