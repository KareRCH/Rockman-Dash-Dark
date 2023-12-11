#pragma once

#include "Base.h"
#include "ImGuiLib_Define.h"

BEGIN(ImGuiLib)

/// <summary>
/// ImGui계층의 기초가 되는 클래스
/// </summary>
class CImGuiWin abstract : public CBase
{
	DERIVED_CLASS(CBase, CImGuiWin)

protected:
	explicit CImGuiWin() = default;
	explicit CImGuiWin(const CImGuiWin& rhs) = delete;
	virtual ~CImGuiWin() = default;

public:
	virtual HRESULT	Initialize() = 0;
	virtual void	Tick(const _float& fTimeDelta);
	virtual HRESULT	Render();

protected:
	virtual void	Free();

public:
	// 자식 윈도우 추가, 새로운 역할을 줄 때 사용된다.
	HRESULT Add_Child(CImGuiWin* pWin);
	// 들어온 Win이 중복되는지 확인하는 것.
	HRESULT Exists_OnChild(CImGuiWin* const pWin);

private:
	CImGuiWin*			m_pParentWin = { nullptr };
	vector<CImGuiWin*>	m_vecChildrenWin;

public:
	void Open_Child(_uint iIndex);
	void Open_JustOneChild(_uint iIndex);
	void Close_Child(_uint iIndex);

public:
	GETSET_1(_bool, m_bOpen, Open, SET)
	GETSET_1(_bool, m_bFirstLoop, FirstLoop, SET)

protected:	// 각종 속성
	_bool					m_bOpen;
	_bool					m_bFirstLoop;
};

END