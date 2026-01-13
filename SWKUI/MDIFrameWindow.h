#pragma once
#include "Window.h"


namespace swktool {
    class MDIChildWindow;

    class MDIFrameWindow : public Window
    {
        using MDIChildren = std::vector<swktool::MDIChildWindow*>;

    public:
        virtual bool IsMDIFrame() const { return true; }
        HWND GetMDIClientHWND() const { return _hMdiClient; }

        PCWSTR ClassName() const override { return L"SWKUI_MDI_FRAME"; }

        void PreRegisterWindow(WindowRegisterClass& wc) override
        {
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.hbrBackground = (HBRUSH)(COLOR_APPWORKSPACE + 1);
            wc.hInstance = ::GetModuleHandle(nullptr);

            // Framework default: no menu.
            // Applications may override PreRegisterWindow() to set lpszMenuName.
            wc.lpszMenuName = nullptr;
        }

        LRESULT OnCreate(CREATESTRUCT* cs) override
        {
            CLIENTCREATESTRUCT ccs{};
            ccs.hWindowMenu = GetDefaultWindowMenu();
            ccs.idFirstChild = GetFirstChildId();

            _hMdiClient = CreateWindowEx(
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

            return _hMdiClient != nullptr;
        }

        LRESULT OnSize(UINT, int cx, int cy) override
        {
            if (_hMdiClient)
                MoveWindow(_hMdiClient, 0, 0, cx, cy, TRUE);

            return TRUE;
        }

        LRESULT OnCommand(WORD id, WORD code, HWND control) override
        {
            // Framework default: do nothing.
            // Applications override this to handle menu commands.
            return FALSE;
        }

        void OnClose() override
        {
            DestroyWindow(GetHwnd());
        }
        void OnDestroy() override 
        { 
            PostQuitMessage(0); 
        }

        void RemoveChild(MDIChildWindow* child)
        {
            auto it = std::find(_children.begin(), _children.end(), child);
            if (it != _children.end())
                _children.erase(it);
        }

    protected:
        // Applications may override this to provide a menu for the MDI client.
        virtual HMENU GetDefaultWindowMenu()
        {
            HMENU hMenu = GetMenu(GetHwnd());
            return hMenu ? GetSubMenu(hMenu, 0) : nullptr;
        }

        // Applications may override this to change the first child ID.
        virtual UINT GetFirstChildId() const { return 100; }

        template<typename TChildWindow>
        TChildWindow* CreateMdiChild(const wchar_t* title)
        {
            auto* child = new TChildWindow();
            child->SetMDIFrameWindow(this);
            child->Register();            

            MDICREATESTRUCT mcs{};
            mcs.szClass = child->ClassName();
            mcs.szTitle = title;
            mcs.hOwner = GetModuleHandle(nullptr);
            mcs.x = mcs.cx = CW_USEDEFAULT;
            mcs.y = mcs.cy = CW_USEDEFAULT;
            mcs.style = WS_VISIBLE | WS_CHILD | WS_OVERLAPPEDWINDOW;

            HWND hwndChild = (HWND)SendMessage(_hMdiClient, WM_MDICREATE, 0, (LPARAM)&mcs);

            if (!hwndChild) {
                delete child;
                return nullptr;
            }

            child->Subclass(hwndChild);
            AddChild(child);
            return child;
        }

        void AddChild(MDIChildWindow* child)
        {
            _children.push_back(child);
        }



    private:
        HWND _hMdiClient = nullptr;
        MDIChildren _children;
    };

} // namespace swktool
