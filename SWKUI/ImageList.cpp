#include "pch.h"
#include"DeviceContext.h"
#include "ImageList.h"

namespace swktool {
	BOOL ImageList::Draw(int ImageIndex, const DeviceContext& DC, int x, int y, UINT fStyle) 
	{
		return ImageList_Draw(hImageList_, ImageIndex, DC.GetHandle(), x, y, fStyle);	
	}

	BOOL ImageList::DrawEx(int ImageIndex, const DeviceContext& DC, int x, int y, int Width, int Height, COLORREF BKColor, COLORREF FGColor, UINT fStyle)
	{
		return ImageList_DrawEx(hImageList_, ImageIndex, DC.GetHandle(), x, y, Width, Height, BKColor, FGColor, fStyle);
	}	
}