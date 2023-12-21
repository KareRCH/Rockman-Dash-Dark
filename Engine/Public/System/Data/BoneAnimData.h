#pragma once

#include "Base.h"

BEGIN(Engine)

struct FKeyFrame
{
	_float fTrackPosition;
	_float3 vPos;
	_float4 qtRot;
	_float3 vScale;
};

// �ܼ� Ű������ ���� �� �� ���δ�. �̋��� Ű �������� ����� ���� �־�� �Ѵ�.
struct FKeyFrameInterpolate
{
	_float				fWeight;
	vector<FKeyFrame>	KeyFrames;
};

// �ִϸ��̼� ������ ����, �ִϸ��̼ǿ� ���� ��ġ�� ���� �����ϱ� ������. ����ũ ������ Ʈ�������� �� ���δ�.
struct FAnimInterpolate
{
	_int			iAnimID;		// ���� �ĺ� �ִϸ��̼� ID
	vector<_int>	vecChannelIDs;	// ���� �ĺ� �ִϸ��̼��� ä�� ID��
	_float			fTrackPos;		// Ʈ�� ��ġ
	_float			fWeight;		// ���� ����ġ
};

/// <summary>
/// �ִϸ��̼� ä���� �����Ӻ� ���
/// </summary>
class ENGINE_DLL FBoneAnimChannelData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimChannelData)
private:
	explicit FBoneAnimChannelData() = default;
	explicit FBoneAnimChannelData(const FBoneAnimChannelData& rhs);
	virtual ~FBoneAnimChannelData() = default;

public:
	static FBoneAnimChannelData* Create();
	virtual void Free() override;

public:
	/// <summary>
	/// ������ �ð����� ������ ����� ��ȯ�ϴ� �Լ�
	/// </summary>
	/// <param name="fTime">������ ������ �־��ش�.</param>
	/// <returns></returns>
	_float4x4 Interpolated_Matrix(const _float& fCurTrackPos) const;
	FKeyFrame Interpolated_KeyFrame(const _float& fCurTrackPos) const;

	// ���� �迭�� �������� ã���ִ� �Լ�, �ش� �������� ���� �ִϸ��̼� ��ġ�� �˾Ƴ���.
	// ��ƿ���� ���ؼ� ���� ��ġ�� �����ϴ� ����� ������� �ʰ�, �������� ã���ش�.
	// �ݺ����� ���� �������� ã�Ƴ��µ�. �⺻���� �������� ������ ���ϸ�, �̴� Baking�� �����Ϳ� ���� ó����
	// �ظ��� ��Ȳ���� ����ð��� �˰����� ���´�.
	_uint Calculate_Pivot(const _float& fCurTrackPos) const;

public:
	_int					iBoneID = -1;			// �� ID
	_uint					m_iNumKeyFrames;		// Ű ������ ����
	vector<FKeyFrame>		vecKeyFrames;			// ��ġ �����͵�
};



/// <summary>
/// �ִϸ��̼� ��带 �����ϴ� ������
/// ��ü ��� �ð��� ����ӵ��� ����
/// </summary>
class ENGINE_DLL FBoneAnimData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimData)
private:
	explicit FBoneAnimData() = default;
	explicit FBoneAnimData(const FBoneAnimData& rhs) = delete;
	virtual ~FBoneAnimData() = default;

public:
	static FBoneAnimData* Create();
	virtual void Free() override;

public:
	const FBoneAnimChannelData* const Find_AnimChannelData(_uint iIndex) const;
	const FBoneAnimChannelData* const Find_AnimChannelData(const wstring& strNodeKey) const;
	void Add_AnimChannelData(const wstring& strNodeKey, FBoneAnimChannelData* pAnimNodeData);

public:
	// �ð� ��ȭ���� �ִϸ��̼� Ÿ�Ӷ����� ���� �ð��� �����ִ� �Լ�, Mod�� �Ѹ� �ݺ���
	_float Calculate_Time(_float fCurTime, _bool bMod = true) const;
	
	
public:
	_uint	iID = 0;				// �ִϸ��̼� ID
	wstring strName = L"";			// �ִϸ��̼� �̸�
	_float	fDuration = 0.0;		// ���� ����
	_float	fTickPerSecond = 0.0;	// �ð��� ������

public:
	map<const wstring, FBoneAnimChannelData*>	mapAnimChannels;	// ��� �̸����� �˻� �ý���
	vector<FBoneAnimChannelData*>				vecAnimChannels;	// �ִϸ��̼� �ε��� �˻� �ý���
	
};



/// <summary>
/// �� �ִϸ��̼ǵ��� �����ϴ� �׷�
/// </summary>
class ENGINE_DLL FBoneAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimGroup)
private:
	explicit FBoneAnimGroup() = default;
	explicit FBoneAnimGroup(const FBoneAnimGroup& rhs) = delete;
	virtual ~FBoneAnimGroup() = default;

public:
	static FBoneAnimGroup* Create();
	virtual void Free() override;

public:
	FBoneAnimData* const	Find_BoneAnim(const _uint iIndex);
	FBoneAnimData* const	Find_BoneAnim(const wstring& strAnimKey);
	void Add_BoneAnim(const wstring& strAnimKey, FBoneAnimData* pAnimData);

	
	// �� �ִϸ��̼ǿ� ���� ���� �������� �޾Ƴ��� �� �� ����Ѵ�.
	FKeyFrame Interpolated_Anim(const _uint iAnimIndex, const _uint iChannelIndex, const _float& fCurTrackPos) const;
	// ������ �ʿ��� ������ �޾� ������ �ִϸ��̼��� �̿��� ������ Ű�������� �����ִ� �Լ�
	void Interpolated_Anims(FKeyFrame* pKeyFrames, _uint iNumKeyFrames, FAnimInterpolate* pArrInterpolateData, _uint iNumInterpolates);
	// Ű�����ӳ��� ����, �̹� ���� Ű������ ������� ���� �����Ѵ�.
	void Interpolated_KeyFrames(FKeyFrame* pKeyFrames, _uint iNumKeyFrames, FKeyFrameInterpolate* pArrInterpolate, _uint iNumInterpolates);
	

private:
	_unmap<wstring, FBoneAnimData*> mapAnimDatas;	// �ִϸ��̼� ���� ��, Ű�� �ִϸ��̼� �̸�
	vector<FBoneAnimData*>			vecAnimDatas;	// �ִϸ��̼� �ε��� �˻�
	
};

END