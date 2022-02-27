#ifndef ___DEVICE_CONTEXT_H___
#define ___DEVICE_CONTEXT_H___

#include <Windows.h>
#include <string>
#include <memory>

#include "GDIObject.h"
#include "Bitmap.h"

namespace swktool {
	class AWindow;
	class Brush;

	enum class DC_CLEAN_METHOD {
		None = 0,
		Release,
		Delete,
		Close,
	};

	enum class DC_MAP_MODE : int {
		TEXT = MM_TEXT,
		LOMETRIC = MM_LOMETRIC,
		HIMETRIC = MM_HIMETRIC,
		LOENGLISH = MM_LOENGLISH,
		HIENGLISH = MM_HIENGLISH,
		TWIPS = MM_TWIPS,
		ISOTROPIC = MM_ISOTROPIC,
		ANISOTROPIC = MM_ANISOTROPIC
	};

	class DeviceContext {
	protected:
		HDC hDC;
		HWND hWndOwner;
		DC_CLEAN_METHOD ReleaseMethod;

	public:
		DeviceContext(HWND hWindow, HDC hdc, DC_CLEAN_METHOD RelMethod) :
			hDC(hdc), hWndOwner(hWindow), ReleaseMethod(RelMethod)
		{

		}

		// RAII, do not allow copy construct
		DeviceContext(const DeviceContext& Other) = delete;

		DeviceContext(DeviceContext&& Other) noexcept
		{
			hDC = Other.hDC;
			hWndOwner = Other.hWndOwner;
			ReleaseMethod = Other.ReleaseMethod;

			Other.hDC = nullptr;
			Other.hWndOwner = nullptr;
			Other.ReleaseMethod = DC_CLEAN_METHOD::None;;
		}

		virtual ~DeviceContext();

		HDC	GetHandle() const {
			return hDC;
		}


		HDC detach() {
			ReleaseMethod = DC_CLEAN_METHOD::None;
			HDC objDC = hDC;
			hDC = nullptr;
			hWndOwner = nullptr;

			return objDC;
		}


		// all the methods of getting device context object
		static std::unique_ptr<DeviceContext> GetDC(HWND hWindow);
		static std::unique_ptr<DeviceContext> GetDC(AWindow* pWindow);
		static std::unique_ptr<DeviceContext> GetDCEx(HWND  hWnd, HRGN  hrgnClip, DWORD flags);
		static std::unique_ptr<DeviceContext> GetDCEx(AWindow* pWindow, HRGN  hrgnClip, DWORD flags);
		static std::unique_ptr<DeviceContext> GetWindowDC(HWND hWindow);
		static std::unique_ptr<DeviceContext> GetWindowDC(AWindow* pWindow);
		static std::unique_ptr<DeviceContext> CreateDC(LPCWSTR Driver, LPCWSTR Device, LPCWSTR szPort, const DEVMODEW* pDM);
		static std::unique_ptr<DeviceContext> CreateIC(LPCWSTR Driver, LPCWSTR Device, LPCWSTR szPort, const DEVMODEW* pDM);
		static std::unique_ptr<DeviceContext> CreateCompatibleDC(const DeviceContext& dc);
		static std::unique_ptr<DeviceContext> CreateMetaFile(LPCWSTR szFileName);

		int Save() {
			return ::SaveDC(hDC);
		}

		BOOL Restore(int id_restore = -1)
		{
			return ::RestoreDC(hDC, id_restore);
		}

		BOOL CancelDC() {
			return ::CancelDC(hDC);
		}

		HWND WindowFromDC() {
			return ::WindowFromDC(hDC);
		}


		DWORD SetLayout(DWORD l) {
			return ::SetLayout(hDC, l);
		}

		COLORREF GetDCPenColor() {
			return ::GetDCPenColor(hDC);
		}

		COLORREF SetDCPenColor(COLORREF color) {
			return ::SetDCPenColor(hDC, color);
		}

		COLORREF SetPixel(int x, int y, COLORREF crColor) {
			return ::SetPixel(hDC, x, y, crColor);
		}

