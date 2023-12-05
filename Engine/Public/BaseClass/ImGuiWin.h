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
	virtual HRESULT	Initialize() = 0;
	virtual void	Tick(const _float& fTimeDelta) = 0;
	virtual HRESULT	Render() = 0;

private:
	virtual void	Free();
};

END