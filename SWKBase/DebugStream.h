#pragma once

#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>

namespace swktool {

    class DebugStreamBufA : public std::stringbuf
    {
    public:
        ~DebugStreamBufA() { sync(); }

        int sync()
        {
            ::OutputDebugStringA(str().c_str());
            str(std::string()); // Clear the string buffer
            return 0;
        }
    };


    class DebugStreamBufW : public std::wstringbuf
    {
    public:
        ~DebugStreamBufW() { sync(); }

        int sync()
        {
            ::OutputDebugStringW(str().c_str());
            str(std::wstring()); // Clear the string buffer
            
            return 0;
        }
    };



    class DebugStreamA : public std::ostream
    {
        DebugStreamBufA buffer_;
    public:
        DebugStreamA() :
            std::ostream(&buffer_, false)
        {
            rdbuf(&buffer_);
        }
    };


    class DebugStreamW : public std::wostream
    {
        DebugStreamBufW buffer_;
    public:
        DebugStreamW() :
            std::wostream(&buffer_, false)
        {
            rdbuf(&buffer_);
        }
    };


    extern DebugStreamA DebugOut;
    extern DebugStreamW WDebugOut;
}


