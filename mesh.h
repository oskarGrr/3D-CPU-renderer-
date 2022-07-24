#pragma once
#include <vector>
#include "vec4.h"

struct triangle
{
    vec4 verticies[3];
    float greyScale = 0.0f;
};

class mesh
{
public:
    std::vector<triangle> tris;
    void unitCube();
    bool selectMeshFromObj(const char* fileName);
};