//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTimeSpan.hpp
///             @brief  時間計測用のクラスです. 
///                     2点間の日時の差を表現する際に使用するクラスであり, 2000年1月1日12:00:00と2000年1月2日18:00:00のTimeSpanは30.0 * TICHS_PER_HOUR
///                     TimeSpanの単位はtickであり, 0.1μs単位で計測されます. 
///                     TimeSpanは差を表現するだけなので, 実際の日付の取得はDateTimeを使用してください. 
///                     DateTimeと組み合わせて, 日付と時刻に基づく演算を行うことが出来ます. 
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
#include "GUString.hpp"
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
	*  @brief     時間計測用のクラスです. 
	              2点間の日時の差を表現する際に使用するクラスであり, 2000年1月1日12:00:00と2000年1月2日18:00:00のTimeSpanは30.0 * TICHS_PER_HOUR
	              TimeSpanの単位はtickであり, 0.1μs単位で計測されます. 
				  TimeSpanは差を表現するだけなので, 実際の日付の取得はDateTimeを使用してください. 
				  DateTimeと組み合わせて, 日付と時刻に基づく演算を行うことが出来ます. 
	*****************************************************************************/
	struct TimeSpan
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*		 　時刻の差を各単位で表現します.
		/*----------------------------------------------------------------------*/
		__forceinline double GetTotalDays        () const { return ((double)_ticks / TICKS_PER_DAY); }
		__forceinline double GetTotalHours       () const {return ((double)_ticks / TICKS_PER_HOUR); }
		__forceinline double GetTotalMinutes     () const {return ((double)_ticks / TICKS_PER_MINUTE); }
		__forceinline double GetTotalSeconds     () const {return ((double)_ticks / TICKS_PER_SECOND);}
		__forceinline double GetTotalMilliseconds() const { return ((double)_ticks / TICKS_PER_MILLISECOND); }
		__forceinline double GetTotalMicroseconds() const { return ((double)_ticks / TICKS_PER_MICROSECOND); };

		/*----------------------------------------------------------------------
		*		 　Create the time span
		/*----------------------------------------------------------------------*/
		static TimeSpan CreateFromSeconds(const double seconds);
		static TimeSpan CreateFromMinutes(const double minutes);
		static TimeSpan CreateFromHours  (const double hours);
		static TimeSpan CreateFromDays   (const double days);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static constexpr int64 MAX_TICKS             =  9223372036854775807;      // TimeSpanで計測可能な最大(未来)のtick = 0.1μs単位 
		static constexpr int64 MIN_TICKS             = -9223372036854775807 - 1;  // TimeSpanで計測可能な最小(過去)のtick
		static constexpr int64 NANOSECONDS_PER_TICK  = 100;                       // 1 tick = 0.1μs単位で計測
		static constexpr int64 TICKS_PER_WEEK        = 6048000000000;             // 1週間にかかるtick数
		static constexpr int64 TICKS_PER_DAY         = 864000000000;              // 1日にかかるtick数
		static constexpr int64 TICKS_PER_HOUR        = 36000000000;               // 1時間にかかるtick数
		static constexpr int64 TICKS_PER_MINUTE      = 600000000;                 // 1分にかかるtick数
		static constexpr int64 TICKS_PER_SECOND      = 10000000;                  // 1秒にかかるtick数
		static constexpr int64 TICKS_PER_MILLISECOND = 10000;                     // 1msにかかるtick数
		static constexpr int64 TICKS_PER_MICROSECOND = 10;                        // 1μsにかかるtick数
		static constexpr int64 TICKS_PER_YEAR        = 365 * TICKS_PER_DAY;       // 1年にかかるtick数

		/*----------------------------------------------------------------------
		*							Getter
		/*----------------------------------------------------------------------*/
		__forceinline int32  GetFractionMicrosecond() const { return (int32)((_ticks & TICKS_PER_SECOND) / TICKS_PER_MICROSECOND); }
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

		__forceinline bool IsZero() const { return (_ticks == 0); }

		/*----------------------------------------------------------------------
		*	@brief : Return the string representation of this time span using default format
		*            p[d.]hh:mm:ss.fff
		/*----------------------------------------------------------------------*/
		gu::string ToString() const;

		gu::string ToString(const gu::char8* format) const;

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