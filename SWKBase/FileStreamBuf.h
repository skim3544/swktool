#pragma once
#include <Windows.h>
#include <string>
#include <iosfwd>
#include "WinFile.h"

namespace swktool {
	class FileStreamBuf : public std::stringbuf
	{
	protected:
		WinFile  File_;
		SYSTEMTIME  LastTime_;
		std::string CachedHeader_;
		DWORD LastMs_ = (DWORD)-1;

	public:
		FileStreamBuf();
		FileStreamBuf(LPCTSTR fname);

		~FileStreamBuf()
		{
			sync();
		}

		int sync()
		{
			// Grab the buffer contents once
			const std::string& buf = str();
			if (buf.empty())
				return 0;

			// Build final log line efficiently
			const std::string& header = GetLogHeader();

			// Reserve once to avoid reallocations
			std::string line;
			line.reserve(header.size() + buf.size());

			line.append(header);
			line.append(buf);

			// Write to file
			File_.Write((LPVOID)line.data(), (DWORD)line.size());

			// Clear internal buffer efficiently
			str(std::string());

			return 0;
		}


		// stabdard output is MM-DD-YYYY HH:MM:SS:ms (ThreadID). "
		std::string GetLogHeader()
		{
			SYSTEMTIME now;
			::GetLocalTime(&now);

			// Cache thread ID (thread-local)
			thread_local DWORD tid = ::GetCurrentThreadId();

			// If same millisecond, reuse cached header
			if (now.wMilliseconds == LastMs_)
			{
				return CachedHeader_;
			}

			LastMs_ = now.wMilliseconds;

			// Preallocate enough space (timestamp is ~40 chars)
			CachedHeader_.clear();
			CachedHeader_.reserve(64);

			// Format manually using fast integer-to-string operations
			char buf[64];
			int len = sprintf_s(
				buf, sizeof(buf),
				"%02d-%02d-%04d %02d:%02d:%02d.%03d(%08u).",
				now.wMonth,
				now.wDay,
				now.wYear,
				now.wHour,
				now.wMinute,
				now.wSecond,
				now.wMilliseconds,
				tid
			);

			CachedHeader_.assign(buf, len);
			return CachedHeader_;
		}
	};

}