#pragma once

#include "Base.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// �ַ� ���� ���� ���̴� Ŭ����
/// </summary>
class ENGINE_DLL FBoneData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneData)

protected:
	explicit FBoneData() = default;
	explicit FBoneData(const FBoneData& rhs);
	virtual ~FBoneData() = default;

public:
	static FBoneData*	Create();
	virtual FBoneData*	Clone();
	virtual void		Free() override;

public:
	_int		iID = -1;				// �� ���п� ID
	wstring		strName;				// ��� �̸�

public:
	_int		iParentID = { -1 };		// �θ� ��� ID
	_float4x4	matOffset;				// ����� ������, ������ ����.
	_float4x4	matTransform;			// ���� ���
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
	explicit FSkeletalData() = default;
	explicit FSkeletalData(const FSkeletalData& rhs);
	virtual ~FSkeletalData() = default;

public:
	static FSkeletalData* Create();
	virtual FSkeletalData* Clone();
	virtual void Free() override;


public:
	FBoneData* Find_BoneData(_int iID);
	FBoneData* Find_BoneData(const wstring& strBoneNodeKey);

public:
	HRESULT				Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode);
	vector<_float4x4>	Provide_FinalTransforms(_bool bNoHierarchi = true);

public:
	// ��� ����Ʈ���� �����ϱ� ���� ��İ��� ���ϴ� ������ �ϴ� �Լ�
	void Add_Transforms(vector<_float4x4>* pVecMatrices) const;
	// ��� ����Ʈ ������� �����ϴ� û�� �Լ�
	void Clear_FinalTransforms();
	
private:
	void Calculate_FinalTransforms();
	void Calculate_FinalTransformsNoHierarchi();


public:
	const wstring&	Get_SkeletalName() { return strName; }
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBoneNodes.size()); }



public:
	_int		iID = -1;	// ID
	wstring		strName;	// �̸�

private:
	map<const wstring, FBoneData*>	mapBoneNodes;		// �� �̸� �˻�
	vector<FBoneData*>				vecBoneNodes;		// �� �ε��� �˻�
	_float4x4						matOffset;			// ���� ���
};



/// <summary>
/// � �𵨿� ���� �׷�
/// </summary>
class FSkeletalGroup final : public CBase
{
	DERIVED_CLASS(CBase, FSkeletalGroup)

private:
	explicit FSkeletalGroup() = default;
	explicit FSkeletalGroup(const FSkeletalGroup& rhs) = delete;
	virtual ~FSkeletalGroup() = default;

public:
	static FSkeletalGroup* Create();
	virtual void Free() override;

public:
	FSkeletalData*			Create_Skeletal(const wstring& strSkeletalKey);
	FSkeletalData*			Clone_Skeletal(const wstring& strSkeletalKey);
	// ���̷�Ż ã��
	FSkeletalData* const	Find_Skeletal(const _uint iIndex) const;
	FSkeletalData* const	Find_Skeletal(const wstring& strName) const;

public:
	_uint	Get_Skeletal_Count() const { return Cast<_uint>(mapSkeletals.size()); }

private:
	_unmap<wstring, FSkeletalData*> mapSkeletals;	// ���̷�Ż �̸� �˻�
	vector<FSkeletalData*>			vecSkeletals;	// ���̷�Ż �ε��� �˻�

};

END