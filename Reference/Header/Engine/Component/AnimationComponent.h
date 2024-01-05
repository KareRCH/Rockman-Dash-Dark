#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class CBoneAnimGroup;
class FBoneAnimData;
class FSkeletalData;
class FModelData;
class CBoneGroup;


struct FAnimMask
{
	wstring			strName;			// 마스크 이름
	_float			fWeight;			// 마스크 적용 가중치

	_uint			iNumMasks;			// 마스크 개수
	vector<_bool>	vecBoneMasks;		// 마스크 적용 뼈

	wstring			strAnimName;		// 애니메이션 이름
	_uint			iAnimID;			// 애니메이션 ID
	_float			fCurTrackPos;		// 현재 재생 중인 트랙 위치
	_float			fDuration;			// 지속시간
	_float			fTickPerSeconds;	// 재생속도
	
	wstring			strPrevAnimName;	// 이전 애니메이션 이름
	_uint			iPrevAnimID;		// 이전 애니메이션 ID
	_float			fPrevTrackPos;		// 이전 애님 재생 중인 트랙 위치
	_float			fTransitionSpeed;	// 애니메이션 전환속도 0~1, 1이면 즉시, 0이면 안바뀌니 최소 0보다 커야함.
	_float			fTransitionGauge;	// 애니메이션 전환게이지, 1보다 작을 때 트랜지션 속도가 더해진다.
										// 1을 넘으면 종료하고 완전히 새로운 애니메이션으로 대체한다.
	
};

struct TAnimData
{
	_uint		iAnimID;
	_float		fDuration;
	_float		fTickPerSeconds;
	_float		fTrackPos;
	_float		fSpeedMultiply;
	_bool		bIsLoop;
	_bool		bIsReverse;
};


/// <summary>
/// 애니메이션에 대한 처리를 담당하는 컴포넌트
/// 뼈 하나에 대응하는 
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent();
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
	HRESULT Bind_BoneGroup(CBoneGroup* pBoneGroup);
	HRESULT Load_Animations(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath);

	// 해당 마스크에 현재 재생 프레임을 전달하여, 적용될 애니메이션 시간을 정한다.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// 마스크에 적용되어 있는 값에 따라 각 뼈에 대한 가중치를 계산해낸다.
	void Apply_FinalMask();

public:
	CBoneGroup* const		Get_BoneGroup() const { return m_pBoneGroup; }
	CBoneAnimGroup* const	Get_AnimGroup() const { return m_pAnimGroup; }

private:
	CBoneGroup*			m_pBoneGroup = { nullptr };
	CBoneAnimGroup*		m_pAnimGroup = { nullptr };		// 단 하나의 뼈에 대한 애니메이션 정보를 가진다.
														// 이 녀석이 설정되어 있어야 기능을 할 수 있다.

public:
	_bool	IsAnimation_Finished();
	void Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse = false);
	void Add_AnimTime(const _float& fTimeDelta);
	void Invalidate_Animation();
	void Invalidate_AnimationWithMask(class CAnimMaskComp* pAnimMaskComp);
	
private:
	TAnimData			m_CurAnim = {};
	TAnimData			m_PrevAnim = {};
	_float				m_fTransitionSpeed;
	_float				m_fTransitionGauge;
	vector<FAnimMask>	m_vecAnimMask;					// 애니메이션이 적용되는 마스크, 기본적으로 0번 마스크에 적용되어 작동한다.
};

END