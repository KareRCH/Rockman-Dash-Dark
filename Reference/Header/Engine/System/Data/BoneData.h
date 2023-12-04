#pragma once

#include "Base.h"
#include "System/Define/ModelMgr_Define.h"

BEGIN(Engine)

/// <summary>
/// 주로 뼈의 노드로 쓰이는 클래스
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
	_int							iID = -1;				// 뼈 구분용 ID
	EModelNodeType					eType;					// 노드 구분용
	EModelBoneType					eBoneType;				// 뼈 속성
	wstring							strName;				// 노드 이름
	FBoneNodeBaseData*				pParent = { nullptr };	// 부모 노드
	vector<class FBoneNodeData*>	vecChildren;			// 자식 노드들
	_float4x4						matOffset;				// 노드의 오프셋
	_float4x4						matTransform;			// 상속관계간에 정해진 변환행렬
};

/// <summary>
/// 주로 뼈의 자식 노드로 쓰이는 노드
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
/// 아마추어 정보가 저장되는 데이터,
/// 단, 아마추어 노드도 따로 노드로 저장하는데.
/// 이는 계층 구조를 만들어 저장하기 위함이다.
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
	// 행렬 웨이트값을 적용하기 위해 행렬값을 더하는 연산을 하는 함수
	void Add_Transforms(vector<_float4x4>* pVecMatrices) const;
	void Clear_FinalTransforms();
	
private:
	void Calculate_FinalTransforms();

private:
	// 아마추어 노드도 같이 저장된다.
	FBoneNodeData*						pSkeletalRootNode = { nullptr };	// 루트 노드를 바로 찾을 수 있게 해놓았다.
	map<const wstring, FBoneNodeData*>	mapBoneNodeDatas;					// 뼈 이름 검색
	vector<FBoneNodeData*>				vecBoneNodeIndexes;					// 뼈 인덱싱

};

/// <summary>
/// 어떤 모델에 대한 그룹
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
	_unmap<wstring, FSkeletalData*> mapSkeletaDatas;	// 스켈레탈 저장 맵
};

END