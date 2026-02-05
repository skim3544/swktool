#pragma once

#pragma once
#include <atomic>
#include <cstdint>
#include <cstring>
#include <unordered_map>
#include <vector>
#include <stdarg.h>
#include <mutex>
#include "WinFile.h"
#include "LoggerStream.h"
#include "Logger.h"


namespace swktool 
{
    //Can modify this to get larger history buffer
    static constexpr size_t TRACE_CAPACITY = 4096;

    static constexpr size_t TRACE_MSG_CAP = 128;     // max chars per chunk
    static constexpr size_t TRACE_MSG_BUF = TRACE_MSG_CAP + 1; // +1 for '\0'

    struct TraceEntry {
        const char* function;      // __FUNCTION__ literal
        uint64_t    timestamp;     // GetTickCount64 or QPC
        uint32_t    threadId;
        char        message[TRACE_MSG_BUF];  // fixed-size chunk
    };

    class TraceBuffer {
    public:
        static void Trace(const char* func, const char* fmt, ...)
        {
            // Format full message into a temporary stack buffer
            char fullMsg[1024];

            va_list args;
            va_start(args, fmt);
            vsnprintf(fullMsg, sizeof(fullMsg), fmt, args);
            va_end(args);

            const size_t len = strlen(fullMsg);
            size_t offset = 0;

            while (offset < len) {
                size_t idx = WriteIndex_.fetch_add(1, std::memory_order_relaxed);
                idx %= TRACE_CAPACITY;

                TraceEntry& e = Buffer_[idx];

                e.function = func;
                e.timestamp = GetTickCount64();
                e.threadId = GetCurrentThreadId();

                // Copy up to TRACE_MSG_CAP bytes
                const size_t remaining = len - offset;
                const size_t chunk = (remaining > TRACE_MSG_CAP) ? TRACE_MSG_CAP : remaining;

                memcpy(e.message, fullMsg + offset, chunk);
                e.message[chunk] = '\0'; // always safe

                offset += chunk;
            }
        }

        static void DumpGrouped(WinFile& file)
        {
            size_t end = WriteIndex_.load(std::memory_order_relaxed);
            size_t start = (end > TRACE_CAPACITY ? end - TRACE_CAPACITY : 0);

            // Extract entries in chronological order
            std::vector<const TraceEntry*> entries;
            entries.reserve(TRACE_CAPACITY);

            for (size_t i = start; i < end; ++i) {
                entries.push_back(&Buffer_[i % TRACE_CAPACITY]);
            }

            // Group by function
            std::unordered_map<const char*, std::vector<const TraceEntry*>> groups;

            for (auto* e : entries) {
                groups[e->function].push_back(e);
            }

            // Dump each group
            for (auto& [func, vec] : groups) {

                char header[256];
                int hlen = sprintf_s(
                    header, sizeof(header),
                    "\n====================\nFunction: %s\n====================\n",
                    func
                );
                file.Write(header, hlen);

                for (auto* e : vec) {
                    char line[256];
                    int len = sprintf_s(
                        line, sizeof(line),
                        "[%llu] (tid %u) %s\n",
                        e->timestamp,
                        e->threadId,
                        e->message
                    );
                    file.Write(line, len);
                }
            }
        }

        static auto SnapshotGrouped()
            -> std::unordered_map<const char*, std::vector<std::string>>
        {
            std::unordered_map<const char*, std::vector<std::string>> grouped;

            // Snapshot the write index
            size_t end = WriteIndex_.load(std::memory_order_relaxed);
            size_t start = (end > TRACE_CAPACITY ? end - TRACE_CAPACITY : 0);

            // Copy entries under lock
            std::vector<TraceEntry> snapshot;
            snapshot.reserve(TRACE_CAPACITY);

            {
                std::lock_guard<std::mutex> lock(Mutex_);
                for (size_t i = start; i < end; ++i)
                    snapshot.push_back(Buffer_[i % TRACE_CAPACITY]);
            }

            // Now group them by function
            for (auto& e : snapshot)
            {
                grouped[e.function].push_back(std::string(e.message));
            }

            return grouped;
        }



        static void DumpGrouped(ILogger* logger)
        {
            if (!logger)
                return;

            // Acquire snapshot of trace entries
            auto entries = TraceBuffer::SnapshotGrouped();

            for (auto& [func, messages] : entries)
            {
                // Function header
                char header[256];
                snprintf(header, sizeof(header), "=== %s ===", func);
                logger->Log(header);

                // Messages inside this function
                for (auto& msg : messages)
                {
                    logger->Log(msg.c_str());
                }

                logger->Log(""); // blank line between groups
            }
        }


    private:
        static inline TraceEntry Buffer_[TRACE_CAPACITY];
        static inline std::atomic<size_t> WriteIndex_ = 0;
        inline static std::mutex Mutex_;
    };


    class ScopedTrace
    {
    public:
        ScopedTrace(const char* func)
            : func_(func)
        {
            TraceBuffer::Trace(func_, "START");
        }

        ~ScopedTrace()
        {
            TraceBuffer::Trace(func_, "END");
        }


        // printf-style formatter
        void Format(const char* fmt, ...) const
        {
            char buffer[256]; // fixed-size, stack-allocated

            va_list args;
            va_start(args, fmt);
            int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
            va_end(args);

            if (len > 0)
                TraceBuffer::Trace(func_, buffer);
            else
                TraceBuffer::Trace(func_, "FORMAT_ERROR");
        }

    private:
        const char* func_;
    };


} // namespace swktool

