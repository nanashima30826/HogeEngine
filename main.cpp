///////////////////////////////////
//2025/02/17 
//nanashima main.cpp
//////////////////////////////////
#include <d3d11.h>
#include <dxgi.h>
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include"CreateCppAndH.h"

#include "main.h"
#include "manager.h"
#include <thread>
#include <filesystem>
// グローバル変数

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "HogeEngine";
WNDCLASSEX wcex = {};
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// ウィンドウメッセージ処理関数
HWND g_Window = nullptr;  // グローバル変数でウィンドウハンドルを保持

HWND GetWindow() {
    return g_Window;  // グローバル変数のウィンドウハンドルを返す
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;
    
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}



// メインエントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   



    WNDCLASSEX wcex = {};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    wcex.hIconSm = nullptr;

    if (!RegisterClassEx(&wcex)) return -1;

    // ウィンドウサイズと位置の設定
    RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // ウィンドウを一度だけ作成
    g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    if (!g_Window) return -1; // g_Windowが無効なら終了

    // COMライブラリの初期化
    CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

  
    
    
    // マネージャの初期化
    Manager::Init();

    // ウィンドウを表示
    ShowWindow(g_Window, nCmdShow);
    UpdateWindow(g_Window);
   
  

    DWORD dwExecLastTime;
    DWORD dwCurrentTime;
    timeBeginPeriod(1);
    dwExecLastTime = timeGetTime();
    dwCurrentTime = 0;

    // メインループ
    MSG msg;
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();

            // 60FPSで更新
            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {
                dwExecLastTime = dwCurrentTime;

                // ゲームの更新と描画

                Manager::Update();
                Manager::Draw();
                //Render();
            }
        }
    }

    // クリーンアップ
    timeEndPeriod(1);
    UnregisterClass(CLASS_NAME, wcex.hInstance);
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    Manager::Uninit();
    CoUninitialize();

   


    return (int)msg.wParam;
}

IDXGISwapChain* GetSwap()
{
    return g_pSwapChain;
}