#include "pch.h"
#include "WinFile.h"
#include "OpenOptions.h"

namespace swktool {

    // ---------------------------
    // Move semantics
    // ---------------------------

    WinFile::WinFile(WinFile&& other) noexcept {
        hFile_ = other.hFile_;
        lastError_ = other.lastError_;
        other.hFile_ = INVALID_HANDLE_VALUE;
    }

    WinFile& WinFile::operator=(WinFile&& other) noexcept {
        if (this != &other) {
            Close();
            hFile_ = other.hFile_;
            lastError_ = other.lastError_;
            other.hFile_ = INVALID_HANDLE_VALUE;
        }
        return *this;
    }

    // ---------------------------
    // Destructor
    // ---------------------------

    WinFile::~WinFile() {
        Close();
    }

    // ---------------------------
    // Explicit low-level open
    // ---------------------------

    bool WinFile::OpenExplicit(
        LPCTSTR path,
        FileAccess access,
        FileShareMode share,
        LPSECURITY_ATTRIBUTES sa,
        CreationDisposition disposition,
        DWORD flags,
        HANDLE templateFile
    ) {
        lastError_ = 0;

        hFile_ = ::CreateFile(
            path,
            static_cast<DWORD>(access),
            static_cast<DWORD>(share),
            sa,
            static_cast<DWORD>(disposition),
            flags,
            templateFile
        );

        if (hFile_ == INVALID_HANDLE_VALUE) {
            lastError_ = ::GetLastError();
            return false;
        }

        return true;
    }

    // ---------------------------
    // Close
    // ---------------------------

    bool WinFile::Close() {
        if (hFile_ != INVALID_HANDLE_VALUE) {
            BOOL ok = ::CloseHandle(hFile_);
            if (!ok) {
                lastError_ = ::GetLastError();
                return false;
            }
            hFile_ = INVALID_HANDLE_VALUE;
        }
        return true;
    }

    // ---------------------------
    // Read / Write
    // ---------------------------

    DWORD WinFile::Read(LPVOID buffer, DWORD bytesToRead, LPOVERLAPPED ov) {
        DWORD bytesRead = 0;
        lastError_ = 0;

        BOOL ok = ::ReadFile(hFile_, buffer, bytesToRead, &bytesRead, ov);
        if (!ok) lastError_ = ::GetLastError();

        return bytesRead;
    }

    DWORD WinFile::Write(const void* buffer, DWORD bytesToWrite, LPOVERLAPPED ov) {
        DWORD bytesWritten = 0;
        lastError_ = 0;

        BOOL ok = ::WriteFile(hFile_, buffer, bytesToWrite, &bytesWritten, ov);
        if (!ok) lastError_ = ::GetLastError();

        return bytesWritten;
    }

    // ---------------------------
    // Seek
    // ---------------------------

    bool WinFile::Seek(LARGE_INTEGER offset, FileSeekMethod method, LARGE_INTEGER* newPos) {
        LARGE_INTEGER result = {};
        lastError_ = 0;

        BOOL ok = ::SetFilePointerEx(hFile_, offset, &result, static_cast<DWORD>(method));
        if (!ok) {
            lastError_ = ::GetLastError();
            return false;
        }

        if (newPos) *newPos = result;
        return true;
    }

    // ---------------------------
    // File size
    // ---------------------------

    LARGE_INTEGER WinFile::GetFileSize() const {
        LARGE_INTEGER size = {};
        lastError_ = 0;

        BOOL ok = ::GetFileSizeEx(hFile_, &size);
        if (!ok) lastError_ = ::GetLastError();

        return size;
    }

    // ---------------------------
    // Cancel I/O
    // ---------------------------

    bool WinFile::CancelIO(LPOVERLAPPED ov) {
        lastError_ = 0;

        BOOL ok = ::CancelIoEx(hFile_, ov);
        if (!ok) {
            lastError_ = ::GetLastError();
            return false;
        }

        return true;
    }

    bool WinFile::GetLastWriteTime(FILETIME& ftWrite) const
    {
        if (!IsOpen())
            return false;

        FILETIME ftCreate, ftAccess;
        if (!::GetFileTime(hFile_, &ftCreate, &ftAccess, &ftWrite))
        {
            lastError_ = ::GetLastError();
            return false;
        }
        return true;
    }

    bool WinFile::GetLastWriteTime(SYSTEMTIME& stLocal) const
    {
        if (!IsOpen())
            return false;

        FILETIME ftCreate{}, ftAccess{}, ftWrite{};
        if (!::GetFileTime(hFile_, &ftCreate, &ftAccess, &ftWrite))
        {
            lastError_ = ::GetLastError();
            return false;
        }

        // Convert FILETIME → SYSTEMTIME (UTC)
        SYSTEMTIME stUTC{};
        if (!::FileTimeToSystemTime(&ftWrite, &stUTC))
        {
            lastError_ = ::GetLastError();
            return false;
        }

        // Convert UTC → Local time
        if (!::SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal))
        {
            lastError_ = ::GetLastError();
            return false;
        }

        return true;
    }



} // namespace swktool
