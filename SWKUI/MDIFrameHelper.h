#pragma once
#include <windows.h>

namespace swktool 
{
    class MDIWindowHelper 
    {
    public:
        static void Cascade(HWND hMdiClient) {
            ::SendMessage(hMdiClient, WM_MDICASCADE, 0, 0);
        }

        static void TileHorizontal(HWND hMdiClient) {
            ::SendMessage(hMdiClient, WM_MDITILE, MDITILE_HORIZONTAL, 0);
        }

        static void TileVertical(HWND hMdiClient) {
            ::SendMessage(hMdiClient, WM_MDITILE, MDITILE_VERTICAL, 0);
        }

        static void ArrangeIcons(HWND hMdiClient) {
            ::SendMessage(hMdiClient, WM_MDIICONARRANGE, 0, 0);
        }

        static void RefreshMenu(HWND hMdiClient) {
            ::SendMessage(hMdiClient, WM_MDIREFRESHMENU, 0, 0);
        }

        static void TileGrid(HWND hMdiClient) {
            if (!hMdiClient)
                return;

            // Collect all visible, non-minimized MDI children
            std::vector<HWND> children;
            EnumChildWindows(hMdiClient, [](HWND hwnd, LPARAM lParam) -> BOOL {
                if (GetWindow(hwnd, GW_OWNER) == nullptr) { // MDI child windows have no owner
                    if (IsWindowVisible(hwnd) && !IsIconic(hwnd)) {
                        reinterpret_cast<std::vector<HWND>*>(lParam)->push_back(hwnd);
                    }
                }
                return TRUE;
                }, reinterpret_cast<LPARAM>(&children));

            const int count = static_cast<int>(children.size());
            if (count == 0)
                return;

            // Compute grid dimensions
            int cols = static_cast<int>(std::ceil(std::sqrt(count)));
            int rows = static_cast<int>(std::ceil(count / static_cast<double>(cols)));

            // Get client area
            RECT rc{};
            GetClientRect(hMdiClient, &rc);

            int totalWidth = rc.right - rc.left;
            int totalHeight = rc.bottom - rc.top;

            int cellWidth = totalWidth / cols;
            int cellHeight = totalHeight / rows;

            // Disable redraw for smoother layout
            SendMessage(hMdiClient, WM_SETREDRAW, FALSE, 0);

            // Position each child
            for (int i = 0; i < count; ++i) {
                int r = i / cols;
                int c = i % cols;

                int x = c * cellWidth;
                int y = r * cellHeight;

                MoveWindow(children[i], x, y, cellWidth, cellHeight, TRUE);
            }

            // Re-enable redraw
            SendMessage(hMdiClient, WM_SETREDRAW, TRUE, 0);
            RedrawWindow(hMdiClient, nullptr, nullptr,
                RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
        }

        static void CloseAll(HWND hMdiClient)
        {
            if (!hMdiClient)
                return;

            // Snapshot all current MDI child windows
            std::vector<HWND> children;

            EnumChildWindows(hMdiClient, [](HWND hwnd, LPARAM lParam) -> BOOL {
                // MDI child windows have WS_EX_MDICHILD
                LONG_PTR styleEx = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
                if (styleEx & WS_EX_MDICHILD) {
                    reinterpret_cast<std::vector<HWND>*>(lParam)->push_back(hwnd);
                }
                return TRUE;
                }, reinterpret_cast<LPARAM>(&children));

            // Now walk the snapshot in order
            for (HWND hwndChild : children)
            {
                if (!IsWindow(hwndChild))
                    continue; // already closed somehow

                // Make it active (optional but nice for UX)
                SendMessage(hMdiClient, WM_MDIACTIVATE, (WPARAM)hwndChild, 0);

                // Ask it to close itself (this will hit your OnClose/OnQueryClose)
                SendMessage(hwndChild, WM_CLOSE, 0, 0);

                // If it's still alive, user probably canceled -> stop Close All
                if (IsWindow(hwndChild))
                    break;
            }

            RedrawWindow(
                hMdiClient,
                nullptr,
                nullptr,
                RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW
            );
        }


    };
} // namespace swktool
