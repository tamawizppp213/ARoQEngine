//////////////////////////////////////////////////////////////////////////////////
///             @file   DateTime.cpp
///             @brief  temp
///             @author toide
///             @date   2023/11/21 1:19:00
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUDateTime.hpp"
#include "../Include/GUAssert.hpp"
#include <stdexcept>
#include <math.h>
#include <vector>

// platform毎の処理
#if defined(_WIN32) || defined(_WIN64)
	#include <Windows.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	template <typename ... Args>
	gu::string Format(const gu::string& fmt, Args ... args)
	{
		size_t len = std::snprintf(nullptr, 0, fmt.CString(), args ...);
		std::vector<char> buf(len + 1);
		std::snprintf(&buf[0], len + 1, fmt.CString(), args ...);
		return gu::string(&buf[0], &buf[0] + len);
	}
}

#pragma region Constructor and Destructor 
DateTime::DateTime(const int32 year, const int32 month, const int32 day,
	const int32 hour, const int32 minute, const int32 second, const int32 millisecond)
{
	if (!Validate(year, month, day, hour, minute, second, millisecond))
	{
		throw std::runtime_error("Invalid date values\n");
	}

	int32 totalDays = 0;

	/*----------------------------------------------------------------------
	*   2月以降であれば2/29日分を調整する
	*----------------------------------------------------------------------*/
	if ((month > 2) && IsLeapYear(year))
	{
		++totalDays;
	}

	/*----------------------------------------------------------------------
	*   今年分, 今月分はまだカウントしないようにする
	*----------------------------------------------------------------------*/
	const int adjustYear  = year - 1;
	const int adjustMonth = month - 1;

	/*----------------------------------------------------------------------
	*   日数カウント
	*----------------------------------------------------------------------*/
	totalDays += adjustYear * 365;
	totalDays += adjustYear / 4;  // うるう年の調整分
	totalDays -= year / 100;      // うるう年の調整分 ...100年毎に例外対応
	totalDays += year / 400;      // ...400年毎の例外対応
	totalDays += DAY_COUNT_TO_MONTH[adjustMonth];
	totalDays += day - 1;         // 今日の分をのぞく

	_ticks = totalDays * TimeSpan::TICKS_PER_DAY
		+ hour         * TimeSpan::TICKS_PER_HOUR
		+ minute       * TimeSpan::TICKS_PER_MINUTE
		+ second       * TimeSpan::TICKS_PER_SECOND
		+ millisecond  * TimeSpan::TICKS_PER_MILLISECOND;
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       Day
****************************************************************************/
/* @fn        DateTime DateTime::Now()
*
*  @brief     Local PCのタイムゾーンと夏時間設定を考慮して現在時刻を返します.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
DateTime DateTime::Now()
{
	int32 year = 0, month  = 0, day    = 0, dayOfWeek   = 0;
	int32 hour = 0, minute = 0, second = 0, millisecond = 0;

	// Platform毎にわける処理分割したい
#if defined(_WIN32) || defined(_WIN64)
	{
		SYSTEMTIME localTime = {};
		GetLocalTime(&localTime);

		year        = localTime.wYear;
		month       = localTime.wMonth;
		day         = localTime.wDay;
		dayOfWeek   = localTime.wDayOfWeek;
		hour        = localTime.wHour;
		minute      = localTime.wMinute;
		second      = localTime.wSecond;
		millisecond = localTime.wMilliseconds;
	}
#endif

	return DateTime(year, month, day, hour, minute, second, millisecond);
}

/****************************************************************************
*                       UTCNow
****************************************************************************/
/* @fn        DateTime DateTime::UTCNow()
*
*  @brief     Local PCのタイムゾーンと夏時間設定を考慮しないで現在時刻を返します.
*
*  @param[in] void
*
*  @return    DateTime
*****************************************************************************/
DateTime DateTime::UTCNow()
{
	int32 year = 0, month  = 0, day    = 0, dayOfWeek   = 0;
	int32 hour = 0, minute = 0, second = 0, millisecond = 0;

	// Platform毎にわける処理分割したい
#if defined(_WIN32) || defined(_WIN64)
	{
		SYSTEMTIME systemTime = {};
		GetSystemTime(&systemTime);

		year        = systemTime.wYear;
		month       = systemTime.wMonth;
		day         = systemTime.wDay;
		dayOfWeek   = systemTime.wDayOfWeek;
		hour        = systemTime.wHour;
		minute      = systemTime.wMinute;
		second      = systemTime.wSecond;
		millisecond = systemTime.wMilliseconds;
	}
#endif

	return DateTime(year, month, day, hour, minute, second, millisecond);
}

/****************************************************************************
*                       DaysInMonth
****************************************************************************/
/* @fn        int32 DateTime::DaysInMonth(const int32 year, const int32 month)
*
*  @brief     年と月からその月の日数を返します. 
*
*  @param[in] const int32 year
*  @param[in] const int32 month
*
*  @return    void
*****************************************************************************/
int32 DateTime::DaysInMonth(const int32 year, const int32 month)
{
	Check((1 <= month) && (month <= 12));

	if ((month == 2) && IsLeapYear(year)) { return 29; }

	return DAY_COUNT_PER_MONTH[month];
}

/****************************************************************************
*                       IsLeapYear
****************************************************************************/
/* @fn        bool DateTime::IsLeapYear(const int32 year)
*
*  @brief     うるう年かどうかを判定します. (true : うるう年)
*
*  @param[in] const int32 year
*
*  @return    bool : true -> うるう年, false -> not 
*****************************************************************************/
bool DateTime::IsLeapYear(const int32 year)
{
	if ((year % 4) == 0)
	{
		return (((year % 100) != 0) || ((year % 400) == 0));
	}

	return false;
}

/****************************************************************************
*							Validate
****************************************************************************/
/* @fn        bool DateTime::Validate(const int32 year, const int32 month, const int32 day,
	const int32 hour, const int32 minute, const int32 second, const int32 millisecond)
*
*  @brief     値の設定が有効かどうかを判定します
*
*  @return    bool : true -> 有効, false -> 範囲外の入力を受け付けた
*****************************************************************************/
bool DateTime::Validate(const int32 year, const int32 month, const int32 day,
	const int32 hour, const int32 minute, const int32 second, const int32 millisecond)
{
	return (year >= 1)     && (year <= 9999) &&
		(month >= 1)       && (month <= 12) &&
		(day >= 1)         && (day <= DaysInMonth(year, month)) &&
		(hour >= 0)        && (hour <= 23) &&
		(minute >= 0)      && (minute <= 59) &&
		(second >= 0)      && (second <= 59) &&
		(millisecond >= 0) && (millisecond <= 999);
}

/****************************************************************************
*							GetDate
****************************************************************************/
/* @fn        int32 DateTime::GetDay() const
*
*  @brief     _tickから算出される日にちを取得します
* 
*  @param[out] int32 year
*  @param[out] int32 month
*  @param[out] int32 day
*
*  @return     void
*****************************************************************************/
void DateTime::GetDate(int32& year, int32& month, int32& day) const
{
	// Based on FORTRAN code in:
	// Fliegel, H. F. and van Flandern, T. C.,
	// Communications of the ACM, Vol. 11, No. 10 (October 1968).

	int32 i = 0, j = 0, k = 0, l = 0, n = 0;

	l = (int32)(__floor(GetDate().GetJulianDay() + 0.5)) + 68569;
	n = 4 * l / 146097;
	l = l - (146097 * n + 3) / 4;
	i = 4000 * (l + 1) / 1461001;
	l = l - 1461 * i / 4 + 31;
	j = 80 * l / 2447;
	k = l - 2447 * j / 80;
	l = j / 11;
	j = j + 2 - 12 * l;
	i = 100 * (n - 49) + i + l;

	year  = i;
	month = j;
	day   = k;
}

/****************************************************************************
*							GetDay
****************************************************************************/
/* @fn        int32 DateTime::GetDay() const
*
*  @brief     一年の中での経過日数を取得する (1/1から)
*
*  @return    int32 : dayOfYear
*****************************************************************************/
int32 DateTime::GetDayOfYear() const
{
	int32 year = 0, month = 0, day = 0;
	GetDate(year, month, day);

	for (int32 currentMonth = 1; currentMonth < month; ++currentMonth)
	{
		day += DaysInMonth(year, currentMonth);
	}

	return day;
}

/****************************************************************************
*							GetDay
****************************************************************************/
/* @fn        int32 DateTime::GetDay() const
*
*  @brief     本日の日付を返します
*
*  @return    int32 : day
*****************************************************************************/
int32 DateTime::GetDay() const
{
	int32 year = 0, month = 0, day = 0;
	GetDate(year, month, day);
	return day;
}

/****************************************************************************
*							GetMonth
****************************************************************************/
/* @fn        int32 DateTime::GetMonth() const
*
*  @brief     本日の月を返します
*
*  @return    int32 : month
*****************************************************************************/
int32 DateTime::GetMonth() const
{
	int32 year = 0, month = 0, day = 0;
	GetDate(year, month, day);
	return month;
}

gu::string DateTime::ToString() const
{
	return ToString("%Y.%m.%d-%H.%M.%S");
}

gu::string DateTime::ToString(const char* format) const
{
	gu::string result;

	int32 year = 0, month = 0, day = 0;
	GetDate(year, month, day);

	while (*format != ('\0'))
	{
		if ((*format == '%') && (*++format != '\0'))
		{
			switch (*format)
			{
				case 'a': result += IsAM() ? "am" : "pm"; break;
				case 'A': result += IsAM() ? "AM" : "PM"; break;
				case 'D': result += Format("%03i", GetDayOfYear()); break;
				case 'd': result += Format("%02i", day); break;
				case 'm': result += Format("%02i", month); break;
				case 'y': result += Format("%02i", year % 100); break;
				case 'Y': result += Format("%04i", year); break;
				case 'h': result += Format("%02i", GetHour12()); break;
				case 'H': result += Format("%02i", GetHour()); break;
				case 'M': result += Format("%02i", GetMinute()); break;
				case 'S': result += Format("%02i", GetSecond()); break;
				case 's': result += Format("%03i", GetMillisecond()); break;
				default:
					result += *format;
			}
		}
		else
		{
			result += *format;
		}

		++format;
	}

	return result;
}
#pragma endregion Main Function