#pragma once

#include "Base.h"
#include "ImGuiLib_Define.h"

BEGIN(ImGuiLib)

/// <summary>
/// ImGui������ ���ʰ� �Ǵ� Ŭ����
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
	// �ڽ� ������ �߰�, ���ο� ������ �� �� ���ȴ�.
	HRESULT Add_Child(CImGuiWin* pWin);
	// ���� Win�� �ߺ��Ǵ��� Ȯ���ϴ� ��.
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

protected:	// ���� �Ӽ�
	_bool					m_bOpen;
	_bool					m_bFirstLoop;
};

END