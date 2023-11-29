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
	virtual _int	Tick(const _float& fTimeDelta) override;
	virtual void	Late_Tick(const _float& fTimeDelta) override;
	virtual void	Render() override;

public:
	static CSkinnedModelComp* Create();
	virtual CComponent* Clone(void* Arg = nullptr) override;

protected:
	virtual void	Free() override;

public:
	// 모델을 바인딩한다. 메시, 스켈레탈, 애니메이션 모두에게 적용된다.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

private:
	FModelData* m_pModelData = { nullptr };



#pragma region 멀티 메시 버퍼
public:
	// 로드된 모델에 따라 사용할 메시를 바인드 한다.
	HRESULT Bind_Mesh(const wstring& strMeshKey);
	// 바인딩 된 메시중 필요없는 메시를 뺀다.
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// 바인딩 된 메시들을 모두 언바인드 한다.
	void	Unbind_AllMeshes();

public:
	CMultiMeshBufComp* m_pMultiMeshBufComp = { nullptr };
#pragma endregion





#pragma region 뼈와 함께 셰이딩 되는 셰이더
private:
	// 이펙트를 바인드 한다.
	HRESULT Bind_Effect(const wstring& strEffectKey);
	// 이펙트를 언바인드 한다.
	HRESULT Unbind_Effect();
	// 렌더링 한다. 모든 계산이 다 끝나있어야 한다.
	HRESULT Render_Effect();


private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion





#pragma region 스켈레탈 컴포넌트
public:
	// 뼈를 등록한다. 스켈레탈과 애니메이션에 쓰인다.
	HRESULT Bind_Skeletal(const wstring& strSkeletalKey);
	// 

public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComponent; }

private:
	CSkeletalComponent* m_pSkeletalComponent = { nullptr };
#pragma endregion





#pragma region 애니메이션 컴포넌트
public:
	// 마스크 정보를 바인드 한다.
	HRESULT Bind_Mask(_uint iIndex);
	// 애니메이션의 최종 포즈의 행렬을 뼈에 적용한다. 
	HRESULT Apply_Pose();
	
public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComponent; }

private:
	CAnimationComponent* m_pAnimationComponent = { nullptr };
#pragma endregion


};

END