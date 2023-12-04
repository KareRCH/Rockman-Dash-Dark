#include "Component/AnimationComponent.h"

#include "System/Data/BoneAnimData.h"
#include "System/ModelMgr.h"

CAnimationComponent::CAnimationComponent(const CAnimationComponent& rhs)
	: Base(rhs)
	, m_pModelData(rhs.m_pModelData)
	, m_pAnimGroup(rhs.m_pAnimGroup)
{
	Safe_AddRef(m_pModelData);
	Safe_AddRef(m_pAnimGroup);
}

HRESULT CAnimationComponent::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CAnimationComponent::Initialize(void* Arg)
{
	return S_OK;
}

CAnimationComponent* CAnimationComponent::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CAnimationComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

CComponent* CAnimationComponent::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CAnimationComponent Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return Cast<CComponent*>(pInstance);
}

void CAnimationComponent::Free()
{
	Safe_Release(m_pModelData);
	Safe_Release(m_pAnimGroup);
}

void CAnimationComponent::Set_ModelData(FModelData* pModelData)
{
	m_pModelData = pModelData;
	Safe_AddRef(m_pModelData);

	Bind_AnimGroup();
}

HRESULT CAnimationComponent::Bind_AnimGroup()
{
	if (!m_pModelData)
		return E_FAIL;

	m_pAnimGroup = m_pModelData->pAnimGroup;
	Safe_AddRef(m_pAnimGroup);

	return S_OK;
}

HRESULT CAnimationComponent::Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive)
{
	if (!m_pModelData)
		return E_FAIL;

	FSkeletalData* pSkeletal = m_pModelData->pBoneGroup->Find_SkeletalData(strSkeletalName);
	if (!pSkeletal)
		return E_FAIL;

	// �⺻ ����
	FAnimMask tMask = {};
	tMask.strName = strMaskName;
	tMask.fWeight = 1.f;
	tMask.vecBoneMasks.resize(pSkeletal->Get_BoneNodeData_Count(), bInitBoneActive);
	tMask.strSkeletaName = strSkeletalName;

	m_vecAnimMask.push_back(tMask);

	return S_OK;
}

FAnimMask* CAnimationComponent::Get_Mask(_uint iIndex)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return nullptr;

	return &m_vecAnimMask[iIndex];
}

void CAnimationComponent::Deactive_BoneMask(_uint iIndex, const wstring& strBoneName)
{
	if (!m_pModelData)
		return;

	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FSkeletalData* pSkeletal = m_pModelData->pBoneGroup->Find_SkeletalData(m_vecAnimMask[iIndex].strSkeletaName);
	if (!pSkeletal)
		return;

	FBoneNodeData* pBoneData = pSkeletal->Find_BoneNodeData(strBoneName);
	if (!pBoneData)
		return;

	m_vecAnimMask[iIndex].vecBoneMasks[pBoneData->iID] = false;
}

void CAnimationComponent::Active_BoneMask(_uint iIndex, const wstring& strBoneName)
{
	if (!m_pModelData)
		return;

	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FSkeletalData* pSkeletal = m_pModelData->pBoneGroup->Find_SkeletalData(m_vecAnimMask[iIndex].strSkeletaName);
	if (!pSkeletal)
		return;

	FBoneNodeData* pBoneData = pSkeletal->Find_BoneNodeData(strBoneName);
	if (!pBoneData)
		return;

	m_vecAnimMask[iIndex].vecBoneMasks[pBoneData->iID] = true;
}

void CAnimationComponent::Set_MaskAnimation(_uint iIndex, const wstring& strAnimName)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	m_vecAnimMask[iIndex].strAnimName = strAnimName;
}

void CAnimationComponent::Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	m_vecAnimMask[iIndex].fCurTime = fCurTime;
	m_vecAnimMask[iIndex].strAnimName = strAnimName;
}

void CAnimationComponent::Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime)
{
	_uint iIndex = 0;

	_bool bFound = false;
	for (_uint i = 0; i < m_vecAnimMask.size(); i++)
	{
		if (m_vecAnimMask[i].strName == strMaskName)
		{
			iIndex = i;
			bFound = true;
			break;
		}
	}

	if (!bFound)
		return;

	m_vecAnimMask[iIndex].fCurTime = fCurTime;
	m_vecAnimMask[iIndex].strAnimName = strAnimName;
}

void CAnimationComponent::Apply_FinalMask()
{
	if (!m_pModelData || !m_pAnimGroup)
		return;

	if (m_vecAnimMask.empty())
		return;
	
	// ���� ����ġ
	vector<_float> vecBoneRemainWeights;
	vector<_float4x4> vecBoneMatrices;

	// ���� ���� ����ġ ����� �Ѵ�.
	for (_int i = Cast<_int>(m_vecAnimMask.size() - 1); i >= 0; --i)
	{
		FAnimMask& rAnimMask = m_vecAnimMask[i];

		if (vecBoneRemainWeights.empty())
		{
			vecBoneRemainWeights.resize(rAnimMask.vecBoneMasks.size(), 0.f);
			vecBoneMatrices.resize(rAnimMask.vecBoneMasks.size(), {});
		}

		// �ִϸ��̼��� ������ �ǳʶٱ�
		if (rAnimMask.strAnimName.empty())
			continue;

		for (_uint j = 0; j < rAnimMask.vecBoneMasks.size(); j++)
		{
			// ���� ���̾��� ����ũ�� �켱������ �����ȴ�.
			// min �Լ��� ����ؼ� �����ִ� �����θ� ����Ʈ�� ���������� �����Ѵ�.
			_float fWeight = Cast<_float>(rAnimMask.vecBoneMasks[j]) * rAnimMask.fWeight;
			fWeight = min(fWeight, (1.f - vecBoneRemainWeights[j]));						// 1�̻����� �ö��� �ʰ� ����

			// ���̽� ����ũ��� ���� ���� �� ���������� �Ѵ�.
			if (i == 0)
				vecBoneRemainWeights[j] = fWeight;
			else
				vecBoneRemainWeights[j] = max((1.f - vecBoneRemainWeights[j]) - fWeight, 0.f);	// 0���Ϸ� �������� �ʰ� ����
		}

		const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_AnimData(rAnimMask.strAnimName);
		_float fCurAnimTime = pBoneAnimData->Calculate_Time(m_fTickDeltaTime, rAnimMask.fCurTime);	// ��ȯ�� �ð� ��

		// ��� ���
		for (_uint j = 0; j < rAnimMask.vecBoneMasks.size(); j++)
		{
			const FBoneAnimNodeData* pBoneAnimNodeData= pBoneAnimData->Find_AnimNodeData(i);
			vecBoneMatrices[j] = pBoneAnimNodeData->Interporated_Matrix(fCurAnimTime);

		}

		FSkeletalData* pSkeletal = ConCast<FSkeletalData*>(m_pModelData->pBoneGroup->Find_SkeletalData(rAnimMask.strSkeletaName));
		if (i == 0)
			pSkeletal->Clear_FinalTransforms();
		pSkeletal->Add_Transforms(&vecBoneMatrices);
	}
}

void CAnimationComponent::Apply_BoneAnimationWithMask()
{
	if (!m_pModelData)
		return;

	for (_uint i = 0; i < m_pModelData->pBoneGroup->Get_Skeletal_Count(); i++)
	{
		
	}
}
