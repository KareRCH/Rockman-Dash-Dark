#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// �ִϸ��̼� ä���� �����Ӻ� ���
/// </summary>
class ENGINE_DLL FBoneAnimNodeData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimNodeData)
private:
	explicit FBoneAnimNodeData() {}
	virtual ~FBoneAnimNodeData() = default;

public:
	static FBoneAnimNodeData* Create();
	virtual void Free() override;

public:
	/// <summary>
	/// ������ �ð����� ������ ����� ��ȯ�ϴ� �Լ�
	/// </summary>
	/// <param name="fTime">��ȯ�� �ð��� �־��־����</param>
	/// <returns></returns>
	_float4x4 Interporated_Matrix(const _float& fCurTime);

	// ���� �迭�� �������� ã���ִ� �Լ�, �ش� �������� ����
	_uint Calculate_PivotPosition(const _float& fCurTime);
	_uint Calculate_PivotRotation(const _float& fCurTime);
	_uint Calculate_PivotScale(const _float& fCurTime);
	
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
	explicit FBoneAnimData() {}
	virtual ~FBoneAnimData() = default;

public:
	static FBoneAnimData* Create();
	virtual void Free() override;

public:
	const FBoneAnimNodeData* const Get_AnimNodeData(const wstring& strNodeKey);
	void Add_AnimNodeData(const wstring& strNodeKey, FBoneAnimNodeData* pAnimNodeData);

public:
	// �ð� ��ȭ���� �ִϸ��̼� Ÿ�Ӷ����� ���� �ð��� �����ִ� �Լ�, Mod�� �Ѹ� �ݺ���
	_float Calculate_Time(const _float& fTimeDelta, _float fCurTime, _bool bMod = true);
	
public:
	_double dfDuration = 0.0;									// ���� ����
	_double dfTickPerSecond = 0.0;								// �ð��� ������
	map<const wstring, FBoneAnimNodeData*>	mapNodeAnim;		// ��� �̸����� �˻� �ý���
	vector<FBoneAnimNodeData*>				vecAnim_BoneIndex;	// �ִϸ��̼� ��� �ε��� ����
	
};

/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class ENGINE_DLL FBoneAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimGroup)
private:
	explicit FBoneAnimGroup() {}
	virtual ~FBoneAnimGroup() = default;

public:
	static FBoneAnimGroup* Create();
	virtual void Free() override;

public:
	const FBoneAnimData* const	Find_AnimData(const wstring& strAnimKey);
	void Add_AnimData(const wstring& strAnimKey, FBoneAnimData* pAnimData);
	
private:
	_unmap<wstring, FBoneAnimData*> mapAnimData;		// �ִϸ��̼� ���� ��, Ű�� �ִϸ��̼� �̸�
	
};

END