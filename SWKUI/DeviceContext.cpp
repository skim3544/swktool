#include "pch.h"
#include <assert.h>
#include "AWindow.h"
#include "Window.h"
#include "Brush.h"
#include "DeviceContext.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {
	DeviceContext::~DeviceContext() {
		try {
			switch (ReleaseMethod)
			{
			case DC_CLEAN_METHOD::Release:
				::ReleaseDC(hWndOwner, hDC);
				break;
			case DC_CLEAN_METHOD::Delete:
				::DeleteDC(hDC);
				break;
			case DC_CLEAN_METHOD::Close:
				::CloseMetaFile(hDC);
				break;
			case DC_CLEAN_METHOD::None:
				break;
			}
		}
		catch (...) {}
	}


	std::unique_ptr<DeviceContext> DeviceContext::GetDCEx(HWND  hWindow, HRGN  hrgnClip, DWORD flags)
	{
		HDC hDC = ::GetDCEx(hWindow, hrgnClip, flags);
		return std::make_unique<DeviceContext>(hWindow, hDC, DC_CLEAN_METHOD::Release);		
	}

	std::unique_ptr<DeviceContext> DeviceContext::GetDCEx(AWindow* pWindow, HRGN  hrgnClip, DWORD flags)
	{
		HWND hWindow = pWindow->GetWindowHWND();

		return DeviceContext::GetDCEx(hWindow, hrgnClip, flags);
	}


	std::unique_ptr<DeviceContext> DeviceContext::GetDC(HWND hWindow)
	{
		HDC hDC = ::GetDC(hWindow);		
		return std::make_unique<DeviceContext>(hWindow, hDC, DC_CLEAN_METHOD::Release);
	}

	std::unique_ptr<DeviceContext> DeviceContext::GetDC(AWindow* pWindow) {
		HWND hWindow = pWindow->GetWindowHWND();

		return DeviceContext::GetDC(hWindow);
	}

	std::unique_ptr<DeviceContext> DeviceContext::GetWindowDC(HWND hWindow)
	{
		HDC hDC = ::GetWindowDC(hWindow);		
		return std::make_unique< DeviceContext>(hWindow, hDC, DC_CLEAN_METHOD::Release);
	}

	std::unique_ptr<DeviceContext> DeviceContext::GetWindowDC(AWindow* pWindow) {
		HWND hWindow = pWindow->GetWindowHWND();
		return DeviceContext::GetWindowDC(hWindow);
	}


	std::unique_ptr<DeviceContext> DeviceContext::CreateDC(LPCWSTR Driver, LPCWSTR Device, LPCWSTR szPort, const DEVMODEW* pDM)
	{
		HDC hDC = ::CreateDC(Driver, Device, szPort, pDM);		
		return std::make_unique<DeviceContext>(nullptr, hDC, DC_CLEAN_METHOD::Delete);
	}

	std::unique_ptr<DeviceContext> DeviceContext::CreateIC(LPCWSTR Driver, LPCWSTR Device, LPCWSTR szPort, const DEVMODEW* pDM)
	{
		HDC hDC = ::CreateIC(Driver, Device, szPort, pDM);		
		return std::make_unique<DeviceContext>(nullptr, hDC, DC_CLEAN_METHOD::None);
	}

	std::unique_ptr<DeviceContext> DeviceContext::CreateCompatibleDC(const DeviceContext& dc)
	{
		HDC hDC = ::CreateCompatibleDC(dc.hDC);
		return std::make_unique<DeviceContext>(nullptr, hDC, DC_CLEAN_METHOD::Delete);
	}

	std::unique_ptr<DeviceContext> DeviceContext::CreateMetaFile(LPCWSTR szFileName) {
		HDC hDC = ::CreateMetaFile(szFileName);		
		return std::make_unique<DeviceContext>(nullptr, hDC, DC_CLEAN_METHOD::Close);
	}

	Bitmap DeviceContext::SelectObject(Bitmap& oBitmap)
	{
		return Bitmap(::SelectObject(hDC, (HGDIOBJ)oBitmap));
	}

	Font DeviceContext::SelectObject(Font& oFont)
	{
		return Font(::SelectObject(hDC, (HGDIOBJ)oFont));
	}

	Pen DeviceContext::SelectObject(Pen& oPen)
	{
		return Pen(::SelectObject(hDC, (HGDIOBJ)oPen));
	}

	Brush DeviceContext::SelectObject(Brush& oBrush) {
		return Brush(::SelectObject(hDC, (HGDIOBJ)oBrush));
	}

	BOOL DeviceContext::MaskBlt(
		int     xDest,
		int     yDest,
		int     width,
		int     height,
		const DeviceContext& SourceDC,
		int     xSrc,
		int     ySrc,
		HBITMAP hbmMask,
		int     xMask,
		int     yMask,
		DWORD   rop)
	{
		return ::MaskBlt(
			hDC,
			xDest,
			yDest,
			width,
			height,
			SourceDC.GetHandle(),
			xSrc,
			ySrc,
			hbmMask,
			xMask,
			yMask,
			rop);
	}

	BOOL DeviceContext::MaskBlt(
		int     xDest,
		int     yDest,
		int     width,
		int     height,
		const DeviceContext& SourceDC,
		int     xSrc,
		int     ySrc,
		Bitmap& bmMask,
		int     xMask,
		int     yMask,
		DWORD   rop) {

		return ::MaskBlt(
			hDC,
			xDest,
			yDest,
			width,
			height,
			SourceDC.GetHandle(),
			xSrc,
			ySrc,
			(HBITMAP)bmMask,
			xMask,
			yMask,
			rop);
	}


	BOOL DeviceContext::PlgBlt(
		const POINT* lpPoint,
		HDC         hdcSrc,
		int         xSrc,
		int         ySrc,
		int         width,
		int         height,
		HBITMAP     hbmMask,
		int         xMask,
		int         yMask
	) {
		return ::PlgBlt(
			hDC,
			lpPoint,
			hdcSrc,
			xSrc,
			ySrc,
			width,
			height,
			hbmMask,
			xMask,
			yMask
		);
	}

	BOOL DeviceContext::PlgBlt(
		const POINT* lpPoint,
		const DeviceContext& SrcDC,
		int         xSrc,
		int         ySrc,
		int         width,
		int         height,
		HBITMAP     hbmMask,
		int         xMask,
		int         yMask
	)
	{
		return ::PlgBlt(
			hDC,
			lpPoint,
			SrcDC.GetHandle(),
			xSrc,
			ySrc,
			width,
			height,
			hbmMask,
			xMask,
			yMask
		);
	}



	UINT DeviceContext::SetDIBColorTable(
		UINT          iStart,
		UINT          cEntries,
		const RGBQUAD* prgbq) 
	{
		return ::SetDIBColorTable(hDC, iStart, cEntries, prgbq);
	}


	int DeviceContext::SetDIBits(
		Bitmap& BM,
		UINT             start,
		UINT             cLines,
		const VOID* lpBits,
		const BITMAPINFO* lpbmi,
		UINT             ColorUse)
	{
		return ::SetDIBits(hDC, (HBITMAP)BM, start, cLines, lpBits, lpbmi, ColorUse);
	}



	BOOL DeviceContext::StretchBlt(
		int   xDest,
		int   yDest,
		int   wDest,
		int   hDest,
		const DeviceContext& DCSrc,
		int   xSrc,
		int   ySrc,
		int   wSrc,
		int   hSrc,
		DWORD rop) {
		return ::StretchBlt(hDC, xDest, yDest, wDest, hDest,
			DCSrc.GetHandle(), xSrc, ySrc, wSrc, hSrc, rop);
	}
	
