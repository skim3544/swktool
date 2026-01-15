#include "pch.h"
#include "Window.h"
#include "MDIFrameWindow.h"
#include "MDIChildWindow.h"

//
namespace swktool 
{
    void MDIFrameWindow::PreRegisterWindow(WindowRegisterClass& wc)
    {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
        wc.hInstance = ::GetModuleHandle(nullptr);

        // Framework default: no menu.
        // Applications may override PreRegisterWindow() to set lpszMenuName.
        wc.lpszMenuName = nullptr;
    }

    LRESULT MDIFrameWindow::OnCreate(CREATESTRUCT* cs) 
    {
        CLIENTCREATESTRUCT ccs{};
        ccs.hWindowMenu = GetDefaultWindowMenu();
        ccs.idFirstChild = GetFirstChildId();

        hMdiClient_ = CreateWindowEx(
            0,
            L"MDICLIENT",
            nullptr,
            WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
            0, 0, 0, 0,
            GetHwnd(),
            (HMENU)1,
            GetModuleHandle(nullptr),
            &ccs
        );

        return hMdiClient_ != nullptr;
    }

    void MDIFrameWindow::OnDestroy() 
    {
        // This is important, quiting frame should terminate application
        PostQuitMessage(0);
    }

    void MDIFrameWindow::RemoveChild(MDIChildWindow* child)
    {
        auto it = std::find(children_.begin(), children_.end(), child);
        if (it != children_.end())
            children_.erase(it);
    }

    HMENU MDIFrameWindow::GetDefaultWindowMenu()
    {
        HMENU hMenu = GetMenu(GetHwnd());
        return hMenu ? GetSubMenu(hMenu, 0) : nullptr;
    }

    void MDIFrameWindow::PropagateThemeToChildren(const Theme& theme)
    {
        for (auto* child : children_)
            child->PropagateTheme(theme);
    }
}

