#pragma once

#include <vector>
#include "Window.h"
#include "WinRegistration.h"
//#include "WinEventHandler.h"
#include "WinAccelerator.h"
#include "..\SWKBase\Logger.h"

namespace swktool {

	typedef std::vector<WindowReg> WinRegistrationList;
	typedef std::vector<WindowReg>::iterator WinRegistrationListItr;

	
	class WindowFrame {
		
	protected:
		HINSTANCE hInstance = 0;
		WinRegistrationList WinRegList;

		swktool::ILogger* pLogger_ = nullptr;
	public:
		WindowFrame() : hInstance(nullptr)
		{ 
			; 
		}
		WindowFrame(HINSTANCE hInst) : 
			hInstance(hInst)
		{
			;
		}
		~WindowFrame() { ; }

		//void SetLogger(swktool::ILogger* p) {
		//	pLogger_ = p;
		//}

		ATOM Register(WNDCLASSEXW& sReg) {
			WindowReg  Reg(sReg);
			WinRegList.push_back(Reg);

			ATOM atom = RegisterClassExW(&sReg);

			return atom;
		}

		ATOM Register(WindowReg& Reg) {
			WNDCLASSEXW& Cls = Reg.GetWndClass();
			ATOM atom = RegisterClassExW(&Cls);

			WinRegList.push_back(Reg);
			return atom;
		}
		
		/// <summary>
		/// Template Virtualization
		/// Return type is Window, but the actual instiantiating object is shared_ptr<T>
		/// Where T is your own Window derived from swktool::Window
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="WinClassName"></param>
		/// <param name="WinTitle"></param>
		/// <param name="nCmdShow"></param>
		/// <returns></returns>
		template <typename T>
		std::shared_ptr<swktool::Window> TCreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow)
		{
			std::shared_ptr<swktool::Window> pWin = std::make_shared<T>(WinClassName, WinTitle, hInstance);

			if (pWin->Create(pWin)) {
				pWin->Show(nCmdShow);
			}

			return pWin;
		}

		
		std::shared_ptr<swktool::Window> CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow);
		std::shared_ptr<swktool::Window> CreateWin(LPCWSTR WinTitle, int nCmdShow);
		

		int DoMessageLoop();		
		int DoMDIMessageLoop(const std::shared_ptr<Window>& MDIFrame);
		int DoMessageLoop(swktool::WinAccelerator& accl);


	};







}