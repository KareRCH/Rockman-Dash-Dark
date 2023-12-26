#pragma once

#include "Component/InternalComponent.h"

BEGIN(Engine)

struct TAnimMaskTrack
{
	_uint		iAnimID;				// 애니메이션 ID
	_float		fTrackPos;				// 애니메이션 재생 위치
	_float		fDuration;				// 애니메이션 지속시간
	_float		fTickPerseconds;		// 애니메이션 재생속도
	_bool		bIsLoop;				// 애니메이션 루프설정
};

struct TAnimMask
{
	wstring			strName;			// 마스크 이름
	_float			fWeight;			// 마스크 적용 가중치

	_uint			iNumBoneMasks;		// 마스크 뼈 개수
	vector<_bool>	vecBoneMasks;		// 마스크 적용 뼈, 애니메이션이 적용되는 뼈가 설정된다.
	vector<_float>	vecBoneMaskWeights;	// 마스크 뼈당 가중치, 마스크 가중치가 바뀌는 동작이 일어날 때 바뀐다.

	TAnimMaskTrack	tCurAnim;			// 현재 애니메이션
	TAnimMaskTrack	tPrevAnim;			// 이전 애니메이션

	_float			fTransitionSpeed;	// 애니메이션 전환속도 0~1, 1이면 즉시, 0이면 안바뀌니 최소 0보다 커야함.
	_float			fTransitionGauge;	// 애니메이션 전환게이지, 1보다 작을 때 트랜지션 속도가 더해진다.
										// 1을 넘으면 종료하고 완전히 새로운 애니메이션으로 대체한다.
};

/// <summary>
/// 애니메이션에 대한 마스크를 만들어 적용하는 기능을 가진 클래스
/// 같은 모델의 애니메이션에 대해 서로 다른 애니메이션을 적용시 사용
/// 실제 키프레임이 적용되는 시점은 애니메이션 컴포넌트에서 진행된다.
/// </summary>
class ENGINE_DLL CAnimMaskComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimMaskComp)
protected:
	explicit CAnimMaskComp() = default;
	explicit CAnimMaskComp(const CAnimMaskComp& rhs);
	virtual ~CAnimMaskComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CAnimMaskComp*	Create();
	virtual CComponent*		Clone(void* Arg = nullptr) override;

protected:
	virtual void			Free() override;

public:		// 애님 마스크 설정
	// 뼈 정보를 기반으로 마스크를 생성한다.
	HRESULT		Create_Mask(const wstring& strMaskName, class CAnimationComponent* pAnimComp, _float fWeight, _bool bInitBoneActive);
	// 마스크 웨이트를 설정하기 위해 쓰인다.
	void		Set_MaskWeight(_uint iMaskIndex, _float fWeight);
	// 마스크 웨이트를 업데이트 한다. 마스크의 웨이트가 변경될 때만 작동하도록 한다.
	void		Invalidate_MaskWeights();

public:		// 뼈 마스크 설정
	// 마스크를 전체 활성화하기 위해 쓰인다.
	void		Active_BoneMask(_uint iMaskIndex);
	// 마스크를 전체 비활성화하기 위해 쓰인다.
	void		Deactive_BoneMask(_uint iMaskIndex);
	// 마스크의 뼈 마스크를 하나하나 설정하기 위해 쓰인다.
	void		Set_BoneMaskAvailable(_uint iMaskIndex, _uint iBoneMaskIndex, _bool bActive);

public:
	const _uint& Get_NumAnimMasks() const { return m_iNumAnimMasks; }
	// 뼈당 남은 Weight를 반환하는 함수. 애니메이션 컴포넌트에서 원 애니메이션을 사용할 때 쓰인다.
	const vector<_float>& Get_RemainWeights() const { return m_vecRemainWeights; }

private:
	_uint					m_iNumAnimMasks = { 0 };		// 마스크 개수
	vector<TAnimMask>		m_vecAnimMasks;					// 마스크
	vector<_float>			m_vecRemainWeights;				// 마스크를 통해 남은 웨이트 값

};

END