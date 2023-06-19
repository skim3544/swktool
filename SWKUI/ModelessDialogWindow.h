//#ifndef ___MODELESS_DIALOG_WINDOW_H___
//#define ___MODELESS_DIALOG_WINDOW_H___
//
//
//#include "WinEventHandler.h"
//
//namespace swktool {
//
//	class ModelessDialogWindow : public DialogWindow {
//		INT				DialogResult = 0;
//		LPCTSTR			DialogTemplateName = nullptr;
//
//	public:
//		ModelessDialogWindow(UINT ID, AWindow* pParent) : 
//			DialogWindow(ID, pParent)
//		{
//			DialogTemplateName = MAKEINTRESOURCE(ID);									
//		}
//
//		BOOL Create(std::shared_ptr<DialogWindow> pDlg) {
//			WinEventHandler::GetInst()->PreRegisterModelessDlgWindow(pDlg);
//
//			hwndWindow = ::CreateDialog(hInst, DialogTemplateName, hwndParent, swktool::WinEventHandler::ProcModelessDlgMessage);
//
//			return TRUE;
//		}
//
//		~ModelessDialogWindow() {
//
//		}
//
//		virtual void ShowWindow()
//		{
//			::ShowWindow(hwndWindow, SW_SHOW);
//		}
//
//	};
//
//}
//
//#endif //___MODELESS_DIALOG_WINDOW_H___
//
