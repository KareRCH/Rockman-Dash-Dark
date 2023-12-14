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
	_float4x4	matTransform;				// 노드의 오프셋, 애니메이션으로 변함
	_float4x4	matFinalTransform;			// 최종 행렬, 부모의 행렬까지 곱해짐
};


/// <summary>
/// 뼈 계층들, 
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
	// 행렬 웨이트값을 적용하기 위해 행렬값을 더하는 연산을 하는 함수
	void Apply_Transforms(vector<_float4x4>* pVecMatrices);
	// 행렬 웨이트 계산전에 적용하는 청소 함수
	void Clear_FinalTransforms();

private:
	void Calculate_FinalTransforms();
	void Calculate_FinalTransformsNoHierarchi();

public:
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBones.size()); }

private:
	_unmap<wstring, FBoneData*>		mapBones;	// 스켈레탈 이름 검색
	vector<FBoneData*>				vecBones;	// 스켈레탈 인덱스 검색

};

END