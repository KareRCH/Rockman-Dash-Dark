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
	_float4x4	matTransform;				// ����� ������, �ִϸ��̼����� ����
	_float4x4	matFinalTransform;			// ���� ���, �θ��� ��ı��� ������
};


/// <summary>
/// �� ������, 
/// </summary>
class FBoneGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneGroup)

private:
	explicit FBoneGroup() = default;
	explicit FBoneGroup(const FBoneGroup& rhs);
	virtual ~FBoneGroup() = default;

public:
	static FBoneGroup* Create();
	virtual FBoneGroup* Clone();
	virtual void Free() override;

public:
	FBoneData* Find_BoneData(_int iID);
	FBoneData* Find_BoneData(const wstring& strBoneNodeKey);

public:
	HRESULT				Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode);
	vector<_float4x4>	Provide_FinalTransforms(_bool bNoHierarchi = true);

public:
	// ��� ����Ʈ���� �����ϱ� ���� ��İ��� ���ϴ� ������ �ϴ� �Լ�
	void Apply_Transforms(vector<_float4x4>* pVecMatrices);
	// ��� ����Ʈ ������� �����ϴ� û�� �Լ�
	void Clear_FinalTransforms();

private:
	void Calculate_FinalTransforms();
	void Calculate_FinalTransformsNoHierarchi();

public:
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBones.size()); }

private:
	_unmap<wstring, FBoneData*>		mapBones;	// ���̷�Ż �̸� �˻�
	vector<FBoneData*>				vecBones;	// ���̷�Ż �ε��� �˻�

};

END