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
/// ImGui â�� ������ �����ϱ� ���� Ŭ����
/// ImGui�� ���� ������Ʈ Tick�� �ٸ��� ���ư���.
/// �̴� 
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
	// ������ ImGuiWin�� �߰��Ѵ�.
	HRESULT Add_ImGuiWinAsRoot(const wstring& strname, CImGuiWin* pImGuiWin);
	HRESULT Add_ImGuiWinAsChild(const wstring& strParentName, const wstring& strName, CImGuiWin* pImGuiWin);
	HRESULT AttachToChild_ImGuiWin(const wstring& strParentName, const wstring& strChildName);
	// ���� ������ ����Ǵ� �����츦 ���ε� �Ѵ�.
	HRESULT Bind_RootWin(const wstring& strKey);

public:
	GETSET_1(ImGuiIO*, m_pIO, IO, GET__C)
		GETSET_1(ImGuiContext*, m_pGuiContext, GuiContext, GET__C)

private:
	ImGuiIO* m_pIO = { nullptr };
	ImGuiContext* m_pGuiContext = { nullptr };

private:
	map<wstring, CImGuiWin*>		m_mapImGuiWin;		// �̸� �˻� �ý��ۿ�, ���� ���� �� �̸� �˻�����.

private:
	vector<CImGuiWin*>				m_vecRootWins;		// ������ ����Ǵ� UI��, �׷��� �ʴٸ� Win�� �ڽ� ��ü�� �����Ѵ�.

};

END
