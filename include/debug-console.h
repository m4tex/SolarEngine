//
// Created by MatexPL on 2022-08-30.
//

#ifndef SOLARENGINE_DEBUG_CONSOLE_H
#define SOLARENGINE_DEBUG_CONSOLE_H
#include <consoleapi.h>
#include <string>

namespace Console {
    extern HANDLE hCslOut;
    extern HANDLE hCslIn;

    void Attach(HWND target);
    void Log(std::string msg);
    void SetVisible(bool visible);
    void Exists();
}

#endif //SOLARENGINE_DEBUG_CONSOLE_H
