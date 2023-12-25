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
/// 뼈 노드 데이터 집합
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
	// 뼈 추가함수
	HRESULT					Add_BoneData(const wstring& strBoneNodeKey, FBoneData* pNode);
	// 뼈의 행렬 주소를 얻는 함수, 셰이더에 바인딩할 때 쓰인다.
	const _float4x4* const	Provide_BoneFinalTransformPtr(_uint iIndex) const;

public:
	// 트랜스폼 행렬을 특정 뼈에 적용한다. 애니메이션을 적용할 때 쓰인다.
	HRESULT			Set_BoneTransform(_uint iIndex, _fmatrix& matTransform);
	// 뼈 업데이트 함수, 애니메이션 적용 후에 불러온다.
	void			Invalidate_FinalTransforms();

public:
	// 뼈의 개수
	const _uint		Get_BoneDatas_Count() const { return Cast<_uint>(vecBones.size()); }

private:
	_uint							m_iNumBones;	// 뼈 개수
	_unmap<wstring, FBoneData*>		mapBones;		// 뼈 이름 검색
	vector<FBoneData*>				vecBones;		// 뼈 인덱스 검색

};

END