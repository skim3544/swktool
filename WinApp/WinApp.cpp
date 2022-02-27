// WinApp.cpp : Defines the entry point for the application.
//

#include "..\SWKBase\pch.h"
#include "..\SWKUI\pch.h"
#include "framework.h"
#include "WinApp.h"
#include "..\SWKUI\WinEventHandler.h"
#include "..\SWKUI\WinFrame.h"
#include <string>
#include "AppWinFrame.h"

#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


#define MAX_LOADSTRING 100
using namespace swktool;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR szWindowClass2[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

LPCTSTR AppName = TEXT("WINAPP");

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // _crtBreakAlloc = 217;

    // Create default Win registration information
    WNDCLASSEXW wcex = swktool::WindowReg::CreateWinClsTemplate(hInstance);
    
    // customize some fields
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPP);
    wcex.lpszClassName = szWindowClass;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPP));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPP, szWindowClass, MAX_LOADSTRING);    

//    std::wstring sInst = std::to_wstring((int)hInstance);
//    OutputDebugString(sInst.c_str());

    //Create Windows Registration object
    swktool::WindowReg WinReg1(wcex);

    //swktool::WindowReg WinReg2(wcex);
    WinReg1.SetWinClassName(AppName);


    AppWindowFrame App(hInstance);       
    App.Register(WinReg1);

    App.CreateWin(AppName, TEXT("Window 1"), nCmdShow);
    
    int Result = (int)App.DoMessageLoop();;

    
    return Result;
}
