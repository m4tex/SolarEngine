//
// Created by wiktoralan.wisniowi on 8/21/2022.
//

#ifndef SOLARENGINE_ENGINE_METHODS_H
#define SOLARENGINE_ENGINE_METHODS_H
#include <windows.h>
#include "engine-structures.h"

namespace EngineMethods {
    void DrawLine(Vec2 p1, Vec2 p2, COLORREF color, COLORREF *lpBitBuffer, int width, int height);
    char ToLower(char c);
}

#endif //SOLARENGINE_ENGINE_METHODS_H