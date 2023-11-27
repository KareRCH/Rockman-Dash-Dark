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

HRESULT CAnimationComponent::Apply_BoneAnimation(const wstring& strAnimName, FArmatureData* const pArmatureData)
{


	return S_OK;
}

void CAnimationComponent::Increase_CurTime(const _float& fTimeDelta)
{
	if (m_pAnimGroup == nullptr)
		return;

	//m_pAnimGroup->Find_AnimData()
}

void CAnimationComponent::Apply_FinalMask()
{
	if (m_vecAnimMask.empty())
		return;
	
	vector<_float> vecBoneWeights;

	// ���� ���� ����ġ ����� �Ѵ�.
	for (_uint i = Cast<_uint>(m_vecAnimMask.size() - 1); i >= 0; --i)
	{
		if (vecBoneWeights.empty())
			vecBoneWeights.resize(m_vecAnimMask[i].vecBoneMasks.size(), 0.f);

		for (_uint j = 0; j < m_vecAnimMask[i].vecBoneMasks.size(); j++)
		{
			// ���� ����ġ�� �ִ�ġ�� ��� ����Ѵ�.
			// ���� ����ġ�� ���ٸ� 0���� �����ȴ�.
			vecBoneWeights[j] = min((1.f - vecBoneWeights[j]), 0.f) 
				+ min(Cast<_float>(m_vecAnimMask[i].vecBoneMasks[j]) * m_vecAnimMask[i].fWeight, (1.f - vecBoneWeights[j]));
		}
	}

	
}