		COLORREF GetPixel(int  x, int y) {
			return ::GetPixel(hDC, x, y);
		}

		BOOL MoveToEx(int x, int y, LPPOINT lppt)
		{
			return ::MoveToEx(hDC, x, y, lppt);
		}

		BOOL LineTo(int x, int y)
		{
			return ::LineTo(hDC, x, y);
		}

		BOOL Polyline(const POINT* apt, int cpt)
		{
			return ::Polyline(hDC, apt, cpt);
		}

		BOOL PolylineTo(const POINT* apt, int cpt) {
			return ::PolylineTo(hDC, apt, cpt);
		}

		BOOL PolyBezier(const POINT* apt, int cpt) {
			return ::PolyBezier(hDC, apt, cpt);
		}

		BOOL PolyBezierTo(const POINT* apt, int cpt) {
			return ::PolyBezierTo(hDC, apt, cpt);
		}

		BOOL Rectangle(int Left, int Top, int Right, int Bottom)
		{
			return ::Rectangle(hDC, Left, Top, Right, Bottom);
		}

		BOOL Ellipse(int Left, int Top, int Right, int Bottom)
		{
			return ::Ellipse(hDC, Left, Top, Right, Bottom);
		}

		BOOL Arc(int x1, int  y1, int x2, int  y2, int  x3, int  y3, int  x4, int  y4)
		{
			return ::Arc(hDC, x1, y1, x2, y2, x3, y3, x4, y4);
		}

		BOOL Chord(int x1, int  y1, int x2, int  y2, int  x3, int  y3, int  x4, int  y4)
		{
			return ::Chord(hDC, x1, y1, x2, y2, x3, y3, x4, y4);
		}

		BOOL Pie(int x1, int  y1, int x2, int  y2, int  x3, int  y3, int  x4, int  y4)
		{
			return ::Pie(hDC, x1, y1, x2, y2, x3, y3, x4, y4);
		}

		HGDIOBJ SelectObject(HGDIOBJ h) {
			return ::SelectObject(hDC, h);
		}

		Bitmap SelectObject(Bitmap& oBitmap);
		Font SelectObject(Font& oFont);
		Pen SelectObject(Pen& oPen);
		Brush SelectObject(Brush& oBrush);

		COLORREF SetBkColor(COLORREF crColor) {
			return ::SetBkColor(hDC, crColor);
		}

		int SetBkMode(int mode) {
			return ::SetBkMode(hDC, mode);
		}

		int SetROP2(int iDrawMode) {
			return ::SetROP2(hDC, iDrawMode);
		}

		int GetROP2() {
			return ::GetROP2(hDC);
		}


		TEXTMETRIC GetTextMetrics() {
			TEXTMETRIC tm = { };
			::GetTextMetrics(hDC, &tm);
			return tm;
		}

		BOOL TextOut(int x, int y, std::wstring& s) {
			return ::TextOut(hDC, x, y, s.c_str(), (int)s.length());
		}

		BOOL TextOut(int x, int y, LPCWSTR s, int Len) {
			return ::TextOut(hDC, x, y, s, Len);
		}


		void DrawText(std::wstring text, RECT* pRect, UINT format) {
			::DrawText(hDC, text.c_str(), (int)text.length(), pRect, format);
		}

		UINT GetDeviceCaps(int index)
		{
			return ::GetDeviceCaps(hDC, index);
		}


		int GetStretchBltMode() {
			return ::GetStretchBltMode(hDC);
		}


		BOOL GradientFill(
			PTRIVERTEX pVertex,
			ULONG      nVertex,
			PVOID      pMesh,
			ULONG      nMesh,
			ULONG      ulMode) {
			return ::GradientFill(hDC, pVertex, nVertex, pMesh, nMesh, ulMode);
		}

