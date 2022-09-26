#pragma once
#include "Window.h"
#include "vec4.h"
#include "mat4.h"
#include "mesh.h"
#include <Windows.h>
#include <time.h>
#include <vector>
#include <chrono>

class Renderer
{
private:   
    static Renderer renderer;//the single instance in the data segment    
    bool isInit = false;//has the singleton been initialized with init() yet
    mat4 projectionMatrix = {};
    mesh m;
    vec4 camera = {0.0f,0.0f,0.0f};
    
private:
    Renderer();
    ~Renderer();
    static void drawTriangle(const Triangle&, HDC&);
    static void drawWireFrame(const Triangle& tri, HDC& dc);
    static void scaleIntoScreenSpace(Triangle&, const int sw, const int sh);

public:
    u64 deltaTime;//time passed since last frame in milliseconds
    float runningDT;//avg deltatime

    void init(const mat4& proj);
    void release();
    
    //make sure the copy ctor cant be used
    Renderer(const Renderer&)=delete;
    
    //returns a ref to the singleton in the data segment
    static Renderer& get();

    //set the projection matrix to use
    void setProjection(const mat4& mat);

    //called once per loop insinde the window procedure
    void update(const int screenWidth, const int screenHeight, HDC& dc);

    void selectMesh(const mesh&);
};