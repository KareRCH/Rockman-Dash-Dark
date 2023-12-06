// Client.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//


#include "framework.h"
#include "Client.h"

#include "Client_Define.h"
#include "MainApp.h"
#include <dwmapi.h>

#define MAX_LOADSTRING 100

// 콘솔창 키고 싶으면 이거 1로 설정
#define _DEBUG_CONSOLE 1

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
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    //_uint creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef _DEBUG
#if _DEBUG_CONSOLE
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
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT));

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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_CROSS); // 윈도우 마우스 커서 변경
    wcex.hbrBackground  = (HBRUSH)(CreateSolidBrush(RGB(28, 28, 28)));
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
            PostQuitMessage(0);
            break;
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



