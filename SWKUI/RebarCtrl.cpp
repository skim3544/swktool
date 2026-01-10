#include "pch.h"
#include "RebarCtrl.h"

namespace swktool
{

    RebarCtrl::RebarCtrl(std::wstring Caption, DWORD Style, int x, int y,
        int Height, int Width, IWindow* pParent, UINT CtrlID)
        : Ctrl(pParent)
    {
        ID = CtrlID;

        // Ensure common controls are initialized
        INITCOMMONCONTROLSEX icex{};
        icex.dwSize = sizeof(icex);
        icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
        InitCommonControlsEx(&icex);

        // Create the rebar window
        hwndCtrl = CreateWindowEx(
            0,
            REBARCLASSNAME,
            Caption.empty() ? nullptr : Caption.c_str(),
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |
            RBS_VARHEIGHT | CCS_NODIVIDER | RBS_BANDBORDERS |
            Style,
            x, y, Width, Height,
            hParent,
            (HMENU)(UINT_PTR)ID,
            hInst,
            nullptr
        );

        if (!hwndCtrl)
            return;

        // Create a simple child control so the rebar is visible
        HWND hChild = CreateWindow(
            L"STATIC",
            L"Rebar Band",
            WS_CHILD | WS_VISIBLE,
            0, 0, 100, 20,
            hwndCtrl,
            nullptr,
            hInst,
            nullptr
        );

        // Insert a band
        REBARBANDINFO rb{};
        rb.cbSize = sizeof(rb);
        rb.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE |
            RBBIM_TEXT | RBBIM_STYLE;
        rb.fStyle = RBBS_CHILDEDGE;
        rb.lpText = const_cast<LPWSTR>(L"Band 1");
        rb.hwndChild = hChild;
        rb.cxMinChild = 100;
        rb.cyMinChild = 20;
        rb.cx = Width;

        SendMessage(hwndCtrl, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rb);
    }


    RebarCtrl::RebarCtrl(IWindow* pParent) :
        Ctrl(pParent)
    {

        // Initialize common controls.
        INITCOMMONCONTROLSEX icex{};
        icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
        icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
        InitCommonControlsEx(&icex);


        // Create the rebar.
        hwndCtrl = CreateWindowEx(WS_EX_TOOLWINDOW,
            REBARCLASSNAME,
            NULL,
            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN | RBS_VARHEIGHT |
            CCS_NODIVIDER | RBS_BANDBORDERS,
            0, 0, 0, 0,
            hParent,
            NULL,
            hInst, // global instance handle
            NULL);


        /* sample code */

     // Create a simple child control to show inside the rebar
        HWND hChild = CreateWindow(
            L"STATIC",
            L"Rebar Band",
            WS_CHILD | WS_VISIBLE,
            0, 0, 100, 20,
            hwndCtrl,
            nullptr,
            hInst,
            nullptr);

        REBARBANDINFO rb{};
        rb.cbSize = sizeof(rb);
        rb.fMask = RBBIM_CHILD | RBBIM_CHILDSIZE | RBBIM_SIZE | RBBIM_STYLE | RBBIM_TEXT;
        rb.fStyle = RBBS_CHILDEDGE;
        rb.lpText = const_cast<LPWSTR>(L"Band 1");
        rb.hwndChild = hChild;
        rb.cxMinChild = 100;
        rb.cyMinChild = 20;
        rb.cx = 200;

        SendMessage(hwndCtrl, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rb);


        //// Initialize band info used by both bands.
        //REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
        //rbBand.fMask =
        //    RBBIM_STYLE       // fStyle is valid.
        //    | RBBIM_TEXT        // lpText is valid.
        //    | RBBIM_CHILD       // hwndChild is valid.
        //    | RBBIM_CHILDSIZE   // child size members are valid.
        //    | RBBIM_SIZE;       // cx is valid
        //rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;

        // Get the height of the toolbar.
        //DWORD dwBtnSize = (DWORD)SendMessage(hwndCtrl, TB_GETBUTTONSIZE, 0, 0);


 //       // Set values unique to the band with the toolbar.
 //       rbBand.lpText = "";
 //       rbBand.hwndChild = hwndToolbar;
 //       rbBand.cyChild = LOWORD(dwBtnSize);
 //       rbBand.cxMinChild = NUMBUTTONS * HIWORD(dwBtnSize);
 //       rbBand.cyMinChild = LOWORD(dwBtnSize);
 //       // The default width is the width of the buttons.
 //       rbBand.cx = 0;

 //       // Add the band that has the toolbar.
 //       SendMessage(hwndCtrl, RB_INSERTBAND, (WPARAM)-1, (LPARAM)&rbBand);
    }
}