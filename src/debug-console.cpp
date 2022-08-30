//
// Created by MatexPL on 2022-08-30.
//

#include "../include/debug-console.h"
#include <string>

namespace Console {
    HANDLE hCslOut;
    HANDLE hCslIn;

    void Attach() {
        AllocConsole();
        hCslOut = GetStdHandle(STD_OUTPUT_HANDLE);
        hCslIn = GetStdHandle(STD_INPUT_HANDLE);
    }

    void Log(std::string msg) {
        msg += '\n';
        WriteConsoleA(hCslOut, msg.c_str(), msg.length(), nullptr, nullptr);
    }
}