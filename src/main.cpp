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

    RegisterClass(&wc);

    HWND hwnd = CreateWindow(   CLASS_NAME,
                                WINDOW_NAME.c_str(),
                                WS_OVERLAPPEDWINDOW,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                REND_WIDTH, REND_HEIGHT,
                                nullptr,
                                nullptr,
                                hInst,
                                nullptr);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    //#endregion

    Console::Attach(hwnd);

    std::string test = "Hello, World.";

    WriteConsoleA(Console::hCslOut, test.c_str(), test.length(), NULL, NULL);

    MSG msg;
    PAINTSTRUCT ps;
    int FPS;
    auto t_fps_old = std::chrono::high_resolution_clock::now();

    auto *frameBuffer = new COLORREF[REND_WIDTH*REND_HEIGHT] { 0 };

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
        //hook and listen for it globally? would be nice for debugging....
        case WM_CHAR:
            switch(EngineMethods::ToLower((char)wParam)) {
                case 'c':
                    Console::ToggleVis();
                    break;

                case 't':
                    PostQuitMessage(0);
                    break;
            }
            return 0;


        case WM_PAINT:
//            Console::Log("Main Proc");
            return 0;


        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}