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
	virtual FBoneNodeBaseData* Clone(FSkeletalData* pArg) PURE;
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
	void Calculate_FinalTransform();

public:
	static FBoneNodeData* Create();
	virtual FBoneNodeBaseData* Clone(FSkeletalData* pArg) override;
	virtual void Free() override;

};

/// <summary>
/// �Ƹ��߾� ������ ����Ǵ� ������,
/// ��, �Ƹ��߾� ��嵵 ���� ���� �����ϴµ�.
/// �̴� ���� ������ ����� �����ϱ� �����̴�.
/// </summary>
class ENGINE_DLL FSkeletalData final : public CBase
{
	DERIVED_CLASS(CBase, FSkeletalData)

private:
	explicit FSkeletalData() {}
	explicit FSkeletalData(const FSkeletalData& rhs);
	virtual ~FSkeletalData() = default;

public:
	static FSkeletalData* Create();
	virtual FSkeletalData* Clone();
	virtual void Free() override;

public:
	wstring Get_SkeletalName();
	_uint Get_BoneNodeData_Count();
	FBoneNodeData* Find_BoneNodeData(_int iID);
	FBoneNodeData* Find_BoneNodeData(const wstring& strBoneNodeKey);
	void Appoint_SkeletalRootNode(const wstring& strBoneNodeKey);
	HRESULT Add_BoneNodeData(const wstring& strBoneNodeKey, FBoneNodeData* pNode);
	vector<_float4x4>	Provide_FinalTransforms();
	// ��� ����Ʈ���� �����ϱ� ���� ��İ��� ���ϴ� ������ �ϴ� �Լ�
	void Add_Transforms(vector<_float4x4>* pVecMatrices) const;
	void Clear_FinalTransforms();
	
private:
	void Calculate_FinalTransforms();

private:
	// �Ƹ��߾� ��嵵 ���� ����ȴ�.
	FBoneNodeData*						pSkeletalRootNode = { nullptr };	// ��Ʈ ��带 �ٷ� ã�� �� �ְ� �س��Ҵ�.
	map<const wstring, FBoneNodeData*>	mapBoneNodeDatas;					// �� �̸� �˻�
	vector<FBoneNodeData*>				vecBoneNodeIndexes;					// �� �ε���

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
	FSkeletalData* Clone_SkeletalData(const wstring& strSkeletalKey);
	FSkeletalData* Find_SkeletalData(const wstring& strSkeletalKey);
	FSkeletalData* Create_SkeletalData(const wstring& strSkeletalKey);
	void Appoint_SkeletalRootNode(const wstring& strSkeletalKey, const wstring& strBoneNodeKey);

	FBoneNodeData* Find_BoneNodeData(const wstring& strSkeletalKey, const wstring& strBoneNodeKey);
	HRESULT Add_BoneNodeData(const wstring& strSkeletalKey, const wstring& strBoneNodeKey, FBoneNodeData* pNode);
	_uint Get_Skeletal_Count();

private:
	_unmap<wstring, FSkeletalData*> mapSkeletaDatas;	// ���̷�Ż ���� ��
};

END