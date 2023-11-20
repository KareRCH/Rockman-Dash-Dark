#pragma once

#include "Base.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// �ַ� ���� ���� ���̴� Ŭ����
/// </summary>
class ENGINE_DLL FModelNodeBaseData abstract : public CBase
{
	DERIVED_CLASS(CBase, FModelNodeBaseData)

protected:
	explicit FModelNodeBaseData() {}
	explicit FModelNodeBaseData(const FModelNodeBaseData& rhs);
	virtual ~FModelNodeBaseData() = default;

protected:
	virtual FModelNodeBaseData* Clone() PURE;
	virtual void Free() override;

public:
	_int							iID = -1;				// �� ���п� ID
	EModelNodeType					eType;					// ��� ���п�
	EModelBoneType					eBoneType;				// �� �Ӽ�
	wstring							strName;				// ��� �̸�
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
	explicit FModelNodeData(const FModelNodeData& rhs);
	virtual ~FModelNodeData() = default;

public:
	_bool	Is_Root() { return (pParent == nullptr); }

public:
	FModelNodeData* Find_NodeFromID(_int iID);

public:
	static FModelNodeData* Create();
	virtual FModelNodeBaseData* Clone() override;
	virtual void Free() override;

};

/// <summary>
/// �Ƹ��߾� ������ ����Ǵ� ������,
/// ��, �Ƹ��߾� ��嵵 ���� ���� �����ϴµ�.
/// �̴� ���� ������ ����� �����ϱ� �����̴�.
/// </summary>
class ENGINE_DLL FArmatureData final : public CBase
{
	DERIVED_CLASS(CBase, FArmatureData)

private:
	explicit FArmatureData() {}
	explicit FArmatureData(const FArmatureData& rhs) = delete;
	virtual ~FArmatureData() = default;

public:
	static FArmatureData* Create();
	virtual FArmatureData* Clone();
	virtual void Free() override;

public:
	FModelNodeData* Find_NodeData(_int iID);
	FModelNodeData* Find_NodeData(const wstring& strModelNodeKey);
	FModelNodeData* Create_NodeData(const wstring& strModelNodeKey);
	void Appoint_ArmatureNode(const wstring& strModelNodeKey);

private:
	// �Ƹ��߾� ��嵵 ���� ����ȴ�.
	FModelNodeData*						pArmatureNode = { nullptr };	// ��Ʈ ��带 �ٷ� ã�� �� �ְ� �س��Ҵ�.
	_unmap<wstring, FModelNodeData*>	mapModelNodeData;

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
	FArmatureData* Clone_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Find_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Create_ArmatureData(const wstring& strArmatureKey);
	void Appoint_ArmatureNode(const wstring& strArmatureKey, const wstring& strModelNodeKey);

	FModelNodeData* Find_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);
	FModelNodeData* Create_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);

private:
	_unmap<wstring, FArmatureData*> mapArmatureData;	// �޽� ���� ��
};

END