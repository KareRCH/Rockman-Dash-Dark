#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 애니메이션 채널의 프레임별 노드
/// </summary>
class ENGINE_DLL FAnimNodeData final : public CBase
{
	DERIVED_CLASS(CBase, FAnimNodeData)
private:
	explicit FAnimNodeData() {}
	virtual ~FAnimNodeData() = default;

public:
	static FAnimNodeData* Create();
	virtual void Free() override;
	
public:
	struct FAnimPosition
	{
		_float vTime;
		_float3 vPos;
	};

	struct FAnimRotation
	{
		_float vTime;
		_float4 qtRot;
	};

	struct FAnimScale
	{
		_float vTime;
		_float3 vScale;
	};

public:
	vector<FAnimPosition>		vecPositions;			// 위치 데이터들
	vector<FAnimRotation>		vecRotations;			// 회전 데이터들
	vector<FAnimScale>			vecScales;				// 크기 데이터들
};

/// <summary>
/// 애니메이션 노드를 포함하는 데이터
/// 전체 재생 시간과 재생속도도 포함
/// </summary>
class ENGINE_DLL FAnimData final : public CBase
{
	DERIVED_CLASS(CBase, FAnimData)
private:
	explicit FAnimData() {}
	virtual ~FAnimData() = default;

public:
	static FAnimData* Create();
	virtual void Free() override;

public:
	const FAnimNodeData* const Get_AnimNodeData(const wstring& strNodeKey);
	void Add_AnimNodeData(const wstring& strNodeKey, FAnimNodeData* pAnimNodeData);
	
public:
	_double fDuration = 0.0;
	_double fTicksPerSecond = 0.0;
	_unmap<wstring, FAnimNodeData*> mapNodeAnim;		// 노드 이름으로 검색
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class ENGINE_DLL FAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FAnimGroup)
private:
	explicit FAnimGroup() {}
	virtual ~FAnimGroup() = default;

public:
	static FAnimGroup* Create();
	virtual void Free() override;

public:
	const FAnimData* const	Get_AnimData(const wstring& strAnimKey);
	void Add_AnimData(const wstring& strAnimKey, FAnimData* pAnimData);
	
private:
	_unmap<wstring, FAnimData*> mapAnimData;	// 애니메이션 저장 맵, 키는 애니메이션 이름

};

END