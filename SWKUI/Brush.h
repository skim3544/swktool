#pragma once

#include "GDIObject.h"




namespace swktool {

	typedef enum class BrushHatchTypes_ : int {
		BDIAGONAL = HS_BDIAGONAL,
		CROSS = HS_CROSS,
		DIAGCROSS = HS_DIAGCROSS,
		FDIAGONAL = HS_FDIAGONAL,
		HORIZONTAL = HS_HORIZONTAL,
		VERTICAL = HS_VERTICAL
	} BrushHatchTypes;


	class Brush : public GDIObject {
	public:
		Brush() { ; }
		Brush(HGDIOBJ hGDI) : GDIObject(hGDI)
		{
		}

		operator HBRUSH() const {
			return (HBRUSH)hGDIHandle_;
		}

		void CreateIndirect(const LOGBRUSH* plbrush)
		{
			Attach(::CreateBrushIndirect(plbrush));
		}

		void CreateSolid(COLORREF Color)
		{
			Attach(::CreateSolidBrush(Color));
		}

		void CreatePattern(Bitmap& bmp);
		void CreateHatch(BrushHatchTypes iHatch, COLORREF Color) {
			Attach(::CreateHatchBrush( (int)iHatch, Color));
		}

		void CreateFromSysColor(int ColorIndex) {
			Attach(::GetSysColorBrush(ColorIndex));
		}

		void CreateDIBPatternBrushPt(const VOID* lpPackedDIB, UINT iUsage) {
			Attach(::CreateDIBPatternBrushPt(lpPackedDIB, iUsage));
		}
	};
}
