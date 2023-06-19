//#pragma once
//
//#include <windows.h>
//#include <vector>
//#include <mutex>
//#include <algorithm>
//#include <string>
//#include <sstream>
//#include <memory>
//
//#include "AWindow.h"
//#include "DialogWindow.h"
//#include "Window.h"
//#include "Window.h"
//
//#ifdef _DEBUG
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
//#else
//#define DBG_NEW new
//#endif
//
//
//namespace swktool {	
//
//	class DialogWindow;
//	class MDIChildWindow;
//	class MDIFrameWindow;
//
//	typedef std::vector<std::shared_ptr<Window>> WinMsgProcList;
//	typedef WinMsgProcList::iterator WinMsgProcListItr;
//
//	typedef std::vector<std::shared_ptr<DialogWindow>> DlgMsgProcList;
//	typedef DlgMsgProcList::iterator DlgMsgProcListItr;
//
//	//typedef std::shared_ptr<BaseDialog>  ModelessDlgPtr;
//	typedef std::vector<std::shared_ptr<DialogWindow>>  ModelessDlgMsgProcList;
//	typedef ModelessDlgMsgProcList::iterator ModlessDlgMsgProcListItr;
//
//	typedef std::vector<std::shared_ptr<MDIFrameWindow>> WinMDIFrameMsgProcList;
//	typedef WinMDIFrameMsgProcList::iterator WinMDIFrameMsgProcListItr;
//
//	typedef std::vector<std::shared_ptr<MDIChildWindow>> WinMDIChildMsgProcList;
//	typedef WinMDIChildMsgProcList::iterator WinMDIChildMsgProcListItr;
//
//	class WinEventHandler
//	{
//		static WinEventHandler* pInst_;
//		
//		static bool	bDebug_;		
//
//		// Preregistered Window, used for Windows Create Dialog box creation where Windows API will not return Handle of dialog box to the caller,
//		// but still sends the message using the dialog handle.  The handling data structure isn't ready but the call back function still gets the message
//		// with unidentified dialog box handle
//		static std::shared_ptr<DialogWindow>  pPreRegistered;
//		static std::shared_ptr<Window>	pPreRegisteredWindow;
//		static std::shared_ptr<DialogWindow>	pPreRegisteredModelessDlg;
//		static std::shared_ptr<MDIChildWindow>	pPreRegisteredMDIChild;
//		static std::shared_ptr<MDIFrameWindow>	pPreRegisteredMDIFrame;
//
//	private:
//		// List of keeping track of all the Standard Windows
//		WinMsgProcList   MessageProcessors;
//
//		// List of processors for all the Dialog boxes
//		DlgMsgProcList   DialogProcessors;
//
//		// 
//		ModelessDlgMsgProcList ModelessDlagProcessors;
//
//		WinMDIFrameMsgProcList MDIFrameProcessors_;
//
//		WinMDIChildMsgProcList MDIChildProcessors_;
//
//
//
//		static std::mutex EventLockWindow;
//		static std::mutex EventLockDialog;
//		static std::mutex EventLockModelessDialog;
//
//		WinEventHandler() { ; }
//		~WinEventHandler() { ; };
//
//	protected:
//		static std::shared_ptr<Window> FindWindowByHandle(HWND hWindow);
//
//		static std::shared_ptr<DialogWindow> FindDialogByHandle(HWND hWindow);
//		static std::shared_ptr<DialogWindow> FindModelessDialogByHandle(HWND hDlg);
//
//		// MDI functions
//		static std::shared_ptr<MDIChildWindow> FindMDIChildByHandle(HWND hDlg);
//		static std::shared_ptr<MDIFrameWindow> FindMDIFrameByHandle(HWND hDlg);
//
//
//	public:
//		static WinEventHandler* GetInst();
//
//		WinMsgProcList& GetMsgProcessorList() {
//			return MessageProcessors;
//		}
//
//		DlgMsgProcList& GetDlgMsgProcessorList() {
//			return DialogProcessors;
//		}
//
//		ModelessDlgMsgProcList& GetModelessDlgMsgProcessorList() {
//			return ModelessDlagProcessors;
//		}
//
//
//		WinMDIChildMsgProcList& GetMDIChildProcessorList() {
//			return MDIChildProcessors_;
//		}
//
//		WinMDIFrameMsgProcList& GetMDIFrameProcessorList() {
//			return MDIFrameProcessors_;
//		}
//
//		std::shared_ptr<DialogWindow>  GetPreRegistedDialog();
//		std::shared_ptr<DialogWindow>  GetPreRegistedModelessDialog();		
//		std::shared_ptr<MDIChildWindow>  GetPreRegistedMDIClient();
//
//		void DbgShowChainedDlgHandles();
//
//
//
//		static void Destroy() {
//			pPreRegistered = nullptr;
//			pPreRegisteredWindow = nullptr;
//			pPreRegisteredModelessDlg = nullptr;
//
//			delete pInst_;
//			pInst_ = nullptr;
//		}
//
//	public:
//		static LRESULT ProcWindowMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
//		static  INT_PTR CALLBACK ProcDialogMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
//		static  INT_PTR CALLBACK ProcModelessDlgMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
//
//		static LRESULT ProcMDIFrameMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
//		static LRESULT ProcMDIChildMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
//
//		static BOOL       IsDialogMessage(MSG* pMsg);
//		
//
//		// Register Window to receive the message
//		void RegisterWindow(std::shared_ptr<Window> Window);
//
//		// Register Dialog Window to receive the message
//		void RegisterDlgWindow(std::shared_ptr<DialogWindow> Window);
//
//		void RegisterMDIChildWindow(std::shared_ptr<MDIChildWindow> Window);
//		void RegisterMDIFrameWindow(std::shared_ptr<MDIFrameWindow> pWindow);
//
//		// Unregister Window to stop receiving message
//		void UnRegisterWindow(HWND hWnd);
//		void UnRegisterModelessDlgWindow(HWND hWnd);
//		void UnRegisterDlgWindow(HWND hWnd);
//		void UnRegisterMDIChildWindow(HWND hWnd);
//		void UnRegisterMDIFrameWindow(HWND hWnd);
//
//		// Chicken and egg proble, let the egg preregister the action so the chicken can take action without gettig fully created
//		void PreRegisterDlgWindow(std::shared_ptr<DialogWindow> pDialog);
//		void PreRegisterModelessDlgWindow(std::shared_ptr<DialogWindow> pDialog);
//		void PreRegisterWindow(std::shared_ptr<Window> pDialog);
//
//		void PreRegisterMDIChildWindow(std::shared_ptr<MDIChildWindow> pDialog);
//		void PreRegisterMDIFramedWindow(std::shared_ptr<MDIFrameWindow> pDialog);
//	};
//
//
//}