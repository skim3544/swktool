#pragma once

#include <mutex>
#include <unordered_map>
#include "WindowInterfaces.h"


namespace swktool
{
    //
    // ------------------------------------------------------------
    //  WindowHandlerRegistry — unchanged
    // ------------------------------------------------------------
    //
    class WindowHandlerRegistry
    {
    public:
        static WindowHandlerRegistry& Instance()
        {
            static WindowHandlerRegistry instance;
            return instance;
        }

        void RegisterHandler(HWND hwnd, IWindowHandler* handler)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            handlers_[hwnd] = handler;
        }

        void UnregisterHandler(HWND hwnd)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            handlers_.erase(hwnd);
        }

        IWindowHandler* GetHandler(HWND hwnd)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            auto it = handlers_.find(hwnd);
            return (it != handlers_.end()) ? it->second : nullptr;
        }

    private:
        WindowHandlerRegistry() = default;
        WindowHandlerRegistry(const WindowHandlerRegistry&) = delete;
        WindowHandlerRegistry& operator=(const WindowHandlerRegistry&) = delete;

        std::mutex mutex_;
        std::unordered_map<HWND, IWindowHandler*> handlers_;
    };
}