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
	_float fWeight;				// ����ũ ���� ����ġ
	vector<_bool> vecBoneMasks;	// ����ũ ���� ��

	wstring strAnimName;		// �ִϸ��̼� �̸�
	_float fCurTime;			// ���� ��� ���� �ð�, �ý��� �ð� ����
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


private:
	FBoneAnimGroup* m_pAnimGroup = { nullptr };			// �� �ϳ��� ���� ���� �ִϸ��̼� ������ ������.
														// �� �༮�� �����Ǿ� �־�� ����� �� �� �ִ�.

public:
	// �ش� ����ũ�� ���� ��� �������� �����Ͽ�, ����� �ִϸ��̼� �ð��� ���Ѵ�.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// ����ũ�� ����Ǿ� �ִ� ���� ���� �� ���� ���� ����ġ�� ����س���.
	void Apply_FinalMask();

	// ����ũ�� ���� ������ �ִϸ��̼ǿ� ���� ���� ���� ����� ������ش�.
	void Apply_BoneAnimationWithMask(FArmatureData* const pArmatureData);

private:
	vector<FAnimMask>	m_vecAnimMask;					// �ִϸ��̼��� ����Ǵ� ����ũ, �⺻������ 0�� ����ũ�� ����Ǿ� �۵��Ѵ�.
	FBoneAnimData*		m_pFinalAnim = { nullptr };		// ���������� ����Ǵ� �ִϸ��̼� ����

	// �ִϸ��̼��� ���� ����ȭ�Ǿ� �۵��Ѵ�.
	// ���� ���� ������ �� �ӵ��� ���� �ε����� ����ϴ� ����� ����Ѵ�.
	// ������ ���� �ε����� �Ϸ�� ���¿��� �ϸ�, �ִϸ��̼� ���� ���� ���� �ε����� �Ϸ�� �����͸� ����ؾ� �Ѵ�.
	// �̴� �� �Ŵ������� �ε��� ������� �۵��ϵ��� �Ѵ�.
};

END