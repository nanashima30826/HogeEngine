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
// �O���[�o���ϐ�

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "HogeEngine";
WNDCLASSEX wcex = {};
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pImmediateContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// �E�B���h�E���b�Z�[�W�����֐�
HWND g_Window = nullptr;  // �O���[�o���ϐ��ŃE�B���h�E�n���h����ێ�

HWND GetWindow() {
    return g_Window;  // �O���[�o���ϐ��̃E�B���h�E�n���h����Ԃ�
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



// ���C���G���g���[�|�C���g
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

    // �E�B���h�E�T�C�Y�ƈʒu�̐ݒ�
    RECT rc = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    // �E�B���h�E����x�����쐬
    g_Window = CreateWindowEx(0, CLASS_NAME, WINDOW_NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

    if (!g_Window) return -1; // g_Window�������Ȃ�I��

    // COM���C�u�����̏�����
    CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

  
    
    
    // �}�l�[�W���̏�����
    Manager::Init();

    // �E�B���h�E��\��
    ShowWindow(g_Window, nCmdShow);
    UpdateWindow(g_Window);
   
  

    DWORD dwExecLastTime;
    DWORD dwCurrentTime;
    timeBeginPeriod(1);
    dwExecLastTime = timeGetTime();
    dwCurrentTime = 0;

    // ���C�����[�v
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

            // 60FPS�ōX�V
            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {
                dwExecLastTime = dwCurrentTime;

                // �Q�[���̍X�V�ƕ`��

                Manager::Update();
                Manager::Draw();
                //Render();
            }
        }
    }

    // �N���[���A�b�v
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