//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#include "../include/engine-methods.h"

namespace EngineMethods {
    void DrawLine(Vec2 p1, Vec2 p2, COLORREF color, COLORREF *pBuffer, int width) {
        int dx = (int)(p2.x - p1.x);
        int dy = (int)(p2.y - p1.y);
        int D = 2*dy-dx;
        int y = (int)p1.y;

        for (int i = (int)p1.x; i < (int)p2.x; ++i) {
            pBuffer[i+y*width] = color;

            y += (D > 0);
            D -= (D > 0)*2*dx;
            D+=2*dy;
        }
    }

    char ToLower(char c){
        return c + 32 * (c > 64 && c < 91);
    }
}