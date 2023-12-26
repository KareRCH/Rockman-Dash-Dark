#pragma once

#include "Component/InternalComponent.h"

BEGIN(Engine)

struct TAnimMaskTrack
{
	_uint		iAnimID;				// �ִϸ��̼� ID
	_float		fTrackPos;				// �ִϸ��̼� ��� ��ġ
	_float		fDuration;				// �ִϸ��̼� ���ӽð�
	_float		fTickPerseconds;		// �ִϸ��̼� ����ӵ�
	_bool		bIsLoop;				// �ִϸ��̼� ��������
};

struct TAnimMask
{
	wstring			strName;			// ����ũ �̸�
	_float			fWeight;			// ����ũ ���� ����ġ

	_uint			iNumBoneMasks;		// ����ũ �� ����
	vector<_bool>	vecBoneMasks;		// ����ũ ���� ��, �ִϸ��̼��� ����Ǵ� ���� �����ȴ�.
	vector<_float>	vecBoneMaskWeights;	// ����ũ ���� ����ġ, ����ũ ����ġ�� �ٲ�� ������ �Ͼ �� �ٲ��.

	TAnimMaskTrack	tCurAnim;			// ���� �ִϸ��̼�
	TAnimMaskTrack	tPrevAnim;			// ���� �ִϸ��̼�

	_float			fTransitionSpeed;	// �ִϸ��̼� ��ȯ�ӵ� 0~1, 1�̸� ���, 0�̸� �ȹٲ�� �ּ� 0���� Ŀ����.
	_float			fTransitionGauge;	// �ִϸ��̼� ��ȯ������, 1���� ���� �� Ʈ������ �ӵ��� ��������.
										// 1�� ������ �����ϰ� ������ ���ο� �ִϸ��̼����� ��ü�Ѵ�.
};

/// <summary>
/// �ִϸ��̼ǿ� ���� ����ũ�� ����� �����ϴ� ����� ���� Ŭ����
/// ���� ���� �ִϸ��̼ǿ� ���� ���� �ٸ� �ִϸ��̼��� ����� ���
/// ���� Ű�������� ����Ǵ� ������ �ִϸ��̼� ������Ʈ���� ����ȴ�.
/// </summary>
class ENGINE_DLL CAnimMaskComp : public CInternalComponent
{
	DERIVED_CLASS(CInternalComponent, CAnimMaskComp)
protected:
	explicit CAnimMaskComp() = default;
	explicit CAnimMaskComp(const CAnimMaskComp& rhs);
	virtual ~CAnimMaskComp() = default;

public:
	virtual HRESULT Initialize_Prototype(void* Arg = nullptr) override;
	virtual HRESULT Initialize(void* Arg = nullptr) override;

public:
	static CAnimMaskComp*	Create();
	virtual CComponent*		Clone(void* Arg = nullptr) override;

protected:
	virtual void			Free() override;

public:		// �ִ� ����ũ ����
	// �� ������ ������� ����ũ�� �����Ѵ�.
	HRESULT		Create_Mask(const wstring& strMaskName, class CAnimationComponent* pAnimComp, _float fWeight, _bool bInitBoneActive);
	// ����ũ ����Ʈ�� �����ϱ� ���� ���δ�.
	void		Set_MaskWeight(_uint iMaskIndex, _float fWeight);
	// ����ũ ����Ʈ�� ������Ʈ �Ѵ�. ����ũ�� ����Ʈ�� ����� ���� �۵��ϵ��� �Ѵ�.
	void		Invalidate_MaskWeights();

public:		// �� ����ũ ����
	// ����ũ�� ��ü Ȱ��ȭ�ϱ� ���� ���δ�.
	void		Active_BoneMask(_uint iMaskIndex);
	// ����ũ�� ��ü ��Ȱ��ȭ�ϱ� ���� ���δ�.
	void		Deactive_BoneMask(_uint iMaskIndex);
	// ����ũ�� �� ����ũ�� �ϳ��ϳ� �����ϱ� ���� ���δ�.
	void		Set_BoneMaskAvailable(_uint iMaskIndex, _uint iBoneMaskIndex, _bool bActive);

public:
	const _uint& Get_NumAnimMasks() const { return m_iNumAnimMasks; }
	// ���� ���� Weight�� ��ȯ�ϴ� �Լ�. �ִϸ��̼� ������Ʈ���� �� �ִϸ��̼��� ����� �� ���δ�.
	const vector<_float>& Get_RemainWeights() const { return m_vecRemainWeights; }

private:
	_uint					m_iNumAnimMasks = { 0 };		// ����ũ ����
	vector<TAnimMask>		m_vecAnimMasks;					// ����ũ
	vector<_float>			m_vecRemainWeights;				// ����ũ�� ���� ���� ����Ʈ ��

};

END