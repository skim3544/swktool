// MDIDemo.cpp : Defines the entry point for the application.
//

#define _CRTDBG_MAP_ALLOC
#include <Windows.h>
#include <string>
#include <stdlib.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


#include "..\SWKBase\DebugStream.h"
#include "..\SWKBase\IOC.h"
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

swktool::IOCContainer& GetDI() {
    static swktool::IOCContainer oIOC;
    return oIOC;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
   // _crtBreakAlloc = 399;

    using namespace swktool;
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    GetDI().Register<ILogger, Logger>(object_type::Singleton);

    auto* pLogger = GetDI().Resolve<ILogger, Logger>();
    pLogger->init(TEXT("DetailedLog.log"));
    //pLogger->SetLogLevel(LogLevel::STATUS);
    pLogger->Register(__FUNCTION__, LogLevel::STATUS);
    pLogger->Log(__FUNCTION__, LogLevel::STATUS, " --------------------------- START ---------------------------");
   
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

    //AppLog::Inst().Log(LogLevel::STATUS, "Application Terminating...");

    pLogger->Log(__FUNCTION__, LogLevel::STATUS, " --------------------------- END ---------------------------");

    _CrtDumpMemoryLeaks();
    return rc;
}
