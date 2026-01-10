#pragma once


#include <Windows.h>
#include <ostream>

namespace swktool
{
	class FileStreamBuf;
	class LoggerStream : public std::ostream
	{
		FileStreamBuf* buffer_ = nullptr;
		std::streambuf* pOldbuf;
		LPCTSTR FileName_;

	public:
		LoggerStream();
		LoggerStream(LPCTSTR FileName);
		~LoggerStream();
	};


} // namespace swktool
