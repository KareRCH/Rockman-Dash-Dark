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
/// �� ��� ������ ����
/// </summary>
class CBoneGroup final : public CBase
{
	DERIVED_CLASS(CBase, CBoneGroup)

private:
	explicit CBoneGroup() = default;
	explicit CBoneGroup(const CBoneGroup& rhs);
	virtual ~CBoneGroup() = default;

public:
	static CBoneGroup* Create();
	virtual CBoneGroup* Clone();
	virtual void Free() override;

public:
	FBoneData* Find_BoneData(_int iID);
	FBoneData* Find_BoneData(const wstring& strBoneNodeKey);

public:
	// �� �߰��Լ�
	HRESULT					Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode);
	// ���� ��� �ּҸ� ��� �Լ�, ���̴��� ���ε��� �� ���δ�.
	const _float4x4* const	Provide_BoneFinalTransformPtr(_uint iIndex) const;

public:
	// Ʈ������ ����� Ư�� ���� �����Ѵ�. �ִϸ��̼��� ������ �� ���δ�.
	HRESULT			Set_BoneTransform(_uint iIndex, _fmatrix& matTransform);
	// �� ������Ʈ �Լ�, �ִϸ��̼� ���� �Ŀ� �ҷ��´�.
	void			Invalidate_FinalTransforms();

public:
	// ���� ����
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBones.size()); }

private:
	_uint							m_iNumBones;	// �� ����
	_unmap<wstring, FBoneData*>		mapBones;		// �� �̸� �˻�
	vector<FBoneData*>				vecBones;		// �� �ε��� �˻�

};

END