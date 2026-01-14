// FormWindowDemo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "FormWindowDemo.h"
#include "../SWKUI/FormWindow.h"
#include "../SWKUI/MsgLoop.h"

#define MAX_LOADSTRING 100


#pragma comment(lib, "SWKBase.lib")
#pragma comment(lib, "SWKUI.lib")

class DemoWinForm : public swktool::FormWindow
{
public:
    PCWSTR ClassName() const override { return L"SWKFormWindow"; }

    // WinForms-like defaults
    void PreRegisterWindow(WindowRegisterClass& wc) override {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = ClassName();
    }

    void OnDestroy() override
    {
        PostQuitMessage(0);
    }
};


// Forward declarations of functions included in this code module:

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    using namespace swktool;
    DemoWinForm form;

    //if (!form.Create(L"Simple Form Window", WS_OVERLAPPEDWINDOW))
    if (!form.CreateForm(L"Simple Form Window"))
        return 0;

    form.ShowCentered();

    // Run the message loop.
    AppMsgLoop MsgLoop;
    return MsgLoop.Run();
}
