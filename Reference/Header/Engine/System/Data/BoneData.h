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
	virtual FBoneNodeBaseData* Clone(FArmatureData* pArg) PURE;
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

public:
	static FBoneNodeData* Create();
	virtual FBoneNodeBaseData* Clone(FArmatureData* pArg) override;
	virtual void Free() override;

};

/// <summary>
/// 아마추어 정보가 저장되는 데이터,
/// 단, 아마추어 노드도 따로 노드로 저장하는데.
/// 이는 계층 구조를 만들어 저장하기 위함이다.
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
	// 아마추어 노드도 같이 저장된다.
	FBoneNodeData*						pArmatureNode = { nullptr };	// 루트 노드를 바로 찾을 수 있게 해놓았다.
	map<const wstring, FBoneNodeData*>	mapModelNodeData;				// 뼈 이름 검색
	vector<FBoneNodeData*>				vecModelNodeIndex;				// 뼈 인덱싱

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
	FArmatureData* Clone_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Find_ArmatureData(const wstring& strArmatureKey);
	FArmatureData* Create_ArmatureData(const wstring& strArmatureKey);
	void Appoint_ArmatureNode(const wstring& strArmatureKey, const wstring& strModelNodeKey);

	FBoneNodeData* Find_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);
	FBoneNodeData* Create_NodeData(const wstring& strArmatureKey, const wstring& strModelNodeKey);

private:
	_unmap<wstring, FArmatureData*> mapArmatureData;	// 메시 저장 맵
};

END