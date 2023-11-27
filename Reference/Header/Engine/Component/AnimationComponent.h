#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneData;

/// <summary>
/// �ִϸ��̼ǿ� ���� ó���� ����ϴ� ������Ʈ
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
	// ���� ���� �ִϸ��̼��� �������ش�.
	HRESULT Apply_BoneAnimation(FBoneData* const pBoneData);

private:
	class FBoneAnimData* m_pAnimData = { nullptr };
	// �ִϸ��̼��� ���� ����ȭ�Ǿ� �۵��Ѵ�.
	// ���� ���� ������ �� �ӵ��� ���� �ε����� ����ϴ� ����� ����Ѵ�.
	// ������ ���� �ε����� �Ϸ�� ���¿��� �ϸ�, �ִϸ��̼� ���� ���� ���� �ε����� �Ϸ�� �����͸� ����ؾ� �Ѵ�.
	// �̴� �� �Ŵ������� �ε��� ������� �۵��ϵ��� �Ѵ�.
};

END