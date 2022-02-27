#pragma once

#include <chrono>
#include <ctime>  
#include <time.h>
#include <ostream>
#include <string>
#include <sstream>
#include <iomanip>

#include <assert.h>
#include "TimeSpan.h"


namespace CSharpDateTime
{	
	typedef unsigned long long DateTime_t;
	enum class EDayOfWeek {
		Sunday = 0,
		Monday,
		Tuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday
	};

	/// <summary>
	///  Microsoft C#  .NET DateTime,
	///  ported over C++, then modify the C# long to long long/unsigned long long data type
	/// </summary>
	class DateTime {
	private:
		const unsigned long long TicksMask = 0x3FFFFFFFFFFFFFFF;
		const unsigned long long  FlagsMask = 0xC000000000000000;
		// Number of days in a non-leap year
		const int DaysPerYear = 365;
		// Number of days in 4 years
		const int DaysPer4Years = DaysPerYear * 4 + 1;       // 1461
		   // Number of days in 100 years
		const int DaysPer100Years = DaysPer4Years * 25 - 1;  // 36524
		   // Number of days in 400 years
		const int DaysPer400Years = DaysPer100Years * 4 + 1; // 146097

		// Number of days from 1/1/0001 to 12/31/1600
		const int DaysTo1601 = DaysPer400Years * 4;          // 584388
		// Number of days from 1/1/0001 to 12/30/1899
		const int DaysTo1899 = DaysPer400Years * 4 + DaysPer100Years * 3 - 367;
		// Number of days from 1/1/0001 to 12/31/1969
		const int DaysTo1970 = DaysPer400Years * 4 + DaysPer100Years * 3 + DaysPer4Years * 17 + DaysPerYear; // 719,162
		// Number of days from 1/1/0001 to 12/31/9999
		const int DaysTo10000 = DaysPer400Years * 25 - 366;  // 3652059

		const long MaxMillis = (long)DaysTo10000 * MillisPerDay;
		const int DatePartYear = 0;
		const int DatePartDayOfYear = 1;
		const int DatePartMonth = 2;
		const int DatePartDay = 3;


	protected:
		DateTime_t dateData;

		static int DaysToMonth365[];
		static int DaysToMonth366[];



	public:
		DateTime(DateTime_t ticks) {
			//if (ticks < MinTicks || ticks > MaxTicks)
			//	throw new ArgumentOutOfRangeException("ticks", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
			//Contract.EndContractBlock();
			dateData = (DateTime_t)ticks;
		}

		DateTime(const DateTime& Other) {
			//if (ticks < MinTicks || ticks > MaxTicks)
			//	throw new ArgumentOutOfRangeException("ticks", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
			//Contract.EndContractBlock();
			dateData = Other.dateData;
		}

		DateTime(std::tm tm) :
		DateTime((tm.tm_year + 1900), tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec) {
			//// ???? Debug
			//int y, m, d = 0;
			//y = GetYear();
			//m = GetMonth();
			//d = GetDay();
		}

		DateTime(int year, int month, int day) {
			this->dateData = static_cast<DateTime_t>(DateToTicks(year, month, day));

		}

		// Constructs a DateTime from a given year, month, day, hour,
		// minute, and second.
		//
		DateTime(int year, int month, int day, int hour, int minute, int second) {
			//if (second == 60 && s_isLeapSecondsSupportedSystem && IsValidTimeWithLeapSeconds(year, month, day, hour, minute, second, DateTimeKind.Unspecified))
			//{
			//	// if we have leap second (second = 60) then we'll need to check if it is valid time.
			//	// if it is valid, then we adjust the second to 59 so DateTime will consider this second is last second
			//	// in the specified minute.
			//	// if it is not valid time, we'll eventually throw.
			//	second = 59;
			//}
			DateTime_t DateTick = DateToTicks(year, month, day);
			DateTime_t TimeTick = TimeToTicks(hour, minute, second);
			DateTime_t TotalTick = DateTick + TimeTick;
			this->dateData = TotalTick;

			// Debug Line
			//int y, m, d = 0;
			//y = GetYear();
			//m = GetMonth();
			//d = GetDay();			
		}


