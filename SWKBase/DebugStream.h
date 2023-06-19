#pragma once

#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>

namespace swktool {

    class DebugStreamBuf : public std::stringbuf
    {
    public:
        ~DebugStreamBuf() { sync(); }

        int sync()
        {
            ::OutputDebugStringA(str().c_str());
            str(std::string()); // Clear the string buffer
            return 0;
        }
    };



    class DebugStream : public std::ostream
    {
        DebugStreamBuf buffer_;
    public:
        DebugStream() : 
            std::ostream(&buffer_, false)
        {
            rdbuf(&buffer_);
        }
    };


    extern DebugStream DebugOut;
}


