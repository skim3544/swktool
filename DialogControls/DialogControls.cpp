#include "framework.h"
#include "..\SWKUI\MsgHandler.h"
#include "..\SWKUI\msg_filter.h"
#include "..\SWKUI\Window.h"
#include "..\SWKUI\WinMenu.h"

#include "..\SWKUI\DialogWindow.h"

#include "..\SWKBase\DebugStream.h"
#include "Resource.h"

//#include "SimpleDialog.h"


#include "..\SWKUI\DialogWindow.h"
#include "Test1Dlg.h"

class MainWindow :
    public  swktool::Window {


public:
    PCWSTR  ClassName() const {
        return L"Sample Window";
    }

    virtual void PreRegisterWindow(WindowRegisterClass& wc) override {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hInstance = ::GetModuleHandle(NULL);
        wc.lpszMenuName = MAKEINTRESOURCE(IDC_DIALOGCONTROLS);
        wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_DIALOGCONTROLS));
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        //(PCWSTR) L"IDC_SIMPLEDIALOG";
        //wc.lpszMenuName = ClassName();
    }

    virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam)
    {
        auto result = 0;
        switch (wParam)
        {
        case IDM_FILE_TEST1:
        {
            Test1Dlg Dlg(IDD_TEST1, this);
            Dlg.SetCaption(L"Test1 Dialog");
            result = Dlg.ShowDialog();
        }break;

        case IDM_FILE_TEST2:
        {
            Test1Dlg Dlg(IDD_TEST2, this);
            Dlg.SetCaption(L"Test2 Dialog");
            result = Dlg.ShowDialog();
        }
            break;

        case IDM_ABOUT:
        {
            swktool::DialogWindow w(IDD_ABOUTBOX, this);
            w.SetCaption(L"About Simple Window");
            result = w.ShowDialog();
        }
        break;

        case IDM_EXIT:
            OnClose();
            break;

        default:
            return DefWindowProc(m_hwnd, WM_COMMAND, wParam, lParam);
        }
        return 0L;
    }

};




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

