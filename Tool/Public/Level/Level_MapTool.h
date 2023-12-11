#pragma once

#include "Tool_Define.h"
#include "BaseClass/Level.h"

BEGIN(Tool)

class CLevel_MapTool final : public Engine::CLevel
{
	DERIVED_CLASS(CLevel, CLevel_MapTool)

protected:
	explicit CLevel_MapTool() = default;
	explicit CLevel_MapTool(const CLevel_MapTool& rhs) = default;
	virtual ~CLevel_MapTool() = default;

public:
	virtual HRESULT		Initialize();
	virtual void		Tick(const _float& fTimeDelta);
	virtual HRESULT		Render();

public:
	static CLevel_MapTool* Create();

protected:
	virtual void		Free();
};

END