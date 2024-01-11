#pragma once

#include "Base.h"

BEGIN(Engine)

class CLight : public CBase
{
	DERIVED_CLASS(CBase, CLight)

private:
	explicit CLight() = default;
	explicit CLight(const CLight& rhs) = delete;
	virtual ~CLight() = default;

public:
	HRESULT				Initialize(const TLIGHT_DESC& LightDesc);
	HRESULT				Render(class CEffectComponent* pEffect, class CVIBufferComp* pVIBuffer);

public:
	static CLight*		Create(const TLIGHT_DESC& LightDesc);
private:
	virtual void		Free();

private:
	TLIGHT_DESC			m_LightDesc;

};

END