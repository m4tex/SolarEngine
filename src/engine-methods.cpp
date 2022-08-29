//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#include "../include/engine-methods.h"

namespace EngineMethods {
    void DrawLine(Vec2 p1, Vec2 p2, COLORREF color, COLORREF *pBuffer, int width, int height) {
        float k;

        k = (p2.y - p1.y) / (p2.x - p1.x);

        //I am so sure this won't work lol
        for (int i = 0; i < (int) p2.x - p1.x; i++) {
            pBuffer[(int) (p1.x + i + k * i * height)] = ((int) (p1.x + i) < width) * color;
        }
    }
}