#include "pch.h"
#include "MDIFrameWindow.h"
#include "MDIChildWindow.h"
namespace swktool
{
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
}