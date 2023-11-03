#pragma once

#include "Base.h"

BEGIN(Engine)

/// <summary>
/// ImGui������ ���ʰ� �Ǵ� Ŭ����
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