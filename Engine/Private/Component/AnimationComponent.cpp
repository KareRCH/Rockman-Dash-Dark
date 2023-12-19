#include "Component/AnimationComponent.h"

#include "Component/SkeletalComponent.h"
#include "System/Data/BoneAnimData.h"
#include "System/ModelMgr.h"

CAnimationComponent::CAnimationComponent(const CAnimationComponent& rhs)
	: Base(rhs)
	, m_pBoneGroup(rhs.m_pBoneGroup)
	, m_pAnimGroup(rhs.m_pAnimGroup)
{
	Safe_AddRef(m_pBoneGroup);
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
	}

	return Cast<CComponent*>(pInstance);
}

void CAnimationComponent::Free()
{
	Safe_Release(m_pBoneGroup);
	Safe_Release(m_pAnimGroup);
}

HRESULT CAnimationComponent::Bind_BoneGroup(CSkeletalComponent* pSkeletalComp)
{
	if (!pSkeletalComp)
		return E_FAIL;

	m_pBoneGroup = pSkeletalComp->Get_BoneGroup();
	Safe_AddRef(m_pBoneGroup);

	Bind_AnimGroup(pSkeletalComp->Get_ModelData());
}

HRESULT CAnimationComponent::Bind_AnimGroup(FModelData* pModelData)
{
	if (!pModelData)
		return E_FAIL;

	m_pAnimGroup = pModelData->pAnimGroup;
	Safe_AddRef(m_pAnimGroup);

	return S_OK;
}

HRESULT CAnimationComponent::Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive)
{
	if (!m_pBoneGroup)
		return E_FAIL;

	// �⺻ ����
	FAnimMask tMask = {};
	tMask.strName = strMaskName;
	tMask.fWeight = 1.f;
	tMask.iNumMasks = m_pBoneGroup->Get_BoneDatas_Count();
	tMask.vecBoneMasks.resize(tMask.iNumMasks, bInitBoneActive);
	tMask.fTransitionSpeed = 0.1f;
	tMask.strAnimName = L"Idle";

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
	if (!m_pBoneGroup)
		return;

	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FBoneData* pBoneData = m_pBoneGroup->Find_BoneData(strBoneName);
	if (!pBoneData)
		return;

	m_vecAnimMask[iIndex].vecBoneMasks[pBoneData->iID] = false;
}

void CAnimationComponent::Active_BoneMask(_uint iIndex, const wstring& strBoneName)
{
	if (!m_pBoneGroup)
		return;

	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FBoneData* pBoneData = m_pBoneGroup->Find_BoneData(strBoneName);
	if (!pBoneData)
		return;

	m_vecAnimMask[iIndex].vecBoneMasks[pBoneData->iID] = true;
}

void CAnimationComponent::Set_MaskAnimation(_uint iIndex, const wstring& strAnimName)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FAnimMask& rMask = m_vecAnimMask[iIndex];
	const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_BoneAnim(strAnimName);
	if (pBoneAnimData == nullptr)
		return;

	if (strAnimName != rMask.strPrevAnimName)
	{
		rMask.strPrevAnimName = rMask.strAnimName;
		rMask.iPrevAnimID = rMask.iAnimID;
		rMask.fPrevTrackPos = rMask.fCurTrackPos;
		rMask.fTransitionGauge = 0.f;
	}

	rMask.strAnimName = pBoneAnimData->strName;
	rMask.iAnimID = pBoneAnimData->iID;
	rMask.fTickPerSeconds = pBoneAnimData->fTickPerSecond;
	rMask.fDuration = pBoneAnimData->fDuration;
	rMask.fCurTrackPos = 0.f;
}

void CAnimationComponent::Set_MaskTime(_uint iIndex, _float fTime)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	FAnimMask& rMask = m_vecAnimMask[iIndex];
	const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_BoneAnim(rMask.strAnimName);
	if (pBoneAnimData == nullptr)
		return;

	// ���� �ð��� ������ ������ ��ȯ
	m_vecAnimMask[iIndex].fCurTrackPos = pBoneAnimData->Calculate_Time(fTime);
	// �������� 1�� �ƴϸ� �ð� �߰�
	if (rMask.fTransitionGauge < 1.f)
		m_vecAnimMask[iIndex].fPrevTrackPos = pBoneAnimData->Calculate_Time(fTime);
}

void CAnimationComponent::Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime)
{
	if (iIndex < 0 || iIndex >= m_vecAnimMask.size())
		return;

	m_vecAnimMask[iIndex].fCurTrackPos = fCurTime;
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

	m_vecAnimMask[iIndex].fCurTrackPos = fCurTime;
	m_vecAnimMask[iIndex].strAnimName = strAnimName;
}

