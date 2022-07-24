#pragma once
#include <Windows.h> //win32 api
#include <cstdint>  //size_t uint32_t

typedef uint32_t u32;
typedef uint64_t u64;

//simple windowing system
class Window
{
private:
    int width = 0, height = 0;   //initial size of window
    HWND m_handle = NULL;        //the handle to this window
    bool wndIsOpen = false;       //is this window running
    static size_t numOfWindows;  //how many open windows are there
    HGDIOBJ pen = NULL;
    HBRUSH brush = NULL;
    HBRUSH bkgroundBrush = NULL;
    u32* myBitmap; //the array of pixels to be coppied to the screen

public:
    //use ctor below
    Window()=delete;

    //init a new window
    Window(uint32_t screenW, uint32_t screenH);

    //no copies or moves should happen
    Window(const Window&)=delete;
    Window(Window&&)=delete;
    Window& operator=(const Window&)=delete;
   
    u32* getBitmapPtr() const;
    HWND getHandle() const;
    int getScreenWidth() const;
    int getScreenHeight() const;
    HGDIOBJ getPen() const;
    HBRUSH getBrush() const;
    HBRUSH getBkGroundBrush() const;

    //used to tell if the window is open
    bool isOpen() const;  

    //sends windows messages to the window procedure 
    void processEvents(); 

    //called durring WM_DESTROY 
    void onDestroy();
};