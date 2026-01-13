#include "framework.h"
#include "..\SWKUI\MsgHandler.h"
#include "../SWKUI/Window.h"

#pragma comment(lib, "SWKBase.lib")
#pragma comment(lib, "SWKUI.lib")

class MainWindow : public swktool::Window {
public:
    // This is the Windows App registration name.  Must be unique
    PCWSTR  ClassName() const { 
        return L"Sample Window"; 
    }
    
    virtual void PreRegisterWindow(WNDCLASSEX& wc) override 
    {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    }

    void OnDestroy() override 
    {
        PostQuitMessage(0);
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

    win.Show(nCmdShow);

    // Run the message loop.
    swktool::AppMsgLoop MsgLoop;
    return MsgLoop.Run();    
 }