void CAnimationComponent::Apply_FinalMask()
{
	if (!m_pBoneGroup || !m_pAnimGroup)
		return;

	if (m_vecAnimMask.empty())
		return;

	_uint iNumAnimMasks = m_vecAnimMask.size();
	_uint iNumBoneMasks = m_vecAnimMask[0].iNumMasks;
	_matrix matIdentity = XMMatrixIdentity();
	
	// ���� ����ġ
	vector<_float> vecBoneRemainWeights;
	vector<_matrix> vecBoneMatrices;
	vector<FKeyFrame> vecKeyFrames;
	vector<FKeyFrameInterpolate> vecKeyFrameInters;

	
	vecBoneRemainWeights.resize(iNumBoneMasks, 0.f);
	vecBoneMatrices.resize(iNumBoneMasks, {});
	vecKeyFrames.resize(iNumBoneMasks, {});
	vecKeyFrameInters.resize(iNumAnimMasks, {});
	for (_uint i = 0; i < iNumAnimMasks; i++)
	{
		vecKeyFrameInters[i].KeyFrames.resize(iNumBoneMasks, {});
	}
	

	_bool bIsTransition = false;

	// ���� ���� ����ġ ����� �Ѵ�.
	for (_int i = Cast<_int>(m_vecAnimMask.size() - 1); i >= 0; --i)
	{
		FAnimMask& rAnimMask = m_vecAnimMask[i];
		_float fCurWeight = 0.f;

		// �ִϸ��̼��� ������ �ǳʶٱ�
		if (rAnimMask.strAnimName.empty())
			continue;

		// ����ġ ����
		for (_uint j = 0; j < iNumBoneMasks; j++)
		{
			// ���� ���̾��� ����ũ�� �켱������ �����ȴ�.
			// min �Լ��� ����ؼ� �����ִ� �����θ� ����Ʈ�� ���������� �����Ѵ�.
			_float fWeight = Cast<_float>(rAnimMask.vecBoneMasks[j]) * rAnimMask.fWeight;
			fWeight = min(fWeight, (1.f - vecBoneRemainWeights[j]));	// 1�̻����� �ö��� �ʰ� ����

			// ���̽� ����ũ��� ���� ���� �� ���������� �Ѵ�.
			if (i == 0)
				vecBoneRemainWeights[j] = fWeight;
			else
				vecBoneRemainWeights[j] = max((1.f - vecBoneRemainWeights[j]) - fWeight, 0.f);	// 0���Ϸ� �������� �ʰ� ����




			// ����ũ Ʈ�������� ������ �׿� ���� ������ �� ������� KeyFrameInterpolate�� �̾Ƴ���.
			if (rAnimMask.fTransitionGauge < 1.f)
			{
				FAnimInterpolate AnimInter = {};
				AnimInter.iAnimID = rAnimMask.iAnimID;
				AnimInter.fTrackPos = rAnimMask.fCurTrackPos;
				AnimInter.fWeight = rAnimMask.fTransitionGauge;
				AnimInter.vecChannelIDs.reserve(iNumBoneMasks);
				for (_uint j = 0; j < iNumBoneMasks; j++)
				{
					_int iID = rAnimMask.vecBoneMasks[j] ? rAnimMask.vecBoneMasks[j] * j : -1;	// ����ũ�� ������ -1�� ����
					AnimInter.vecChannelIDs.push_back(iID);
				}

				FAnimInterpolate PrevAnimInter = AnimInter;
				PrevAnimInter.iAnimID = rAnimMask.iPrevAnimID;
				PrevAnimInter.fTrackPos = rAnimMask.fPrevTrackPos;
				PrevAnimInter.fWeight = 1.f - rAnimMask.fTransitionGauge;

				FAnimInterpolate ArrAnimInter[2] = {
					AnimInter, PrevAnimInter
				};

				// ����ũ �ִϸ��̼ǳ��� �����Ѵ�.
				m_pAnimGroup->Interpolated_Anims(vecKeyFrames.data(), vecKeyFrames.size(), ArrAnimInter, 2);

				vecKeyFrameInters[i].fWeight = fWeight;
				vecKeyFrameInters[i].KeyFrames[j] = vecKeyFrames[j];
			}
			// ������ KeyFrameInterpolate�� �� �ִϸ��̼����κ��� ���´�.
			else
			{
				FKeyFrame KeyFrame = m_pAnimGroup->Interpolated_Anim(rAnimMask.iAnimID, j, rAnimMask.fCurTrackPos);
				vecKeyFrameInters[i].fWeight = fWeight;
				vecKeyFrameInters[i].KeyFrames[j] = KeyFrame;
			}
		}

		if (rAnimMask.fTransitionGauge < 1.f)
			rAnimMask.fTransitionGauge += rAnimMask.fTransitionSpeed;
	}

	// ���������͸� �Ѱܼ� ����Ѵ�.
	m_pAnimGroup->Interpolated_KeyFrames(vecKeyFrames.data(), vecKeyFrames.size(), vecKeyFrameInters.data(), vecKeyFrameInters.size());
	FAnimMask& rAnimMask = m_vecAnimMask[0];
	const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_BoneAnim(rAnimMask.strAnimName);
	for (_uint i = 0; i < iNumBoneMasks; i++)
	{
		const FBoneAnimChannelData* pBoneAnimChannelData = pBoneAnimData->Find_AnimChannelData(i);

		if (nullptr != pBoneAnimChannelData)
		{
			vecBoneMatrices[i] =
				XMMatrixAffineTransformation(
					XMLoadFloat3(&vecKeyFrames[i].vScale),
					XMQuaternionIdentity(),
					XMLoadFloat4(&vecKeyFrames[i].qtRot),
					XMLoadFloat3(&vecKeyFrames[i].vPos));
		}
		else
			vecBoneMatrices[i] = matIdentity;
	}
	

	// �� Ʈ�������� �ִϸ��̼� ����
	for (_uint i = 0; i < iNumBoneMasks; i++)
	{
		m_pBoneGroup->Set_BoneTransform(i, vecBoneMatrices[i]);
	}
}
