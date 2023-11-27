#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneAnimGroup;
class FBoneAnimData;
class FArmatureData;


struct FAnimMask
{
	wstring	strName;			// ����ũ �̸�
	_float fWeight;			// ����ũ ���� ����ġ
	vector<_bool> vecBoneMasks;	// ����ũ ���� ��

	wstring strAnimName;		// �ִϸ��̼� �̸�
	_float fCurFrame;			// ���� ��� ������, �ý��� �ð��� �ƴ� ������ ����
};

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
	HRESULT Apply_BoneAnimation(const wstring& strAnimName, FArmatureData* const pArmatureData);
	// ���� ���� �ִϸ��̼��� ������ �� � ���� ���� �������� �����ϴ����� ���� ������ �޴´�. �Ű����� �ʿ�
	HRESULT Apply_BoneAnimation(const wstring& strAnimName, FArmatureData* const pArmatureData);

	// �ý����� �ð� ��ȭ���� �ִϸ��̼��� ���� ����ð��� ��ȭ�ϴ� �Լ�
	void Increase_CurTime(const _float& fTimeDelta);
private:
	FBoneAnimGroup* m_pAnimGroup = { nullptr };			// �� �ϳ��� ���� ���� �ִϸ��̼� ������ ������.
	FBoneAnimData* m_pFinalAnim = { nullptr };			// ���������� ����Ǵ� �ִϸ��̼�



public:
	// ����ũ�� ����Ǿ� �ִ� ���� ���� �� ���� ���� ����ġ�� ����س���.
	void Apply_FinalMask();

private:
	vector<FAnimMask>	m_vecAnimMask;			// �ִϸ��̼��� ����Ǵ� ����ũ, �⺻������ 0�� ����ũ�� ����Ǿ� �۵��Ѵ�.

	// �ִϸ��̼��� ���� ����ȭ�Ǿ� �۵��Ѵ�.
	// ���� ���� ������ �� �ӵ��� ���� �ε����� ����ϴ� ����� ����Ѵ�.
	// ������ ���� �ε����� �Ϸ�� ���¿��� �ϸ�, �ִϸ��̼� ���� ���� ���� �ε����� �Ϸ�� �����͸� ����ؾ� �Ѵ�.
	// �̴� �� �Ŵ������� �ε��� ������� �۵��ϵ��� �Ѵ�.
};

END