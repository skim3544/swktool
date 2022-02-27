// DateTime.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"

#include <iostream>
#include <ostream>

#include "DateTime.h"

using namespace std;
using namespace  CSharpDateTime;

 int DateTime::DaysToMonth365[] = {
	0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

  int DateTime::DaysToMonth366[] = {
	0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

static ostream& operator<<(ostream& os, const CSharpDateTime::DateTime& dt)
{
	os << dt.ToString();
	return os;
}



bool DateTime::IsLeapYear(int year) {
	if (year % 4 != 0)
		return false;
	if (year % 100 != 0)
		return true;
	if (year % 400 == 0)
		return true;
	else
		return false;
}


DateTime DateTime::Now() {
	time_t timeNow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	tm now;

	localtime_s(&now, &timeNow);

	return DateTime(now.tm_year + 1900, now.tm_mon+1, now.tm_mday, now.tm_hour, now.tm_min, now.tm_sec);
}

int DateTime::GetDatePart(int part) const {
	long long ticks = InternalTicks();
	// n = number of days since 1/1/0001
	int n = (int)(ticks / TicksPerDay);
	// y400 = number of whole 400-year periods since 1/1/0001
	int y400 = n / DaysPer400Years;
	// n = day number within 400-year period
	n -= y400 * DaysPer400Years;
	// y100 = number of whole 100-year periods within 400-year period
	int y100 = n / DaysPer100Years;
	// Last 100-year period has an extra day, so decrement result if 4
	if (y100 == 4) y100 = 3;
	// n = day number within 100-year period
	n -= y100 * DaysPer100Years;
	// y4 = number of whole 4-year periods within 100-year period
	int y4 = n / DaysPer4Years;
	// n = day number within 4-year period
	n -= y4 * DaysPer4Years;
	// y1 = number of whole years within 4-year period
	int y1 = n / DaysPerYear;
	// Last year has an extra day, so decrement result if 4
	if (y1 == 4) y1 = 3;
	// If year was requested, compute and return it
	if (part == DatePartYear) {
		return y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
	}
	// n = day number within year
	n -= y1 * DaysPerYear;
	// If day-of-year was requested, return it
	if (part == DatePartDayOfYear) return n + 1;
	// Leap year calculation looks different from IsLeapYear since y1, y4,
	// and y100 are relative to year 1, not year 0
	bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
	//int[] days = leapYear ? DaysToMonth366 : DaysToMonth365;
	int* days = (leapYear ? &DaysToMonth366[0] : &DaysToMonth365[0]);
	// All months have less than 32 days, so n >> 5 is a good conservative
	// estimate for the month
	int m = (n >> 5) + 1;
	// m = 1-based month number
	while (n >= days[m]) m++;
	// If month was requested, return it
	if (part == DatePartMonth) return m;
	// Return 1-based day-of-month
	return n - days[m - 1] + 1;
}


void DateTime::GetDatePart(int& year, int& month, int& day) const
{
	long long ticks = InternalTicks();
	// n = number of days since 1/1/0001
	int n = (int)(ticks / TicksPerDay);
	// y400 = number of whole 400-year periods since 1/1/0001
	int y400 = n / DaysPer400Years;
	// n = day number within 400-year period
	n -= y400 * DaysPer400Years;
	// y100 = number of whole 100-year periods within 400-year period
	int y100 = n / DaysPer100Years;
	// Last 100-year period has an extra day, so decrement result if 4
	if (y100 == 4) y100 = 3;
	// n = day number within 100-year period
	n -= y100 * DaysPer100Years;
	// y4 = number of whole 4-year periods within 100-year period
	int y4 = n / DaysPer4Years;
	// n = day number within 4-year period
	n -= y4 * DaysPer4Years;
	// y1 = number of whole years within 4-year period
	int y1 = n / DaysPerYear;
	// Last year has an extra day, so decrement result if 4
	if (y1 == 4) y1 = 3;
	// compute year
	year = y400 * 400 + y100 * 100 + y4 * 4 + y1 + 1;
	// n = day number within year
	n -= y1 * DaysPerYear;
	// dayOfYear = n + 1;
	// Leap year calculation looks different from IsLeapYear since y1, y4,
	// and y100 are relative to year 1, not year 0
	bool leapYear = y1 == 3 && (y4 != 24 || y100 == 3);
	//int[] days = leapYear ? DaysToMonth366 : DaysToMonth365;
	int* days = (leapYear ? &DaysToMonth366[0] : &DaysToMonth365[0]);
	// All months have less than 32 days, so n >> 5 is a good conservative
	// estimate for the month
	int m = (n >> 5) + 1;
	// m = 1-based month number
	while (n >= days[m]) m++;
	// compute month and day
	month = m;
	day = n - days[m - 1] + 1;
}


/*
int DateTime::Compare(const DateTime t1, const DateTime t2) {
	unsigned long long lt1 = t1.GetAsBigNumber();
	unsigned long long lt2 = t2.GetAsBigNumber();

	if (lt1 < lt2) {
		return -1;
	}
	else if (lt2 > lt1) {
		return 1;
	}
	else {
		return 0;
	}
}
*/
