#include "Component/AnimationComponent.h"

#include "Component/SkeletalComponent.h"
#include "System/Data/BoneAnimData.h"
#include "System/ModelMgr.h"
#include "Component/AnimMaskComp.h"

CAnimationComponent::CAnimationComponent()
	: m_fTransitionGauge(1.f), m_fTransitionSpeed(0.1f)
{

}

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
	SUPER::Free();

	Safe_Release(m_pBoneGroup);
	Safe_Release(m_pAnimGroup);
}


HRESULT CAnimationComponent::Bind_BoneGroup(CBoneGroup* pBoneGroup)
{
	if (pBoneGroup == nullptr)
		return E_FAIL;

	m_pBoneGroup = pBoneGroup;

	return S_OK;
}

HRESULT CAnimationComponent::Load_Animations(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath)
{
	if (nullptr == m_pGI)
		return E_FAIL;

	auto pAnimGroup = m_pGI->Find_AnimGroup(eGroupIndex, strModelFilePath);
	if (nullptr == pAnimGroup)
		return E_FAIL;

	m_pAnimGroup = pAnimGroup;
	Safe_AddRef(m_pAnimGroup);

	return S_OK;
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

	_uint iNumAnimMasks = Cast<_uint>(m_vecAnimMask.size());
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

_bool CAnimationComponent::IsAnimation_Finished()
{
	return (m_CurAnim.bIsReverse) ? (m_CurAnim.fTrackPos <= 0) : (m_CurAnim.fTrackPos >= m_CurAnim.fDuration);
}

void CAnimationComponent::Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse)
{
	if (iAnimIndex < 0 || iAnimIndex >= m_pAnimGroup->Get_NumAnims())
		return;

	auto pBoneAnimData = m_pAnimGroup->Find_BoneAnim(iAnimIndex);
	if (pBoneAnimData == nullptr)
		return;

	// ���� �ִϸ��̼� ����
	if (m_CurAnim.iAnimID != iAnimIndex)
	{
		m_PrevAnim = m_CurAnim;
		m_fTransitionGauge = 0.f;
	}

	m_CurAnim.iAnimID = iAnimIndex;
	m_CurAnim.fDuration = pBoneAnimData->fDuration;
	m_CurAnim.fTickPerSeconds = pBoneAnimData->fTickPerSecond;
	m_CurAnim.fSpeedMultiply = fSpeedMultiply;
	m_CurAnim.bIsLoop = bIsLoop;
	m_CurAnim.bIsReverse = bReverse;
	m_CurAnim.fTrackPos = (!bReverse) ? 0.f : m_CurAnim.fDuration;
}

void CAnimationComponent::Add_AnimTime(const _float& fTimeDelta)
{
	m_CurAnim.fTrackPos += m_CurAnim.fTickPerSeconds * m_CurAnim.fSpeedMultiply * fTimeDelta * (m_CurAnim.bIsReverse ? -1 : 1);
	if (!m_CurAnim.bIsReverse)
	{
		if (m_CurAnim.fTrackPos > m_CurAnim.fDuration)
			m_CurAnim.fTrackPos = (m_CurAnim.bIsLoop) ? (0.f) : (m_CurAnim.fDuration);
	}
	else
	{
		if (m_CurAnim.fTrackPos < 0.f)
			m_CurAnim.fTrackPos = (m_CurAnim.bIsLoop) ? (m_CurAnim.fDuration) : (0.f);
	}

	if (m_fTransitionGauge < 1.f)
	{
		m_PrevAnim.fTrackPos += m_PrevAnim.fTickPerSeconds * m_PrevAnim.fSpeedMultiply * fTimeDelta * (m_PrevAnim.bIsReverse ? -1 : 1);
		if (!m_PrevAnim.bIsReverse)
		{
			if (m_PrevAnim.fTrackPos > m_PrevAnim.fDuration)
				m_PrevAnim.fTrackPos = (m_PrevAnim.bIsLoop) ? (0.f) : (m_PrevAnim.fDuration);
		}
		else
		{
			if (m_PrevAnim.fTrackPos < 0.f)
				m_PrevAnim.fTrackPos = (m_PrevAnim.bIsLoop) ? (m_PrevAnim.fDuration) : (0.f);
		}
	}
}

