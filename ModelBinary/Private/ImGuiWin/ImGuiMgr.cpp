#include "ImGuiWin/ImGuiMgr.h"
#include "ImGuiWin/ImGuiWin.h"

IMPLEMENT_SINGLETON(CImGuiMgr)

CImGuiMgr::CImGuiMgr()
{
}

HRESULT CImGuiMgr::Initialize(const FInitImGuiMgr tInit)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	m_pGuiContext = ImGui::CreateContext();
	m_pIO = &ImGui::GetIO();
	m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	m_pIO->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	m_pIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	m_pIO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	//io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// 한글폰트추가
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("wanju.ttf", 15.f, NULL, io.Fonts->GetGlyphRangesKorean());

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (m_pIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	io.DisplaySize = ImVec2(1920.f, 1080.f);

	// Setup Platform/Renderer backends
	if (!ImGui_ImplWin32_Init(tInit.hWnd))
		return E_FAIL;

	if (!ImGui_ImplDX11_Init(tInit.pDevice.Get(), tInit.pContext.Get()))
		return E_FAIL;

    return S_OK;
}

void CImGuiMgr::Tick(const _float fTimeDelta)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (_uint i = 0; i < m_vecRootWins.size(); i++)
	{
		m_vecRootWins[i]->Tick(fTimeDelta);
	}
}

HRESULT CImGuiMgr::Render()
{
	for (_uint i = 0; i < m_vecRootWins.size(); i++)
	{
		m_vecRootWins[i]->Render();
	}

	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	
	// Update and Render additional Platform Windows
	if (m_pIO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return S_OK;
}

CImGuiMgr* CImGuiMgr::Create(const FInitImGuiMgr tInit)
{
	ThisClass* pInstance = new ThisClass();

	if (FAILED(pInstance->Initialize(tInit)))
	{
		MSG_BOX("ImGuiMgr Create Failed");
		Safe_Release(pInstance);

		return nullptr;
	}

	return pInstance;
}

void CImGuiMgr::Free()
{
	for (auto& Pair : m_mapImGuiWin)
		Safe_Release(Pair.second);
	m_mapImGuiWin.clear();
	m_vecRootWins.clear();

	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT CImGuiMgr::Add_ImGuiWinAsRoot(const wstring& strKey, CImGuiWin* pImGuiWin)
{
	if (!pImGuiWin)
		return E_FAIL;

	auto iter = m_mapImGuiWin.find(strKey);
	if (iter != m_mapImGuiWin.end())
		return E_FAIL;

	m_mapImGuiWin.emplace(strKey, pImGuiWin);
	
	Bind_RootWin(strKey);

	return S_OK;
}

HRESULT CImGuiMgr::Add_ImGuiWinAsChild(const wstring& strParentName, const wstring& strName, CImGuiWin* pImGuiWin)
{
	if (!pImGuiWin)
		return E_FAIL;

	// 부모가 없을 시
	auto iterParent = m_mapImGuiWin.find(strParentName);
	if (iterParent == m_mapImGuiWin.end())
	{
		Safe_Release(pImGuiWin);
		return E_FAIL;
	}

	// 중복시
	auto iterChild = m_mapImGuiWin.find(strName);
	if (iterChild != m_mapImGuiWin.end())
	{
		Safe_Release(pImGuiWin);
		return E_FAIL;
	}

	m_mapImGuiWin.emplace(strName, pImGuiWin);
	(*iterParent).second->Add_Child(pImGuiWin);
	
	return S_OK;
}

HRESULT CImGuiMgr::AttachToChild_ImGuiWin(const wstring& strParentName, const wstring& strChildName)
{
	auto iterParent = m_mapImGuiWin.find(strParentName);
	if (iterParent == m_mapImGuiWin.end())
		return E_FAIL;

	auto iterChild = m_mapImGuiWin.find(strChildName);
	if (iterChild == m_mapImGuiWin.end())
		return E_FAIL;

	(*iterParent).second->Add_Child((*iterChild).second);

	return S_OK;
}

HRESULT CImGuiMgr::Bind_RootWin(const wstring& strKey)
{
	// 있는 윈도우인지 확인한다.
	auto iterMap = m_mapImGuiWin.find(strKey);
	if (iterMap == m_mapImGuiWin.end())
		return E_FAIL;

	CImGuiWin* pFoundWin = iterMap->second;

	// 루트 윈도우에 있는지 확인하고 없으면 추가하지 않는다.
	auto iter = find_if(m_vecRootWins.begin(), m_vecRootWins.end(), 
		[&pFoundWin](CImGuiWin* pImGuiWin) {
			return (pImGuiWin == pFoundWin);
		});
	if (iter != m_vecRootWins.end())
		return E_FAIL;

	m_vecRootWins.push_back(pFoundWin);

	return S_OK;
}
