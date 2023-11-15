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
	wstring							strname;		// 노드 이름
	vector<class FModelNodeData*>	vecChildren;	// 자식 노드들
	_matrix							matOffset;		// 노드의 오프셋
	_matrix							matTransform;	// 상속관계간에 정해진 변환행렬
};


/// <summary>
/// 주로 뼈의 루트로 쓰이는 노드
/// </summary>
class ENGINE_DLL FModelRootNodeData final : public FModelNodeBaseData
{
	DERIVED_CLASS(FModelNodeBaseData, FModelRootNodeData)

private:
	explicit FModelRootNodeData() {}
	explicit FModelRootNodeData(const FModelRootNodeData& rhs) = delete;
	virtual ~FModelRootNodeData() = default;

public:
	static FModelRootNodeData* Create();
	virtual void Free() override;

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
	static FModelNodeData* Create();
	virtual void Free() override;

};

END