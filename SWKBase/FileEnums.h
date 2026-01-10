#pragma once

#include <Windows.h>

namespace swktool {

	enum class FileAccess : DWORD {
		None = 0,
		ReadData = FILE_READ_DATA,
		WriteData = FILE_WRITE_DATA,
		AppendData = FILE_APPEND_DATA,
		ReadAttributes = FILE_READ_ATTRIBUTES,
		WriteAttributes = FILE_WRITE_ATTRIBUTES,
		ReadEA = FILE_READ_EA,
		WriteEA = FILE_WRITE_EA,
		Delete = DELETE,
	};



	DEFINE_ENUM_FLAG_OPERATORS(FileAccess);


	enum class FileShareMode : DWORD {
		None = 0,
		Read = FILE_SHARE_READ,
		Write = FILE_SHARE_WRITE,
		Delete = FILE_SHARE_DELETE,
		ReadWrite = FILE_SHARE_READ | FILE_SHARE_WRITE,
		All = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE
	} ;

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
}