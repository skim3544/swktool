#include "framework.h"
#include "..\SWKUI\MsgHandler.h"
#include "..\SWKUI\msg_filter.h"
#include "..\SWKUI\Window.h"



class MainWindow : public  swktool::Window  {
public:
    PCWSTR  ClassName() const { 
        return L"Sample Window"; 
    }

    virtual void PreRegisterWindow(WNDCLASSEX& wc) override {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    }
};




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
 {
    MainWindow win;


    if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW))
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

