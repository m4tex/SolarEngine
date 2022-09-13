//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#include "../include/engine-methods.h"

namespace EngineMethods {
    void DrawLine(Vec2 p1, Vec2 p2, COLORREF color, COLORREF *pBuffer, int width) {
        float dx = p2.x - p1.x;
        float dy = p2.y - p1.y;
        float D = 2*dy-dx;
        float y = p1.y;

        for (int i = p1.x; i < p2.x; ++i) {
            pBuffer[i+y*width] = color;
            if(D > 0) {
                y += 1;
                D -= 2*dx;
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