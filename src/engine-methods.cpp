//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#include "../include/engine-methods.h"
#include "../include/debug-console.h"

namespace EngineMethods {
    bool logged = false;

    void DrawLine(Vec2 p1, Vec2 p2, COLORREF color, COLORREF *pBuffer, int width, int height) {
        float k;

        k = (p2.y - p1.y) / (p2.x - p1.x);

        //I am so sure this won't work lol
        for (int i = 0; i < (int) p2.x - p1.x; i++) {
            pBuffer[(int) (p1.x + i + (k * i * height))] = ((int) (p1.x + i) < width) * color;

            //Logs 10 first steps of the algoritm, delta x and the actual delta x the algorithm calculated
            if(i < 10 && !logged) {
                Console::Log(std::to_string(i) + " " + std::to_string((int) (p1.x + i + k * i * height) - i*height));
            } else {
                logged = true;
            }
        }
    }

    char ToLower(char c){
        return c + 32 * (c > 64 && c < 91);
    }
}