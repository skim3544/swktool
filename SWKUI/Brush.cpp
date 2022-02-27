#include "pch.h"
#include "Brush.h"

namespace swktool {

	void Brush::CreatePattern(Bitmap& bmp) {
		Attach(CreatePatternBrush( (HBITMAP)bmp));
	}




}