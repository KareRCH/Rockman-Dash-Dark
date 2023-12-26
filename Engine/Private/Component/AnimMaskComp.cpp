#include "Component/AnimMaskComp.h"

#include "Component/AnimationComponent.h"
#include "System/Data/BoneData.h"
#include "System/Data/BoneAnimData.h"

CAnimMaskComp::CAnimMaskComp(const CAnimMaskComp& rhs)
{
}

HRESULT CAnimMaskComp::Initialize_Prototype(void* Arg)
{
	return S_OK;
}

HRESULT CAnimMaskComp::Initialize(void* Arg)
{
	return S_OK;
}

CAnimMaskComp* CAnimMaskComp::Create()
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("CAnimMaskComp Create Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimMaskComp::Clone(void* Arg)
{
	ThisClass* pInstance = new ThisClass(*this);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("CAnimMaskComp Create Failed");
		Safe_Release(pInstance);
	}

	return Cast<CComponent*>(pInstance);
}

void CAnimMaskComp::Free()
{
	SUPER::Free();

}

HRESULT CAnimMaskComp::Create_Mask(const wstring& strMaskName, CAnimationComponent* pAnimComp, _float fWeight, _bool bInitBoneActive)
{
	if (nullptr == pAnimComp || nullptr == pAnimComp->Get_BoneGroup() || nullptr == pAnimComp->Get_AnimGroup())
		return E_FAIL;

	auto pBoneGroup = pAnimComp->Get_BoneGroup();
	auto pAnimGroup = pAnimComp->Get_AnimGroup();

	// �⺻ ����
	TAnimMask tMask = {};
	tMask.strName = strMaskName;
	tMask.fWeight = fWeight;

	tMask.iNumBoneMasks = pBoneGroup->Get_BoneDatas_Count();
	tMask.vecBoneMasks.resize(tMask.iNumBoneMasks, bInitBoneActive);
	tMask.vecBoneMaskWeights.resize(tMask.iNumBoneMasks, fWeight);

	// �ʱ� �����ø� �����Ҵ�
	if (m_iNumAnimMasks == 0)
		m_vecRemainWeights.resize(tMask.iNumBoneMasks, 1.f);

	tMask.tCurAnim.iAnimID = 0;
	tMask.tCurAnim.fTrackPos = 0.f;
	tMask.tCurAnim.fDuration = pAnimGroup->Find_BoneAnim(0)->fDuration;
	tMask.tCurAnim.fTickPerseconds = pAnimGroup->Find_BoneAnim(0)->fTickPerSecond;
	tMask.tCurAnim.bIsLoop = true;

	tMask.fTransitionSpeed = 0.1f;
	tMask.fTransitionGauge = 0.f;

	tMask.tPrevAnim = tMask.tCurAnim;

	m_vecAnimMasks.push_back(tMask);
	++m_iNumAnimMasks;

	if (m_iNumAnimMasks > 1)
		Invalidate_MaskWeights();

	return S_OK;
}

void CAnimMaskComp::Set_MaskWeight(_uint iMaskIndex, _float fWeight)
{
	if (iMaskIndex < 0 || iMaskIndex >= m_iNumAnimMasks)
		return;

	m_vecAnimMasks[iMaskIndex].fWeight = fWeight;

	if (m_iNumAnimMasks > 1)
		Invalidate_MaskWeights();
}

void CAnimMaskComp::Invalidate_MaskWeights()
{
	for (_uint i = 0; i < m_vecRemainWeights.size(); i++)
		m_vecRemainWeights[i] = 1.f;

	for (_int i = Cast<_int>(m_iNumAnimMasks - 1); i >= 0; --i)
	{
		TAnimMask& AnimMask = m_vecAnimMasks[i];

		for (_uint j = 0; j < AnimMask.iNumBoneMasks; j++)
		{
			// ���� ���̾��� ����ũ�� �켱������ �����ȴ�.
			// min �Լ��� ����ؼ� �����ִ� �����θ� ����Ʈ�� ���������� �����Ѵ�.
			_float fWeight = Cast<_float>(AnimMask.vecBoneMasks[j]) * AnimMask.fWeight;
			fWeight = min(fWeight, (1.f - m_vecRemainWeights[j]));	// 1�̻����� �ö��� �ʰ� ����

			// ���̽� ����ũ��� ���� ���� �� ���������� �Ѵ�.
			m_vecRemainWeights[j] = max((1.f - m_vecRemainWeights[j]) - fWeight, 0.f);	// 0���Ϸ� �������� �ʰ� ����
		}
	}
}

void CAnimMaskComp::Active_BoneMask(_uint iMaskIndex)
{
	if (iMaskIndex < 0 || iMaskIndex >= m_iNumAnimMasks)
		return;

	TAnimMask& AnimMask = m_vecAnimMasks[iMaskIndex];
	for (_uint i = 0; i < AnimMask.iNumBoneMasks; i++)
	{
		AnimMask.vecBoneMasks[i] = true;
	}
}

void CAnimMaskComp::Deactive_BoneMask(_uint iMaskIndex)
{
	if (iMaskIndex < 0 || iMaskIndex >= m_iNumAnimMasks)
		return;

	TAnimMask& AnimMask = m_vecAnimMasks[iMaskIndex];
	for (_uint i = 0; i < AnimMask.iNumBoneMasks; i++)
	{
		AnimMask.vecBoneMasks[i] = false;
	}
}

void CAnimMaskComp::Set_BoneMaskAvailable(_uint iMaskIndex, _uint iBoneMaskIndex, _bool bActive)
{
	if (iMaskIndex < 0 || iMaskIndex >= m_iNumAnimMasks)
		return;

	TAnimMask& AnimMask = m_vecAnimMasks[iMaskIndex];

	if (iBoneMaskIndex < 0 || iBoneMaskIndex >= AnimMask.iNumBoneMasks)
		AnimMask.vecBoneMasks[iBoneMaskIndex] = bActive;
}
