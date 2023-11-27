#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneAnimGroup;
class FBoneAnimData;
class FArmatureData;


struct FAnimMask
{
	wstring	strName;			// 마스크 이름
	_float fWeight;			// 마스크 적용 가중치
	vector<_bool> vecBoneMasks;	// 마스크 적용 뼈

	wstring strAnimName;		// 애니메이션 이름
	_float fCurFrame;			// 현재 재생 프레임, 시스템 시간이 아닌 프레임 값임
};

/// <summary>
/// 애니메이션에 대한 처리를 담당하는 컴포넌트
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent() = default;
	explicit CAnimationComponent(const CAnimationComponent& rhs);
	virtual ~CAnimationComponent() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CAnimationComponent* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;


public:
	// 뼈에 대해 애니메이션을 적용해준다.
	HRESULT Apply_BoneAnimation(const wstring& strAnimName, FArmatureData* const pArmatureData);
	// 뼈에 대한 애니메이션을 적용할 때 어떤 뼈에 대한 정보만을 적용하는지에 대한 정보를 받는다. 매개변수 필요
	HRESULT Apply_BoneAnimation(const wstring& strAnimName, FArmatureData* const pArmatureData);

	// 시스템의 시간 변화율로 애니메이션의 현재 재생시간을 변화하는 함수
	void Increase_CurTime(const _float& fTimeDelta);
private:
	FBoneAnimGroup* m_pAnimGroup = { nullptr };			// 단 하나의 뼈에 대한 애니메이션 정보를 가진다.
	FBoneAnimData* m_pFinalAnim = { nullptr };			// 최종적으로 적용되는 애니메이션



public:
	// 마스크에 적용되어 있는 값에 따라 각 뼈에 대한 가중치를 계산해낸다.
	void Apply_FinalMask();

private:
	vector<FAnimMask>	m_vecAnimMask;			// 애니메이션이 적용되는 마스크, 기본적으로 0번 마스크에 적용되어 작동한다.

	// 애니메이션은 뼈와 동기화되어 작동한다.
	// 뼈에 대해 적용할 때 속도를 위해 인덱스를 사용하는 방식을 사용한다.
	// 때문에 뼈는 인덱싱이 완료된 상태여야 하며, 애니메이션 또한 뼈에 대한 인덱싱이 완료된 데이터를 사용해야 한다.
	// 이는 모델 매니저에서 로드한 정보대로 작동하도록 한다.
};

END