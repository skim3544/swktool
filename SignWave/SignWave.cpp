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


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

LPCTSTR AppName = TEXT("SIGNWAVE");

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    // Create default Win registration information
    WNDCLASSEXW wcex = swktool::WindowReg::CreateWinClsTemplate(hInstance);

    // customize some fields
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SIGNWAVE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SIGNWAVE));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SIGNWAVE, szWindowClass, MAX_LOADSTRING);


    //Create Windows Registration object
    swktool::WindowReg WinReg1(wcex);   
    WinReg1.SetWinClassName(AppName);

    // Create App Frame, Register
    AppSignWaveFrame App(hInstance);
    App.Register(WinReg1);

    App.CreateWin(AppName, TEXT("Sign Wave Using Polyline"), nCmdShow);

    int Result = (int)App.DoMessageLoop();;

    return Result;
}



