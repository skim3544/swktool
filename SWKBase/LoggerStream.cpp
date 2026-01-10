#include "pch.h"
#include "FileStreamBuf.h"
#include "Logger.h"
#include <ios>
#include "LoggerStream.h"

namespace swktool
{
    /////////////////////////////////////////////////////////////////////
    LoggerStream::LoggerStream() : 
          std::ostream(nullptr)
        , pOldbuf(nullptr)
        , FileName_(nullptr)
    {
        buffer_ = new FileStreamBuf();
    }

    LoggerStream::LoggerStream(LPCTSTR FileName) :
        std::ostream(nullptr)
        , FileName_(FileName) // preserve file name
        , pOldbuf(nullptr)        
    {
        buffer_ = new FileStreamBuf(FileName_);
        pOldbuf = rdbuf(buffer_);
    }

    LoggerStream::~LoggerStream()
    {
        if (pOldbuf != nullptr)
            rdbuf(pOldbuf);

        delete buffer_;
        buffer_ = nullptr;
    }
}