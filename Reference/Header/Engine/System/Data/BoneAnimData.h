#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 애니메이션 채널의 프레임별 노드
/// </summary>
class ENGINE_DLL FBoneAnimNodeData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimNodeData)
private:
	explicit FBoneAnimNodeData() {}
	virtual ~FBoneAnimNodeData() = default;

public:
	static FBoneAnimNodeData* Create();
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
	struct FPosition
	{
		_float fTime;
		_float3 vPos;
	};

	struct FRotation
	{
		_float fTime;
		_float4 qtRot;
	};

	struct FScale
	{
		_float fTime;
		_float3 vScale;
	};

public:
	_int					iBoneID = -1;			// 뼈 ID
	vector<FPosition>		vecPositions;			// 위치 데이터들
	vector<FRotation>		vecRotations;			// 회전 데이터들
	vector<FScale>			vecScales;				// 크기 데이터들
};

/// <summary>
/// 애니메이션 노드를 포함하는 데이터
/// 전체 재생 시간과 재생속도도 포함
/// </summary>
class ENGINE_DLL FBoneAnimData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimData)
private:
	explicit FBoneAnimData() {}
	virtual ~FBoneAnimData() = default;

public:
	static FBoneAnimData* Create();
	virtual void Free() override;

public:
	const FBoneAnimNodeData* const Get_AnimNodeData(const wstring& strNodeKey);
	void Add_AnimNodeData(const wstring& strNodeKey, FBoneAnimNodeData* pAnimNodeData);

public:
	// 시간 변화율로 애니메이션 타임라인의 현재 시간을 구해주는 함수, Mod를 켜면 반복됨
	_float Calculate_Time(const _float& fTimeDelta, _float fCurTime, _bool bMod = true);
	
public:
	_double dfDuration = 0.0;									// 진행 길이
	_double dfTickPerSecond = 0.0;								// 시간당 프레임
	map<const wstring, FBoneAnimNodeData*>	mapNodeAnim;		// 노드 이름으로 검색 시스템
	vector<FBoneAnimNodeData*>				vecAnim_BoneIndex;	// 애니메이션 노드 인덱스 관리
	
};

/// <summary>
/// 어떤 모델에 대한 그룹
/// </summary>
class ENGINE_DLL FBoneAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimGroup)
private:
	explicit FBoneAnimGroup() {}
	virtual ~FBoneAnimGroup() = default;

public:
	static FBoneAnimGroup* Create();
	virtual void Free() override;

public:
	const FBoneAnimData* const	Find_AnimData(const wstring& strAnimKey);
	void Add_AnimData(const wstring& strAnimKey, FBoneAnimData* pAnimData);
	
private:
	_unmap<wstring, FBoneAnimData*> mapAnimData;		// 애니메이션 저장 맵, 키는 애니메이션 이름
	
};

END