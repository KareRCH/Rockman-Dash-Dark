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
	// ���� ���ε��Ѵ�. �޽�, ���̷�Ż, �ִϸ��̼� ��ο��� ����ȴ�.
	HRESULT Bind_Model(EModelGroupIndex eGroup, const wstring& strModelKey);

private:
	FModelData* m_pModelData = { nullptr };



#pragma region ��Ƽ �޽� ����
public:
	// �ε�� �𵨿� ���� ����� �޽ø� ���ε� �Ѵ�.
	HRESULT Bind_Mesh(const wstring& strMeshKey);
	// ���ε� �� �޽��� �ʿ���� �޽ø� ����.
	HRESULT Unbind_Mesh(const wstring& strMeshKey);
	// ���ε� �� �޽õ��� ��� ����ε� �Ѵ�.
	void	Unbind_AllMeshes();

public:
	CMultiMeshBufComp* m_pMultiMeshBufComp = { nullptr };
#pragma endregion





#pragma region ���� �Բ� ���̵� �Ǵ� ���̴�
private:
	// ����Ʈ�� ���ε� �Ѵ�.
	HRESULT Bind_Effect(const wstring& strEffectKey);
	// ����Ʈ�� ����ε� �Ѵ�.
	HRESULT Unbind_Effect();
	// ������ �Ѵ�. ��� ����� �� �����־�� �Ѵ�.
	HRESULT Render_Effect();


private:
	CEffectComponent* m_pEffectComp = { nullptr };
#pragma endregion





#pragma region ���̷�Ż ������Ʈ
public:
	// ���� ����Ѵ�. ���̷�Ż�� �ִϸ��̼ǿ� ���δ�.
	HRESULT Bind_Skeletal(const wstring& strSkeletalKey);
	// 

public:
	virtual CSkeletalComponent* SkeletalComp() override { return m_pSkeletalComponent; }

private:
	CSkeletalComponent* m_pSkeletalComponent = { nullptr };
#pragma endregion





#pragma region �ִϸ��̼� ������Ʈ
public:
	// ����ũ ������ ���ε� �Ѵ�.
	HRESULT Bind_Mask(_uint iIndex);
	// �ִϸ��̼��� ���� ������ ����� ���� �����Ѵ�. 
	HRESULT Apply_Pose();
	
public:
	virtual CAnimationComponent* AnimComp() override { return m_pAnimationComponent; }

private:
	CAnimationComponent* m_pAnimationComponent = { nullptr };
#pragma endregion


};

END