#include "framework.h"
#include "..\SWKUI\MsgHandler.h"
#include "..\SWKUI\msg_filter.h"
#include "..\SWKUI\Window.h"
#include "..\SWKUI\WinMenu.h"

#include "..\SWKUI\DialogWindow.h"
#include "..\SWKBase\DebugStream.h"

#include "SimpleDialog.h"




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MainWindow win;

    //if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, (HMENU)MainMenu))
    if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW, WS_EX_WINDOWEDGE))
    {
        return 0;
    }

    ShowWindow(win.WindowHandle(), nCmdShow);
    UpdateWindow(win.WindowHandle());

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

