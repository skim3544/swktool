#pragma once

#include <Windows.h>
#include <string>
#include <vector>

namespace swktool
{

    class WinReg {
    public:
        WinReg() = default;

        WinReg(HKEY hKey)
            : _hKey(hKey), _owns(false)
        {
        }

        ~WinReg() {
            Close();
        }

        // Non-copyable
        WinReg(const WinReg&) = delete;
        WinReg& operator=(const WinReg&) = delete;

        // Movable
        WinReg(WinReg&& other) noexcept {
            MoveFrom(other);
        }

        WinReg& operator=(WinReg&& other) noexcept {
            if (this != &other) {
                Close();
                MoveFrom(other);
            }
            return *this;
        }

        // -----------------------------
        // Opening / Creating Keys
        // -----------------------------
        bool Open(HKEY root, const std::wstring& subkey, REGSAM access = KEY_READ | KEY_WRITE) {
            Close();
            DWORD result = RegOpenKeyExW(root, subkey.c_str(), 0, access, &_hKey);
            _owns = (result == ERROR_SUCCESS);
            _lastError = result;
            return _owns;
        }

        bool Create(HKEY root, const std::wstring& subkey, REGSAM access = KEY_READ | KEY_WRITE) {
            Close();
            DWORD disposition = 0;
            DWORD result = RegCreateKeyExW(
                root,
                subkey.c_str(),
                0,
                nullptr,
                REG_OPTION_NON_VOLATILE,
                access,
                nullptr,
                &_hKey,
                &disposition
            );
            _owns = (result == ERROR_SUCCESS);
            _lastError = result;
            return _owns;
        }

        void Close() {
            if (_hKey && _owns) {
                RegCloseKey(_hKey);
            }
            _hKey = nullptr;
            _owns = false;
        }

        bool IsValid() const { return _hKey != nullptr; }

        // -----------------------------
        // Reading Values
        // -----------------------------
        bool ReadDWORD(const std::wstring& name, DWORD& outValue) {
            DWORD type = REG_DWORD;
            DWORD size = sizeof(DWORD);
            DWORD result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_DWORD, &type,
                &outValue, &size);
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        bool ReadQWORD(const std::wstring& name, unsigned long long& outValue) {
            DWORD type = REG_QWORD;
            DWORD size = sizeof(unsigned long long);
            DWORD result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_QWORD, &type,
                &outValue, &size);
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        bool ReadString(const std::wstring& name, std::wstring& outValue) {
            DWORD type = REG_SZ;
            DWORD size = 0;

            DWORD result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_SZ, &type,
                nullptr, &size);
            if (result != ERROR_SUCCESS) {
                _lastError = result;
                return false;
            }

            std::wstring buffer(size / sizeof(wchar_t), L'\0');

            result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_SZ, &type,
                buffer.data(), &size);

            _lastError = result;
            if (result != ERROR_SUCCESS)
                return false;

            if (!buffer.empty() && buffer.back() == L'\0')
                buffer.pop_back();

            outValue = std::move(buffer);
            return true;
        }

        bool ReadBinary(const std::wstring& name, std::vector<BYTE>& outData) {
            DWORD type = REG_BINARY;
            DWORD size = 0;

            DWORD result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_BINARY, &type,
                nullptr, &size);
            if (result != ERROR_SUCCESS) {
                _lastError = result;
                return false;
            }

            outData.resize(size);

            result = RegGetValueW(_hKey, nullptr, name.c_str(),
                RRF_RT_REG_BINARY, &type,
                outData.data(), &size);

            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        // -----------------------------
        // Writing Values
        // -----------------------------
        bool WriteDWORD(const std::wstring& name, DWORD value) {
            DWORD result = RegSetValueExW(_hKey, name.c_str(), 0,
                REG_DWORD,
                reinterpret_cast<const BYTE*>(&value),
                sizeof(value));
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        bool WriteQWORD(const std::wstring& name, unsigned long long value) {
            DWORD result = RegSetValueExW(_hKey, name.c_str(), 0,
                REG_QWORD,
                reinterpret_cast<const BYTE*>(&value),
                sizeof(value));
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        bool WriteString(const std::wstring& name, const std::wstring& value) {
            DWORD result = RegSetValueExW(_hKey, name.c_str(), 0,
                REG_SZ,
                reinterpret_cast<const BYTE*>(value.c_str()),
                static_cast<DWORD>((value.size() + 1) * sizeof(wchar_t)));
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        bool WriteBinary(const std::wstring& name, const std::vector<BYTE>& data) {
            DWORD result = RegSetValueExW(_hKey, name.c_str(), 0,
                REG_BINARY,
                data.data(),
                static_cast<DWORD>(data.size()));
            _lastError = result;
            return result == ERROR_SUCCESS;
        }

        // -----------------------------
        // Error Handling
        // -----------------------------
        DWORD GetLastErrorCode() const {
            return _lastError;
        }

        std::wstring GetLastErrorMessage() const {
            if (_lastError == ERROR_SUCCESS)
                return L"No error";

            LPWSTR buffer = nullptr;
            DWORD size = FormatMessageW(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr,
                _lastError,
                0,
                (LPWSTR)&buffer,
                0,
                nullptr
            );

            std::wstring msg = (size && buffer) ? buffer : L"Unknown error";
            if (buffer)
                LocalFree(buffer);

            return msg;
        }

    private:
        void MoveFrom(WinReg& other) {
            _hKey = other._hKey;
            _owns = other._owns;
            _lastError = other._lastError;
            other._hKey = nullptr;
            other._owns = false;
            other._lastError = ERROR_SUCCESS;
        }

    private:
        HKEY _hKey = nullptr;
        bool _owns = false;
        DWORD _lastError = ERROR_SUCCESS;
    };

}
