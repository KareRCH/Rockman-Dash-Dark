#pragma once

#include "Base.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// 주로 뼈의 노드로 쓰이는 클래스
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
	_int		iID = -1;				// 뼈 구분용 ID
	wstring		strName;				// 노드 이름

public:
	_int		iParentID = { -1 };		// 부모 노드 ID
	_float4x4	matOffset;				// 노드의 오프셋, 변하지 않음.
	_float4x4	matTransform;			// 최종 행렬
};



/// <summary>
/// 아마추어 정보가 저장되는 데이터,
/// 단, 아마추어 노드도 따로 노드로 저장하는데.
/// 이는 계층 구조를 만들어 저장하기 위함이다.
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
	// 행렬 웨이트값을 적용하기 위해 행렬값을 더하는 연산을 하는 함수
	void Add_Transforms(vector<_float4x4>* pVecMatrices) const;
	// 행렬 웨이트 계산전에 적용하는 청소 함수
	void Clear_FinalTransforms();
	
private:
	void Calculate_FinalTransforms();
	void Calculate_FinalTransformsNoHierarchi();


public:
	const wstring&	Get_SkeletalName() { return strName; }
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBoneNodes.size()); }



public:
	_int		iID = -1;	// ID
	wstring		strName;	// 이름

private:
	map<const wstring, FBoneData*>	mapBoneNodes;		// 뼈 이름 검색
	vector<FBoneData*>				vecBoneNodes;		// 뼈 인덱스 검색
	_float4x4						matOffset;			// 기준 행렬
};



/// <summary>
/// 어떤 모델에 대한 그룹
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
	// 스켈레탈 찾기
	FSkeletalData* const	Find_Skeletal(const _uint iIndex) const;
	FSkeletalData* const	Find_Skeletal(const wstring& strName) const;

public:
	_uint	Get_Skeletal_Count() const { return Cast<_uint>(mapSkeletals.size()); }

private:
	_unmap<wstring, FSkeletalData*> mapSkeletals;	// 스켈레탈 이름 검색
	vector<FSkeletalData*>			vecSkeletals;	// 스켈레탈 인덱스 검색

};

END