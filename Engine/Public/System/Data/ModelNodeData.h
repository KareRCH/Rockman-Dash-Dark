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
	wstring							strname;		// ��� �̸�
	vector<class FModelNodeData*>	vecChildren;	// �ڽ� ����
	_matrix							matOffset;		// ����� ������
	_matrix							matTransform;	// ��Ӱ��谣�� ������ ��ȯ���
};


/// <summary>
/// �ַ� ���� ��Ʈ�� ���̴� ���
/// </summary>
class ENGINE_DLL FModelRootNodeData final : public FModelNodeBaseData
{
	DERIVED_CLASS(FModelNodeBaseData, FModelRootNodeData)

private:
	explicit FModelRootNodeData() {}
	explicit FModelRootNodeData(const FModelRootNodeData& rhs) = delete;
	virtual ~FModelRootNodeData() = default;

public:
	static FModelRootNodeData* Create();
	virtual void Free() override;

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
	static FModelNodeData* Create();
	virtual void Free() override;

};

END