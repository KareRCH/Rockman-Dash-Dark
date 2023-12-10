#pragma once

#include "Component/ModelComponent.h"

#include "Component/MultiMeshBufComp.h"
#include "Component/SkinnedModelShaderComp.h"
#include "Component/SkeletalComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/EffectComponent.h"


BEGIN(Engine)

class FModelData;

/// <summary>
/// 메쉬가 뼈를 둘러싸는 형태의 모델을 사용하는 컴포넌트
/// </summary>
class ENGINE_DLL CSkinnedModelComp final : public CModelComponent, public ISkeletalComponent, public IAnimationComponent
{
	DERIVED_CLASS(CModelComponent, CSkinnedModelComp)

protected:
	explicit CSkinnedModelComp() = default;
	explicit CSkinnedModelComp(const CSkinnedModelComp& rhs);
	virtual ~CSkinnedModelComp() = default;

public:
	virtual HRESULT	Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;
	virtual void	Priority_Tick(const _float& fTimeDelta) override;
	virtual void	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual HRESULT	Render() override;

public:
	static CSkinnedModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;




#pragma region 모델 데이터 바인딩
public:	// 외부 설정용
	// 모델을 바인딩한다. 메시, 스켈레탈, 애니메이션 모두에게 적용된다.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

private:
	FModelData* m_pModelData = { nullptr };
#pragma endregion






#pragma region 멀티 메시 버퍼
	// 외부 설정용
public:
	// 로드된 모델에 따라 사용할 메시를 바인드 한다.
	HRESULT Bind_Mesh(const wstring& strMeshKey);
	// 바인딩 된 메시중 필요없는 메시를 뺀다.
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// 바인딩 된 메시들을 모두 언바인드 한다.
	void	Unbind_AllMeshes();

private:
	CMultiMeshBufComp* m_pMultiMeshBufComp = { nullptr };
#pragma endregion





#pragma region 스켈레탈 컴포넌트
public:		// 외부 설정용
	// 뼈를 등록한다. 스켈레탈과 애니메이션에 쓰인다.
	HRESULT Bind_Skeletal(const wstring& strSkeletalKey);

public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComp; }

private:
	CSkeletalComponent* m_pSkeletalComp = { nullptr };
#pragma endregion





#pragma region 애니메이션 컴포넌트
	// 외부 설정용
public:
	// 마스크 정보를 바인드 한다.
	HRESULT Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive);
	// 마스크 뼈를 활성화 한다.
	void Deactive_BoneMask(_uint iIndex, const wstring& strBoneName);
	// 마스크 뼈를 활성화 한다.
	void Active_BoneMask(_uint iIndex, const wstring& strBoneName);
	// 마스크의 애니메이션을 설정한다.
	void Set_MaskAnimation(_uint iIndex, const wstring& strAnimName);
	// 마스크의 애니메이션 시간을 설정한다.
	void Set_MaskTime(_uint iIndex, _float fTime);
	// 애니메이션의 기준이 되는 시간변화율을 설정한다. Tick에서 
	void Set_TickDeltaTime(_float fDeltaTime);
	// 애니메이션의 최종 포즈의 행렬을 뼈에 적용한다. 
	HRESULT Apply_Pose();

public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComp; }

private:
	CAnimationComponent* m_pAnimationComp = { nullptr };
#pragma endregion






#pragma region 뼈와 함께 셰이딩 되는 셰이더
	// 외부 설정용
public:
	// 이펙트를 바인드 한다.
	HRESULT Bind_Effect(const wstring& strEffectKey, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	// 이펙트를 언바인드 한다. 안쓸듯
	HRESULT Unbind_Effect();


private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


};

END