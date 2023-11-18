#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// 뼈에 대한 정보
/// 오프셋과 변환 행렬, 이름을 가진다.
/// </summary>
class FBoneData final : public CBase
{
	DERIVED_CLASS(CBase, FBoneData)
private:
	explicit FBoneData() {}
	virtual ~FBoneData() = default;

public:
	static FBoneData* Create();
	virtual void Free() override;
	
public:
	wstring					strName;
	_float4x4				matOffset;
	_float4x4				matTransform;
};

/// <summary>
/// 뼈를 관리하는 그룹
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
	const FBoneData* const	Get_Bone(const wstring& strBoneKey);
	void Add_Bone(const wstring& strBoneKey, FBoneData* pBoneData);
	

public:
	_unmap<wstring, FBoneData*> mapBoneData;	// 뼈 저장 맵
};


END