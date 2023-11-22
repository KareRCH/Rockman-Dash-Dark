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
	/// <summary>
	/// 설정한 시간으로 보간된 행렬을 반환하는 함수
	/// </summary>
	/// <param name="fTime">변환된 시간을 넣어주어야함</param>
	/// <returns></returns>
	_float4x4 Interporated_Matrix(const _float& fCurTime);

	// 각각 배열의 기준점을 찾아주는 함수, 해당 기준점을 통해
	_uint Calculate_PivotPosition(const _float& fCurTime);
	_uint Calculate_PivotRotation(const _float& fCurTime);
	_uint Calculate_PivotScale(const _float& fCurTime);
	
public:
	struct FAnimPosition
	{
		_float fTime;
		_float3 vPos;
	};

	struct FAnimRotation
	{
		_float fTime;
		_float4 qtRot;
	};

	struct FAnimScale
	{
		_float fTime;
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
	// 시간 변화율로 애니메이션 타임라인의 현재 시간을 구해주는 함수, Mod를 켜면 반복됨
	_float Calculate_Time(const _float& fTimeDelta, _float fCurTime, _bool bMod = true);
	
public:
	_double dfDuration = 0.0;							// 진행 길이
	_double dfTickPerSecond = 0.0;						// 시간당 프레임
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