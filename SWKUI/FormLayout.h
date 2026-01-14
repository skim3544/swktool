#pragma once
#include <windows.h>
#include <vector>
#include <memory>
#include <algorithm>

namespace swktool {

    enum class DockStyle {
        None,
        Top,
        Bottom,
        Left,
        Right,
        Fill
    };

    enum AnchorStyle {
        AnchorNone = 0,
        AnchorLeft = 1 << 0,
        AnchorTop = 1 << 1,
        AnchorRight = 1 << 2,
        AnchorBottom = 1 << 3
    };

    inline AnchorStyle operator|(AnchorStyle a, AnchorStyle b) {
        return static_cast<AnchorStyle>(static_cast<int>(a) | static_cast<int>(b));
    }

    inline bool HasAnchor(AnchorStyle mask, AnchorStyle flag) {
        return (static_cast<int>(mask) & static_cast<int>(flag)) != 0;
    }

    class FormControl {
    public:
        explicit FormControl(HWND hWnd = nullptr)
            : _hWnd(hWnd)
        {
        }

        HWND GetHwnd() const { return _hWnd; }
        void SetHwnd(HWND hWnd) { _hWnd = hWnd; }

        void SetDock(DockStyle dock) { _dock = dock; }
        DockStyle GetDock() const { return _dock; }

        void SetAnchor(AnchorStyle anchor) { _anchor = anchor; }
        AnchorStyle GetAnchor() const { return _anchor; }

        void SetMargin(int left, int top, int right, int bottom) {
            _marginLeft = left;
            _marginTop = top;
            _marginRight = right;
            _marginBottom = bottom;
        }

        void GetMargin(int& left, int& top, int& right, int& bottom) const {
            left = _marginLeft;
            top = _marginTop;
            right = _marginRight;
            bottom = _marginBottom;
        }

        void SetInitialBounds(const RECT& rc) { _initialBounds = rc; _hasInitial = true; }
        bool HasInitialBounds() const { return _hasInitial; }
        RECT GetInitialBounds() const { return _initialBounds; }

    private:
        HWND _hWnd{};
        DockStyle _dock{ DockStyle::None };
        AnchorStyle _anchor{ AnchorNone };
        int _marginLeft{ 0 }, _marginTop{ 0 }, _marginRight{ 0 }, _marginBottom{ 0 };
        RECT _initialBounds{};
        bool _hasInitial{ false };
    };

    class FormLayoutEngine {
    public:
        void AddControl(const std::shared_ptr<FormControl>& ctrl) {
            _controls.push_back(ctrl);
        }

        void RemoveControl(HWND hWnd) {
            _controls.erase(
                std::remove_if(_controls.begin(), _controls.end(),
                    [hWnd](const std::shared_ptr<FormControl>& c) {
                        return c->GetHwnd() == hWnd;
                    }),
                _controls.end());
        }

        void OnInitialLayout(HWND hParent) {
            RECT rcClient{};
            GetClientRect(hParent, &rcClient);
            _initialParentWidth = rcClient.right - rcClient.left;
            _initialParentHeight = rcClient.bottom - rcClient.top;

            for (auto& ctrl : _controls) {
                HWND h = ctrl->GetHwnd();
                if (!h) continue;

                RECT rc{};
                GetWindowRect(h, &rc);
                MapWindowPoints(nullptr, hParent, reinterpret_cast<POINT*>(&rc), 2);
                ctrl->SetInitialBounds(rc);
            }
        }

        void OnSize(HWND hParent) {
            RECT rcClient{};
            GetClientRect(hParent, &rcClient);

            int parentWidth = rcClient.right - rcClient.left;
            int parentHeight = rcClient.bottom - rcClient.top;

            // Dock pass
            RECT remaining = rcClient;

            auto dockOrder = {
                DockStyle::Top,
                DockStyle::Bottom,
                DockStyle::Left,
                DockStyle::Right,
                DockStyle::Fill
            };

            for (auto dockType : dockOrder) {
                for (auto& ctrl : _controls) {
                    if (ctrl->GetDock() != dockType)
                        continue;

                    HWND h = ctrl->GetHwnd();
                    if (!h) continue;

                    int ml, mt, mr, mb;
                    ctrl->GetMargin(ml, mt, mr, mb);

                    RECT rc = remaining;

                    switch (dockType) {
                    case DockStyle::Top:
                        rc.bottom = rc.top + ((rc.bottom - rc.top) / 4);
                        break;
                    case DockStyle::Bottom:
                        rc.top = rc.bottom - ((rc.bottom - rc.top) / 4);
                        break;
                    case DockStyle::Left:
                        rc.right = rc.left + ((rc.right - rc.left) / 4);
                        break;
                    case DockStyle::Right:
                        rc.left = rc.right - ((rc.right - rc.left) / 4);
                        break;
                    case DockStyle::Fill:
                        break;
                    default:
                        break;
                    }

                    rc.left += ml;
                    rc.top += mt;
                    rc.right -= mr;
                    rc.bottom -= mb;

                    MoveWindow(h, rc.left, rc.top,
                        rc.right - rc.left,
                        rc.bottom - rc.top,
                        TRUE);

                    switch (dockType) {
                    case DockStyle::Top:
                        remaining.top = rc.bottom + mb;
                        break;
                    case DockStyle::Bottom:
                        remaining.bottom = rc.top - mt;
                        break;
                    case DockStyle::Left:
                        remaining.left = rc.right + mr;
                        break;
                    case DockStyle::Right:
                        remaining.right = rc.left - ml;
                        break;
                    case DockStyle::Fill:
                        remaining = rc;
                        break;
                    default:
                        break;
                    }
                }
            }

            // Anchor pass
            for (auto& ctrl : _controls) {
                if (ctrl->GetDock() != DockStyle::None)
                    continue;

                HWND h = ctrl->GetHwnd();
                if (!h) continue;
                if (!ctrl->HasInitialBounds()) continue;

                RECT init = ctrl->GetInitialBounds();

                int ml, mt, mr, mb;
                ctrl->GetMargin(ml, mt, mr, mb);

                int initParentWidth = _initialParentWidth > 0 ? _initialParentWidth : parentWidth;
                int initParentHeight = _initialParentHeight > 0 ? _initialParentHeight : parentHeight;

                int dx = parentWidth - initParentWidth;
                int dy = parentHeight - initParentHeight;

                RECT rc = init;

                AnchorStyle anchor = ctrl->GetAnchor();

                if (HasAnchor(anchor, AnchorRight) && !HasAnchor(anchor, AnchorLeft)) {
                    rc.left += dx;
                    rc.right += dx;
                }
                else if (HasAnchor(anchor, AnchorLeft) && HasAnchor(anchor, AnchorRight)) {
                    rc.right += dx;
                }

                if (HasAnchor(anchor, AnchorBottom) && !HasAnchor(anchor, AnchorTop)) {
                    rc.top += dy;
                    rc.bottom += dy;
                }
                else if (HasAnchor(anchor, AnchorTop) && HasAnchor(anchor, AnchorBottom)) {
                    rc.bottom += dy;
                }

                rc.left += ml;
                rc.top += mt;
                rc.right -= mr;
                rc.bottom -= mb;

                MoveWindow(h, rc.left, rc.top,
                    rc.right - rc.left,
                    rc.bottom - rc.top,
                    TRUE);
            }
        }

    private:
        std::vector<std::shared_ptr<FormControl>> _controls;
        int _initialParentWidth{ 0 };
        int _initialParentHeight{ 0 };
    };

} // namespace swktool
