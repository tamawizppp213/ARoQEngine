//////////////////////////////////////////////////////////////////////////////////
///             @file   GUDateTime.hpp
///             @brief  日付に関するUtilityクラスです. 
///                     現在時刻の取得を行います.
///             @author toide
///             @date   2023/11/21 0:45:14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_DATE_TIME_HPP
#define GU_DATE_TIME_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUTimeSpan.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	enum class DayOfWeek
	{
		Monday    = 0,
		Tuesday   = 1,
		Wednesday = 2,
		Thursday  = 3,
		Friday    = 4,
		Saturday  = 5,
		Sunday    = 6
	};

	enum class MonthOfYear
	{
		January   = 1,
		Fabruary  = 2,
		March     = 3,
		April     = 4,
		May       = 5,
		June      = 6,
		July      = 7,
		August    = 8,
		September = 9,
		Octorber  = 10,
		November  = 11,
		December  = 12
	};

	/****************************************************************************
	*				  			   DateTime
	*************************************************************************//**
	*  @class     DateTime
	*  @brief     Show date and time
	*         　　 This class represents date and times between Midnight 00:00:00, January 1 and
	*             Midnight 23:59:59:999999, December 31, in the Gregorian calendar,
	*****************************************************************************/
	struct DateTime
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Local PCのタイムゾーンと夏時間設定を考慮して現在時刻を返します.
		/*----------------------------------------------------------------------*/
		static DateTime Now();

		/*----------------------------------------------------------------------
		*  @brief : Local PCのタイムゾーンと夏時間設定を考慮せずに現在時刻を返します.
		/*----------------------------------------------------------------------*/
		static DateTime UTCNow();

		/*----------------------------------------------------------------------
		*  @brief : 現在の日にちを取得します
		/*----------------------------------------------------------------------*/
		static DateTime Today() { return Now().GetDate(); }

		/*----------------------------------------------------------------------
		*  @brief : 年と月からその月の日数を返します.
		/*----------------------------------------------------------------------*/
		static int32 DaysInMonth(const int32 year, const int32 month);

		/*----------------------------------------------------------------------
		*  @brief : 年からその年の日数を返します.
		/*----------------------------------------------------------------------*/
		static int32 DaysInYear(const int32 year){ return IsLeapYear(year) ? 366 : 365; }

		/*----------------------------------------------------------------------
		*  @brief : うるう年かどうか
		/*----------------------------------------------------------------------*/
		static bool IsLeapYear(const int32 year);

		/*----------------------------------------------------------------------
		*  @brief : 指定日時が正しいかどうか
		*           year   : 1 - 9999
		*           month  : 1 - 12
		*           day    : 1 - DayInMonth
		*           hour   : 0 - 23
		*           minute : 0 - 59
		*           second : 0 - 59
		*           millisecond : 0 - 999
		/*----------------------------------------------------------------------*/
		static bool Validate(const int32 year, const int32 month, const int32 day,
			const int32 hour, const int32 minute, const int32 second, const int32 millisecond);
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : 日にちのみを取得する. 自国の部分は00:00:00.000に切り捨てられる
		/*----------------------------------------------------------------------*/
		__forceinline DateTime GetDate() const { return DateTime(_ticks - (_ticks % TimeSpan::TICKS_PER_DAY)); }

		void GetDate(int32& year, int32& month, int32& day) const;

		/*----------------------------------------------------------------------
		*  @brief : 一年の中での経過日数を取得する 
		/*----------------------------------------------------------------------*/
		int32 GetDayOfYear() const;

		/*----------------------------------------------------------------------
		*  @brief : 現DateTimeにおける曜日を取得する
		/*----------------------------------------------------------------------*/
		__forceinline DayOfWeek GetDayOfWeek() const { return static_cast<DayOfWeek>((_ticks / TimeSpan::TICKS_PER_DAY) % 7); }

		/*----------------------------------------------------------------------
		*  @brief : 何月かを返す
		/*----------------------------------------------------------------------*/
		__forceinline MonthOfYear GetMonthOfYear() const { return static_cast<MonthOfYear>(GetMonth()); }

		/*----------------------------------------------------------------------
		*  @brief : 日にちを1～31までの範囲で取得する
		/*----------------------------------------------------------------------*/
		int32 GetDay() const;

		/*----------------------------------------------------------------------
		*  @brief : 現在の月を取得する
		/*----------------------------------------------------------------------*/
		int32 GetMonth() const;

		/*----------------------------------------------------------------------
		*  @brief : 一日のうちの現在時間を取得する
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetHour() const { return (int32)((_ticks / TimeSpan::TICKS_PER_HOUR) % 24); }

		__forceinline int32 GetHour12() const { auto hour = GetHour(); return hour > 12 ? hour - 12 : hour; }

		/*----------------------------------------------------------------------
		*  @brief : 分を取得する (0～59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetMinute() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MINUTE) % 60); }

		/*----------------------------------------------------------------------
		*  @brief : 秒を取得する (0～59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetSecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_SECOND) % 60); }

		/*----------------------------------------------------------------------
		*  @brief : millisecondを取得する (0～59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetMillisecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MILLISECOND) % 1000); }

		/*----------------------------------------------------------------------
		*  @brief : 今日の日付を取得するTicksを取得する
		/*----------------------------------------------------------------------*/
		__forceinline int64 GetTicks() const { return _ticks; }

		/*----------------------------------------------------------------------
		*  @brief : 正午までか
		/*----------------------------------------------------------------------*/
		__forceinline bool IsAM() const { return GetHour() < 12; }

		/*----------------------------------------------------------------------
		*  @brief : 正午より後か
		/*----------------------------------------------------------------------*/
		__forceinline bool IsPM() const { return GetHour() >= 12; }

		/*----------------------------------------------------------------------
		*  @brief : 現在時刻をもとに, ユリウス日を取得する
		*           紀元前4713年1/1にからの日数
		/*----------------------------------------------------------------------*/
		__forceinline double GetJulianDay() const {return 1721425.5 + double(_ticks / TimeSpan::TICKS_PER_DAY) + GetTimeOfDay().GetTotalDays();}

		/*----------------------------------------------------------------------
		*  @brief : 現在時刻をもとに, 修正ユリウス日を取得する
		            http://manome.world.coocan.jp/astrology/julian.html
		*           1858年11/17日0時時点の時刻
		/*----------------------------------------------------------------------*/
		__forceinline double GetModifiedJulianDay() const { return (GetJulianDay() - 2400000.5); }

		/*----------------------------------------------------------------------
		*  @brief : 1970/1/1からの秒数を取得する
		/*----------------------------------------------------------------------*/
		__forceinline int64 ToUnixTimestampsSecond() const {return (_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND;}

		/*----------------------------------------------------------------------
		*  @brief : 1970/1/1からの秒数を取得する
		/*----------------------------------------------------------------------*/
		__forceinline double ToUnixTimestampsSecondDecimal () const { return double(_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND; }
		/*----------------------------------------------------------------------
		*  @brief : この日付の午前0時からの経過時間を取得します。
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan GetTimeOfDay() const { return TimeSpan(_ticks % TimeSpan::TICKS_PER_DAY); }
		
		/*----------------------------------------------------------------------
		*  @brief : ユリウス暦からグレゴリオ暦による日にちを取得する
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromJulianDay(const double julianDay)
		{
			return DateTime((int64)((julianDay - 1721425.5) * TimeSpan::TICKS_PER_DAY));
		}
		/*----------------------------------------------------------------------
		*  @brief : Unix時間からグレゴリオ暦による日にちを取得する
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromUnixTimestamp(const int64 unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(unixTime * TimeSpan::TICKS_PER_SECOND);
		}
		/*----------------------------------------------------------------------
		*  @brief : Unix時間からグレゴリオ暦による日にちを取得する
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromUnixTimestampDecimal(const double unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(int64(unixTime * TimeSpan::TICKS_PER_SECOND));
		}
		/*----------------------------------------------------------------------
		*  @brief : 文字列を返す
		/*----------------------------------------------------------------------*/
		gu::string ToString() const;
		gu::string ToString(const char8* format) const;

		__forceinline DateTime operator +(const TimeSpan& right) const { return DateTime(_ticks + right.GetTicks()); }
		
		__forceinline DateTime& operator+=(const TimeSpan& right) { _ticks += right.GetTicks(); return *this; }

		/*----------------------------------------------------------------------
		*  @brief : 指定した日付と与えられた日にちの間のタイムスパンを返す
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator -(const DateTime& right) const { return TimeSpan(_ticks - right._ticks); }

		__forceinline DateTime operator -(const TimeSpan& right) const { return DateTime(_ticks - right.GetTicks()); }

		__forceinline DateTime& operator -=(const TimeSpan& right) { _ticks -= right.GetTicks(); return *this; }

		/*----------------------------------------------------------------------
		*  @brief : 指定した日付と同じかどうか, 違うかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool operator==(const DateTime& right) const { return (_ticks == right._ticks); }
		__forceinline bool operator!=(const DateTime& right) const { return (_ticks != right._ticks); }
		__forceinline bool operator> (const DateTime& right) const { return (_ticks >  right._ticks); }
		__forceinline bool operator>=(const DateTime& right) const { return (_ticks >= right._ticks); }
		__forceinline bool operator< (const DateTime& right) const { return (_ticks <  right._ticks); }
		__forceinline bool operator<=(const DateTime& right) const { return (_ticks <= right._ticks); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Zero initialize constructor
		/*----------------------------------------------------------------------*/
		DateTime() : _ticks(0) {};

		/*----------------------------------------------------------------------
		*  @brief : Creates and initializes a new instance with the specified number of ticks
		/*----------------------------------------------------------------------*/
		DateTime(const int64 ticks) : _ticks(ticks) {};

		/*----------------------------------------------------------------------
		*  @brief : Decide the ticks from year month .... milliseconds
		/*----------------------------------------------------------------------*/
		DateTime(const int32 year, const int32 month, const int32 day,
			const int32 hour = 0, const int32 minute = 0, const int32 second = 0, const int32 millisecond = 0);


	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		static constexpr int32  DAY_COUNT_PER_MONTH[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		static constexpr int32  DAY_COUNT_TO_MONTH [] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
		static constexpr char8* SHORT_DAY_NAMES[]     = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
		static constexpr char8* LONG_DAY_NAMES[]      = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		static constexpr char8* SHORT_MONTH_NAMES[]   = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		static constexpr char8* LONG_MONTH_NAMES[]    = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

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