#pragma comment(lib, "Msimg32.lib")

	BOOL DeviceContext::TransparentBlt(
		int  xoriginDest, int  yoriginDest, int  wDest, int  hDest,
		const DeviceContext& SrcDC,	int  xoriginSrc,int  yoriginSrc, int  wSrc, 	int  hSrc,	UINT crTransparent ) {
		
		return ::TransparentBlt(hDC,xoriginDest,yoriginDest,wDest,hDest,
			SrcDC.GetHandle(),xoriginSrc,yoriginSrc,wSrc,hSrc,crTransparent);

	}

	BOOL DeviceContext::BitBlt(
		int   x,
		int   y,
		int   cx,
		int   cy,
		const DeviceContext& SrcDC,
		int   x1,
		int   y1,
		DWORD rop) {
		return ::BitBlt(hDC, x, y, cx, cy, SrcDC.GetHandle(), x1, y1, rop);
	}


	BOOL DeviceContext::AlphaBlend(
		int           xoriginDest,
		int           yoriginDest,
		int           wDest,
		int           hDest,
		const DeviceContext& SrcDC,
		int           xoriginSrc,
		int           yoriginSrc,
		int           wSrc,
		int           hSrc,
		BLENDFUNCTION ftn) 
	{

		return ::AlphaBlend(hDC, xoriginDest,
			yoriginDest,
			wDest,
			hDest,
			SrcDC.GetHandle(),
			xoriginSrc,
			yoriginSrc,
			wSrc,
			hSrc,
			ftn);

	}




	PaintDeviceContext::PaintDeviceContext(Window* pWin) :		
		DeviceContext(pWin ? pWin->GetWindowHandle() : nullptr, nullptr, DC_CLEAN_METHOD::None)
	{
		assert(pWin != nullptr);
		hDC = ::BeginPaint(hWndOwner, &ps);
	}

	PaintDeviceContext::PaintDeviceContext(DialogWindow* pWin) :
		DeviceContext(pWin ? pWin->GetWindowHandle() : nullptr, nullptr, DC_CLEAN_METHOD::None)
	{
		assert(pWin != nullptr);
		hDC = ::BeginPaint(hWndOwner, &ps);
	}



}