		BOOL MaskBlt(
			HDC     hdcDest,
			int     xDest,
			int     yDest,
			int     width,
			int     height,
			HDC     hdcSrc,
			int     xSrc,
			int     ySrc,
			HBITMAP hbmMask,
			int     xMask,
			int     yMask,
			DWORD   rop)
		{
			return ::MaskBlt(
				hdcDest,
				xDest,
				yDest,
				width,
				height,
				hdcSrc,
				xSrc,
				ySrc,
				hbmMask,
				xMask,
				yMask,
				rop);
		}


		BOOL MaskBlt(
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
			DWORD   rop);

		BOOL MaskBlt(
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
			DWORD   rop);

		BOOL PlgBlt(
			HDC         hdcDest,
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
				hdcDest,
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
		};


		BOOL PlgBlt(
			const POINT* lpPoint,
			HDC         hdcSrc,
			int         xSrc,
			int         ySrc,
			int         width,
			int         height,
			HBITMAP     hbmMask,
			int         xMask,
			int         yMask
		);

		BOOL PlgBlt(
			const POINT* lpPoint,
			const DeviceContext& SrcDC,
			int         xSrc,
			int         ySrc,
			int         width,
			int         height,
			HBITMAP     hbmMask,
			int         xMask,
			int         yMask
		);

		UINT SetDIBColorTable(
			UINT          iStart,
			UINT          cEntries,
			const RGBQUAD* prgbq
		);

		int SetDIBits(
			HBITMAP hBM,
			UINT             start,
			UINT             cLines,
			const VOID* lpBits,
			const BITMAPINFO* lpbmi,
			UINT             ColorUse)
		{
			return ::SetDIBits(hDC, hBM, start, cLines, lpBits, lpbmi, ColorUse);
		}

		int SetDIBits(
			Bitmap& BM,
			UINT             start,
			UINT             cLines,
			const VOID* lpBits,
			const BITMAPINFO* lpbmi,
			UINT             ColorUse
		);

		int SetDIBitsToDevice(
			HDC              hdc,
			int              xDest,
			int              yDest,
			DWORD            w,
			DWORD            h,
			int              xSrc,
			int              ySrc,
			UINT             StartScan,
			UINT             cLines,
			const VOID* lpvBits,
			const BITMAPINFO* lpbmi,
			UINT             ColorUse) {
			return ::SetDIBitsToDevice(
				hDC,
				xDest,
				yDest,
				w,
				h,
				xSrc,
				ySrc,
				StartScan,
				cLines,
				lpvBits,
				lpbmi,
				ColorUse);
		}

		COLORREF SetPixelIV(int x, int y, COLORREF color) {
			return ::SetPixelV(hDC, x, y, color);
		}

		int SetStretchBltMode(int mode) {
			return ::SetStretchBltMode(hDC, mode);
		}

		BOOL StretchBlt(
			int   xDest,
			int   yDest,
			int   wDest,
			int   hDest,
			HDC   hdcSrc,
			int   xSrc,
			int   ySrc,
			int   wSrc,
			int   hSrc,
			DWORD rop) {
			return ::StretchBlt(hDC, xDest, yDest, wDest, hDest,
				hdcSrc, xSrc, ySrc, wSrc, hSrc, rop);
		}

		BOOL StretchBlt(
			int   xDest,
			int   yDest,
			int   wDest,
			int   hDest,
			const DeviceContext& DCSrc,
			int   xSrc,
			int   ySrc,
			int   wSrc,
			int   hSrc,
			DWORD rop);

		int StretchDIBits(
			int              xDest,
			int              yDest,
			int              DestWidth,
			int              DestHeight,
			int              xSrc,
			int              ySrc,
			int              SrcWidth,
			int              SrcHeight,
			const VOID* lpBits,
			const BITMAPINFO* lpbmi,
			UINT             iUsage,
			DWORD            rop)
		{
			return ::StretchDIBits(hDC, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop);
		}

		//BOOL TransparentBlt(int  xoriginDest, int  yoriginDest, int  wDest, int  hDest,
		//	HDC  hdcSrc,int  xoriginSrc,int  yoriginSrc,int  wSrc,	int  hSrc,	UINT crTransparent) 
		//{
		//	return ::TransparentBlt(hDC, xoriginDest, yoriginDest,wDest, hDest, hdcSrc,	xoriginSrc, yoriginSrc,	wSrc, hSrc,	crTransparent);
		//}


