#include "Component/AnimationComponent.h"

#include "System/Data/BoneAnimData.h"

CAnimationComponent::CAnimationComponent(const CAnimationComponent& rhs)
	: Base(rhs)
{
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
	if (m_vecAnimMask.empty())
		return;
	
	vector<_float> vecBoneWeights;

	// 뼈에 대한 가중치 계산을 한다.
	for (_int i = Cast<_int>(m_vecAnimMask.size() - 1); i >= 0; --i)
	{
		if (vecBoneWeights.empty())
			vecBoneWeights.resize(m_vecAnimMask[i].vecBoneMasks.size(), 0.f);

		for (_uint j = 0; j < m_vecAnimMask[i].vecBoneMasks.size(); j++)
		{
			// 이전 가중치를 최대치로 잡고 계산한다.
			// 남은 가중치가 없다면 0으로 조정된다.
			vecBoneWeights[j] = max((1.f - vecBoneWeights[j]), 0.f) 
				+ min(Cast<_float>(m_vecAnimMask[i].vecBoneMasks[j]) * m_vecAnimMask[i].fWeight, (1.f - vecBoneWeights[j]));
		}
	}

	
}

void CAnimationComponent::Apply_BoneAnimationWithMask(FArmatureData* const pArmatureData)
{

}
