#include "pch.h"
#include "TimeSpan.h"
using namespace std;
using namespace  CSharpDateTime;

TimeSpan TimeSpan::Zero = TimeSpan(0);


long long TimeSpan::TimeToTicks(int hour, int minute, int second) {
	// totalSeconds is bounded by 2^31 * 2^12 + 2^31 * 2^8 + 2^31,
	// which is less than 2^44, meaning we won't overflow totalSeconds.
	long long totalSeconds = (long long)hour * 3600 + (long long)minute * 60 + (long long)second;
	//            if (totalSeconds > MaxSeconds || totalSeconds < MinSeconds)
	//                throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("Overflow_TimeSpanTooLong"));
	long long TotalTicks = (totalSeconds * TicksPerSecond);
	return TotalTicks;
}

