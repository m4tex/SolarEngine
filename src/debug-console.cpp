//
// Created by MatexPL on 2022-08-30.
//

#include "../include/debug-console.h"
#include <string>

namespace Console {
    HANDLE hCslOut;
    HANDLE hCslIn;

    void Attach(HWND target) {
        AllocConsole();
        hCslOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hCslIn = GetStdHandle(STD_INPUT_HANDLE);

        HWND cslHwnd = GetConsoleWindow();
        SetParent(cslHwnd, target);
        SetWindowLongPtr(cslHwnd, GWL_STYLE, WS_VISIBLE);

        SetWindowPos(cslHwnd,0,0,0,0,0,
            SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_DRAWFRAME);

        MoveWindow(cslHwnd, 0, 0, 150, 512, false);
    }

    void SetVisible(bool visible) {
        HWND cslHwnd = GetConsoleWindow();

        SetWindowLongPtr(cslHwnd, GWL_STYLE, visible ? WS_VISIBLE : 0);

        SetWindowPos(cslHwnd,0,0,0,0,0,
                     SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE|SWP_DRAWFRAME);
    }

    void Log(std::string msg) {
        msg += '\n';
        WriteConsoleA(hCslOut, msg.c_str(), msg.length(), NULL, NULL);
    }
}