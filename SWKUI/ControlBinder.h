#pragma once


#include <list>
#include <memory>
#include <vector>
#include <assert.h>

#include "Theme.h"
#include "Controls.h"
#include "Ctrl.h"


namespace swktool {

	/// <summary>
	///  Binds dialog control with a control object
	///  or used to create the control with a control object
	/// </summary>
	class ControlBinder
	{
	public:
		using TControlData = std::unique_ptr<Ctrl>;
		using TControlList = std::vector<TControlData>;
		using TControlItr = TControlList::iterator;
		using TControlConstItr = TControlList::const_iterator;


	private:
		TControlList ControlList_;


	public:
		/// <summary>
		///  Adds control to the binder
		/// </summary>
		/// <param name="ctrl"></param>
		void Add(std::unique_ptr<Ctrl> ctrl)
		{
			// set backward pointer, set binder pointer (this) to the control
			ctrl->SetBinder(this);
			ControlList_.push_back(std::move(ctrl));
		}


		// Binds Dialog resource control with the control
		template <class CtrlItem, class TParent = IWindow>
		CtrlItem* Bind(UINT ID, TParent* pParent) 
		{
			static_assert(std::is_base_of_v<Ctrl, CtrlItem>, "CtrlItem must derive from Ctrl");
			static_assert(std::is_base_of_v<IWindow, TParent>, "TParent must derive from IWindow");

			auto ctrl = std::make_unique<CtrlItem>(ID, pParent);
			if (ctrl->GetCtrlHandle() == nullptr)
				return nullptr;
			
			ctrl->SetBinder(this);
			CtrlItem* raw = ctrl.get(); 
			ControlList_.push_back(std::move(ctrl)); 
			return raw;
		}

		// Creates the Dialog Control dynamically
		template <class TParent, class CtrlItem>
		CtrlItem* Create(TParent* pParent, std::wstring Caption, DWORD dwStyle, int x, int y, int Height, int Width, UINT CtrlID)
		{
			static_assert(std::is_base_of_v<Ctrl, CtrlItem>, "CtrlItem must derive from Ctrl");
			static_assert(std::is_base_of_v<IWindow, TParent>, "TParent must derive from IWindow");

			std::unique_ptr<CtrlItem> data = std::make_unique<CtrlItem>(Caption, dwStyle, x, y, Height, Width, pParent, CtrlID);
			data->SetBinder(this);
			CtrlItem* raw = data.get();

			ControlList_.push_back(std::move(data));
			return raw;
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
		CtrlItem* Get(UINT ID)
		{
			auto it = std::find_if(ControlList_.begin(), ControlList_.end(),
				[ID](const auto& ctrl) { return ctrl->GetID() == ID; });

			return (it != ControlList_.end())
				? dynamic_cast<CtrlItem*>(it->get())
				: nullptr;
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
			auto it = std::find_if(ControlList_.begin(), ControlList_.end(),
				[hWnd](const auto& ctrl) { return ctrl->GetCtrlHandle() == hWnd; });

			return (it != ControlList_.end())
				? dynamic_cast<CtrlItem*>(it->get())
				: nullptr;
		}

		// Remove by Control ID
		bool Remove(UINT ID)
		{
			auto it = std::find_if(ControlList_.begin(), ControlList_.end(),
				[ID](const auto& ctrl) { return ctrl->GetID() == ID; });

			if (it != ControlList_.end())
			{
				ControlList_.erase(it);
				return true;
			}
			return false;
		}

		// Remove by HWND
		bool Remove(HWND hWnd)
		{
			auto it = std::find_if(ControlList_.begin(), ControlList_.end(),
				[hWnd](const auto& ctrl) { return ctrl->GetCtrlHandle() == hWnd; });

			if (it != ControlList_.end())
			{
				ControlList_.erase(it);
				return true;
			}
			return false;
		}

		bool Remove(Ctrl* ctrl)
		{
			auto it = std::find_if(ControlList_.begin(), ControlList_.end(),
				[ctrl](const auto& ptr) { return ptr.get() == ctrl; });

			if (it != ControlList_.end()) {
				ControlList_.erase(it);
				return true;
			}
			return false;
		}

		/// <summary>
		/// Returns collection of one specific type for group processing
		/// Example:
		/// 
		/// for (auto edit : binder.GetAllOfType<EditCtrl>())
		//		edit->SetText(L"");
		/// </summary>
		/// <typeparam name="CtrlItem"></typeparam>
		/// <returns></returns>
		template <class CtrlItem>
		std::vector<CtrlItem*> GetAllOfType()
		{
			static_assert(std::is_base_of_v<Ctrl, CtrlItem>, "CtrlItem must derive from Ctrl");

			std::vector<CtrlItem*> out;
			for (auto& c : ControlList_)
				if (auto casted = dynamic_cast<CtrlItem*>(c.get()))
					out.push_back(casted);
			return out;
		}

		/// <summary>
		/// Go through all the list, set theme
		/// </summary>
		/// <param name="theme"></param>
		void PropagateTheme(const Theme& theme)
		{
			auto size = ControlList_.size();
			for (auto& ctrl : ControlList_) {
				assert(ctrl != nullptr);
				assert(IsWindow(ctrl->GetCtrlHandle()));
				ctrl->ApplyTheme(theme);
			}
		}

		auto begin() { return ControlList_.begin(); }
		auto end() { return ControlList_.end(); }


	};
}
