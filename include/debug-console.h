//
// Created by MatexPL on 2022-08-30.
//

#ifndef SOLARENGINE_DEBUG_CONSOLE_H
#define SOLARENGINE_DEBUG_CONSOLE_H
#include <windows.h>
#include <string>

namespace Console {
    extern HANDLE hCslOut;
    extern HANDLE hCslIn;
    extern bool visible;
    extern bool isInputting;

    void Attach(HWND target);
    void Log(std::string msg);
    void SendInput(char c);
    void StartInput();
    void SetVisible(bool vis);
    void ToggleVis();
    void Exists();
}

#endif //SOLARENGINE_DEBUG_CONSOLE_H
