#pragma once

#include "Base.h"

BEGIN(Engine)

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
	/// <param name="fTime">��ȯ�� �ð��� �־��־����</param>
	/// <returns></returns>
	_float4x4 Interporated_Matrix(const _float& fCurTime) const;

	// ���� �迭�� �������� ã���ִ� �Լ�, �ش� �������� ����
	_uint Calculate_PivotPosition(const _float& fCurTime) const;
	_uint Calculate_PivotRotation(const _float& fCurTime) const;
	_uint Calculate_PivotScale(const _float& fCurTime) const;
	
public:
	struct FPosition
	{
		_float fTime;
		_float3 vPos;
	};

	struct FRotation
	{
		_float fTime;
		_float4 qtRot;
	};

	struct FScale
	{
		_float fTime;
		_float3 vScale;
	};

public:
	_int					iBoneID = -1;			// �� ID
	vector<FPosition>		vecPositions;			// ��ġ �����͵�
	vector<FRotation>		vecRotations;			// ȸ�� �����͵�
	vector<FScale>			vecScales;				// ũ�� �����͵�
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
	_float fDuration = 0.0;			// ���� ����
	_float fTickPerSecond = 0.0;	// �ð��� ������

public:
	map<const wstring, FBoneAnimChannelData*>	mapAnimNodes;	// ��� �̸����� �˻� �ý���
	vector<FBoneAnimChannelData*>				vecAnimNodes;	// �ִϸ��̼� �ε��� �˻� �ý���
	
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

private:
	_unmap<wstring, FBoneAnimData*> mapAnimDatas;	// �ִϸ��̼� ���� ��, Ű�� �ִϸ��̼� �̸�
	vector<FBoneAnimData*>			vecAnimDatas;	// �ִϸ��̼� �ε��� �˻�
	
};

END