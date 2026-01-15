#pragma once
#include <windows.h>
#include <string>
#include "WinReg.h"

namespace swktool 
{
    class WinRegOpenOptions {
    public:
        WinRegOpenOptions() = default;

        // --- Access flags ---
        WinRegOpenOptions& Read() {
            _access |= KEY_READ;
            return *this;
        }

        WinRegOpenOptions& Write() {
            _access |= KEY_WRITE;
            return *this;
        }

        WinRegOpenOptions& ReadWrite() {
            _access |= (KEY_READ | KEY_WRITE);
            return *this;
        }

        WinRegOpenOptions& AllAccess() {
            _access |= KEY_ALL_ACCESS;
            return *this;
        }

        // --- WOW64 flags ---
        WinRegOpenOptions& Wow64_64() {
            _access |= KEY_WOW64_64KEY;
            return *this;
        }

        WinRegOpenOptions& Wow64_32() {
            _access |= KEY_WOW64_32KEY;
            return *this;
        }

        // --- Open existing key ---
        WinReg Open(HKEY root, const std::wstring& subkey) const {
            WinReg reg;
            reg.Open(root, subkey, _access);
            return reg;
        }

        // --- Create or open ---
        WinReg OpenOrCreate(HKEY root, const std::wstring& subkey) const {
            WinReg reg;
            if (!reg.Open(root, subkey, _access)) {
                reg.Create(root, subkey, _access);
            }
            return reg;
        }

        // --- Create new key only ---
        WinReg Create(HKEY root, const std::wstring& subkey) const {
            WinReg reg;
            reg.Create(root, subkey, _access);
            return reg;
        }

        // --- Accessor for debugging ---
        REGSAM GetAccessMask() const { return _access; }

    private:
        REGSAM _access = 0;
    };

} // namespace swktool
