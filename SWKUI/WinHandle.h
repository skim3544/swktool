#pragma once

#include <Windows.h>

struct HandleDeleter
{
    // By defining the pointer type, we can delete a type other than T*.
    // In other words, we can declare unique_ptr<HANDLE, Deleter> instead of
    // unique_ptr<void, Deleter> which leaks the HANDLE abstraction.
    //typedef HANDLE pointer;
    using pointer = HANDLE;

    void operator()(HANDLE h)
    {
        if (INVALID_HANDLE_VALUE != h)
        {
            CloseHandle(h);
        }
    }
};


