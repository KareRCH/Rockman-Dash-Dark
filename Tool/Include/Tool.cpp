// Tool.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "Tool.h"

#include "Tool_Define.h"
#include "MainApp.h"
#include <dwmapi.h>

#define MAX_LOADSTRING 100

// 콘솔창 키고 싶으면 이거 1로 설정
#define _DEBUG_CONSOLE 0

// 전역 변수:
HWND	    g_hWnd;                             // 윈도우 창 변수입니다.
HINSTANCE   g_hInst;                            // 현재 인스턴스입니다.
_bool       g_bLockEsc;                         // 비정상적인 종료를 막기위한 락입니다.
_bool       g_bFullScreen;                      // 전체화면 설정
WCHAR       szTitle[MAX_LOADSTRING];            // 제목 표시줄 텍스트입니다.
WCHAR       szWindowClass[MAX_LOADSTRING];      // 기본 창 클래스 이름입니다.

_bool		g_bResizeOnScreen = 0;
_uint		g_iResizeWidth = g_iWindowSizeX;
_uint		g_iResizeHeight = g_iWindowSizeY;

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#ifdef _DEBUG
#if _TEST_CONSOLE
    // 디버그용 콘솔창
    if (::AllocConsole() == TRUE)
    {
        FILE* nfp[3];
        freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
        freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
        freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
        std::ios::sync_with_stdio();
    }
#endif
#endif // _DEBUG


    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDC_TOOL, szWindowClass, MAX_LOADSTRING);
    lstrcpy(szTitle, L"Rockman Dash Tool");
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TOOL));

    MSG msg;
    msg.message = WM_NULL;

    CMainApp* pMainApp = CMainApp::Create();

    if (nullptr == pMainApp)
        return FALSE;

    // 기본 메시지 루프입니다:
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message && !g_bLockEsc)
                break;

            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (GI() && g_iResizeWidth != 0 && g_iResizeHeight != 0)
            {
                GI()->Resize_SwapChain(g_iResizeWidth, g_iResizeHeight);
                GI()->Regist_RenderTarget(0);
                GI()->Regist_RenderTarget(1);
                GI()->Bind_RenderTargetsOnDevice();
                GI()->Set_SystemViewport(1, {0.f, 0.f, (_float)g_iResizeWidth , (_float)g_iResizeHeight, 0.f, 1.f });
            }

            ::GameInstance()->Tick_Timer(L"Timer_Immediate");

            _float	fTimeDelta_Immediate = GameInstance()->Get_TimerDelta(L"Timer_Immediate");

            // 프레임이 넘어갈 때 틱 함수 작동
            if (::GameInstance()->IsPermit_Call(L"Frame", fTimeDelta_Immediate))
            {
                ::GameInstance()->Tick_Timer(L"Timer_FPS");
                _float	fTimeDelta = ::GameInstance()->Get_TimerDelta(L"Timer_FPS");

                // 틱 함수
                pMainApp->Priority_Tick(fTimeDelta);
                pMainApp->Tick(fTimeDelta);
                pMainApp->Late_Tick(fTimeDelta);
                pMainApp->Render();
            }
        }
    }

#ifdef _DEBUG
#if _DEBUG_CONSOLE
    // 콘솔 사용 해제
    FreeConsole();
#endif
#endif // _DEBUG

    if (0 != Safe_Release(pMainApp))
    {
        MSG_BOX("메인 앱 누수");
    }

    if (0 != Release_GameInstance())
    {
        MSG_BOX("게임 인스턴스 누수");
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_CLASSDC;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(28, 28, 28)));
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = NULL;

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    RECT	rc{ 0, 0, g_iWindowSizeX, g_iWindowSizeY };

    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0,
        rc.right - rc.left,
        rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    /*DEVMODE devmode = {};
     EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
     HWND hWnd = CreateWindowEx(WS_EX_APPWINDOW, szWindowClass, szTitle, WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
         0, 0, devmode.dmPelsWidth, devmode.dmPelsHeight, nullptr, nullptr, hInstance, nullptr);*/

         // 테두리 없애기
         //SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX);

    if (!hWnd)
    {
        return FALSE;
    }

    g_hWnd = hWnd;

    ShowWindow(hWnd, nCmdShow);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

#ifndef WM_DPICHANGED
#define WM_DPICHANGED 0x02E0 // From Windows SDK 8.1+ headers
#endif

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    switch (message)
    {
    case WM_CREATE:
    {
        // 눈뽕 방지
        RECT rect;
        GetClientRect(hWnd, &rect);
        HDC hdc = GetDC(hWnd);
        HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
        ReleaseDC(hWnd, hdc);

        g_bLockEsc = false;

        // 검은색 타이틀바
        COLORREF DARK_COLOR = 0x00505050;
        BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(
            hWnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
            &DARK_COLOR, sizeof(DARK_COLOR)));

        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;

        g_iResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_iResizeHeight = (UINT)HIWORD(lParam);
        break;
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 메뉴 선택을 구문 분석합니다.
        switch (wmId)
        {
        case IDM_EXIT:
            if (!g_bLockEsc)
                DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            //PostQuitMessage(0);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
