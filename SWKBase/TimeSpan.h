#pragma once

namespace CSharpDateTime
{
	const long    TicksPerMillisecond = 10000;
	const double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
	const long TicksPerSecond = (TicksPerMillisecond * 1000);   // 10,000,000
	const double SecondsPerTick = 1.0 / TicksPerSecond;         // 0.0001

	const long long TicksPerMinute = TicksPerSecond * 60;
	const double MinutesPerTick = 1.0 / TicksPerMinute; // 1.6666666666667e-9

	const long long TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
	const double HoursPerTick = 1.0 / TicksPerHour; // 2.77777777777777778e-11
	const long long TicksPerDay = TicksPerHour * 24;
	const double DaysPerTick = 1.0 / TicksPerDay; // 1.1574074074074074074e-12

	const long MillisPerSecond = 1000;
	const long MillisPerMinute = MillisPerSecond * 60; //     60,000
	const long MillisPerHour = MillisPerMinute * 60;   //  3,600,000
	const long MillisPerDay = MillisPerHour * 24;      // 86,400,000


	class TimeSpan {

	private:
		long long _ticks;

	public:
		TimeSpan(long long ticks) {
			_ticks = ticks;
		}

		TimeSpan(int hours, int minutes, int seconds) {
			_ticks = TimeToTicks(hours, minutes, seconds);
		}

		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
		{
			long totalMilliSeconds = ((long)days * 3600 * 24 + (long)hours * 3600 + (long)minutes * 60 + seconds) * 1000 + milliseconds;
			//if (totalMilliSeconds > MaxMilliSeconds || totalMilliSeconds < MinMilliSeconds)
			//	throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("Overflow_TimeSpanTooLong"));
			_ticks = (long long)totalMilliSeconds * TicksPerMillisecond;
		}

		static TimeSpan Zero;

		//static TimeSpan Zero = TimeSpan.TimeSpan(0);

		long long Ticks() {
			return _ticks;
		}

		int Days() {
			return (int)(_ticks / TicksPerDay);
		}

		int Hours() {
			return (int)((_ticks / TicksPerHour) % 24);
		}

		int Milliseconds() {
			return (int)((_ticks / TicksPerMillisecond) % 1000);
		}

		int Minutes() {
			return (int)((_ticks / TicksPerMinute) % 60);
		}

		int Seconds() {
			return (int)((_ticks / TicksPerSecond) % 60);
		}

		/*

		*/
		double TotalDays() const {
			return ((double)_ticks) * DaysPerTick;
		}

		double TotalHours() const {
			return (double)_ticks * HoursPerTick;
		}

		double TotalMilliseconds() {

			double temp = (double)_ticks * MillisecondsPerTick;
			/*
			if (temp > MaxMilliSeconds)
				return (double)MaxMilliSeconds;

			if (temp < MinMilliSeconds)
				return (double)MinMilliSeconds;
				*/
			return temp;
		}

		double TotalMinutes() {
			return (double)_ticks * MinutesPerTick;
		}

		double TotalSeconds() {
			return (double)_ticks * SecondsPerTick;
		}

		TimeSpan Add(TimeSpan ts) const {
			long long result = _ticks + ts._ticks;
			// Overflow if signs of operands was identical and result's
			// sign was opposite.
			// >> 63 gives the sign bit (either 64 1's or 64 0's).

			//if ((_ticks >> 63 == ts._ticks >> 63) && (_ticks >> 63 != result >> 63))
			//	throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
			return TimeSpan(result);
		}

		static int Compare(TimeSpan t1, TimeSpan t2) {
			if (t1._ticks > t2._ticks) return 1;
			if (t1._ticks < t2._ticks) return -1;
			return 0;
		}


		static TimeSpan Interval(double value, int scale) {
			//			if (Double.IsNaN(value))
			//				throw new ArgumentException(Environment.GetResourceString("Arg_CannotBeNaN"));
			//			Contract.EndContractBlock();
			double tmp = value * scale;
			double millis = tmp + (value >= 0 ? 0.5 : -0.5);
			//			if ((millis > Int64.MaxValue / TicksPerMillisecond) || (millis < Int64.MinValue / TicksPerMillisecond))
			//				throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
			return TimeSpan((long long)millis * TicksPerMillisecond);
		}

		static long long TimeToTicks(int hour, int minute, int second);

		static TimeSpan FromDays(double value) {
			return Interval(value, MillisPerDay);
		}

		static TimeSpan FromHours(double value) {
			return Interval(value, MillisPerHour);
		}

		TimeSpan Duration() {
			//			if (Ticks == TimeSpan.MinValue.Ticks)
			//				throw new OverflowException(Environment.GetResourceString("Overflow_Duration"));

			return TimeSpan(_ticks >= 0 ? _ticks : -_ticks);
		}

		static TimeSpan FromMilliseconds(double value) {
			return Interval(value, 1);
		}

		static TimeSpan FromMinutes(double value) {
			return Interval(value, MillisPerMinute);
		}

		TimeSpan Negate() {
			//if (Ticks == TimeSpan.MinValue.Ticks)
			//	throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));

			return TimeSpan(-_ticks);
		}


		TimeSpan Subtract(TimeSpan ts) const {
			long long result = _ticks - ts._ticks;
			// Overflow if signs of operands was different and result's
			// sign was opposite from the first argument's sign.
			// >> 63 gives the sign bit (either 64 1's or 64 0's).
//			if ((_ticks >> 63 != ts._ticks >> 63) && (_ticks >> 63 != result >> 63))
//				throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
			return TimeSpan(result);
		}

		static TimeSpan FromTicks(long value) {
			return TimeSpan(value);
		}

		TimeSpan operator-(const TimeSpan& t) {
			//			if (t._ticks == TimeSpan.MinValue._ticks)
			//				throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
			return TimeSpan(-t._ticks);
		}

		TimeSpan operator+(const TimeSpan& t) {
			return t;
		}



		friend bool operator==(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks == t2._ticks;
		}

		friend bool operator !=(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks != t2._ticks;
		}

		friend bool operator <(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks < t2._ticks;
		}

		friend bool operator <=(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks <= t2._ticks;
		}

		friend bool operator >(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks > t2._ticks;
		}

		friend bool operator >=(const TimeSpan& t1, const TimeSpan& t2) {
			return t1._ticks >= t2._ticks;
		}
	};
};
