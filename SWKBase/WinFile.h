#pragma once

#include <Windows.h>


#include <string>
#include "FileEnums.h"   // FileAccess, FileShareMode, CreationDisposition

namespace swktool {

    class OpenOptions; // forward declaration

    class WinFile {
        HANDLE hFile_ = INVALID_HANDLE_VALUE;
        mutable DWORD  lastError_ = 0;

    public:
        WinFile() = default;

        // Non-copyable
        WinFile(const WinFile&) = delete;
        WinFile& operator=(const WinFile&) = delete;

        // Movable
        WinFile(WinFile&& other) noexcept;
        WinFile& operator=(WinFile&& other) noexcept;

        ~WinFile();

        bool IsOpen() const noexcept { return hFile_ != INVALID_HANDLE_VALUE; }
        DWORD LastError() const noexcept { return lastError_; }

        // Called only by OpenOptions
        bool OpenExplicit(
            LPCTSTR path,
            FileAccess access,
            FileShareMode share,
            LPSECURITY_ATTRIBUTES sa,
            CreationDisposition disposition,
            DWORD flags,
            HANDLE templateFile
        );

        bool Close();

        DWORD Read(LPVOID buffer, DWORD bytesToRead, LPOVERLAPPED ov = nullptr);
        DWORD Write(const void* buffer, DWORD bytesToWrite, LPOVERLAPPED ov = nullptr);

        bool Seek(LARGE_INTEGER offset, FileSeekMethod method, LARGE_INTEGER* newPos = nullptr);
        LARGE_INTEGER GetFileSize() const;

        bool CancelIO(LPOVERLAPPED ov = nullptr);

        bool GetLastWriteTime(SYSTEMTIME& stLocal) const;
        bool GetLastWriteTime(FILETIME& ftWrite) const;


    };

} // namespace swktool
