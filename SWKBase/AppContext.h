#pragma once

#include "IOC.h"

namespace swktool
{
    class IServiceProvider;

    class AppContext
    {
    public:
        // Access the global IOC container
        static swktool::IOCContainer& IOC()
        {
            return s_ioc;
        }

    private:
        // Header-only, safe, single instance
        inline static swktool::IOCContainer s_ioc{};
    };


}
