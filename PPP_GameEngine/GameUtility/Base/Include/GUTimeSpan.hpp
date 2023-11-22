//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTimeSpan.hpp
///             @brief  temp
///             @author toide (reference : ue5)
///             @date   2023/11/21 1:25:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TIME_SPAN_HPP
#define GU_TIME_SPAN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			   GUTimeSpan
	*************************************************************************//**
	*  @class     GUTimeSpan
	*  @brief     Time span
	*****************************************************************************/
	struct TimeSpan
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		double GetTotalDays        () const { return ((double)_ticks / TICKS_PER_DAY); }
		double GetTotalHours       () const {return ((double)_ticks / TICKS_PER_HOUR); }
		double GetTotalMinutes     () const {return ((double)_ticks / TICKS_PER_MINUTE); }
		double GetTotalSeconds     () const {return ((double)_ticks / TICKS_PER_SECOND);}
		double GetTotalMilliseconds() const { return ((double)_ticks / TICKS_PER_MILLISECOND); }
		double GetTotalMicroseconds() const { return ((double)_ticks / TICKS_PER_MICROSECOND); };

		static TimeSpan FromSeconds(const double seconds);
		static TimeSpan FromMinutes(const double minutes);
		static TimeSpan FromHours  (const double hours);
		static TimeSpan FromDays   (const double days);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static constexpr int64 MAX_TICKS             =  9223372036854775807;
		static constexpr int64 MIN_TICKS             = -9223372036854775807 - 1;
		static constexpr int64 NANOSECONDS_PER_TICK  = 100;           // 1 tick = 0.1ƒÊs’PˆÊ‚ÅŒv‘ª
		static constexpr int64 TICKS_PER_WEEK        = 6048000000000; 
		static constexpr int64 TICKS_PER_DAY         = 864000000000;
		static constexpr int64 TICKS_PER_HOUR        = 36000000000;
		static constexpr int64 TICKS_PER_MINUTE      = 600000000;
		static constexpr int64 TICKS_PER_SECOND      = 10000000;
		static constexpr int64 TICKS_PER_MILLISECOND = 10000;
		static constexpr int64 TICKS_PER_MICROSECOND = 10;            // 10 tick
		static constexpr int64 TICKS_PER_YEAR        = 365 * TICKS_PER_DAY;

		/*----------------------------------------------------------------------
		*							Getter
		/*----------------------------------------------------------------------*/
		__forceinline int32  GetFractionMillisecond() const {return (int32)((_ticks % TICKS_PER_SECOND) / TICKS_PER_MILLISECOND);}
		__forceinline int32  GetFractionNanosecond() const {return (int32)((_ticks % TICKS_PER_SECOND) * NANOSECONDS_PER_TICK);}
		__forceinline int32  GetFractionTicks() const { return (int32)(_ticks % TICKS_PER_SECOND); }
		__forceinline int32  GetDays   () const { return (int32)(_ticks / TICKS_PER_DAY); }
		__forceinline int32  GetHours  () const { return (int32)((_ticks / TICKS_PER_HOUR  ) % 24); }
		__forceinline int32  GetMinutes() const { return (int32)((_ticks / TICKS_PER_MINUTE) % 60); }
		__forceinline int32  GetSeconds() const { return (int32)((_ticks / TICKS_PER_SECOND) % 60); }
		__forceinline int64  GetTicks  () const { return _ticks; }

		/*----------------------------------------------------------------------
		*				Get Duration from start in unit of 100 nanoseconds
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan GetDuration() const { return TimeSpan(_ticks >= 0 ? _ticks : -_ticks); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		TimeSpan() : _ticks(0) {};

		explicit TimeSpan(const int64 ticks);

		TimeSpan(const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(0, hours, minutes, seconds, 0);
		}
		
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(days, hours, minutes, seconds, 0);
		}
		
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano)
		{
			Assign(days, hours, minutes, seconds, fractionNano);
		}

#pragma region Operator
		/*----------------------------------------------------------------------
		*   Return the result of adding the given time span to this time span
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator+(const TimeSpan& other) const { return TimeSpan(_ticks + other._ticks); }
		
		/*----------------------------------------------------------------------
		*   Return the result of adding time span to this time span
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan& operator+=(const TimeSpan& right) { _ticks += right._ticks; return *this; }

		/*----------------------------------------------------------------------
		*   Return the inverse of this time span. 
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator-() const { return TimeSpan(-_ticks); }

		/*----------------------------------------------------------------------
		*   Return the result of subtracting the given time span from this time span.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator-(const TimeSpan& right) const { return TimeSpan(_ticks - right._ticks); }

		/*----------------------------------------------------------------------
		*   Subtract the given time span from this time span
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan& operator-=(const TimeSpan& right) { _ticks -= right._ticks; return *this; }

		/*----------------------------------------------------------------------
		*   Return the result of multiplying the this time span with the given scalar.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator*(const double scalar) const
		{
			return TimeSpan((int64)((double)_ticks * scalar));
		}

		/*----------------------------------------------------------------------
		*   Return the result of multiplying the this time span with the given scalar.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan& operator*=(const double scalar)
		{
			_ticks = (int64)((double)_ticks * scalar);
			return *this;
		}

		/*----------------------------------------------------------------------
		*   Return the result of dividing the this time span with the given scalar.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator / (const double scalar) const { return TimeSpan((int64)((double)_ticks / scalar)); }

		/*----------------------------------------------------------------------
		*   Return the result of dividing the this time span with the given scalar.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan& operator/=(const double scalar)
		{
			_ticks = (int64)((double)_ticks / scalar);
			return *this;
		}

		/*----------------------------------------------------------------------
		*   Return the result of calculating the modulus of this time span with another time span.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator% (const TimeSpan& right) const { return TimeSpan(_ticks % right._ticks); }
		
		/*----------------------------------------------------------------------
		*   Return the result of calculating the modulus of this time span with another time span.
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan& operator%= (const TimeSpan& right)
		{
			_ticks = _ticks % right._ticks;
			return *this;
		}

		bool operator==(const TimeSpan& right) const { return (_ticks == right._ticks); }
		bool operator!=(const TimeSpan& right) const { return (_ticks != right._ticks); }
		bool operator> (const TimeSpan& right) const { return (_ticks >  right._ticks); }
		bool operator>=(const TimeSpan& right) const { return (_ticks >= right._ticks); }
		bool operator< (const TimeSpan& right) const { return (_ticks <  right._ticks); }
		bool operator<=(const TimeSpan& right) const { return (_ticks <= right._ticks); }

#pragma endregion Operator


	protected:
		void Assign(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano);

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		int64 _ticks = 0;
	};

}
#endif