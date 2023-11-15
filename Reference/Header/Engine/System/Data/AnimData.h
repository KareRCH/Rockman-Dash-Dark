#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// �ִϸ��̼� ä���� �����Ӻ� ���
/// </summary>
class ENGINE_DLL FAnimNodeData final : public CBase
{
	DERIVED_CLASS(CBase, FAnimNodeData)
private:
	explicit FAnimNodeData() {}
	virtual ~FAnimNodeData() = default;

public:
	static FAnimNodeData* Create();
	virtual void Free() override;
	
public:
	struct FAnimPosition
	{
		_float vTime;
		_float3 vPos;
	};

	struct FAnimRotation
	{
		_float vTime;
		_float4 qtRot;
	};

	struct FAnimScale
	{
		_float vTime;
		_float3 vScale;
	};

public:
	vector<FAnimPosition>		vecPositions;			// ��ġ �����͵�
	vector<FAnimRotation>		vecRotations;			// ȸ�� �����͵�
	vector<FAnimScale>			vecScales;				// ũ�� �����͵�
};

/// <summary>
/// �ִϸ��̼� ��带 �����ϴ� ������
/// ��ü ��� �ð��� ����ӵ��� ����
/// </summary>
class ENGINE_DLL FAnimData final : public CBase
{
	DERIVED_CLASS(CBase, FAnimData)
private:
	explicit FAnimData() {}
	virtual ~FAnimData() = default;

public:
	static FAnimData* Create();
	virtual void Free() override;

public:
	const FAnimNodeData* const Get_AnimNodeData(const wstring& strNodeKey);
	void Add_AnimNodeData(const wstring& strNodeKey, FAnimNodeData* pAnimNodeData);
	
public:
	_double fDuration = 0.0;
	_double fTicksPerSecond = 0.0;
	_unmap<wstring, FAnimNodeData*> mapNodeAnim;		// ��� �̸����� �˻�
};

/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class ENGINE_DLL FAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FAnimGroup)
private:
	explicit FAnimGroup() {}
	virtual ~FAnimGroup() = default;

public:
	static FAnimGroup* Create();
	virtual void Free() override;

public:
	const FAnimData* const	Get_AnimData(const wstring& strAnimKey);
	void Add_AnimData(const wstring& strAnimKey, FAnimData* pAnimData);
	
private:
	_unmap<wstring, FAnimData*> mapAnimData;	// �ִϸ��̼� ���� ��, Ű�� �ִϸ��̼� �̸�

};

END