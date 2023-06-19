// SignWave.cpp : 
// Ported from Programming Windows 5th Edition
// By Charles Petzold
//
#include "..\SWKBase\pch.h"
#include "..\SWKUI\pch.h"
#include "..\SWKUI\WinEventHandler.h"
#include "..\SWKUI\WinFrame.h"
#include <string>
#include <crtdbg.h>
#include "SignWave.h"
#include "Resource.h"
#include "framework.h"
#include "AppSignWaveFrame.h"
#include "SignWaveWindow.h"


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    MainWindow win;
    
    if (!win.Create(L"Signwave Windows"))
    {
        return 0;
    }

    ShowWindow(win.WindowHandle(), nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}



