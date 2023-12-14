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

	FBoneGroup* pBoneGroup = m_pModelData->pBoneGroup;
	if (!pBoneGroup)
		return E_FAIL;

	// 기본 세팅
	FAnimMask tMask = {};
	tMask.strName = strMaskName;
	tMask.fWeight = 1.f;
	tMask.vecBoneMasks.resize(pBoneGroup->Get_BoneDatas_Count(), bInitBoneActive);
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

	FBoneGroup* pBoneGroup = m_pModelData->pBoneGroup;
	if (!pBoneGroup)
		return;

	FBoneData* pBoneData = pBoneGroup->Find_BoneData(strBoneName);
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

	FBoneGroup* pBoneGroup = m_pModelData->pBoneGroup;
	if (!pBoneGroup)
		return;

	FBoneData* pBoneData = pBoneGroup->Find_BoneData(strBoneName);
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

void CAnimationComponent::Set_MaskTime(_uint iIndex, _float fTime)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	m_vecAnimMask[iIndex].fCurTime = fTime;
}

void CAnimationComponent::Set_TickDeltaTime(_float fDeltaTime)
{
	m_fSystemTPS = fDeltaTime;
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
	
	// 남은 가중치
	vector<_float> vecBoneRemainWeights;
	vector<_float4x4> vecBoneMatrices;

	// 뼈에 대한 가중치 계산을 한다.
	for (_int i = Cast<_int>(m_vecAnimMask.size() - 1); i >= 0; --i)
	{
		FAnimMask& rAnimMask = m_vecAnimMask[i];

		if (vecBoneRemainWeights.empty())
		{
			vecBoneRemainWeights.resize(rAnimMask.vecBoneMasks.size(), 0.f);
			vecBoneMatrices.resize(rAnimMask.vecBoneMasks.size(), {});
		}

		// 애니메이션이 없으면 건너뛰기
		if (rAnimMask.strAnimName.empty())
			continue;

		for (_uint j = 0; j < rAnimMask.vecBoneMasks.size(); j++)
		{
			// 상위 레이어의 마스크가 우선적으로 조정된다.
			// min 함수를 사용해서 남아있는 값으로만 웨이트가 정해지도록 조정한다.
			_float fWeight = Cast<_float>(rAnimMask.vecBoneMasks[j]) * rAnimMask.fWeight;
			fWeight = min(fWeight, (1.f - vecBoneRemainWeights[j]));	// 1이상으로 올라가지 않게 보정

			// 베이스 마스크라면 남은 값을 다 가져가도록 한다.
			if (i == 0)
				vecBoneRemainWeights[j] = fWeight;
			else
				vecBoneRemainWeights[j] = max((1.f - vecBoneRemainWeights[j]) - fWeight, 0.f);	// 0이하로 떨어지지 않게 보정
		}

		const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_BoneAnim(rAnimMask.strAnimName);
		_float fCurAnimTime = pBoneAnimData->Calculate_Time(rAnimMask.fCurTime);	// 변환된 시간 값

		// 행렬 계산
		for (_uint j = 0; j < rAnimMask.vecBoneMasks.size(); j++)
		{
			_float4x4 matTemp = {};
			const FBoneAnimChannelData* pBoneAnimNodeData= pBoneAnimData->Find_AnimNodeData(j);
			if (!pBoneAnimNodeData)
				vecBoneMatrices[j] = {};
			else
				XMStoreFloat4x4(&vecBoneMatrices[j], 
					XMLoadFloat4x4(&(matTemp = pBoneAnimNodeData->Interporated_Matrix(fCurAnimTime))) * vecBoneRemainWeights[j]);
			int t = 0;
		}

		FBoneGroup* pBoneGroup = ConCast<FBoneGroup*>(m_pModelData->pBoneGroup);
		if (i == 0)
			pBoneGroup->Clear_FinalTransforms();
		pBoneGroup->Add_Transforms(&vecBoneMatrices);
	}
}

void CAnimationComponent::Apply_BoneAnimationWithMask()
{
	if (!m_pModelData)
		return;

	/*for (_uint i = 0; i < m_pModelData->pBoneGroup->Get_Bone_Count(); i++)
	{
		
	}*/
}
