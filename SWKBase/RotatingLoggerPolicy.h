#pragma once
#include "WinFile.h"

namespace swktool
{
	struct RotationContext
	{

		SYSTEMTIME CurrentTime_{};
		SYSTEMTIME LastWriteTime_{};
		LARGE_INTEGER LastFileSize_{};
	};


	class ILoggerFileRatationPolicy
	{
	public:
		virtual ~ILoggerFileRatationPolicy() = default;
		virtual bool ShouldRotate(const RotationContext& ctx) = 0;
	};


	class DailyRotationPolicy : public ILoggerFileRatationPolicy
	{
	public:
		bool ShouldRotate(const RotationContext& ctx) override
		{
			return ctx.CurrentTime_.wDay != ctx.LastWriteTime_.wDay;
		}
	};

	class RotatingLoggerFileSizePolicy : public ILoggerFileRatationPolicy
	{
		LONGLONG MaxSize_;
	public:
		RotatingLoggerFileSizePolicy(LONGLONG nMaxSize) :
			MaxSize_(nMaxSize)
		{
		}

		bool ShouldRotate(const RotationContext& ctx) override
		{			
			return ctx.LastFileSize_.QuadPart >= (LONGLONG)MaxSize_;		
		}
	};

}