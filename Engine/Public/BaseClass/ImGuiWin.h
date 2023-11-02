#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ImGui계층의 기초가 되는 클래스
/// </summary>
class ENGINE_DLL_DBG CImGuiWin abstract : public CBase
{
	DERIVED_CLASS(CBase, CImGuiWin)

private:
	explicit CImGuiWin();
	explicit CImGuiWin(const CImGuiWin& rhs) = delete;
	virtual ~CImGuiWin() = default;

public:
	HRESULT	Initialize();

private:
	virtual void	Free();
};

END