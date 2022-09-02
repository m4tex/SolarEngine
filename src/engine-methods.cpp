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

        for (int i = p1.x; i < p2.x; ++i) {
            pBuffer[i+y*width] = color;
            if(D > 0) {
                y += 1;
                D -= 2*dy;
            }
            D+=2*dy;

//            y += (D > 0);
//            D += 2*(D > 0)*dy;
        }
    }

    char ToLower(char c){
        return c + 32 * (c > 64 && c < 91);
    }
}