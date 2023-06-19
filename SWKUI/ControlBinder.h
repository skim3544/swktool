#pragma once


#include <list>
#include <memory>

#include "Controls.h"


namespace swktool {

	/// <summary>
	///  Binds dialog control with a control object
	///  or used to create the control with a control object
	/// </summary>
	class ControlBinder
	{
	public:
		typedef std::unique_ptr<Ctrl> TControlData;
		typedef std::list<TControlData> TControlList;
		typedef std::list<TControlData>::iterator TControlItr;

	private:
		TControlList ControlList_;

	public:
		// Binds Dialog resource control with the control
		template <class CtrlItem, class TParent = DialogWindow>
		CtrlItem* Bind(UINT ID, TParent* pParent) {
			std::unique_ptr<CtrlItem> data = std::make_unique<CtrlItem>(ID, pParent);
			ControlList_.push_back(std::move(data));
			return dynamic_cast<CtrlItem*>(ControlList_.rbegin()->get());
		}

		// Creates the Dialog Control dynamically
		template <class TParent, class CtrlItem>
		CtrlItem* Create(TParent* pParent, std::wstring Caption, DWORD dwStyle, int x, int y, int Height, int Width, UINT CtrlID)
		{
			std::unique_ptr<CtrlItem> data = std::make_unique<CtrlItem>(Caption, dwStyle, x, y, Height, Width, pParent, CtrlID);
			ControlList_.push_back(std::move(data));
			return dynamic_cast<CtrlItem*>(ControlList_.rbegin()->get());
		}


		/// <summary>
		/// Get the control by the Control ID
		/// The control must be either bind (control in Resource) or Create into ControlList_
		/// Before this function can be called
		/// </summary>
		/// <typeparam name="CtrlItem"></typeparam>
		/// <param name="ID"></param>
		/// <returns></returns>
		template <class CtrlItem>
		CtrlItem* Get(UINT ID) {

			Ctrl* pData = nullptr;

			// find it
			TControlItr it = std::find_if(
				ControlList_.begin(), ControlList_.end(), 
				[&ID](const std::unique_ptr<Ctrl>& Data)
				{ return (Data->GetID() == ID); });

			// set the pointer to return if found
			if (it != ControlList_.end())
				pData = it->get();

			return  dynamic_cast<CtrlItem*>(pData);
		}

		/// <summary>
		/// Gets the control pointer using the control hWnd
		/// The control must be either bind (control in Resource) or Create into ControlList_
		/// Before this function can be called
		/// </summary>
		/// <typeparam name="CtrlItem"></typeparam>
		/// <param name="hWnd"></param>
		/// <returns></returns>
		template <class CtrlItem>
		CtrlItem* Get(HWND hWnd) {

			Ctrl* pData = nullptr;

			// find it
			TControlItr it = std::find_if(
				ControlList_.begin(), ControlList_.end(),
				[&hWnd](const std::unique_ptr<Ctrl>& Data)
				{ return (Data->GetCtrlHandle() == hWnd); });

			// set the pointer to return if found
			if (it != ControlList_.end())
				pData = it->get();

			return  dynamic_cast<CtrlItem*>(pData);
		}

	};
}