		// Constructs a DateTime from a given year, month, day, hour,
		// minute, and second.
		//
		DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond) {
			//if (millisecond < 0 || millisecond >= MillisPerSecond) {
			//	throw new ArgumentOutOfRangeException("millisecond", Environment.GetResourceString("ArgumentOutOfRange_Range", 0, MillisPerSecond - 1));
			//}
			//Contract.EndContractBlock();

			//if (second == 60 && s_isLeapSecondsSupportedSystem && IsValidTimeWithLeapSeconds(year, month, day, hour, minute, second, DateTimeKind.Unspecified))
			//{
			//	// if we have leap second (second = 60) then we'll need to check if it is valid time.
			//	// if it is valid, then we adjust the second to 59 so DateTime will consider this second is last second
			//	// in the specified minute.
			//	// if it is not valid time, we'll eventually throw.
			//	second = 59;
			//}

			DateTime_t ticks = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
			ticks += millisecond * (long long)TicksPerMillisecond;
			//			if (ticks < MinTicks || ticks > MaxTicks)
			//				throw new ArgumentException(Environment.GetResourceString("Arg_DateTimeRange"));
			this->dateData = ticks;
		}



		DateTime Add(TimeSpan value) {
			return AddTicks(value.Ticks());
		}

		// Returns the DateTime resulting from adding a fractional number of
		// days to this DateTime. The result is computed by rounding the
		// fractional number of days given by value to the nearest
		// millisecond, and adding that interval to this DateTime. The
		// value argument is permitted to be negative.
		//
		DateTime AddDays(double value) {
			return Add(value, MillisPerDay);
		}

		// Returns the DateTime resulting from adding a fractional number of
		// hours to this DateTime. The result is computed by rounding the
		// fractional number of hours given by value to the nearest
		// millisecond, and adding that interval to this DateTime. The
		// value argument is permitted to be negative.
		//
		DateTime AddHours(double value) {
			return Add(value, MillisPerHour);
		}

		// Returns the DateTime resulting from the given number of
		// milliseconds to this DateTime. The result is computed by rounding
		// the number of milliseconds given by value to the nearest integer,
		// and adding that interval to this DateTime. The value
		// argument is permitted to be negative.
		//
		DateTime AddMilliseconds(double value) {
			return Add(value, 1);
		}

		// Returns the DateTime resulting from adding a fractional number of
		// minutes to this DateTime. The result is computed by rounding the
		// fractional number of minutes given by value to the nearest
		// millisecond, and adding that interval to this DateTime. The
		// value argument is permitted to be negative.
		//
		DateTime AddMinutes(double value) {
			return Add(value, MillisPerMinute);
		}

		// Returns the DateTime resulting from adding the given number of
		// months to this DateTime. The result is computed by incrementing
		// (or decrementing) the year and month parts of this DateTime by
		// months months, and, if required, adjusting the day part of the
		// resulting date downwards to the last day of the resulting month in the
		// resulting year. The time-of-day part of the result is the same as the
		// time-of-day part of this DateTime.
		//
		// In more precise terms, considering this DateTime to be of the
		// form y / m / d + t, where y is the
		// year, m is the month, d is the day, and t is the
		// time-of-day, the result is y1 / m1 / d1 + t,
		// where y1 and m1 are computed by adding months months
		// to y and m, and d1 is the largest value less than
		// or equal to d that denotes a valid day in month m1 of year
		// y1.
		//
		DateTime AddMonths(int months) {
			//if (months < -120000 || months > 120000) throw new ArgumentOutOfRangeException("months", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadMonths"));
			//Contract.EndContractBlock();

			int y, m, d = 0;
			// ?????
			//GetDatePart(out y, out m, out d);
			GetDatePart(y, m, d);

			int i = m - 1 + months;
			if (i >= 0) {
				m = i % 12 + 1;
				y = y + i / 12;
			}
			else {
				m = 12 + (i + 1) % 12;
				y = y + (i - 11) / 12;
			}
			if (y < 1 || y > 9999) {
				//throw new ArgumentOutOfRangeException("months", Environment.GetResourceString("ArgumentOutOfRange_DateArithmetic"));
			}
			int days = DaysInMonth(y, m);
			if (d > days) d = days;
			return DateTime((DateTime_t)(DateToTicks(y, m, d) + InternalTicks() % TicksPerDay) | InternalKind());
		}

