#pragma once

#include "InternalComponent.h"
#include "Component/Interface/IAnimationComponent.h"

BEGIN(Engine)

class CBoneAnimGroup;
class FBoneAnimData;
class FSkeletalData;
class FModelData;
class CBoneGroup;


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

struct TAnimData
{
	_uint		iAnimID;
	_float		fDuration;
	_float		fTickPerSeconds;
	_float		fTrackPos;
	_float		fSpeedMultiply;
	_bool		bIsLoop;
	_bool		bIsReverse;
};


/// <summary>
/// �ִϸ��̼ǿ� ���� ó���� ����ϴ� ������Ʈ
/// �� �ϳ��� �����ϴ� 
/// </summary>
class ENGINE_DLL CAnimationComponent final : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimationComponent)
protected:
	explicit CAnimationComponent();
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
	HRESULT Bind_BoneGroup(CBoneGroup* pBoneGroup);
	HRESULT Load_Animations(EModelGroupIndex eGroupIndex, const wstring& strModelFilePath);

	// �ش� ����ũ�� ���� ��� �������� �����Ͽ�, ����� �ִϸ��̼� �ð��� ���Ѵ�.
	void Apply_MaskTime(_uint iIndex, const wstring& strAnimName, _float fCurTime);
	void Apply_MaskTime(const wstring& strMaskName, const wstring& strAnimName, _float fCurTime);

	// ����ũ�� ����Ǿ� �ִ� ���� ���� �� ���� ���� ����ġ�� ����س���.
	void Apply_FinalMask();

public:
	CBoneGroup* const		Get_BoneGroup() const { return m_pBoneGroup; }
	CBoneAnimGroup* const	Get_AnimGroup() const { return m_pAnimGroup; }

private:
	CBoneGroup*			m_pBoneGroup = { nullptr };
	CBoneAnimGroup*		m_pAnimGroup = { nullptr };		// �� �ϳ��� ���� ���� �ִϸ��̼� ������ ������.
														// �� �༮�� �����Ǿ� �־�� ����� �� �� �ִ�.

public:
	_bool	IsAnimation_Finished();
	void Set_Animation(_uint iAnimIndex, _float fSpeedMultiply, _bool bIsLoop, _bool bReverse = false);
	void Add_AnimTime(const _float& fTimeDelta);
	void Invalidate_Animation();
	void Invalidate_AnimationWithMask(class CAnimMaskComp* pAnimMaskComp);
	
private:
	TAnimData			m_CurAnim = {};
	TAnimData			m_PrevAnim = {};
	_float				m_fTransitionSpeed;
	_float				m_fTransitionGauge;
	vector<FAnimMask>	m_vecAnimMask;					// �ִϸ��̼��� ����Ǵ� ����ũ, �⺻������ 0�� ����ũ�� ����Ǿ� �۵��Ѵ�.
};

END