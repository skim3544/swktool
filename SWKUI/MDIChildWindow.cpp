#include "pch.h"
#include "MDIFrameWindow.h"
#include "MDIChildWindow.h"
namespace swktool
{
    bool MDIChildWindow::AttachToMDI(HWND hwnd) 
    {
        return Subclass(hwnd);
    }

    void MDIChildWindow::SetMDIFrameWindow(MDIFrameWindow* pFrame)
    {
        Frame_ = pFrame;
    }

    LRESULT MDIChildWindow::OnNcDestroy() 
    {
        if (Frame_)
            Frame_->RemoveChild(this);

        return Window::OnNcDestroy();
    }

    void   MDIChildWindow::PreRegisterWindow(WindowRegisterClass& wc)
    {
        wc = WindowsDefaultRegister::CreateMDIChild(ClassName());
    }

    bool MDIChildWindow::Register()
    {
        WindowRegisterClass wc{};
        wc.cbSize = sizeof(WindowRegisterClass);
        PreRegisterWindow(wc);

        ATOM atom = RegisterClassEx(&wc);
        if (atom == 0)
        {
            DWORD err = GetLastError();
            if (err != ERROR_CLASS_ALREADY_EXISTS)
                return false;
        }

        return true;
    }
}