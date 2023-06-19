//#ifndef ___COMMON_DIALOG_H___
//#define ___COMMON_DIALOG_H___
//
//#pragma once
//
//#include "pch.h"
//#include <string>
//#include <Commctrl.h>
//
//#include "Ctrl.h"
//
//
//
//
//namespace swktool {
//
//	struct FileFilter {
//		std::wstring Description;
//		std::wstring Filter;	
//
//		FileFilter(std::wstring d, std::wstring f) : Description(d), Filter(f)  { ; }
//		FileFilter(FileFilter& Other) noexcept {
//			Description = Other.Description;
//			Filter = Other.Filter;
//		}
//
//		FileFilter(FileFilter&& Other) noexcept {
//			Description = Other.Description;
//			Filter = Other.Filter;
//		}
//	};
//
//	class FileOpenDlg {
//		OPENFILENAME  ofn;
//
//		std::wstring  FileName;
//		std::wstring  FileTitle;
//
//		std::vector<wchar_t> FilterList;
//
//	public:
//		// FileName and TitleName should least be 256 bytes of storage
//		FileOpenDlg(HINSTANCE hInst, HWND hParent, DWORD nFlags = OFN_FILEMUSTEXIST) : ofn{}
//		{			
//			FileName.reserve(256);
//			FileTitle.reserve(256);
//
//			Init(hParent);
//			ofn.hInstance = hInst;
//			ofn.lpstrFile = (LPWSTR)FileName.c_str();
//			ofn.lpstrFileTitle = (LPWSTR)FileTitle.c_str();
//			ofn.lpstrFilter = nullptr;
//			ofn.Flags = nFlags;
//		}
//
//		BOOL ShowDialog()
//		{
//			return GetOpenFileName(&ofn);
//		}
//
//		void SetDefaultExtension(LPCTSTR szExtension) {
//			ofn.lpstrDefExt = (LPCWSTR)szExtension;
//		}
//
//		void SetFilter(TCHAR szFilter[]) {
//			ofn.lpstrFilter = &szFilter[0];
//		}
//
//		void SetFilter(std::vector<FileFilter>& szFilter) {
//
//			// pre-reserve space to save time
//			const size_t rough_size = 80 * szFilter.size();
//			FilterList.reserve(rough_size);
//
//			// Create string of items with null in between, then terminate with double null
//			for (FileFilter& Item : szFilter) {
//				FilterList.insert(FilterList.end(), Item.Description.begin(), Item.Description.end());
//				FilterList.push_back(L'\0');
//				FilterList.insert(FilterList.end(), Item.Filter.begin(), Item.Filter.end());
//				FilterList.push_back(L'\0');
//			}
//			// terminate stream with another null
//			FilterList.push_back(L'\0');						
//
//			ofn.lpstrFilter = &FilterList[0];
//		}
//
//		const std::wstring& GetFile() const {
//			return FileName;
//		}
//
//		void SetFile(LPCTSTR pFileName) {
//			FileName = pFileName;
//		}
//
//		const std::wstring& GetFileTitle() const {
//			return FileTitle;
//		}
//
//		void SetFileTitle(LPCTSTR pFileTitle) {
//			FileTitle = pFileTitle;
//		}
//
//
//
//	private:
//		void Init(HWND hParent)
//		{
//			ofn.lStructSize = sizeof(OPENFILENAME);
//			ofn.hwndOwner = hParent;
//			ofn.hInstance = nullptr;
//			ofn.nMaxFile = MAX_PATH;
//			ofn.nMaxFileTitle = MAX_PATH;
//		}
//	};
//
//
//
//
//
//	class FileSaveDlg {
//		OPENFILENAME  ofn;
//
//		std::wstring  FileName;
//		std::wstring  FileTitle;
//
//		std::vector<wchar_t> FilterList;
//	public:
//		FileSaveDlg(HINSTANCE hInst, HWND hParent, DWORD nFlags = OFN_OVERWRITEPROMPT) :
//			ofn{}
//		{
//			Init(hParent);
//
//			FileName.reserve(256);
//			FileTitle.reserve(256);
//
//			ofn.hInstance = hInst;
//			ofn.lpstrFile = (LPWSTR)FileName.c_str();
//			ofn.lpstrFileTitle = (LPWSTR)FileTitle.c_str();
//			ofn.Flags = nFlags;
//		}
//
//
//		BOOL ShowDialog()
//		{
//			return GetSaveFileName(&ofn);
//		}
//
//		void SetDefaultExtension(std::wstring sExtension) {
//			ofn.lpstrDefExt = (LPWSTR)sExtension.c_str();
//		}
//
//		void SetFilter(TCHAR szFilter[]) {
//			ofn.lpstrFilter = &szFilter[0];
//		}
//
//		void SetFilter(std::vector<FileFilter>& szFilter) {
//
//			// pre-reserve space to save time
//			const size_t rough_size = 80 * szFilter.size();
//			FilterList.reserve(rough_size);
//
//			// Create string of items with null in between, then terminate with double null
//			for (FileFilter& Item : szFilter) {
//				FilterList.insert(FilterList.end(), Item.Description.begin(), Item.Description.end());
//				FilterList.push_back(L'\0');
//				FilterList.insert(FilterList.end(), Item.Filter.begin(), Item.Filter.end());
//				FilterList.push_back(L'\0');
//			}
//			// terminate stream with another null
//			FilterList.push_back(L'\0');
//
//			ofn.lpstrFilter = &FilterList[0];
//		}
//
//		const std::wstring& GetFile() const {
//			return FileName;
//		}
//
//		void SetFile(LPCTSTR pFileName) {
//			FileName = pFileName;
//		}
//
//		const std::wstring& GetFileTitle() const {
//			return FileTitle;
//		}
//
//		void SetFileTitle(LPCTSTR pFileTitle) {
//			FileTitle = pFileTitle;
//		}
//
//	protected:
//		void Init(HWND hParent)
//		{
//			ofn.lStructSize = sizeof(OPENFILENAME);
//			ofn.hwndOwner = hParent;
//			ofn.hInstance = nullptr;
//			ofn.nMaxFile = MAX_PATH;
//			ofn.nMaxFileTitle = MAX_PATH;
//		}
//	};
//
//
//
//	class FindTextDlg {
//		FINDREPLACE fr;
//
//	public:
//		FindTextDlg(HWND hParent, LPCTSTR szFindText) :
//			fr{}
//		{
//			Init();
//			fr.hwndOwner = hParent;
//			fr.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;
//			fr.lpstrFindWhat = (LPWSTR)szFindText;
//		}
//
//		FindTextDlg(HWND hParent, std::wstring& FindText) :
//			FindTextDlg(hParent, FindText.c_str()) {
//
//		}
//
//		HWND ShowDialog()
//		{
//			return FindText(&fr);
//		}
//
//	private:
//		void Init() {
//			fr.lStructSize = sizeof(FINDREPLACE);
//			fr.wFindWhatLen = 256;
//		}
//	};
//
//
//
//	class FindReplaceTextDlg {
//		FINDREPLACE fr;
//
//	public:
//		FindReplaceTextDlg(HWND hParent, LPCTSTR szFindText, LPCTSTR szReplaceText, DWORD nFlags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD) : fr{}
//		{
//			Init();
//
//			fr.hwndOwner = hParent;
//			fr.Flags = nFlags;
//			fr.lpstrFindWhat = (LPWSTR)szFindText;
//			fr.lpstrReplaceWith = (LPWSTR)szReplaceText;
//			fr.wFindWhatLen = 256;
//			fr.wReplaceWithLen = 256;
//		}
//
//
//		HWND ShowDialog()
//		{
//			return ReplaceText(&fr);
//		}
//
//	private:
//		void Init() {
//			fr.lStructSize = sizeof(FINDREPLACE);
//			fr.wFindWhatLen = 256;
//		}
//	};
//
//
//
//	class ChooseFontDlg {
//		CHOOSEFONT cf;
//
//	public:
//		ChooseFontDlg(HWND hParent) : cf{}
//		{
//			Init();
//			cf.hwndOwner = hParent;
//			//cf.hInstance = hInst;
//			cf.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
//		}
//
//
//		BOOL ShowDialog()
//		{
//			return ChooseFont(&cf);
//		}
//
//	private:
//		void Init() {
//			cf.lStructSize = sizeof(CHOOSEFONT);
//		}
//	};
//
//
//
//	class ChooseColorDlg {
//		CHOOSECOLOR cc;
//		DWORD rgbCurrent;
//		COLORREF acrCustClr[16];
//	public:
//		ChooseColorDlg(HINSTANCE hInst, HWND hParent) : cc{}
//		{
//			Init();
//			cc.hwndOwner = hParent;
//			cc.hInstance = (HWND) hInst;
//			cc.rgbResult = rgbCurrent;
//			cc.lpCustColors = &acrCustClr[0];
//		}
//
//		BOOL ShowDialog()
//		{
//			return ChooseColor(&cc);
//		}
//	private:
//		void Init() {
//			cc.lStructSize = sizeof(CHOOSECOLOR);
//			cc.Flags = CC_RGBINIT | CC_FULLOPEN;
//
//		}
//	};
//
//
//	class PageSetupDlg {
//		PAGESETUPDLGW ps;
//
//	public:
//		PageSetupDlg(HWND hParent) : ps{}
//		{
//			Init();
//		}
//
//		BOOL ShowDialog()
//		{
//			return ::PageSetupDlg(&ps);
//		}
//	protected:
//		void Init() {
//			ps.lStructSize = sizeof(PAGESETUPDLGW);
//		}
//
//	};
//}
//
//#endif // ___COMMON_DIALOG_H___