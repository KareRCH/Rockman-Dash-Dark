#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 주로 뼈의 노드로 쓰이는 클래스
/// </summary>
class ENGINE_DLL FModelNodeBaseData abstract : public CBase
{
	DERIVED_CLASS(CBase, FModelNodeBaseData)

protected:
	explicit FModelNodeBaseData() {}
	explicit FModelNodeBaseData(const FModelNodeBaseData& rhs) = delete;
	virtual ~FModelNodeBaseData() = default;

protected:
	virtual void Free() override;

public:
	wstring							strname;				// 노드 이름
	FModelNodeBaseData*				pParent = { nullptr };	// 부모 노드
	vector<class FModelNodeData*>	vecChildren;			// 자식 노드들
	_float4x4						matOffset;				// 노드의 오프셋
	_float4x4						matTransform;			// 상속관계간에 정해진 변환행렬
};

/// <summary>
/// 주로 뼈의 자식 노드로 쓰이는 노드
/// </summary>
class ENGINE_DLL FModelNodeData final : public FModelNodeBaseData
{
	DERIVED_CLASS(FModelNodeBaseData, FModelNodeData)

private:
	explicit FModelNodeData() {}
	explicit FModelNodeData(const FModelNodeData& rhs) = delete;
	virtual ~FModelNodeData() = default;

public:
	_bool	Is_Root() { return (pParent == nullptr); }

public:
	static FModelNodeData* Create();
	virtual void Free() override;

};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class FModelNodeGroup final : public CBase
{
	DERIVED_CLASS(CBase, FModelNodeGroup)
private:
	explicit FModelNodeGroup() {}
	virtual ~FModelNodeGroup() = default;

public:
	static FModelNodeGroup* Create();
	virtual void Free() override;

public:
	_unmap<wstring, FModelNodeBaseData*> mapModelNodeData;	// 메시 저장 맵
};

END