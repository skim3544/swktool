#include "pch.h"
#include "RebarCtrl.h"

namespace swktool {
	//RebarCtrl::RebarCtrl(Window* pParent) : Ctrl(pParent) {

 //       // Initialize common controls.
 //       INITCOMMONCONTROLSEX icex{};
 //       icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
 //       icex.dwICC = ICC_COOL_CLASSES | ICC_BAR_CLASSES;
 //       InitCommonControlsEx(&icex);


 //       // Create the rebar.
 //       hwndCtrl = CreateWindowEx(WS_EX_TOOLWINDOW,
 //           REBARCLASSNAME,
 //           NULL,
 //           WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS |
 //           WS_CLIPCHILDREN | RBS_VARHEIGHT |
 //           CCS_NODIVIDER | RBS_BANDBORDERS,
 //           0, 0, 0, 0,
 //           pParent->GetHwnd(),
 //           NULL,
 //           hInst, // global instance handle
 //           NULL);


 //       // Initialize band info used by both bands.
 //       REBARBANDINFO rbBand = { sizeof(REBARBANDINFO) };
 //       rbBand.fMask =
 //           RBBIM_STYLE       // fStyle is valid.
 //           | RBBIM_TEXT        // lpText is valid.
 //           | RBBIM_CHILD       // hwndChild is valid.
 //           | RBBIM_CHILDSIZE   // child size members are valid.
 //           | RBBIM_SIZE;       // cx is valid
 //       rbBand.fStyle = RBBS_CHILDEDGE | RBBS_GRIPPERALWAYS;

 //       // Get the height of the toolbar.
 //       DWORD dwBtnSize = (DWORD)SendMessage(hwndCtrl, TB_GETBUTTONSIZE, 0, 0);


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
	//}

	//RebarCtrl::RebarCtrl(DialogWindow* pParent) : Ctrl(pParent) {

	//}

}