#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneAnimGroup;
class FBoneAnimData;
class FSkeletalData;
class FModelData;


struct FAnimMask
{
	wstring	strName;			// 마스크 이름
	_float fWeight;				// 마스크 적용 가중치
	vector<_bool> vecBoneMasks;	// 마스크 적용 뼈

	wstring strSkeletaName;		// 스켈레탈 이름
	wstring strAnimName;		// 애니메이션 이름
	_float fCurTime;			// 현재 재생 중인 시간, 시스템 시간 기준
};


/// <summary>
/// 애니메이션에 대한 처리를 담당하는 컴포넌트
/// 뼈 하나에 대응하는 
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
	void Set_ModelData(FModelData* pModelData);

	HRESULT Bind_AnimGroup();

	// 마스크 생성
	HRESULT Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive);
	FAnimMask* Get_Mask(_uint iIndex);

	void Deactive_BoneMask(_uint iIndex, const wstring& strBoneName);
	void Active_BoneMask(_uint iIndex, const wstring& strBoneName);

	void Set_MaskAnimation(_uint iIndex, const wstring& strAnimName);
	void Set_MaskTime(_uint iIndex, _float fTime);
	void Set_TickDeltaTime(_float fDeltaTime);

	// 해당 마스크에 현재 재생 프레임을 전달하여, 적용될 애니메이션 시간을 정한다.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// 마스크에 적용되어 있는 값에 따라 각 뼈에 대한 가중치를 계산해낸다.
	void Apply_FinalMask();

private:
	FModelData*			m_pModelData = { nullptr };		// 모델 데이터, 해당 모델을 바인딩 시켜놓기 위해 해놓는다.
	FBoneAnimGroup*		m_pAnimGroup = { nullptr };		// 단 하나의 뼈에 대한 애니메이션 정보를 가진다.
														// 이 녀석이 설정되어 있어야 기능을 할 수 있다.
	
	vector<FAnimMask>	m_vecAnimMask;					// 애니메이션이 적용되는 마스크, 기본적으로 0번 마스크에 적용되어 작동한다.
	_float				m_fSystemTPS;				// 1 프레임당 속도값
};

END