		BOOL TransparentBlt(int xoriginDest, int  yoriginDest, int  wDest, int  hDest, const DeviceContext& hdcSrc,
			int  xoriginSrc, int  yoriginSrc, int  wSrc, int  hSrc, UINT crTransparent);

		BOOL BitBlt(int   x, int   y, int   cx, int   cy, HDC   hdcSrc, int   x1, int   y1, DWORD rop)
		{
			return ::BitBlt(hDC, x, y, cx, cy, hdcSrc, x1, y1, rop);
		}


		BOOL BitBlt(
			int   x,
			int   y,
			int   cx,
			int   cy,
			const DeviceContext& SrcDC,
			int   x1,
			int   y1,
			DWORD rop);

		BOOL AlphaBlend(
			int           xoriginDest,
			int           yoriginDest,
			int           wDest,
			int           hDest,
			HDC           hdcSrc,
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
				hdcSrc,
				xoriginSrc,
				yoriginSrc,
				wSrc,
				hSrc,
				ftn);
		}

		BOOL AlphaBlend(
			int           xoriginDest,
			int           yoriginDest,
			int           wDest,
			int           hDest,
			const DeviceContext& SrcDC,
			int           xoriginSrc,
			int           yoriginSrc,
			int           wSrc,
			int           hSrc,
			BLENDFUNCTION ftn);

		BOOL GetBrushOrgEx(LPPOINT lppt) {
			return ::GetBrushOrgEx(hDC, lppt);
		}

		BOOL SetBrushOrgEx(int x, int y, LPPOINT lppt) {
			return ::SetBrushOrgEx(hDC, x, y, lppt);
		}

		COLORREF SetDCBrushColor(COLORREF color) {
			return ::SetDCBrushColor(hDC, color);
		}

		void SetMapMode(DC_MAP_MODE MapMode) {
			::SetMapMode(hDC, (int)MapMode);
		}

		DC_MAP_MODE GetMapMode() {
			DC_MAP_MODE MapMode = (DC_MAP_MODE)::GetMapMode(hDC);
			return MapMode;
		}

		BOOL DPtoLP(PPOINT Data, int nCount) {
			return ::DPtoLP(hDC, Data, nCount);
		}
		BOOL LPtoDP(PPOINT Data, int nCount) {
			return ::LPtoDP(hDC, Data, nCount);
		}

		BOOL SetViewportOrgEx(int x, int y, LPPOINT lppt) {
			return ::SetViewportOrgEx(hDC, x, y, lppt);
		}

		BOOL SetWindowOrgEx(int x, int y, LPPOINT lppt) {
			return ::SetWindowOrgEx(hDC, x, y, lppt);
		}

		BOOL SetViewportExtEx(int x, int y, LPSIZE lpsz) {
			return ::SetViewportExtEx(hDC, x, y, lpsz);
		}

		BOOL SetWindowExtEx(int x, int y, LPSIZE lpsz) {
			return ::SetWindowExtEx(hDC, x, y, lpsz);
		}

		COLORREF SetTextColor(COLORREF oColor) {
			return ::SetTextColor(hDC, oColor);
		}


	};




	/// <summary>
	/// RAII, BeginPaint at creation and EndPaint at destruction
	/// Also inherited from DeviceContext so offers everything that you can do with DeviceContext
	/// </summary>
	class PaintDeviceContext : public DeviceContext {
		PAINTSTRUCT ps = {};

	public:
		PaintDeviceContext(HWND hWnd) : DeviceContext(hWnd, nullptr, DC_CLEAN_METHOD::None)
		{
			hDC = ::BeginPaint(hWndOwner, &ps);
		}

		PaintDeviceContext(AWindow* pWin);


		~PaintDeviceContext() {
			::EndPaint(hWndOwner, &ps);
		}
	};


}

#endif //___DEVICE_CONTEXT_H___