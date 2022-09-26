#include "Window.h"    //my window class
#include "Renderer.h"
#include <Windows.h> //win32 api
#include <wchar.h>

//static counter for how many windows are open
size_t Window::numOfWindows = 0;

LRESULT CALLBACK procedure(HWND handle, 
                           UINT message, 
                           WPARAM wParam, 
                           LPARAM lParam)
{   
    //ptr to the current window
    Window* cw = (Window*)GetWindowLongPtr(handle, GWLP_USERDATA);

    //ref to the static renderer
    Renderer& r = Renderer::get();
    
    switch(message)
    {
    case WM_DESTROY:
    {
        cw->onDestroy();
        break;
    }
    case WM_PAINT:
    {
        const int height = cw->getScreenHeight();
        const int width = cw->getScreenWidth();
        PAINTSTRUCT ps;

        HDC dc = BeginPaint(handle, &ps); 
        /////////////////////////////////////////////////// 

        //get background rectanlge
        RECT bkground;
        GetClientRect(handle, &bkground);

        HDC bitmapDC = CreateCompatibleDC(dc);
        HBITMAP bitmap = CreateCompatibleBitmap(dc, bkground.right - bkground.left,
                                                bkground.bottom - bkground.top);
        SelectObject(bitmapDC, cw->getPen());
        SelectObject(bitmapDC, cw->getBrush());
        SelectObject(bitmapDC, bitmap);

        //1 update cycle of rendering the triangles in the mesh
        r.update(width, height, bitmapDC);

        //calculate the running average fps
        r.runningDT = r.runningDT * .99f + r.deltaTime * 0.00001f;
        wchar_t buffer[64];
        int bytes = swprintf(buffer, 64, L"%f", 1.0f/r.runningDT);      
        DrawTextW(bitmapDC, buffer, bytes, &bkground, NULL);
        
        //copy the bitmap buffer to the screen
        BitBlt(dc, bkground.left,
               bkground.top, 
               bkground.right - bkground.left, 
               bkground.bottom - bkground.top,
               bitmapDC, 0, 0, SRCCOPY);

        DeleteObject(bitmap);
        DeleteDC(bitmapDC);

        //////////////////////////////////////////////////
        EndPaint(handle, &ps);

        return 0;
    }
    case WM_ERASEBKGND:
    {
        return 1;//tell windows the erasing was handled
    }
    default: 
        DefWindowProc(handle, message, wParam, lParam);
    }
    return 1;
}

//inits a new win32 window
Window::Window(u32 width, u32 height)
{
    const wchar_t* className = L"window class";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = &procedure;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = className;
    wc.hbrBackground = NULL;
    RegisterClassW(&wc);

    //make a new window and get the handle
    m_handle = CreateWindowExW
    (
        NULL,
        className,                              
        L"3Dgfx",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    if(m_handle)
    {
        wndIsOpen = true;
        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)this);
        myBitmap = (u32*)VirtualAlloc(nullptr, width*height*sizeof(u32), 
                                       MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);       
        this->width = width;
        this->height = height;
        pen = GetStockObject(DC_PEN);
        brush = (HBRUSH)GetStockObject(DC_BRUSH);
        bkgroundBrush = CreateSolidBrush(RGB(0,0,0));
        ++numOfWindows;
        ShowWindow(m_handle, SW_SHOW);
        UpdateWindow(m_handle);
    }
    else throw "the window handle was NULL\n";
}

HBRUSH Window::getBkGroundBrush() const
{
    return bkgroundBrush;
}

//called in WM_DESTROY
void Window::onDestroy()
{
    wndIsOpen = false;
    VirtualFree(myBitmap, NULL, MEM_RELEASE);
    PostQuitMessage(0);
    DestroyWindow(m_handle);
    --numOfWindows;
}

void Window::processEvents()
{
    MSG message;
    while(PeekMessage(&message, m_handle, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    } 
}

bool Window::isOpen() const 
{
    return wndIsOpen;
}

HGDIOBJ Window::getPen() const
{
    return pen;
}

HWND Window::getHandle() const 
{
    return m_handle;
}

HBRUSH Window::getBrush() const
{
    return brush;
}

u32* Window::getBitmapPtr() const
{
    return myBitmap;
}

int Window::getScreenHeight() const
{
    return height;
}

int Window::getScreenWidth() const
{
    return width;
}
