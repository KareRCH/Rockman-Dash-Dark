#pragma once

#include "Base.h"
#include "ModelBinary_Define.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

BEGIN(ModelBinary)

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
	// ù��°�� ã�� Ÿ���� �����츦 ��´�.
	template<typename WinClass>
	HRESULT Find_Child(WinClass** ppFindChild);

public:
	GETSET_2(CImGuiWin*, m_pParentWin, Parent, GET__C, SET__C)

protected:
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

template<typename WinClass>
inline HRESULT CImGuiWin::Find_Child(WinClass** ppFindChild)
{
	for (_uint i = 0; i < m_vecChildrenWin.size(); i++)
	{
		WinClass* pWin = DynCast<WinClass*>(m_vecChildrenWin[i]);
		if (pWin)
		{
			(*ppFindChild) = pWin;
			return S_OK;
		}
	}

	return E_FAIL;
}
