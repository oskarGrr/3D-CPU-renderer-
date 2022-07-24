#include "Renderer.h"
#include <chrono>
#include <cstdint>
#include "mesh.h"
#include "mat4.h"
#include "Window.h"
#include <Windows.h>
#include <iostream>

int WINAPI wWinMain(HINSTANCE hi, HINSTANCE hPrevInstance, PWSTR cmdLine, int nCmdShow)
{
    const uint32_t screenWidth = 1280, screenHeight = 720;
    Window window(screenWidth, screenHeight);
    auto& r = Renderer::get();
    mesh m;
    m.selectMeshFromObj("theUtahTeapot.obj");
    r.selectMesh(m);
    r.setProjection
    (
        mat4::createProjectionMatrix
        (
            90.0f,
            1000.0f,
            0.1f, 
            static_cast<float>(screenHeight) / screenWidth
        )
    );

    while(window.isOpen())
    {    
        const auto start = std::chrono::steady_clock::now();
        InvalidateRect(window.getHandle(), NULL, RDW_INVALIDATE | RDW_ERASE);
        window.processEvents();
        const auto end = std::chrono::steady_clock::now();
        const auto delta = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        r.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    return 0;
}