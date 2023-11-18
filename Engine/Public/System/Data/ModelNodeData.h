#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// �ַ� ���� ���� ���̴� Ŭ����
/// </summary>
class ENGINE_DLL FModelNodeBaseData abstract : public CBase
{
	DERIVED_CLASS(CBase, FModelNodeBaseData)

protected:
	explicit FModelNodeBaseData() {}
	explicit FModelNodeBaseData(const FModelNodeBaseData& rhs) = delete;
	virtual ~FModelNodeBaseData() = default;

protected:
	virtual void Free() override;

public:
	wstring							strname;				// ��� �̸�
	FModelNodeBaseData*				pParent = { nullptr };	// �θ� ���
	vector<class FModelNodeData*>	vecChildren;			// �ڽ� ����
	_float4x4						matOffset;				// ����� ������
	_float4x4						matTransform;			// ��Ӱ��谣�� ������ ��ȯ���
};

/// <summary>
/// �ַ� ���� �ڽ� ���� ���̴� ���
/// </summary>
class ENGINE_DLL FModelNodeData final : public FModelNodeBaseData
{
	DERIVED_CLASS(FModelNodeBaseData, FModelNodeData)

private:
	explicit FModelNodeData() {}
	explicit FModelNodeData(const FModelNodeData& rhs) = delete;
	virtual ~FModelNodeData() = default;

public:
	_bool	Is_Root() { return (pParent == nullptr); }

public:
	static FModelNodeData* Create();
	virtual void Free() override;

};

/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class FModelNodeGroup final : public CBase
{
	DERIVED_CLASS(CBase, FModelNodeGroup)
private:
	explicit FModelNodeGroup() {}
	virtual ~FModelNodeGroup() = default;

public:
	static FModelNodeGroup* Create();
	virtual void Free() override;

public:
	_unmap<wstring, FModelNodeBaseData*> mapModelNodeData;	// �޽� ���� ��
};

END