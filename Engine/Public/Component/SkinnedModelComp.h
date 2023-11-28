#pragma once

#include "Component/ModelComponent.h"

#include "Component/MultiMeshBufComp.h"
#include "Component/SkinnedModelShaderComp.h"
#include "Component/SkeletalComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/EffectComponent.h"


BEGIN(Engine)



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


#pragma region 멀티 메시 버퍼

public:
	CMultiMeshBufComp* m_pVIBufferComp = { nullptr };
#pragma endregion


#pragma region 뼈와 함께 셰이딩 되는 셰이더

public:
	CSkinnedModelShaderComp* m_pShaderComp = { nullptr };

public:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


#pragma region 스켈레탈 컴포넌트
public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComponent; }

private:
	CSkeletalComponent* m_pSkeletalComponent = { nullptr };
#pragma endregion



#pragma region 애니메이션 컴포넌트
public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComponent; }

private:
	CAnimationComponent* m_pAnimationComponent = { nullptr };
#pragma endregion


};

END