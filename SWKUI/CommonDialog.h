
#pragma once


#include <Windows.h>
#include <string>
#include <vector>


namespace swktool
{
	template<typename T>
	class CommonDialogBase
	{
	protected:
		T data_{};
		CommonDialogBase()
		{
			data_.lStructSize = sizeof(T);
		}
	};


	struct FileFilter
	{
		std::wstring Description;
		std::wstring Filter;

		FileFilter(std::wstring d, std::wstring f) :
			Description(d),
			Filter(f) {
			;
		}
		FileFilter(const FileFilter& Other) noexcept = default;
		FileFilter(FileFilter&&) noexcept = default;
	};



	class FileOpenDlg :
		public CommonDialogBase<OPENFILENAMEW>
	{
		std::wstring  FileName;
		std::wstring  FileTitle;

		std::vector<wchar_t> FilterList;

	public:
		// FileName and TitleName should least be 256 bytes of storage
		FileOpenDlg(HINSTANCE hInst, HWND hParent, DWORD nFlags = OFN_FILEMUSTEXIST) :
			CommonDialogBase<OPENFILENAMEW>()
		{
			FileName.resize(MAX_PATH);
			FileTitle.resize(MAX_PATH);

			data_.hwndOwner = hParent;
			data_.nMaxFile = MAX_PATH;
			data_.nMaxFileTitle = MAX_PATH;

			data_.hInstance = hInst;
			data_.lpstrFile = (LPWSTR)FileName.data();
			data_.lpstrFileTitle = (LPWSTR)FileTitle.data();
			data_.lpstrFilter = nullptr;
			data_.Flags = nFlags;
			data_.lpstrInitialDir = nullptr;
		}

		BOOL ShowDialog()
		{
			return GetOpenFileName(&data_);
		}

		void SetDefaultExtension(LPCTSTR szExtension)
		{
			data_.lpstrDefExt = (LPCWSTR)szExtension;
		}
		void SetDefaultExtension(const wchar_t* ext) 
		{ 
			data_.lpstrDefExt = ext; 
		}

		void SetFilter(const TCHAR* szFilter)
		{
			if (!szFilter) 
			{
				data_.lpstrFilter = nullptr;
				return;
			}

			FilterList.clear();

			// Copy until we hit the double-null terminator
			const TCHAR* p = szFilter;
			while (true)
			{
				FilterList.push_back(*p);

				if (*p == '\0' && *(p + 1) == '\0')
				{
					FilterList.push_back('\0'); // second null terminator
					break;
				}

				++p;
			}

			data_.lpstrFilter = FilterList.data();
		}



		void SetFilter(std::vector<FileFilter>& szFilter) 
		{
			// pre-reserve space to save time
			const size_t rough_size = 80 * szFilter.size();
			FilterList.clear();
			FilterList.reserve(rough_size);

			// Create string of items with null in between, then terminate with double null
			for (FileFilter& Item : szFilter) {
				FilterList.insert(FilterList.end(), Item.Description.begin(), Item.Description.end());
				FilterList.push_back(L'\0');
				FilterList.insert(FilterList.end(), Item.Filter.begin(), Item.Filter.end());
				FilterList.push_back(L'\0');
			}
			// terminate stream with another null
			FilterList.push_back(L'\0');

			data_.lpstrFilter = &FilterList[0];
		}

		const std::wstring& GetFile() const 
		{
			return FileName;
		}

		void SetFile(LPCTSTR pFileName)
		{
			wcsncpy_s(FileName.data(), FileName.size(), pFileName, _TRUNCATE);
		}


		const std::wstring& GetFileTitle() const 
		{
			return FileTitle;
		}

		void SetFileTitle(LPCTSTR pFileTitle) 
		{
			wcsncpy_s(FileTitle.data(), FileTitle.size(), pFileTitle, _TRUNCATE);
		}
	};





