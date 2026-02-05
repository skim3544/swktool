#pragma once
#include <filesystem>
#include "Logger.h"
#include "WinFile.h"
#include "FileStreamBuf.h"
#include "RotatingLoggerPolicy.h"

namespace swktool 
{
	class RotatingFileLogger : public swktool::Logger 
	{		
        std::unique_ptr<ILoggerFileRatationPolicy> policy_;
        std::wstring BaseFileName_;
	public:
        explicit RotatingFileLogger(std::unique_ptr<ILoggerFileRatationPolicy> policy) :
            policy_(std::move(policy)) 
        {
        }

        void init(LPCTSTR LogFileName) override 
        { 
            BaseFileName_ = LogFileName; 
            Logger::init(LogFileName); 
        }

    private:
        RotationContext BuildContext()
        {
            RotationContext ctx;

            // Current time
            ::GetLocalTime(&ctx.CurrentTime_);

            // Access underlying WinFile
            FileStreamBuf* buf = static_cast<FileStreamBuf*>(pStream_->rdbuf());
            WinFile& file = buf->GetFile();

            // Last write time
            file.GetLastWriteTime(ctx.LastWriteTime_);

            // File size
            ctx.LastFileSize_ = file.GetFileSize();

            return ctx;
        }

        void CheckRotation()
        {
            if (!pStream_)
                return;

            RotationContext ctx = BuildContext();

            if (policy_->ShouldRotate(ctx))
            {
                //RotateFiles();
                Logger::init(BaseFileName_.c_str());
            }
        }

        void RotateFiles(const SYSTEMTIME& lastWrite)
        {
            // Close current file
            pStream_.reset();

            // Build rotated filename: BaseFileName_.DD
            wchar_t ext[8];
            swprintf_s(ext, L".%02d", lastWrite.wDay);

            std::filesystem::path src(BaseFileName_);
            std::filesystem::path dst = src;
            dst += ext;

            std::error_code ec;

            // Remove existing rotated file if it exists
            std::filesystem::remove(dst, ec);

            // Rename current log ? BaseFileName_.DD
            std::filesystem::rename(src, dst, ec);

            // Reopen fresh log file
            Logger::init(BaseFileName_.c_str());
        }

	};
}