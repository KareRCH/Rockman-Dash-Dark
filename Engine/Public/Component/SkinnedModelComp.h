#pragma once

#include "Component/ModelComponent.h"

#include "Component/MultiMeshBufComp.h"
#include "Component/SkinnedModelShaderComp.h"
#include "Component/SkeletalComponent.h"
#include "Component/AnimationComponent.h"
#include "Component/EffectComponent.h"


BEGIN(Engine)



/// <summary>
/// �޽��� ���� �ѷ��δ� ������ ���� ����ϴ� ������Ʈ
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


#pragma region ��Ƽ �޽� ����

public:
	CMultiMeshBufComp* m_pVIBufferComp = { nullptr };
#pragma endregion


#pragma region ���� �Բ� ���̵� �Ǵ� ���̴�

public:
	CSkinnedModelShaderComp* m_pShaderComp = { nullptr };

public:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion


#pragma region ���̷�Ż ������Ʈ
public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComponent; }

private:
	CSkeletalComponent* m_pSkeletalComponent = { nullptr };
#pragma endregion



#pragma region �ִϸ��̼� ������Ʈ
public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComponent; }

private:
	CAnimationComponent* m_pAnimationComponent = { nullptr };
#pragma endregion


};

END