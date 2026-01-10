#pragma once

#include <Windows.h>
#include <string>
#include "WinFile.h"
#include "FileEnums.h"

namespace swktool 
{    
    class OpenOptions 
    {
    private:
        FileAccess access_ = FileAccess::None;
        FileShareMode share_ = FileShareMode::None;
        CreationDisposition disposition_ = CreationDisposition::OpenExisting;
        DWORD flags_ = FILE_ATTRIBUTE_NORMAL;
        LPSECURITY_ATTRIBUTES security_ = nullptr;
        HANDLE templateFile_ = nullptr;

    public:
        OpenOptions() = default;

        // Fluent setters
        OpenOptions& Access(FileAccess access);
        OpenOptions& Share(FileShareMode share);
        OpenOptions& Disposition(CreationDisposition disp);
        OpenOptions& Flags(DWORD flags);
        OpenOptions& Security(LPSECURITY_ATTRIBUTES sa);
        OpenOptions& Template(HANDLE hTemplate);

        // Open the file using WinFile
        WinFile Open(LPCTSTR path) const;

        // Optional: UTF-8 overload
        WinFile Open(const std::string& utf8Path) const;
    };

} // namespace swktool