		// Returns the DateTime resulting from adding a fractional number of
		// seconds to this DateTime. The result is computed by rounding the
		// fractional number of seconds given by value to the nearest
		// millisecond, and adding that interval to this DateTime. The
		// value argument is permitted to be negative.
		//
		DateTime AddSeconds(double value) {
			return Add(value, MillisPerSecond);
		}


		// Returns the DateTime resulting from adding a fractional number of
		// time units to this DateTime.
		DateTime AddTicks(long long value) {
			long long ticks = InternalTicks();
			//if (value > MaxTicks - ticks || value < MinTicks - ticks) {
			//	throw new ArgumentOutOfRangeException("value", Environment.GetResourceString("ArgumentOutOfRange_DateArithmetic"));
			//}
			//return DateTime((unsigned long long )(ticks + value) | InternalKind);
		    return DateTime((DateTime_t)(ticks + value) | InternalKind());
		}

		// Returns the DateTime resulting from adding the given number of
		// years to this DateTime. The result is computed by incrementing
		// (or decrementing) the year part of this DateTime by value
		// years. If the month and day of this DateTime is 2/29, and if the
		// resulting year is not a leap year, the month and day of the resulting
		// DateTime becomes 2/28. Otherwise, the month, day, and time-of-day
		// parts of the result are the same as those of this DateTime.
		//
		DateTime AddYears(int value) {
			//			if (value < -10000 || value > 10000) throw new ArgumentOutOfRangeException("years", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadYears"));
			//			Contract.EndContractBlock();
			return AddMonths(value * 12);
		}

		int GetYear() const {
			return GetDatePart(DatePartYear);
		}


		// Returns the month part of this DateTime. The returned value is an
		// integer between 1 and 12.
		//
		int GetMonth() const {
			return GetDatePart(DatePartMonth);
		}

		int GetDay() const {
			return GetDatePart(DatePartDay);
		}

		int GetHour() const {
			return (int)((InternalTicks() / TicksPerHour) % 24);
		}


		// Returns the minute part of this DateTime. The returned value is
		// an integer between 0 and 59.
		//
		int GetMinute() const {
			return (int)((InternalTicks() / TicksPerMinute) % 60);
		}


		// Returns the second part of this DateTime. The returned value is
		// an integer between 0 and 59.
		//
		int GetSecond() const {
			return (int)((InternalTicks() / TicksPerSecond) % 60);
		}

		// Returns the millisecond part of this DateTime. The returned value
		// is an integer between 0 and 999.
		//
		int GetMillisecond() const {
			return (int)((InternalTicks() / TicksPerMillisecond) % 1000);
		}


		static DateTime Now();

		static DateTime Today() {
			DateTime TimeNow = DateTime::Now();

			return DateTime(TimeNow.GetYear(), TimeNow.GetMonth(), TimeNow.GetDay());
		}

		std::string ToLongDateString() {
			return this->ToString("%A %B, %#d %Y");
		}

		std::string ToLongTimeString() {
			return this->ToString("%#T %p");
		}

		std::string ToShortDateString() {
			return this->ToString("%#D");
		}

		std::string ToShortTimeString() {
			return this->ToString("%#R %p");
		}

		static bool IsLeapYear(int year);


		void GetTM(struct std::tm* out) const
		{
			int y, m, d = 0;
			GetDatePart(y, m, d);
		//	d = GetDay();

			out->tm_year = y - 1900;
			out->tm_mon = m - 1;
			out->tm_mday = d;

			out->tm_hour = GetHour();
			out->tm_min = GetMinute();
			out->tm_sec = GetSecond();

			// using thwe whatever the data we have, make it a complete time
			std::time_t time_temp = std::mktime(out);

			// load the complete time information back into the structure
			localtime_s(out, &time_temp);
		}

		int DayOfYear() {
			struct std::tm out;

			GetTM(&out);

			return out.tm_yday;
		}

		//EDayOfWeek DayOfWeek() const {
		//	struct std::tm out;

		//	GetTM(&out);

		//	return static_cast<EDayOfWeek>(out.tm_wday);
		//}

		EDayOfWeek DayOfWeek() const {
			return (EDayOfWeek)((InternalTicks() / TicksPerDay + 1) % 7);
		}


		virtual std::string ToString() const
		{
			std::string Output = std::to_string(GetMonth()) + "/" + std::to_string(GetDay()) + "/" + std::to_string(GetYear());
			return Output;
		}

