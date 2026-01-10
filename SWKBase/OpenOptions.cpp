#include "pch.h"
#include "OpenOptions.h"
#include <stringapiset.h>

namespace swktool 
{

    // ---------------------------
    // Fluent setters
    // ---------------------------

    OpenOptions& OpenOptions::Access(FileAccess access) {
        access_ = access;
        return *this;
    }

    OpenOptions& OpenOptions::Share(FileShareMode share) {
        share_ = share;
        return *this;
    }

    OpenOptions& OpenOptions::Disposition(CreationDisposition disp) {
        disposition_ = disp;
        return *this;
    }

    OpenOptions& OpenOptions::Flags(DWORD flags) {
        flags_ = flags;
        return *this;
    }

    OpenOptions& OpenOptions::Security(LPSECURITY_ATTRIBUTES sa) {
        security_ = sa;
        return *this;
    }

    OpenOptions& OpenOptions::Template(HANDLE hTemplate) {
        templateFile_ = hTemplate;
        return *this;
    }

    // ---------------------------
    // Open using WinFile
    // ---------------------------

    WinFile OpenOptions::Open(LPCTSTR path) const {
        WinFile file;
        file.OpenExplicit(
            path,
            access_,
            share_,
            security_,
            disposition_,
            flags_,
            templateFile_
        );
        return file;
    }

    // ---------------------------
    // UTF-8 overload
    // ---------------------------

    WinFile OpenOptions::Open(const std::string& utf8Path) const {
        int len = MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, nullptr, 0);
        std::wstring wpath(len, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, utf8Path.c_str(), -1, &wpath[0], len);

        return Open(wpath.c_str());
    }

} // namespace swktool
