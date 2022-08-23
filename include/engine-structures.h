//
// Created by wiktoralan.wisniowi on 8/22/2022.
//

#ifndef SOLARENGINE_ENGINE_STRUCTURES_H
#define SOLARENGINE_ENGINE_STRUCTURES_H

#include <vector>

struct Vec2 {
    float x,y;
};

struct Vec3 {
    float x,y,z;
};

struct Triangle {
    Vec2 verts[3];
    //Color?
};

struct Mesh {
    std::vector<Triangle> tris;
};

struct SolarObject {
    Vec3 pos, rot, vel, angularVel;
    Mesh mesh;
};

#endif //SOLARENGINE_ENGINE_STRUCTURES_H
