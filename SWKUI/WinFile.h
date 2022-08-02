#pragma once

#include <Windows.h>
#include <string>


namespace swktool {

	typedef enum class _FileDesiredAccess : DWORD {
		ALL = GENERIC_ALL,
		EXECUTE = GENERIC_EXECUTE,
		WRITE = GENERIC_WRITE,
		READ = GENERIC_READ
	} FileDesiredAccess;

	DEFINE_ENUM_FLAG_OPERATORS(FileDesiredAccess);


	typedef enum class _FileShareMode : DWORD {
		NONE = 0,
		REMOVE = FILE_SHARE_DELETE,
		READ = FILE_SHARE_READ,
		WRITE = FILE_SHARE_WRITE
	} FileShareMode;

	DEFINE_ENUM_FLAG_OPERATORS(FileShareMode);



	typedef enum class _CreationDisposition : DWORD {
		CreateNew = CREATE_NEW,
		AlwaysCreate = CREATE_ALWAYS,
		OpenExisting = OPEN_EXISTING,
		OpenAlways = OPEN_ALWAYS,
		TruncateExisting = TRUNCATE_EXISTING
	} CreationDisposition;



	typedef enum class _FileAttribute : DWORD {
		ReadOnly = FILE_ATTRIBUTE_READONLY,
		Hidden = FILE_ATTRIBUTE_HIDDEN,
		System = FILE_ATTRIBUTE_SYSTEM,
		Directory = FILE_ATTRIBUTE_DIRECTORY,
		Archive = FILE_ATTRIBUTE_ARCHIVE,
		Device = FILE_ATTRIBUTE_DEVICE,
		Normal = FILE_ATTRIBUTE_NORMAL,
		Temporary = FILE_ATTRIBUTE_TEMPORARY,
		SparseFile = FILE_ATTRIBUTE_SPARSE_FILE,
		ReparsePoint = FILE_ATTRIBUTE_REPARSE_POINT,
		Compressed = FILE_ATTRIBUTE_COMPRESSED,
		Offline = FILE_ATTRIBUTE_OFFLINE,
		NotContentIndexed = FILE_ATTRIBUTE_NOT_CONTENT_INDEXED,
		ERncrypted = FILE_ATTRIBUTE_ENCRYPTED,
		IntegrityStream = FILE_ATTRIBUTE_INTEGRITY_STREAM,
		Virtual = FILE_ATTRIBUTE_VIRTUAL,
		NoScrubData = FILE_ATTRIBUTE_NO_SCRUB_DATA,
		EA = FILE_ATTRIBUTE_EA,
		Pinned = FILE_ATTRIBUTE_PINNED,
		Unpinned = FILE_ATTRIBUTE_UNPINNED,
		RecallOnOpen = FILE_ATTRIBUTE_RECALL_ON_OPEN,
		RecallonAccess = FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS
	} FileAttribute;

	DEFINE_ENUM_FLAG_OPERATORS(FileAttribute);



	typedef enum class _FileLock : DWORD {
		FileImmediately = LOCKFILE_FAIL_IMMEDIATELY,
		Exclusive = LOCKFILE_EXCLUSIVE_LOCK

	} FileLock;
	DEFINE_ENUM_FLAG_OPERATORS(FileLock);

	typedef enum class _FileSeekMethod : DWORD {
		BEGIN = FILE_BEGIN,
		CURRENT = FILE_CURRENT,
		END = FILE_END
	} FileSeekMethod;


	typedef enum class _FileAccessMode : DWORD {
		ReadOnly,
		WriteOnly,
		Execute,
		ReadWrite,
		ReadWriteExecute,

		SharedRead,
		SharedWrite,
		SharedReadWrite,
		SharedReadWriteDelete
	} FileAccessMode;


	class WinFile {

		HANDLE 	hFile_ = INVALID_HANDLE_VALUE;
		DWORD	LastError_ = 0;

	public:
		WinFile() { ; }
		//WinFile(LPCTSTR lpFileName, FileAccessMode Mode, LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr,
		//	CreationDisposition dwDisposition = CreationDisposition::OpenExisting,
		//	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
		//	HANDLE					hTemplateFile = NULL)
		//{
		//	Open(lpFileName, Mode, lpSecurityAttributes, dwDisposition, dwFlagsAndAttributes, hTemplateFile);
		//}
		//WinFile(LPCTSTR             lpFileName,
		//	FileDesiredAccess		dwDesiredAccess,
		//	FileShareMode			dwShareMode,
		//	LPSECURITY_ATTRIBUTES	lpSecurityAttributes,
		//	CreationDisposition  	dwCreationDisposition,
		//	DWORD					dwFlagsAndAttributes,
		//	HANDLE					hTemplateFile)
		//{
		//	CreateFile(lpFileName,
		//		dwDesiredAccess,
		//		dwShareMode,
		//		lpSecurityAttributes,
		//		dwCreationDisposition,
		//		dwFlagsAndAttributes,
		//		hTemplateFile);
		//}
		WinFile(LPCTSTR lpFileName,
			FileAccessMode Mode,
			LPSECURITY_ATTRIBUTES lpSecurityAttributes = nullptr,
			CreationDisposition dwDisposition = CreationDisposition::OpenExisting,
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE					hTemplateFile = NULL)
		{
			Open(lpFileName, Mode, lpSecurityAttributes, dwDisposition, dwFlagsAndAttributes, hTemplateFile);
		}



