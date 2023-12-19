#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class FBoneAnimGroup;
class FBoneAnimData;
class FSkeletalData;
class FModelData;
class FBoneGroup;


struct FAnimMask
{
	wstring			strName;			// ����ũ �̸�
	_float			fWeight;			// ����ũ ���� ����ġ

	_uint			iNumMasks;			// ����ũ ����
	vector<_bool>	vecBoneMasks;		// ����ũ ���� ��

	wstring			strAnimName;		// �ִϸ��̼� �̸�
	_uint			iAnimID;			// �ִϸ��̼� ID
	_float			fCurTrackPos;		// ���� ��� ���� Ʈ�� ��ġ
	_float			fDuration;			// ���ӽð�
	_float			fTickPerSeconds;	// ����ӵ�
	
	wstring			strPrevAnimName;	// ���� �ִϸ��̼� �̸�
	_uint			iPrevAnimID;		// ���� �ִϸ��̼� ID
	_float			fPrevTrackPos;		// ���� �ִ� ��� ���� Ʈ�� ��ġ
	_float			fTransitionSpeed;	// �ִϸ��̼� ��ȯ�ӵ� 0~1, 1�̸� ���, 0�̸� �ȹٲ�� �ּ� 0���� Ŀ����.
	_float			fTransitionGauge;	// �ִϸ��̼� ��ȯ������, 1���� ���� �� Ʈ������ �ӵ��� ��������.
										// 1�� ������ �����ϰ� ������ ���ο� �ִϸ��̼����� ��ü�Ѵ�.
	
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
	// 
	HRESULT Bind_BoneGroup(class CSkeletalComponent* pSkeletalComp);
	HRESULT Bind_AnimGroup(FModelData* pModelData);

	// ����ũ ����
	HRESULT Create_Mask(const wstring& strMaskName, const wstring& strSkeletalName, _bool bInitBoneActive);
	FAnimMask* Get_Mask(_uint iIndex);

	void Deactive_BoneMask(_uint iIndex, const wstring& strBoneName);
	void Active_BoneMask(_uint iIndex, const wstring& strBoneName);

	void Set_MaskAnimation(_uint iIndex, const wstring& strAnimName);
	void Set_MaskTime(_uint iIndex, _float fTime);

	// �ش� ����ũ�� ���� ��� �������� �����Ͽ�, ����� �ִϸ��̼� �ð��� ���Ѵ�.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// ����ũ�� ����Ǿ� �ִ� ���� ���� �� ���� ���� ����ġ�� ����س���.
	void Apply_FinalMask();

private:
	FBoneGroup*			m_pBoneGroup = { nullptr };		// �� ������, �ش� ���� ���ε� ���ѳ��� ���� �س��´�.
	FBoneAnimGroup*		m_pAnimGroup = { nullptr };		// �� �ϳ��� ���� ���� �ִϸ��̼� ������ ������.
														// �� �༮�� �����Ǿ� �־�� ����� �� �� �ִ�.
	
	vector<FAnimMask>	m_vecAnimMask;					// �ִϸ��̼��� ����Ǵ� ����ũ, �⺻������ 0�� ����ũ�� ����Ǿ� �۵��Ѵ�.
	_float				m_fSystemTPS;				// 1 �����Ӵ� �ӵ���
};

END