	class FileSaveDlg :
		public CommonDialogBase<OPENFILENAMEW>
	{
		std::wstring  FileName;
		std::wstring  FileTitle;

		std::vector<wchar_t> FilterList;
	public:
		FileSaveDlg(HINSTANCE hInst, HWND hParent, DWORD nFlags = OFN_OVERWRITEPROMPT) :
			CommonDialogBase<OPENFILENAMEW>()
		{
			data_.hwndOwner = hParent;
			data_.nMaxFile = MAX_PATH;
			data_.nMaxFileTitle = MAX_PATH;

			FileName.resize(MAX_PATH);
			FileTitle.resize(MAX_PATH);

			data_.hInstance = hInst;
			data_.lpstrFile = (LPWSTR)FileName.data();
			data_.lpstrFileTitle = (LPWSTR)FileTitle.data();
			data_.Flags = nFlags;
		}


		BOOL ShowDialog()
		{
			return GetSaveFileName(&data_);
		}

		void SetDefaultExtension(const std::wstring& sExtension)
		{
			data_.lpstrDefExt = (LPWSTR)sExtension.c_str();
		}

		void SetDefaultExtension(const wchar_t* ext) 
		{ 
			data_.lpstrDefExt = ext; 
		}

		void SetFilter(const TCHAR* szFilter)
		{
			if (!szFilter) 
			{
				data_.lpstrFilter = nullptr;
				return;
			}

			FilterList.clear();

			// Copy until we hit the double-null terminator
			const TCHAR* p = szFilter;
			while (true)
			{
				FilterList.push_back(*p);

				if (*p == '\0' && *(p + 1) == '\0')
				{
					FilterList.push_back('\0'); // second null terminator
					break;
				}

				++p;
			}

			data_.lpstrFilter = FilterList.data();
		}

		void SetFilter(std::vector<FileFilter>& szFilter)
		{

			// pre-reserve space to save time
			const size_t rough_size = 80 * szFilter.size();
			FilterList.clear();
			FilterList.reserve(rough_size);

			// Create string of items with null in between, then terminate with double null
			for (FileFilter& Item : szFilter) {
				FilterList.insert(FilterList.end(), Item.Description.begin(), Item.Description.end());
				FilterList.push_back(L'\0');
				FilterList.insert(FilterList.end(), Item.Filter.begin(), Item.Filter.end());
				FilterList.push_back(L'\0');
			}
			// terminate stream with another null
			FilterList.push_back(L'\0');

			data_.lpstrFilter = &FilterList[0];
		}

		const std::wstring& GetFile() const
		{
			return FileName;
		}

		void SetFile(LPCTSTR pFileName)
		{
			wcsncpy_s(FileName.data(), FileName.size(), pFileName, _TRUNCATE);
		}

		const std::wstring& GetFileTitle() const
		{
			return FileTitle;
		}

		void SetFileTitle(LPCTSTR pFileTitle)
		{
			wcsncpy_s(FileTitle.data(), FileTitle.size(), pFileTitle, _TRUNCATE);
		}
	};



	class FindTextDlg :
		public CommonDialogBase<FINDREPLACEW>
	{
		std::wstring findBuf_;
	public:
		FindTextDlg(HWND hParent, LPCTSTR szFindText) :
			CommonDialogBase<FINDREPLACEW>()
		{
			data_.hwndOwner = hParent;
			data_.wFindWhatLen = 256;
			data_.Flags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD;

			findBuf_ = szFindText;
			data_.lpstrFindWhat = findBuf_.data();
		}

		FindTextDlg(HWND hParent, std::wstring& FindText) :
			FindTextDlg(hParent, FindText.c_str()) {

		}

		HWND ShowDialog()
		{
			return FindText(&data_);
		}
	};



	class FindReplaceTextDlg :
		public CommonDialogBase<FINDREPLACEW>
	{
		std::wstring findBuf_;
		std::wstring replaceBuf_;

	public:
		FindReplaceTextDlg(HWND hParent, LPCTSTR szFindText, LPCTSTR szReplaceText, DWORD nFlags = FR_HIDEUPDOWN | FR_HIDEMATCHCASE | FR_HIDEWHOLEWORD) :
			CommonDialogBase<FINDREPLACEW>()
		{
			data_.hwndOwner = hParent;
			data_.Flags = nFlags;

			findBuf_ = szFindText;
			data_.lpstrFindWhat = (LPWSTR)findBuf_.data();
			replaceBuf_ = szReplaceText;
			data_.lpstrReplaceWith = (LPWSTR)replaceBuf_.data();
			data_.wFindWhatLen = 256;
			data_.wReplaceWithLen = 256;
		}


		HWND ShowDialog()
		{
			return ReplaceText(&data_);
		}
	};



	class ChooseFontDlg :
		public CommonDialogBase<CHOOSEFONTW>
	{
		LOGFONT lf_{};
	public:
		ChooseFontDlg(HWND hParent) :
			CommonDialogBase<CHOOSEFONTW>()
		{
			data_.hwndOwner = hParent;
			data_.Flags = CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS | CF_EFFECTS;
			data_.lpLogFont = &lf_;
		}


		BOOL ShowDialog()
		{
			return ChooseFont(&data_);
		}
	};



	class ChooseColorDlg :
		public CommonDialogBase<CHOOSECOLORW>
	{
		DWORD rgbCurrent = RGB(0, 0, 0);
		COLORREF acrCustClr[16] = {};
	public:
		ChooseColorDlg(HINSTANCE hInst, HWND hParent) :
			CommonDialogBase<CHOOSECOLORW>()
		{
			data_.Flags = CC_RGBINIT | CC_FULLOPEN;
			data_.hwndOwner = hParent;
			data_.hInstance = hInst;
			data_.rgbResult = rgbCurrent;
			data_.lpCustColors = &acrCustClr[0];
		}

		BOOL ShowDialog()
		{
			return ChooseColor(&data_);
		}

	};


	class PageSetupDlg :
		public CommonDialogBase<PAGESETUPDLGW>
	{

	public:
		PageSetupDlg(HWND hParent) :
			CommonDialogBase<PAGESETUPDLGW>()
		{
			data_.hwndOwner = hParent;
			data_.Flags = PSD_MARGINS | PSD_INTHOUSANDTHSOFINCHES;
		}

		BOOL ShowDialog()
		{
			return ::PageSetupDlg(&data_);
		}


	};
}
