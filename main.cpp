#include "Renderer.h"
#include <chrono>
#include <cstdint>
#include "mesh.h"
#include "mat4.h"
#include "Window.h"
#include <Windows.h>
#include <iostream>
#include <fstream>

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow)
{
    const uint32_t screenWidth = 1280, screenHeight = 720;
    Window window(screenWidth, screenHeight);
    auto& r = Renderer::get();

    if(!mesh::tryToLoadMesh("3DModels/donut.obj"))
        return EXIT_FAILURE;

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
        r.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    return EXIT_SUCCESS;
}
