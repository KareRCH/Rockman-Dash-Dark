#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 애니메이션 채널의 프레임별 노드
/// </summary>
class ENGINE_DLL FBoneAnimChannelData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimChannelData)
private:
	explicit FBoneAnimChannelData() = default;
	explicit FBoneAnimChannelData(const FBoneAnimChannelData& rhs);
	virtual ~FBoneAnimChannelData() = default;

public:
	static FBoneAnimChannelData* Create();
	virtual void Free() override;

public:
	/// <summary>
	/// 설정한 시간으로 보간된 행렬을 반환하는 함수
	/// </summary>
	/// <param name="fTime">변환된 시간을 넣어주어야함</param>
	/// <returns></returns>
	_float4x4 Interporated_Matrix(const _float& fCurTime) const;

	// 각각 배열의 기준점을 찾아주는 함수, 해당 기준점을 통해
	_uint Calculate_PivotPosition(const _float& fCurTime) const;
	_uint Calculate_PivotRotation(const _float& fCurTime) const;
	_uint Calculate_PivotScale(const _float& fCurTime) const;
	
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
	explicit FBoneAnimData() = default;
	explicit FBoneAnimData(const FBoneAnimData& rhs) = delete;
	virtual ~FBoneAnimData() = default;

public:
	static FBoneAnimData* Create();
	virtual void Free() override;

public:
	const FBoneAnimChannelData* const Find_AnimChannelData(_uint iIndex) const;
	const FBoneAnimChannelData* const Find_AnimChannelData(const wstring& strNodeKey) const;
	void Add_AnimChannelData(const wstring& strNodeKey, FBoneAnimChannelData* pAnimNodeData);

public:
	// 시간 변화율로 애니메이션 타임라인의 현재 시간을 구해주는 함수, Mod를 켜면 반복됨
	_float Calculate_Time(_float fCurTime, _bool bMod = true) const;
	
public:
	_float fDuration = 0.0;			// 진행 길이
	_float fTickPerSecond = 0.0;	// 시간당 프레임

public:
	map<const wstring, FBoneAnimChannelData*>	mapAnimNodes;	// 노드 이름으로 검색 시스템
	vector<FBoneAnimChannelData*>				vecAnimNodes;	// 애니메이션 인덱스 검색 시스템
	
};



/// <summary>
/// 뼈 애니메이션들을 저장하는 그룹
/// </summary>
class ENGINE_DLL FBoneAnimGroup final : public CBase
{
	DERIVED_CLASS(CBase, FBoneAnimGroup)
private:
	explicit FBoneAnimGroup() = default;
	explicit FBoneAnimGroup(const FBoneAnimGroup& rhs) = delete;
	virtual ~FBoneAnimGroup() = default;

public:
	static FBoneAnimGroup* Create();
	virtual void Free() override;

public:
	FBoneAnimData* const	Find_BoneAnim(const _uint iIndex);
	FBoneAnimData* const	Find_BoneAnim(const wstring& strAnimKey);
	void Add_BoneAnim(const wstring& strAnimKey, FBoneAnimData* pAnimData);

private:
	_unmap<wstring, FBoneAnimData*> mapAnimDatas;	// 애니메이션 저장 맵, 키는 애니메이션 이름
	vector<FBoneAnimData*>			vecAnimDatas;	// 애니메이션 인덱스 검색
	
};

END