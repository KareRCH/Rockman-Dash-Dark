#pragma once

#include "Base.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// �ַ� ���� ���� ���̴� Ŭ����
/// </summary>
class ENGINE_DLL FBoneNodeBaseData abstract : public CBase
{
	DERIVED_CLASS(CBase, FBoneNodeBaseData)

protected:
	explicit FBoneNodeBaseData() {}
	explicit FBoneNodeBaseData(const FBoneNodeBaseData& rhs);
	virtual ~FBoneNodeBaseData() = default;

protected:
	virtual FBoneNodeBaseData* Clone(FArmatureData* pArg) PURE;
	virtual void Free() override;

public:
	_int							iID = -1;				// �� ���п� ID
	EModelNodeType					eType;					// ��� ���п�
	EModelBoneType					eBoneType;				// �� �Ӽ�
	wstring							strName;				// ��� �̸�
	FBoneNodeBaseData*				pParent = { nullptr };	// �θ� ���
	vector<class FBoneNodeData*>	vecChildren;			// �ڽ� ����
	_float4x4						matOffset;				// ����� ������
	_float4x4						matTransform;			// ��Ӱ��谣�� ������ ��ȯ���
};

/// <summary>
/// �ַ� ���� �ڽ� ���� ���̴� ���
/// </summary>
class ENGINE_DLL FBoneNodeData final : public FBoneNodeBaseData
{
	DERIVED_CLASS(FBoneNodeBaseData, FBoneNodeData)

private:
	explicit FBoneNodeData() {}
	explicit FBoneNodeData(const FBoneNodeData& rhs);
	virtual ~FBoneNodeData() = default;

public:
	_bool	Is_Root() { return (pParent == nullptr); }

public:
	FBoneNodeData* Find_NodeFromID(_int iID);

public:
	static FBoneNodeData* Create();
	virtual FBoneNodeBaseData* Clone(FArmatureData* pArg) override;
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
	explicit FArmatureData(const FArmatureData& rhs);
	virtual ~FArmatureData() = default;

public:
	static FArmatureData* Create();
	virtual FArmatureData* Clone();
	virtual void Free() override;

public:
	FBoneNodeData* Find_NodeData(_int iID);
	FBoneNodeData* Find_NodeData(const wstring& strModelNodeKey);
	FBoneNodeData* Create_NodeData(const wstring& strModelNodeKey);
	void Appoint_ArmatureNode(const wstring& strModelNodeKey);
	HRESULT Add_NodeData(const wstring& strModelNodeKey, FBoneNodeData* pNode);
	vector<const _float4x4*>	Provide_FinalTransforms();

private:
	// �Ƹ��߾� ��嵵 ���� ����ȴ�.
	FBoneNodeData*						pArmatureNode = { nullptr };	// ��Ʈ ��带 �ٷ� ã�� �� �ְ� �س��Ҵ�.
	map<const wstring, FBoneNodeData*>	mapModelNodeData;				// �� �̸� �˻�
	vector<FBoneNodeData*>				vecModelNodeIndex;				// �� �ε���

};

/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class FBoneGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneGroup)
private:
	explicit FBoneGroup() {}
	virtual ~FBoneGroup() = default;

public:
	static FBoneGroup* Create();
	virtual void Free() override;

public:
	FArmatureData* Clone_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Find_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Create_ArmatureData(const wstring& strArmatureKey);
	void Appoint_ArmatureNode(const wstring& strArmatureKey, const wstring& strModelNodeKey);

	FBoneNodeData* Find_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);
	FBoneNodeData* Create_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);

private:
	_unmap<wstring, FArmatureData*> mapArmatureData;	// �޽� ���� ��
};

END