#pragma once

#pragma once

// Set to 0 to completely disable tracing
#ifndef SWK_TRACE_ENABLED
#define SWK_TRACE_ENABLED 1
#endif

#if SWK_TRACE_ENABLED

    #define TRACE(msg) \
            swktool::TraceBuffer::Trace(__FUNCTION__, "%s", msg)

    #define TRACEF(fmt, ...) \
            swktool::TraceBuffer::Trace(__FUNCTION__, fmt, __VA_ARGS__)

        // Scoped trace uses RAII
    #define TRACE_SCOPE() \
            swktool::ScopedTrace _swk_scoped_trace(__FUNCTION__)

#else // SWK_TRACE_ENABLED

        // When disabled, macros compile to nothing
    #define TRACE(msg)            ((void)0)
    #define TRACEF(fmt, ...)      ((void)0)
    #define TRACE_SCOPE()         ((void)0)

#endif //SWK_TRACE_ENABLED
