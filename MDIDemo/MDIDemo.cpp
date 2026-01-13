//// MDIDemo.cpp : Defines the entry point for the application.
////
//
#define _CRTDBG_MAP_ALLOC
#include <Windows.h>
#include <string>
#include <stdlib.h>

#pragma comment(lib, "SWKBase.lib")
#pragma comment(lib, "SWKUI.lib")

#include "framework.h"
#include "MDIDemo.h"
#include "AppMDIWinFrame.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MdiMainWindow win;
    if (!win.Create(L"MDI Demo", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN))
        return 0;

    win.Show(nCmdShow); 
    
    // Run the message loop 
    swktool::AppMsgLoop loop; 
    return loop.Run();    
}
