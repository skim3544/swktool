#pragma once


#include <Windows.h>
#include <ostream>

namespace swktool
{
	class FileStreamBuf;
	class LoggerStream : public std::ostream
	{
		FileStreamBuf* buffer_ = nullptr;
		std::streambuf* pOldbuf = nullptr;
		LPCTSTR FileName_ = nullptr;

	public:
		LoggerStream();
		LoggerStream(LPCTSTR FileName);
		~LoggerStream();
	};


} // namespace swktool
