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
	HRESULT				Initialize(const TLIGHT_DESC& LightDesc, const _uint iID);
	HRESULT				Render(class CEffectComponent* pEffect, class CVIBufferComp* pVIBuffer);

public:
	static CLight*		Create(const TLIGHT_DESC& LightDesc, const _uint iID);
private:
	virtual void		Free();

public:
	void Update_Light(const TLIGHT_DESC& Desc)
	{
		m_LightDesc = Desc;
	}

public:
	const _uint Get_ID() const { return m_iID; }
	const TLIGHT_DESC Get_Desc() const { return m_LightDesc; }

private:
	_uint				m_iID = { UINT_MAX };
	TLIGHT_DESC			m_LightDesc;

};

END