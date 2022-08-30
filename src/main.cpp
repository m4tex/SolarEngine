#include <windows.h>
#include <string>
#include <chrono>
#include "../include/engine-methods.h"
#include "../include/debug-console.h"

#define REND_WIDTH 512
#define REND_HEIGHT 512

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ConsoleProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int nCmdShow) {
    //#region Creating and showing the engine window
    const std::string WINDOW_NAME = "Solar Engine";
    const char CLASS_NAME[] = "SolarEngineWindow";

    WNDCLASS wc = {};
    wc.hbrBackground = GetSysColorBrush(BLACK_BRUSH);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0,
                                CLASS_NAME,
                                WINDOW_NAME.c_str(),
                                WS_OVERLAPPEDWINDOW | WS_THICKFRAME,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                REND_WIDTH, REND_HEIGHT,
                                nullptr,
                                nullptr,
                                hInst,
                                nullptr);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    //#endregion

    WNDCLASS cslWnd = {};
    cslWnd.lpszClassName = "ConsoleWindowClass";
    cslWnd.lpfnWndProc = ConsoleProc;

    RegisterClass(&cslWnd);

    HWND cslHwnd = CreateWindow("ConsoleWindowClass", "HM", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 50, 50, 100, 100, NULL, NULL, NULL, NULL);

    ShowWindow(cslHwnd, SW_SHOWDEFAULT);

    Console::Attach();
    Console::Log("Hello, World!!!");

    MSG msg;
    PAINTSTRUCT ps;
    int FPS;
    auto t_fps_old = std::chrono::high_resolution_clock::now();

    COLORREF *frameBuffer = new COLORREF[REND_WIDTH*REND_HEIGHT] { 0 };

    //Engine loop
    while (true) {
        //#region Message Handling
        if (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }

        if (msg.message == WM_QUIT)
            break;
        //#endregion

        //Frame painting

        EngineMethods::DrawLine({0, 0},
                                {450, 450},
                                RGB(255,255,255),
                                frameBuffer,
                                REND_WIDTH,
                                REND_HEIGHT);

        //#region Draw a bitmap from frameBuffer
        HBITMAP bitmap = CreateBitmap(REND_WIDTH,
                                      REND_HEIGHT,
                                      1,
                                      8*4,
                                      frameBuffer);

        HDC deviceCtx = BeginPaint(hwnd, &ps);
        HDC srcHdc = CreateCompatibleDC(deviceCtx);

        SelectObject(srcHdc, bitmap);

        BitBlt(deviceCtx,
               0, 0,
               500, 500,
               srcHdc,
               0, 0,
               SRCCOPY);

        EndPaint(hwnd, &ps);

        DeleteObject(bitmap);
        DeleteDC(srcHdc);
        //#endregion

        //#region FPS Display
        auto t_fps_new = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> dt = t_fps_new - t_fps_old;
        FPS = 1/dt.count();
        SetWindowTextA(hwnd, (WINDOW_NAME + " FPS: " + std::to_string(FPS)).c_str());
        t_fps_old = t_fps_new;
        //#endregion
    }
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

            CreateWindow("BUTTON", "HM", WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 10, 150, 100,
                         hwnd, NULL, hInst, NULL);

            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK ConsoleProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}