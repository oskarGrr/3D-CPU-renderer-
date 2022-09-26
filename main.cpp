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

    mesh m;
    char const* objFilePath = "3DModels/donut.obj";
    try//try to load the object file specified
    {
        m.selectMeshFromObj(objFilePath);
    }
    catch(std::exception& e)
    {
        std::ofstream ofs("errorLog.txt");
        std::cerr << e.what() << "execption thrown trying to load " 
                  << objFilePath << '\n';
        ofs << e.what() << "execption thrown trying to load " << objFilePath << '\n';
        return 1;
    }
    catch(char const* throwMsg)
    {
        std::ofstream ofs("errorLog.txt");
        std::cerr << throwMsg << "(error logged in errorLog.txt)\n";
        ofs << throwMsg << '\n';
        return 1;
    }
    catch(...)
    {
         char const* msg = "an exeption was thrown\n" 
            " (that isnt derived from std::exception or a char const* message)\n"
            " while trying to load ";

         std::cerr << msg << objFilePath << '\n';
         std::ofstream ofs("errorLog.txt");
         ofs << msg << objFilePath << '\n';
         return 1;
    }

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
        r.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }

    return 0;
}
