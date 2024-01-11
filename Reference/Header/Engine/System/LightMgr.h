#pragma once

#include "Base.h"

BEGIN(Engine)

class CLightMgr final : public CBase
{
	DERIVED_CLASS(CBase, CLightMgr)

private:
	explicit CLightMgr() = default;
	explicit CLightMgr(const CLightMgr& rhs) = delete;
	virtual ~CLightMgr() = default;

public:
	HRESULT				Initialize();
	HRESULT				Render(class CEffectComponent* pEffect, class CRectBufferComp* pVIBuffer);

public:
	static CLightMgr*	Create();
private:
	virtual void		Free();

public:
	HRESULT Add_Light(const TLIGHT_DESC& LightDesc);

private:
	list<class CLight*>			m_Lights;
};

END