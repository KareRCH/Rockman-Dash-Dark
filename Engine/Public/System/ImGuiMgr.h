#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ImGui 창의 계층을 관리하기 위한 클래스
/// </summary>
class ENGINE_DLL_DBG CImGuiMgr final : public CBase
{
	DERIVED_CLASS(CBase, CImGuiMgr)

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr() = default;

public:
	HRESULT	Initialize();

public:
	static CImGuiMgr* Create();

private:
	virtual void	Free();
};

END