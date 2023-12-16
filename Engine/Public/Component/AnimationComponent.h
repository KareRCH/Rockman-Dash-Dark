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
	wstring	strName;			// ����ũ �̸�
	_float fWeight;				// ����ũ ���� ����ġ
	vector<_bool> vecBoneMasks;	// ����ũ ���� ��

	wstring strSkeletaName;		// ���̷�Ż �̸�
	wstring strAnimName;		// �ִϸ��̼� �̸�
	_float fCurTime;			// ���� ��� ���� �ð�, �ý��� �ð� ����
};


/// <summary>
/// �ִϸ��̼ǿ� ���� ó���� ����ϴ� ������Ʈ
/// �� �ϳ��� �����ϴ� 
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

	// ����ũ ����
	HRESULT Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive);
	FAnimMask* Get_Mask(_uint iIndex);

	void Deactive_BoneMask(_uint iIndex, const wstring& strBoneName);
	void Active_BoneMask(_uint iIndex, const wstring& strBoneName);

	void Set_MaskAnimation(_uint iIndex, const wstring& strAnimName);
	void Set_MaskTime(_uint iIndex, _float fTime);
	void Set_TickDeltaTime(_float fDeltaTime);

	// �ش� ����ũ�� ���� ��� �������� �����Ͽ�, ����� �ִϸ��̼� �ð��� ���Ѵ�.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// ����ũ�� ����Ǿ� �ִ� ���� ���� �� ���� ���� ����ġ�� ����س���.
	void Apply_FinalMask();

private:
	FModelData*			m_pModelData = { nullptr };		// �� ������, �ش� ���� ���ε� ���ѳ��� ���� �س��´�.
	FBoneAnimGroup*		m_pAnimGroup = { nullptr };		// �� �ϳ��� ���� ���� �ִϸ��̼� ������ ������.
														// �� �༮�� �����Ǿ� �־�� ����� �� �� �ִ�.
	
	vector<FAnimMask>	m_vecAnimMask;					// �ִϸ��̼��� ����Ǵ� ����ũ, �⺻������ 0�� ����ũ�� ����Ǿ� �۵��Ѵ�.
	_float				m_fSystemTPS;				// 1 �����Ӵ� �ӵ���
};

END