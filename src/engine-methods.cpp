//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#include "../include/engine-methods.h";

namespace EngineMethods {
    void DrawLine(float p1[2], float p2[2], COLORREF color, COLORREF *lpBitBuffer) {
        float dx, dy, k;
        dx = p2[0] - p1[0];
        dy = p2[1] - p1[1];

        k = dy/dx;

        //I am so sure this won't work lol
        for(int i = 0; i < (int)p2[0]; i++) {
            lpBitBuffer[(int)(p1[0]+i+k*i*512)] = color;
        }
    }
}