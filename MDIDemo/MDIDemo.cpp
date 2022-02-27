// MDIDemo.cpp : Defines the entry point for the application.
//
#include <Windows.h>
#include <string>


#include "..\SWKBase\DebugStream.h"
#include "..\SWKBase\Logger.h"
#include "..\SWKUI\MDIChildWindow.h"
#include "..\SWKUI\WinRegistration.h"
#include "..\SWKUI\WinEventHandler.h"
#include "..\SWKUI\WinMenu.h"
#include "framework.h"
#include "MDIDemo.h"
#include "AppWinFrame.h"
#include "resource.h"
#include "AppLog.h"

#define MAX_LOADSTRING 100
using namespace std;
using swktool::DebugOut;

// Global Variables:
//HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

TCHAR szAppName[] = TEXT("MDIDemo");
TCHAR szFrameClass[] = TEXT("MDIFrame");
TCHAR szHelloClass[] = TEXT("MDIHelloChild");
TCHAR szRectClass[] = TEXT("MDIRectChild");

swktool::WinMenu MdiMenuInit;
swktool::WinMenu MdiMenuHello;
swktool::WinMenu MdiMenuRect;

swktool::WinMenu GetHelloMenu() {
    return MdiMenuHello;
}

swktool::WinMenu GetInitMenu() {
    return MdiMenuInit;
}

swktool::WinMenu GetRectMenu() {
    return MdiMenuRect;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    using namespace swktool;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    LevelLogger::init(TEXT("DetailedLog.log"));
    LevelLogger::SetLogLevel(2);
    LevelLogger::Register(0, __FUNCTION__);
    LevelLogger::Log(__FUNCTION__, 0, " --------------------------- START ---------------------------");


    // Create default Win registration information
    WNDCLASSEXW wcex = swktool::WindowReg::CreateMDIFrameTemplate(hInstance);
    
    // customize some fields
   // wcex.lpszMenuName = MAKEINTRESOURCEW(MDIMENUINIT);
    //wcex.lpszMenuName = MAKEINTRESOURCEW(MDIMENUHELLO);
    wcex.lpszClassName = szFrameClass;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MDIDEMO));
    wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    //Create Windows Registration object
    swktool::WindowReg WinMDIFrameReg(wcex);

    //swktool::WindowReg WinReg2(wcex);
    WinMDIFrameReg.SetWinClassName(szFrameClass);


    WNDCLASSEXW wcex_Child1Reg = swktool::WindowReg::CreateMDIChildTemplate(hInstance);
    swktool::WindowReg WinChild1Reg(wcex_Child1Reg);
    WinChild1Reg.SetWinClassName(szHelloClass);

    //WNDCLASSEXW wcex_Child2Reg = swktool::WindowReg::CreateMDIChildTemplate(hInstance);
    //swktool::WindowReg WinChild2Reg(wcex_Child2Reg);
    //WinChild2Reg.SetWinClassName(szRectClass);




    AppWindowFrame App(hInstance);
    //App.SetLogger(&Log2);

    App.Register(WinMDIFrameReg);

    App.Register(WinChild1Reg);
    //App.Register(WinChild2Reg);


    {
        using namespace swktool;
        MdiMenuInit = WinMenu::Load(hInstance, MAKEINTRESOURCE(MDIMENUINIT));        
        MdiMenuHello = WinMenu::Load(hInstance, MAKEINTRESOURCE(MDIMENUHELLO));
        MdiMenuRect = WinMenu::Load(hInstance, MAKEINTRESOURCE(MDIMENURECT));
    }

    std::shared_ptr<swktool::Window> MDIFrame = App.CreateWin(szFrameClass, TEXT("MDI Demonstration"), MdiMenuInit, nCmdShow);

    int rc = App.DoMDIMessageLoop(MDIFrame);
    AppLog::Inst().Log(0, "Application Terminating...");

    LevelLogger::Log(__FUNCTION__, 0, " --------------------------- END ---------------------------");

    return rc;
}
