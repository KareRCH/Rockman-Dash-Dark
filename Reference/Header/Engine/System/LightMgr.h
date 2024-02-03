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
	HRESULT Add_Light(const TLIGHT_DESC& LightDesc, _uint& iReturnID, class CLight** GetLight);
	HRESULT Remove_Light(const _uint iID);

private:
	static _uint g_iID;
	list<class CLight*>			m_Lights;
};

END