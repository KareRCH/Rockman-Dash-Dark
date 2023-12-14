#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL FMaterialData final : public CBase
{
	DERIVED_CLASS(CBase, FMaterialData)

private:
	explicit FMaterialData() = default;
	explicit FMaterialData(const FMaterialData& rhs) = delete;
	virtual ~FMaterialData() = default;

public:
	static FMaterialData* Create();
	virtual void Free() override;

public:
	wstring		strTexture[AI_TEXTURE_TYPE_MAX] = { TEXT("") };
};



class ENGINE_DLL FMaterialGroup final : public CBase
{
	DERIVED_CLASS(CBase, FMaterialGroup)

private:
	explicit FMaterialGroup() = default;
	explicit FMaterialGroup(const FMaterialGroup& rhs) = delete;
	virtual ~FMaterialGroup() = default;

public:
	static FMaterialGroup* Create();
	virtual void Free() override;

public:
	FMaterialData* Find_Material(const _uint iIndex);
	FMaterialData* Find_Material(const wstring& strName);
	HRESULT Add_Material(const wstring& strName, FMaterialData* pMatData);


private:
	map<wstring, FMaterialData*>	mapMaterialDatas;
	vector<FMaterialData*>			vecMaterialDatas;
};

END