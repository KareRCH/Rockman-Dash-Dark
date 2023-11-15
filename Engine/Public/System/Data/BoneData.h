#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// »À¿¡ ´ëÇÑ Á¤º¸
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
	vector<_uint>			vecVtxIndex;
	vector<_float>			vecVtxWeights;
	_matrix					matOffset;
};

/// <summary>
/// »À¸¦ °ü¸®ÇÏ´Â ±×·ì
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
	_unmap<wstring, FBoneData*> mapBoneData;	// »À ÀúÀå ¸Ê
};


END