		/// <summary>
		///  This function takes formatting string used by C++ put_time fuction, and not C# formatting string
		/// </summary>
		/// <param name="Format"></param>
		/// <returns></returns>
		std::string ToString(const char* Format) const
		{
			std::stringstream ss;
			struct std::tm t;

			this->GetTM(&t);

			ss << std::put_time(&t, Format);

			return ss.str();
		}


		//static int Compare(const DateTime t1, const DateTime t2);
		// Compares two DateTime values, returning an integer that indicates
		// their relationship.
		//
		static int Compare(DateTime t1, DateTime t2) {
			long long ticks1 = t1.InternalTicks();
			long long ticks2 = t2.InternalTicks();
			if (ticks1 > ticks2) return 1;
			if (ticks1 < ticks2) return -1;
			return 0;
		}

		
		bool operator!=(const DateTime t1) const {
			return (Compare(*this, t1) != 0);
		}

		friend bool operator==(const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();

			return ticks1 == ticks2;
		}

		friend bool operator !=(const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();

			return ticks1 != ticks2;
		}

		friend bool operator <=(const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();
			return ticks1 <= ticks2;
		}

		friend bool operator >=(const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();
			return ticks1 >= ticks2;
		}


		friend bool operator < (const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();
			return ticks1 < ticks2;
		}

		friend bool operator > (const DateTime& d1, const DateTime& d2) {
			long long ticks1 = d1.InternalTicks();
			long long ticks2 = d2.InternalTicks();
			return ticks1 > ticks2;
		}

		// Returns the number of days in the month given by the year and
		// month arguments.
		//
		static int DaysInMonth(int year, int month) {
			assert(month >= 1 && month <= 12);

			//if (month < 1 || month > 12) throw new ArgumentOutOfRangeException("month", Environment.GetResourceString("ArgumentOutOfRange_Month"));
			//Contract.EndContractBlock();
			// IsLeapYear checks the year argument
			//int* days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			int* days = (IsLeapYear(year) ? &DaysToMonth366[0] : &DaysToMonth365[0]);
			return days[month] - days[month - 1];
		}

		// Returns the tick count corresponding to the given year, month, and day.
		// Will check the if the parameters are valid.
		static long long DateToTicks(int year, int month, int day) {
			if (year >= 1 && year <= 9999 && month >= 1 && month <= 12) {
				//int[] days = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
				int* days = (IsLeapYear(year) ? &DaysToMonth366[0] : &DaysToMonth365[0]);
				if (day >= 1 && day <= days[month] - days[month - 1]) {
					int y = year - 1;
					int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
					return n * TicksPerDay;
				}
			}

			return 0;
			//throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("ArgumentOutOfRange_BadYearMonthDay"));
		}

		// Return the tick count corresponding to the given hour, minute, second.
// Will check the if the parameters are valid.
		static long long TimeToTicks(int hour, int minute, int second)
		{
			long long Ticks = 0;
			//TimeSpan.TimeToTicks is a family access function which does no error checking, so
			//we need to put some error checking out here.
			if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60)
			{
				Ticks = (TimeSpan::TimeToTicks(hour, minute, second));
			}

			//throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("ArgumentOutOfRange_BadHourMinuteSecond"));
			return Ticks;
		}

	private:
		DateTime Add(double value, int scale) {
			long long millis = (long long)(value * scale + (value >= 0 ? 0.5 : -0.5));
			//			if (millis <= -MaxMillis || millis >= MaxMillis)
			//				throw new ArgumentOutOfRangeException("value", Environment.GetResourceString("ArgumentOutOfRange_AddValue"));
			long long TotalAdd = millis * TicksPerMillisecond;
			return AddTicks(TotalAdd);
		}


		int GetDatePart(int part) const;

		// Exactly the same as GetDatePart(int part), except computing all of
		// year/month/day rather than just one of them.  Used when all three
		// are needed rather than redoing the computations for each.
		void GetDatePart(int& year, int& month, int& day) const;

		long long InternalTicks() const {
			return (long long)(dateData & TicksMask);
		}

		unsigned long long InternalKind() {
			return (dateData & FlagsMask);
		}


	};
} //namespace CSharpDateTime

