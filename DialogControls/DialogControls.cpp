
#include <Windows.h>

//#include "framework.h"

#include "..\SWKUI\MsgHandler.h"
//#include "..\SWKUI\msg_filter.h"
#include "..\SWKUI\Window.h"
//#include "..\SWKUI\WinMenu.h"
//#include "..\SWKUI\DialogWindow.h"
//#include "..\SWKBase\DebugStream.h"
#include "..\SWKUI\StatusBarCtrl.h"
#include "..\SWKUI\HeaderCtrl.h"
#include "..\SWKUI\RebarCtrl.h"
#include "..\SWKUI\ScrollBarCtrl.h"
//
//
#include "Resource.h"
//
#include "Test1Dlg.h"
#include "Test2Dlg.h"
//
//
class MainWindow : 
    public swktool::Window 
{
protected:
    std::unique_ptr<swktool::StatusBarCtrl> StatusBar_;
    std::unique_ptr<swktool::HeaderCtrl> HeaderCtrl_;
    std::unique_ptr<swktool::RebarCtrl> RebarCtrl_;
    //std::unique_ptr<swktool::HScrollBarCtrl> HScrollBar_;
//    
public:
    PCWSTR  ClassName() const {
        return L"Sample Window";
    }

    void PreRegisterWindow(WindowRegisterClass& wc) override 
    {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW  + 1);
        wc.hInstance = ::GetModuleHandle(NULL);
        wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_DIALOGCONTROLS));
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

        // Added menu
        wc.lpszMenuName = MAKEINTRESOURCE(IDC_DIALOGCONTROLS);
    }

//
    LRESULT OnCreate(LPCREATESTRUCT lpCreateStruct) override 
    {
        // status bar
        StatusBar_ = std::make_unique<swktool::StatusBarCtrl>(TEXT("Status"), 0, 0, 0, 0, 0, this, 0);
        if (StatusBar_)
        {
            int iRightEdge[] = {500, 300, -1};
            StatusBar_->SetParts(iRightEdge, _countof(iRightEdge));
            StatusBar_->SetText(TEXT("Hello World"));

            StatusBar_->SetStatusText(2, TEXT("Hello World2"));
            auto bar = StatusBar_->AddProgressBarToPart(2);
            bar->SetRange(0, 100);
            bar->SetPosition(0);
        }

        RebarCtrl_ = std::make_unique<swktool::RebarCtrl>(this);
        RebarCtrl_->Show();
//
//        HScrollBar_ = std::make_unique<swktool::HScrollBarCtrl>(this);
//
//        //// Add header control with 2 header labels
        HeaderCtrl_ = std::make_unique<swktool::HeaderCtrl>(this);
        HeaderCtrl_->InsertItem(0, 300, (WCHAR*)L"Test1");
        HeaderCtrl_->InsertItem(1, 600, (WCHAR*)L"Test2");
        HeaderCtrl_->Show();


        return TRUE;
    }
//
    virtual LRESULT OnSize(UINT state, int nWidth, int nHeight) 
    {
        if (StatusBar_)
        {
            StatusBar_->OnSize(state, nWidth, nHeight);
        }

        if (HeaderCtrl_)
        {
            HeaderCtrl_->MoveWindow(0, 0, nWidth, 25);
        }
        //HScrollBar_->OnSize(state, nWidth, nHeight);

        if (RebarCtrl_)
            RebarCtrl_->MoveWindow(0, 0, nWidth, 25);

        return 0;
    }
//
//    //void OnSysCommand(UINT nID, LPARAM lParam) override {        
//    //    Window::OnSysCommand(nID, lParam);
//    //}
//
//    LRESULT DoTest1Dlg(WPARAM wParam, LPARAM lParam) {
//        LRESULT result;
//        StatusBar_->SetText(TEXT("Showing Dialog 1"));
//        Test1Dlg Dlg(IDD_TEST1, this);
//        Dlg.SetCaption(L"Test1 Dialog");
//        result = Dlg.ShowDialog();
//        StatusBar_->SetText(TEXT(""));
//
//        return result;
//    }
//
//
//    LRESULT DoTest2Dlg(WPARAM wParam, LPARAM lParam) {
//        LRESULT result = 0;
//
//        Test2Dlg Dlg(IDD_TEST2, this);
//        Dlg.SetCaption(L"Test2 Dialog");
//        result = Dlg.ShowDialog();
//        StatusBar_->SetText(TEXT(""));
//
//        return result;
//    }
//
//    LRESULT DoAboutDlg(WPARAM wParam, LPARAM lParam) {
//        LRESULT result = 0;
//
//        swktool::DialogWindow w(IDD_ABOUTBOX, this);
//        w.SetCaption(L"About Simple Window");
//        result = w.ShowDialog();
//
//        return result;
//    }
//
//
//#define BEGIN_CMD_MSG() \
//        LRESULT lresult = 0; \
//        switch(wParam) {
//
//#define END_CMD_MSG() \
//        default:\
//        return DefWindowProc(GetHwnd(), WM_COMMAND, wParam, lParam);\
//    }\
//    return 0L;\
//
//#define ON_CMD_HANDLE(CMD, HANDLER) case CMD: lresult = HANDLER(wParam, lParam); break;
//    virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam)
//    {
//        BEGIN_CMD_MSG();
//            ON_CMD_HANDLE(IDM_FILE_TEST1, DoTest1Dlg);
//            ON_CMD_HANDLE(IDM_FILE_TEST2, DoTest2Dlg);
//            ON_CMD_HANDLE(IDM_ABOUT, DoAboutDlg);
//
//        case IDM_EXIT:
//            OnClose();
//            break;
//
//        END_CMD_MSG()
//
//    }
//

        // Called for WM_COMMAND
    virtual LRESULT OnCommand(WORD id, WORD code, HWND control) override 
    { 
        switch (id)
        {
        case IDM_ABOUT:
            StatusBar_->SetText(TEXT("About..."));
            break;

        case IDM_EXIT:
            StatusBar_->SetText(TEXT("Exit..."));
            OnClose();
            break;

        case IDM_FILE_TEST1:
            StatusBar_->SetText(TEXT("Dialog Test 1"));
            {
                ShowTest1Dialog();
            }
            StatusBar_->SetText(TEXT(""));
            break;

        default:
            return WindowHandlerBase::OnCommand(id, code,control);
        }
        return 0; 
    }

    void ShowTest1Dialog()
    {
        Test1Dlg dlg(IDD_TEST1, this);  // parent is IWindow*

        auto val = dlg.ShowDialog();   // clean, simple, and consistent
    }

    void OnDestroy() override
    {
        PostQuitMessage(0);
    }

};


//
//
//


#pragma comment(lib, "SWKBase.lib")
#pragma comment(lib, "SWKUI.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Simple Window", WS_OVERLAPPEDWINDOW,  WS_EX_WINDOWEDGE))
    {
        return 0;
    }

    ShowWindow(win.GetHwnd(), nCmdShow);
    UpdateWindow(win.GetHwnd());

    // Run the message loop.    
    swktool::AppMsgLoop MsgLoop;
    return MsgLoop.Run();
}

