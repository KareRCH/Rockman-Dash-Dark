#pragma once



#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"





#include "Base.h"
#include "Tool_Define.h"





struct ImGuiIO;

BEGIN(Tool)

class CImGuiWin;

struct FInitImGuiMgr
{
	HWND hWnd;
	ComPtr<ID3D11Device> pDevice;
	ComPtr<ID3D11DeviceContext> pContext;
};

/// <summary>
/// ImGui 창의 계층을 관리하기 위한 클래스
/// ImGui는 게임 오브젝트 Tick과 다르게 돌아간다.
/// 이는 
/// </summary>
class CImGuiMgr final : public Engine::CBase
{
	DERIVED_CLASS_SINGLETON(CBase, CImGuiMgr)

private:
	explicit CImGuiMgr();
	virtual ~CImGuiMgr() = default;

public:
	HRESULT	Initialize(const FInitImGuiMgr tInit);
	void Tick(const _float fTimeDelta);
	HRESULT Render();

public:
	static CImGuiMgr* Create(const FInitImGuiMgr tInit);

private:
	virtual void	Free();


public:
	// 생성한 ImGuiWin을 추가한다.
	HRESULT Add_ImGuiWinAsRoot(const wstring& strname, CImGuiWin* pImGuiWin);
	HRESULT Add_ImGuiWinAsChild(const wstring& strParentName, const wstring& strName, CImGuiWin* pImGuiWin);
	HRESULT AttachToChild_ImGuiWin(const wstring& strParentName, const wstring& strChildName);
	// 가장 상위로 노출되는 윈도우를 바인딩 한다.
	HRESULT Bind_RootWin(const wstring& strKey);

public:
	GETSET_1(ImGuiIO*, m_pIO, IO, GET__C)
		GETSET_1(ImGuiContext*, m_pGuiContext, GuiContext, GET__C)

private:
	ImGuiIO* m_pIO = { nullptr };
	ImGuiContext* m_pGuiContext = { nullptr };

private:
	map<wstring, CImGuiWin*>		m_mapImGuiWin;		// 이름 검색 시스템용, 저장 목적 및 이름 검색용임.

private:
	vector<CImGuiWin*>				m_vecRootWins;		// 상위에 노출되는 UI들, 그렇지 않다면 Win의 자식 객체로 존재한다.

};

END
