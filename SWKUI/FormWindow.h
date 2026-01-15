#pragma once
#include "Window.h"
#include "FormLayout.h"

namespace swktool {

    class FormWindow : public Window {
    public:
        PCWSTR ClassName() const override { return L"SWKFormWindow"; }

        // WinForms-like defaults
        void PreRegisterWindow(WindowRegisterClass& wc) override {
            wc.style = CS_HREDRAW | CS_VREDRAW;
            wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
            wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            wc.lpszClassName = ClassName();
        }

        bool CreateForm(PCWSTR title,
            int width = 800,
            int height = 600,
            DWORD style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            DWORD exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE)
        {
            return Create(title, style, exStyle,
                CW_USEDEFAULT, CW_USEDEFAULT,
                width, height, nullptr, nullptr);
        }

        template<typename TControl, typename... Args>
        std::shared_ptr<FormControl> AddControl(DockStyle dock,
            AnchorStyle anchor,
            Args&&... args)
        {
            // Assumes you have something like:
            // auto ctrl = std::make_shared<TControl>(); ctrl->CreateChild(...);
            auto ctrlObj = std::make_shared<TControl>(std::forward<Args>(args)...);
            // You may need to call ctrlObj->Create(...) here depending on your API.

            HWND hCtrl = ctrlObj->GetHwnd();
            auto fc = std::make_shared<FormControl>(hCtrl);
            fc->SetDock(dock);
            fc->SetAnchor(anchor);
            _layout.AddControl(fc);
            return fc;
        }

        // Or: overload taking an existing SWKUI control object
        template<typename TControl>
        std::shared_ptr<FormControl> AddControl(const std::shared_ptr<TControl>& ctrlObj,
            DockStyle dock,
            AnchorStyle anchor)
        {
            HWND hCtrl = ctrlObj->GetHwnd();
            auto fc = std::make_shared<FormControl>(hCtrl);
            fc->SetDock(dock);
            fc->SetAnchor(anchor);
            _layout.AddControl(fc);
            return fc;
        }

        void ShowCentered(int nCmdShow = SW_SHOW) {
            RECT rc{};
            GetWindowRect(GetHwnd(), &rc);
            int width = rc.right - rc.left;
            int height = rc.bottom - rc.top;

            RECT wa{};
            SystemParametersInfo(SPI_GETWORKAREA, 0, &wa, 0);
            int screenW = wa.right - wa.left;
            int screenH = wa.bottom - wa.top;

            int x = wa.left + (screenW - width) / 2;
            int y = wa.top + (screenH - height) / 2;

            SetWindowPos(GetHwnd(), nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
            Window::Show(nCmdShow);
        }

        // --- SWKUI virtuals ---

        LRESULT OnCreate(LPCREATESTRUCT pcs) override {
            _layout.OnInitialLayout(GetHwnd());
            return 0;
        }

        LRESULT OnSize(UINT type, int cx, int cy) override {
            _layout.OnSize(GetHwnd());
            return 0;
        }
        /*
        */
        void Show() { Window::Show(SW_SHOW); }
        void Hide() { Window::Show(SW_HIDE); }
        void Close() { ::DestroyWindow(GetHwnd()); }

        void CenterToScreen() { ShowCentered(); }

        void SetBounds(int x, int y, int width, int height) {
            SetWindowPos(GetHwnd(), nullptr, x, y, width, height, SWP_NOZORDER);
        }

        void SetClientSize(int width, int height) {
            RECT rc = { 0, 0, width, height };
            AdjustWindowRectEx(&rc, GetWindowStyle(GetHwnd()), FALSE, GetWindowExStyle(GetHwnd()));
            SetBounds(0, 0, rc.right - rc.left, rc.bottom - rc.top);
        }

        void Activate() {
            SetForegroundWindow(GetHwnd());
        }

        void Focus() {
            ::SetFocus(GetHwnd());
        }

        void Invalidate() {
            InvalidateRect(GetHwnd(), nullptr, TRUE);
        }

        void SuspendLayout() {
            SendMessage(GetHwnd(), WM_SETREDRAW, FALSE, 0);
        }

        void ResumeLayout() {
            SendMessage(GetHwnd(), WM_SETREDRAW, TRUE, 0);
            RedrawWindow(GetHwnd(), nullptr, nullptr,
                RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
        }

    private:
        FormLayoutEngine _layout;
    };

} // namespace swktool
