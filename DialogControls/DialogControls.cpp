#include "framework.h"
#include "..\SWKUI\MsgHandler.h"
#include "..\SWKUI\msg_filter.h"
#include "..\SWKUI\Window.h"
#include "..\SWKUI\WinMenu.h"
#include "..\SWKUI\DialogWindow.h"
#include "..\SWKBase\DebugStream.h"
#include "..\SWKUI\StatusBarCtrl.h"
#include "..\SWKUI\HeaderCtrl.h"
#include "..\SWKUI\RebarCtrl.h"
#include "..\SWKUI\ScrollBarCtrl.h"


#include "Resource.h"

#include "Test1Dlg.h"
#include "Test2Dlg.h"


class MainWindow :
    public  swktool::Window {

protected:
    std::unique_ptr<swktool::StatusBarCtrl> StatusBar_;
    std::unique_ptr<swktool::HeaderCtrl> HeaderCtrl_;
    //std::unique_ptr<swktool::RebarCtrl> RebarCtrl_;
    std::unique_ptr<swktool::HScrollBarCtrl> HScrollBar_;
    
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
    }

    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct) override {
    //virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) {

        StatusBar_ = std::make_unique<swktool::StatusBarCtrl>(TEXT("Status"), 0, 0, 0, 0, 0, this, 0);
        //RebarCtrl_ = std::make_unique<swktool::RebarCtrl>(this);

        HScrollBar_ = std::make_unique<swktool::HScrollBarCtrl>(this);

        //// Add header control with 2 header labels
        HeaderCtrl_ = std::make_unique<swktool::HeaderCtrl>(this);
        HeaderCtrl_->InsertItem(0, 300, (WCHAR*)L"Test1");
        HeaderCtrl_->InsertItem(1, 300, (WCHAR*)L"Test2");

        return TRUE;
    }

    virtual void OnSize(UINT state, int nWidth, int nHeight) {
        StatusBar_->OnSize(state, nWidth, nHeight);
        HeaderCtrl_->OnSize(state, nWidth, nHeight);
        HScrollBar_->OnSize(state, nWidth, nHeight);

        //RebarCtrl_->OnSize(state, nWidth, nHeight);    
    }

    //void OnSysCommand(UINT nID, LPARAM lParam) override {        
    //    Window::OnSysCommand(nID, lParam);
    //}

    LRESULT DoTest1Dlg(WPARAM wParam, LPARAM lParam) {
        LRESULT result;
        StatusBar_->SetText(TEXT("Showing Dialog 1"));
        Test1Dlg Dlg(IDD_TEST1, this);
        Dlg.SetCaption(L"Test1 Dialog");
        result = Dlg.ShowDialog();
        StatusBar_->SetText(TEXT(""));

        return result;
    }


    LRESULT DoTest2Dlg(WPARAM wParam, LPARAM lParam) {
        LRESULT result = 0;

        Test2Dlg Dlg(IDD_TEST2, this);
        Dlg.SetCaption(L"Test2 Dialog");
        result = Dlg.ShowDialog();
        StatusBar_->SetText(TEXT(""));

        return result;
    }

    LRESULT DoAboutDlg(WPARAM wParam, LPARAM lParam) {
        LRESULT result = 0;

        swktool::DialogWindow w(IDD_ABOUTBOX, this);
        w.SetCaption(L"About Simple Window");
        result = w.ShowDialog();

        return result;
    }


#define BEGIN_CMD_MSG() \
        LRESULT lresult = 0; \
        switch(wParam) {

#define END_CMD_MSG() \
        default:\
        return DefWindowProc(GetHwnd(), WM_COMMAND, wParam, lParam);\
    }\
    return 0L;\

#define ON_CMD_HANDLE(CMD, HANDLER) case CMD: lresult = HANDLER(wParam, lParam); break;
    virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam)
    {
        BEGIN_CMD_MSG();
            ON_CMD_HANDLE(IDM_FILE_TEST1, DoTest1Dlg);
            ON_CMD_HANDLE(IDM_FILE_TEST2, DoTest2Dlg);
            ON_CMD_HANDLE(IDM_ABOUT, DoAboutDlg);

        case IDM_EXIT:
            OnClose();
            break;

        END_CMD_MSG()

    }

};




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MainWindow win;

    //if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW, 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, (HMENU)MainMenu))
    if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL, WS_EX_WINDOWEDGE))
    {
        return 0;
    }

    ShowWindow(win.GetHwnd(), nCmdShow);
    //UpdateWindow(win.WindowHandle());

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