void CAnimationComponent::Invalidate_Animation()
{
	if (!m_pBoneGroup || !m_pAnimGroup)
		return;

	_uint iNumBones = m_pBoneGroup->Get_BoneDatas_Count();

	vector<_matrix> vecBoneMatrices;
	vector<FKeyFrame> vecKeyFrames;
	vector<FKeyFrameInterpolate> vecKeyFrameInters;


	vecBoneMatrices.resize(iNumBones, {});
	vecKeyFrames.resize(iNumBones, {});
	if (m_fTransitionGauge < 1.f)
	{
		FAnimInterpolate AnimInter = {};
		AnimInter.iAnimID = m_CurAnim.iAnimID;
		AnimInter.fTrackPos = m_CurAnim.fTrackPos;
		AnimInter.fWeight = m_fTransitionGauge;
		AnimInter.vecChannelIDs.reserve(iNumBones);
		for (_uint i = 0; i < iNumBones; i++)
			AnimInter.vecChannelIDs.push_back(i);

		FAnimInterpolate PrevAnimInter = AnimInter;
		PrevAnimInter.iAnimID = m_PrevAnim.iAnimID;
		PrevAnimInter.fTrackPos = m_PrevAnim.fTrackPos;
		PrevAnimInter.fWeight = 1.f - m_fTransitionGauge;

		FAnimInterpolate ArrAnimInter[2] = {
			AnimInter, PrevAnimInter
		};

		// ����ũ �ִϸ��̼ǳ��� �����Ѵ�.
		m_pAnimGroup->Interpolated_Anims(vecKeyFrames.data(), vecKeyFrames.size(), ArrAnimInter, 2);
	}
	// ������ KeyFrameInterpolate�� �� �ִϸ��̼����κ��� ���´�.
	else
	{
		FAnimInterpolate AnimInter = {};
		AnimInter.iAnimID = m_CurAnim.iAnimID;
		AnimInter.fTrackPos = m_CurAnim.fTrackPos;
		AnimInter.fWeight = 1.f;
		AnimInter.vecChannelIDs.reserve(iNumBones);
		for (_uint i = 0; i < iNumBones; i++)
			AnimInter.vecChannelIDs.push_back(i);

		m_pAnimGroup->Interpolated_Anims(vecKeyFrames.data(), vecKeyFrames.size(), &AnimInter, 1);
	}


	const FBoneAnimData* pBoneAnimData = m_pAnimGroup->Find_BoneAnim(m_CurAnim.iAnimID);
	for (_uint i = 0; i < iNumBones; i++)
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
			vecBoneMatrices[i] = XMMatrixIdentity();
	}


	// �� Ʈ�������� �ִϸ��̼� ����
	for (_uint i = 0; i < iNumBones; i++)
	{
		m_pBoneGroup->Set_BoneTransform(i, vecBoneMatrices[i]);
	}

	if (m_fTransitionGauge < 1.f)
		m_fTransitionGauge += m_fTransitionSpeed;
}

void CAnimationComponent::Invalidate_AnimationWithMask(CAnimMaskComp* pAnimMaskComp)
{
	if (!m_pBoneGroup || !m_pAnimGroup)
		return;

	if (pAnimMaskComp == nullptr)
	{
		Invalidate_Animation();
		return;
	}

	auto vecRemainWeights = pAnimMaskComp->Get_RemainWeights();
	_uint iNumMasks = pAnimMaskComp->Get_NumAnimMasks();
	_uint iNumBones = m_pBoneGroup->Get_BoneDatas_Count();

	//for (_uint i = 0; i < iNumMasks; i++)
	//{

	//	// ����ũ�� Ű�������� ����
	//	for (_uint j = 0; j < iNumBones; j++)
	//	{
	//		if (rAnimMask.fTransitionGauge < 1.f)
	//		{
	//			FAnimInterpolate AnimInter = {};
	//			AnimInter.iAnimID = rAnimMask.iAnimID;
	//			AnimInter.fTrackPos = rAnimMask.fCurTrackPos;
	//			AnimInter.fWeight = rAnimMask.fTransitionGauge;
	//			AnimInter.vecChannelIDs.reserve(iNumBoneMasks);
	//			for (_uint j = 0; j < iNumBoneMasks; j++)
	//			{
	//				_int iID = rAnimMask.vecBoneMasks[j] ? rAnimMask.vecBoneMasks[j] * j : -1;	// ����ũ�� ������ -1�� ����
	//				AnimInter.vecChannelIDs.push_back(iID);
	//			}

	//			FAnimInterpolate PrevAnimInter = AnimInter;
	//			PrevAnimInter.iAnimID = rAnimMask.iPrevAnimID;
	//			PrevAnimInter.fTrackPos = rAnimMask.fPrevTrackPos;
	//			PrevAnimInter.fWeight = 1.f - rAnimMask.fTransitionGauge;

	//			FAnimInterpolate ArrAnimInter[2] = {
	//				AnimInter, PrevAnimInter
	//			};

	//			// ����ũ �ִϸ��̼ǳ��� �����Ѵ�.
	//			m_pAnimGroup->Interpolated_Anims(vecKeyFrames.data(), vecKeyFrames.size(), ArrAnimInter, 2);

	//			vecKeyFrameInters[i].fWeight = fWeight;
	//			vecKeyFrameInters[i].KeyFrames[j] = vecKeyFrames[j];
	//		}
	//		// ������ KeyFrameInterpolate�� �� �ִϸ��̼����κ��� ���´�.
	//		else
	//		{
	//			FKeyFrame KeyFrame = m_pAnimGroup->Interpolated_Anim(rAnimMask.iAnimID, j, rAnimMask.fCurTrackPos);
	//			vecKeyFrameInters[i].fWeight = fWeight;
	//			vecKeyFrameInters[i].KeyFrames[j] = KeyFrame;
	//		}
	//	}
	//}
}
