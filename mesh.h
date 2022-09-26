#pragma once
#include <vector>
#include "vec4.h"

struct Triangle
{
    vec4 verticies[3];
    float greyScale = 0.0f;
};

class mesh
{
public:
    std::vector<Triangle> tris;
    void unitCube();
    void selectMeshFromObj(const char* fileName);
    static bool tryToLoadMesh(char const* fileName);
};