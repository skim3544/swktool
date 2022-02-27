#pragma once

#define OEMRESOURCE
#include <Windows.h>


//#include <WinUser.h>

namespace swktool {

	class GDIObject {
	protected:
		HGDIOBJ  hGDIHandle_;


	public:
		GDIObject() : hGDIHandle_(nullptr){ ; }
		GDIObject(HGDIOBJ hGDI) : hGDIHandle_(hGDI)
		{
		}

		virtual ~GDIObject() {
			Destroy();
		}

		operator HGDIOBJ() const {
			return hGDIHandle_;
		}

		void Attach(HGDIOBJ hGDI) {
			hGDIHandle_ = hGDI;
		}

		HGDIOBJ Detach() {
			HGDIOBJ ReturningData = hGDIHandle_;
			hGDIHandle_ = nullptr;
			return ReturningData;
		}

		BOOL Destroy() {
			auto RetCode= DeleteObject(hGDIHandle_);
			hGDIHandle_ = nullptr;

			return RetCode;
		}
	};



	



	class Font : public GDIObject {
	public:
		Font(HGDIOBJ hGDI) : GDIObject(hGDI)
		{
		}


		operator HFONT() const {
			return (HFONT)hGDIHandle_;
		}
	};

	class Pen : public GDIObject {
	public:
		Pen(HGDIOBJ hGDI) : GDIObject(hGDI)
		{
		}
		operator HPEN() const {
			return (HPEN)hGDIHandle_;
		}

		void Create(int iStyle, int cWidth, COLORREF color) {
			Attach(::CreatePen(iStyle, cWidth, color));
		}
		void CreateIndirect(const LOGPEN* plpen) {
			Attach(::CreatePenIndirect(plpen));
		}

		void ExtCreatePen(
			DWORD          iPenStyle,
			DWORD          cWidth,
			const LOGBRUSH* plbrush,
			DWORD          cStyle,
			const DWORD* pstyle)
		{
			Attach(::ExtCreatePen(iPenStyle, cWidth, plbrush, cStyle, pstyle));
		}
	};

	class Region : public GDIObject {
		//operator HREGION() const {
		//	return (HREGION)hGDIHandle_;
		//}
	};
}