		bool Open(LPCTSTR lpFileName,
			FileAccessMode Mode,
			LPSECURITY_ATTRIBUTES
			lpSecurityAttributes = nullptr,
			CreationDisposition dwDisposition = CreationDisposition::OpenExisting,
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL,
			HANDLE					hTemplateFile = NULL)
		{
			hFile_ = INVALID_HANDLE_VALUE;

			FileDesiredAccess FileAccess = FileDesiredAccess::READ;
			FileShareMode	  dwShareMode = FileShareMode::NONE;

			switch (Mode) {
			case FileAccessMode::ReadOnly:
				FileAccess = FileDesiredAccess::READ;
				dwShareMode = FileShareMode::NONE;
				break;

			case FileAccessMode::WriteOnly:
				FileAccess = FileDesiredAccess::WRITE;
				dwShareMode = FileShareMode::NONE;
				break;

			case FileAccessMode::Execute:
				FileAccess = FileDesiredAccess::EXECUTE;
				dwShareMode = FileShareMode::NONE;
				break;

			case FileAccessMode::ReadWrite:
				FileAccess = FileDesiredAccess::READ | FileDesiredAccess::WRITE;
				dwShareMode = FileShareMode::NONE;
				break;

			case FileAccessMode::ReadWriteExecute:
				FileAccess = FileDesiredAccess::READ | FileDesiredAccess::WRITE | FileDesiredAccess::EXECUTE;
				dwShareMode = FileShareMode::NONE;
				break;

			case FileAccessMode::SharedRead:
				FileAccess = FileDesiredAccess::READ;
				dwShareMode = FileShareMode::READ;
				break;

			case FileAccessMode::SharedWrite:
				FileAccess = FileDesiredAccess::WRITE;
				dwShareMode = FileShareMode::WRITE;
				break;

			case FileAccessMode::SharedReadWrite:
				FileAccess = FileDesiredAccess::READ | FileDesiredAccess::WRITE;
				dwShareMode = FileShareMode::READ | FileShareMode::WRITE;
				break;

			case FileAccessMode::SharedReadWriteDelete:
				FileAccess = FileDesiredAccess::READ | FileDesiredAccess::WRITE;
				dwShareMode = FileShareMode::READ | FileShareMode::WRITE | FileShareMode::REMOVE;
				break;
			}

			LastError_ = 0;
			hFile_ = ::CreateFile(lpFileName,
				(DWORD)FileAccess,
				(DWORD)dwShareMode,
				lpSecurityAttributes,
				(DWORD)dwDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);

			if (hFile_ == INVALID_HANDLE_VALUE)
			{
				LastError_ = GetLastError();
			}

			return (hFile_ != INVALID_HANDLE_VALUE);
		}

		bool Close() {
			if (hFile_ != INVALID_HANDLE_VALUE) {
				::CloseHandle(hFile_);
				hFile_ = INVALID_HANDLE_VALUE;
			}

			return true;
		}

		bool CreateFile(LPCTSTR             lpFileName,
			FileDesiredAccess		dwDesiredAccess,
			FileShareMode			dwShareMode,
			LPSECURITY_ATTRIBUTES	lpSecurityAttributes,
			CreationDisposition  	dwCreationDisposition,
			DWORD					dwFlagsAndAttributes,
			HANDLE					hTemplateFile) {
			LastError_ = 0;
			hFile_ = ::CreateFile(lpFileName,
				(DWORD)dwDesiredAccess,
				(DWORD)dwShareMode,
				lpSecurityAttributes,
				(DWORD)dwCreationDisposition,
				dwFlagsAndAttributes,
				hTemplateFile);
			if (hFile_ == INVALID_HANDLE_VALUE)
			{
				LastError_ = GetLastError();
			}

			return (hFile_ != INVALID_HANDLE_VALUE);
		}

		DWORD Read(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped = NULL) {
			DWORD lpNumberOfBytesRead = 0;
			LastError_ = 0;			
			BOOL fReadSuccess = ::ReadFile(hFile_, lpBuffer, nNumberOfBytesToRead, &lpNumberOfBytesRead, lpOverlapped);
			if (!fReadSuccess) {
				LastError_ = GetLastError();
			}
			return lpNumberOfBytesRead;
		}

		DWORD Write(LPVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped = NULL) {
			DWORD lpNumberOfBytesWrite = 0;
			LastError_ = 0;
			BOOL fWriteSuccess = ::WriteFile(hFile_, lpBuffer, nNumberOfBytesToWrite, &lpNumberOfBytesWrite, lpOverlapped);
			if (!fWriteSuccess) {
				LastError_ = GetLastError();
			}
			return lpNumberOfBytesWrite;
		}

		BOOL Seek(LARGE_INTEGER offset, FileSeekMethod dwMoveMethod) {
			LARGE_INTEGER NewFilePointer = {};
			NewFilePointer.QuadPart = 0;
			LastError_ = 0;
			BOOL Success = SetFilePointerEx(hFile_, offset, &NewFilePointer, (DWORD)dwMoveMethod);
			if (!Success) {
				LastError_ = GetLastError();
			}
			return Success;
		}

		LARGE_INTEGER GetFileSize()
		{
			LARGE_INTEGER lpFileSize = {};
			lpFileSize.QuadPart = 0;

			BOOL bSuccess = GetFileSizeEx(hFile_, &lpFileSize);
			if (!bSuccess)
			{
				LastError_ = GetLastError();
			}
			return lpFileSize;
		}

		bool CancelIO() {
			BOOL bSuccess = CancelIo(hFile_);
			if (!bSuccess)
			{
				LastError_ = GetLastError();
			}

			return bSuccess;
		}

		/*
		WriteEx() {
			::WriteFileEx(hFile, lpBuffer, nNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine)
		}
		*/

		virtual ~WinFile() {
			if (hFile_ != INVALID_HANDLE_VALUE) {
				Close();
			}
		}
	};
}