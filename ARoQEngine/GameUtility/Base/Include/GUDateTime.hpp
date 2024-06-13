//////////////////////////////////////////////////////////////////////////////////
/// @file   GUDateTime.hpp
/// @brief  日付に関するUtilityクラスです. @n 
///         現在時刻の取得を行います.
/// @author toide
/// @date   2023/11/21 0:45:14
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
	/*!***********************************************************************
	*  @brief      曜日
	**************************************************************************/
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

	/*!***********************************************************************
	*  @brief      月
	**************************************************************************/
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
	****************************************************************************/
	/*  @brief     日付や時刻を取得するクラスです. @n
	*              このクラスは、グレゴリオ暦の1月1日午前0時00分00秒から12月31日午前0時59分59秒999999までの日付と時刻を表します@n
	*              Nowを使って現在時刻を持つDateTimeを作成し, ToString()で日付や時刻の文字列を返すことが出来ます. @n
	*              数値データを取得したい場合はGet...で取得可能です. 
	*  @note       最小単位は0.1μsです
	*****************************************************************************/
	struct DateTime
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      ローカルPCに対する現在のタイムゾーン(指定地域が示す時刻)を考慮して現在時刻を返します.
		*  @param[in]  void
		*  @return     DateTime : 現在時刻を示す_ticksを持った構造体
		**************************************************************************/
		static DateTime Now();

		/*!***********************************************************************
		*  @brief      ローカルPCのタイムゾーンを考慮せずに世界標準時を返します.
		*  @param[in]  void
		*  @return     DateTime : 現在時刻を示す_ticksを持った構造体
		**************************************************************************/
		static DateTime UTCNow();

		/*!***********************************************************************
		*  @brief      現在の日にちを取得します. DateTimeの各時刻部分は全て0となります.
		*  @param[in]  void
		*  @return     DateTime : 現在時刻を示す_ticksを持った構造体
		**************************************************************************/
		static DateTime Today() { return Now().GetDate(); }

		/*!***********************************************************************
		*  @brief      年と月からその月における日数を返します. 
		*  @param[in]  const int32 : 西暦
		*  @param[in]  const month : 月
		*  @return     int32 : 4月は30日などの, 指定月に対する日数
		**************************************************************************/
		static int32 DaysInMonth(const int32 year, const int32 month);

		/*!***********************************************************************
		*  @brief      指定年が何日あるかを返します
		*  @param[in]  const int32 : 西暦
		*  @return     int32 : 366日 or 365日
		**************************************************************************/
		static int32 DaysInYear(const int32 year){ return IsLeapYear(year) ? 366 : 365; }

		/*!***********************************************************************
		*  @brief      うるう年かどうか
		*  @param[in]  const int32 : 西暦
		*  @return     bool : trueであればうるう年
		**************************************************************************/
		static bool IsLeapYear(const int32 year);

		/*!***********************************************************************
		*  @brief     指定日時がそれぞれ正しい範囲内にあるかを調べます 
		*  @param[in] const int32 : 西暦 (1～9999)
		*  @param[in] const int32 : 月 (1～12)
		*  @param[in] const int32 : 日 (1-DayInMonth)
		*  @param[in] const int32 : 時間 (0～23)
		*  @param[in] const int32 : 分 (0～59)
		*  @param[in] const int32 : 秒 (0～59)
		*  @param[in] const int32 : ミリ秒 (0～999)
		*  @return     bool : trueであれば全て正しい範囲内
		**************************************************************************/
		static bool Validate(const int32 year, const int32 month, const int32 day,
			const int32 hour, const int32 minute, const int32 second, const int32 millisecond);
		
		#pragma endregion Public Function

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      日にちのみを取得する. 時刻の部分は00:00:00.000に切り捨てられる
		*  @return     DateTime : 現在時刻を示す_ticksを持った構造体
		**************************************************************************/
		__forceinline DateTime GetDate() const { return DateTime(_ticks - (_ticks % TimeSpan::TICKS_PER_DAY)); }

		/*!***********************************************************************
		*  @brief      日にちをそれぞれのint32型に割り振る
		*  @param[out] int32 年
		*  @param[out] int32 月
		*  @param[out] int32 日
		*  @return     void
		**************************************************************************/
		void GetDate(int32& year, int32& month, int32& day) const;

		/*!***********************************************************************
		*  @brief      一年の中での経過日数を取得する 
		*  @return     int32 : 一年のうちの経過日数
		**************************************************************************/
		int32 GetDayOfYear() const;

		/*!***********************************************************************
		*  @brief      現DateTimeにおける曜日を取得する
		*  @return     DayOfWeek : 曜日
		**************************************************************************/
		__forceinline DayOfWeek GetDayOfWeek() const { return static_cast<DayOfWeek>((_ticks / TimeSpan::TICKS_PER_DAY) % 7); }

		/*!***********************************************************************
		*  @brief      何月かを返す
		*  @return     MonthOfYear : 月
		**************************************************************************/
		__forceinline MonthOfYear GetMonthOfYear() const { return static_cast<MonthOfYear>(GetMonth()); }

		/*!***********************************************************************
		*  @brief      日にちを1～31までの範囲で取得する
		*  @return     int32 : 日
		**************************************************************************/
		int32 GetDay() const;

		/*!***********************************************************************
		*  @brief      日にちを1～31までの範囲で取得する
		*  @return     int32 : 日
		**************************************************************************/
		int32 GetMonth() const;

		/*!***********************************************************************
		*  @brief      一日のうちの現在時間を取得する (0～23)
		*  @return     int32 : 現在の時間 (0～23)
		**************************************************************************/
		__forceinline int32 GetHour() const { return (int32)((_ticks / TimeSpan::TICKS_PER_HOUR) % 24); }

		/*!***********************************************************************
		*  @brief      一日のうちの現在時間を取得する (0～11)
		*  @return     int32 : 時間
		**************************************************************************/
		__forceinline int32 GetHour12() const { auto hour = GetHour(); return hour > 12 ? hour - 12 : hour; }

		/*!***********************************************************************
		*  @brief      分を取得する (0～59)
		*  @return     int32 : 分 (0～59)
		**************************************************************************/
		__forceinline int32 GetMinute() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MINUTE) % 60); }

		/*!***********************************************************************
		*  @brief       秒を取得する (0～59)
		*  @return     int32 : 秒 (0～59)
		**************************************************************************/
		__forceinline int32 GetSecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_SECOND) % 60); }

		/*!***********************************************************************
		*  @brief      millisecondを取得する (0～59)
		*  @return     int32 : ミリ秒 (0～59)
		**************************************************************************/
		__forceinline int32 GetMillisecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MILLISECOND) % 1000); }

		/*!***********************************************************************
		*  @brief      今日の日付を取得するTicksを取得する
		*  @return     int64 : 経過時刻 (0.1μs単位)
		**************************************************************************/
		__forceinline int64 GetTicks() const { return _ticks; }

		/*!***********************************************************************
		*  @brief      午前中かどうかを返します
		**************************************************************************/
		__forceinline bool IsAM() const { return GetHour() < 12; }

		/*!***********************************************************************
		*  @brief      午後かどうかを返します
		**************************************************************************/
		__forceinline bool IsPM() const { return GetHour() >= 12; }

		/*!***********************************************************************
		*  @brief      現在時刻をもとに, ユリウス日(紀元前4713年1/1からの日数)を取得する
		**************************************************************************/
		__forceinline double GetJulianDay() const {return 1721425.5 + double(_ticks / TimeSpan::TICKS_PER_DAY) + GetTimeOfDay().GetTotalDays();}

		/*!***********************************************************************
		*  @brief      現在時刻をもとに, 修正ユリウス日を取得する @n
		*              http://manome.world.coocan.jp/astrology/julian.html
		*              1858年11/17日0時時点の時刻
		**************************************************************************/
		__forceinline double GetModifiedJulianDay() const { return (GetJulianDay() - 2400000.5); }

		/*!***********************************************************************
		*  @brief    1970/1/1からの秒数を取得する (int64型)
		**************************************************************************/
		__forceinline int64 ToUnixTimestampsSecond() const {return (_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND;}

		/*!***********************************************************************
		*  @brief    1970/1/1からの秒数を取得する (double型)
		**************************************************************************/
		__forceinline double ToUnixTimestampsSecondDecimal () const { return double(_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND; }
		
		/*!***********************************************************************
		*  @brief    午前0時からの経過時間を取得します。
		**************************************************************************/
		__forceinline TimeSpan GetTimeOfDay() const { return TimeSpan(_ticks % TimeSpan::TICKS_PER_DAY); }
		
		/*!***********************************************************************
		*  @brief    ユリウス暦からグレゴリオ暦による日にちを取得する
		**************************************************************************/
		__forceinline static DateTime CreateFromJulianDay(const double julianDay)
		{
			return DateTime((int64)((julianDay - 1721425.5) * TimeSpan::TICKS_PER_DAY));
		}

		/*!***********************************************************************
		*  @brief    Unix時間からグレゴリオ暦による日にちを取得する
		**************************************************************************/
		__forceinline static DateTime CreateFromUnixTimestamp(const int64 unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(unixTime * TimeSpan::TICKS_PER_SECOND);
		}

		/*!***********************************************************************
		*  @brief    Unix時間からグレゴリオ暦による日にちを取得する
		**************************************************************************/
		__forceinline static DateTime CreateFromUnixTimestampDecimal(const double unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(int64(unixTime * TimeSpan::TICKS_PER_SECOND));
		}

		/*!***********************************************************************
		*  @brief   文字列を%Y.%m.%d-%H.%M.%Sのフォーマットで返す
		**************************************************************************/
		gu::string ToString() const;
		gu::string ToString(const char* format) const;

		__forceinline DateTime operator +(const TimeSpan& right) const { return DateTime(_ticks + right.GetTicks()); }
		
		__forceinline DateTime& operator+=(const TimeSpan& right) { _ticks += right.GetTicks(); return *this; }

		/*!***********************************************************************
		*  @brief   指定した日付と与えられた日にちの間のタイムスパンを返す
		**************************************************************************/
		__forceinline TimeSpan operator -(const DateTime& right) const { return TimeSpan(_ticks - right._ticks); }

		__forceinline DateTime operator -(const TimeSpan& right) const { return DateTime(_ticks - right.GetTicks()); }

		__forceinline DateTime& operator -=(const TimeSpan& right) { _ticks -= right.GetTicks(); return *this; }

		#pragma endregion Public Property

		#pragma region Public Operator 
		/*----------------------------------------------------------------------
		*  @brief : 指定した日付と同じかどうか, 違うかどうか
		/*----------------------------------------------------------------------*/
		__forceinline bool operator==(const DateTime& right) const { return (_ticks == right._ticks); }
		__forceinline bool operator!=(const DateTime& right) const { return (_ticks != right._ticks); }
		__forceinline bool operator> (const DateTime& right) const { return (_ticks > right._ticks); }
		__forceinline bool operator>=(const DateTime& right) const { return (_ticks >= right._ticks); }
		__forceinline bool operator< (const DateTime& right) const { return (_ticks < right._ticks); }
		__forceinline bool operator<=(const DateTime& right) const { return (_ticks <= right._ticks); }
		#pragma endregion Public Operator

		#pragma region Public Constructor and Destructor
		/*!***********************************************************************
		*  @brief   Zero initialize constructor
		**************************************************************************/
		DateTime() : _ticks(0) {};

		/*!***********************************************************************
		*  @brief  Creates and initializes a new instance with the specified number of ticks
		**************************************************************************/
		DateTime(const int64 ticks) : _ticks(ticks) {};

		/*!***********************************************************************
		*  @brief   Decide the ticks from year month .... milliseconds
		**************************************************************************/
		DateTime(const int32 year, const int32 month, const int32 day,
			const int32 hour = 0, const int32 minute = 0, const int32 second = 0, const int32 millisecond = 0);
		#pragma endregion Public Constructor and Destructor

	protected:
		#pragma region Protected Function

		#pragma endregion Protected Function

		#pragma region Protected Property
		static constexpr int32  DAY_COUNT_PER_MONTH[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
		static constexpr int32  DAY_COUNT_TO_MONTH [] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
		static constexpr char* SHORT_DAY_NAMES[]     = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
		static constexpr char* LONG_DAY_NAMES[]      = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
		static constexpr char* SHORT_MONTH_NAMES[]   = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
		static constexpr char* LONG_MONTH_NAMES[]    = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
		#pragma endregion Protected Property

	private:
		#pragma region Private Function

		#pragma endregion Private Function

		#pragma region Private Property

		int64 _ticks = 0;

		#pragma endregion Private Property
	};

}
#endif