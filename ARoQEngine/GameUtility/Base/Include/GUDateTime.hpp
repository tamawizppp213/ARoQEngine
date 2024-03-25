//////////////////////////////////////////////////////////////////////////////////
///             @file   GUDateTime.hpp
///             @brief  ���t�Ɋւ���Utility�N���X�ł�. 
///                     ���ݎ����̎擾���s���܂�.
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
	*         �@�@ This class represents date and times between Midnight 00:00:00, January 1 and
	*             Midnight 23:59:59:999999, December 31, in the Gregorian calendar,
	*****************************************************************************/
	struct DateTime
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Local PC�̃^�C���]�[���ƉĎ��Ԑݒ���l�����Č��ݎ�����Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		static DateTime Now();

		/*----------------------------------------------------------------------
		*  @brief : Local PC�̃^�C���]�[���ƉĎ��Ԑݒ���l�������Ɍ��ݎ�����Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		static DateTime UTCNow();

		/*----------------------------------------------------------------------
		*  @brief : ���݂̓��ɂ����擾���܂�
		/*----------------------------------------------------------------------*/
		static DateTime Today() { return Now().GetDate(); }

		/*----------------------------------------------------------------------
		*  @brief : �N�ƌ����炻�̌��̓�����Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		static int32 DaysInMonth(const int32 year, const int32 month);

		/*----------------------------------------------------------------------
		*  @brief : �N���炻�̔N�̓�����Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		static int32 DaysInYear(const int32 year){ return IsLeapYear(year) ? 366 : 365; }

		/*----------------------------------------------------------------------
		*  @brief : ���邤�N���ǂ���
		/*----------------------------------------------------------------------*/
		static bool IsLeapYear(const int32 year);

		/*----------------------------------------------------------------------
		*  @brief : �w����������������ǂ���
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
		*  @brief : ���ɂ��݂̂��擾����. �����̕�����00:00:00.000�ɐ؂�̂Ă���
		/*----------------------------------------------------------------------*/
		__forceinline DateTime GetDate() const { return DateTime(_ticks - (_ticks % TimeSpan::TICKS_PER_DAY)); }

		void GetDate(int32& year, int32& month, int32& day) const;

		/*----------------------------------------------------------------------
		*  @brief : ��N�̒��ł̌o�ߓ������擾���� 
		/*----------------------------------------------------------------------*/
		int32 GetDayOfYear() const;

		/*----------------------------------------------------------------------
		*  @brief : ��DateTime�ɂ�����j�����擾����
		/*----------------------------------------------------------------------*/
		__forceinline DayOfWeek GetDayOfWeek() const { return static_cast<DayOfWeek>((_ticks / TimeSpan::TICKS_PER_DAY) % 7); }

		/*----------------------------------------------------------------------
		*  @brief : ��������Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline MonthOfYear GetMonthOfYear() const { return static_cast<MonthOfYear>(GetMonth()); }

		/*----------------------------------------------------------------------
		*  @brief : ���ɂ���1�`31�܂ł͈̔͂Ŏ擾����
		/*----------------------------------------------------------------------*/
		int32 GetDay() const;

		/*----------------------------------------------------------------------
		*  @brief : ���݂̌����擾����
		/*----------------------------------------------------------------------*/
		int32 GetMonth() const;

		/*----------------------------------------------------------------------
		*  @brief : ����̂����̌��ݎ��Ԃ��擾����
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetHour() const { return (int32)((_ticks / TimeSpan::TICKS_PER_HOUR) % 24); }

		__forceinline int32 GetHour12() const { auto hour = GetHour(); return hour > 12 ? hour - 12 : hour; }

		/*----------------------------------------------------------------------
		*  @brief : �����擾���� (0�`59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetMinute() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MINUTE) % 60); }

		/*----------------------------------------------------------------------
		*  @brief : �b���擾���� (0�`59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetSecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_SECOND) % 60); }

		/*----------------------------------------------------------------------
		*  @brief : millisecond���擾���� (0�`59)
		/*----------------------------------------------------------------------*/
		__forceinline int32 GetMillisecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MILLISECOND) % 1000); }

		/*----------------------------------------------------------------------
		*  @brief : �����̓��t���擾����Ticks���擾����
		/*----------------------------------------------------------------------*/
		__forceinline int64 GetTicks() const { return _ticks; }

		/*----------------------------------------------------------------------
		*  @brief : ���߂܂ł�
		/*----------------------------------------------------------------------*/
		__forceinline bool IsAM() const { return GetHour() < 12; }

		/*----------------------------------------------------------------------
		*  @brief : ���߂��ォ
		/*----------------------------------------------------------------------*/
		__forceinline bool IsPM() const { return GetHour() >= 12; }

		/*----------------------------------------------------------------------
		*  @brief : ���ݎ��������Ƃ�, �����E�X�����擾����
		*           �I���O4713�N1/1�ɂ���̓���
		/*----------------------------------------------------------------------*/
		__forceinline double GetJulianDay() const {return 1721425.5 + double(_ticks / TimeSpan::TICKS_PER_DAY) + GetTimeOfDay().GetTotalDays();}

		/*----------------------------------------------------------------------
		*  @brief : ���ݎ��������Ƃ�, �C�������E�X�����擾����
		            http://manome.world.coocan.jp/astrology/julian.html
		*           1858�N11/17��0�����_�̎���
		/*----------------------------------------------------------------------*/
		__forceinline double GetModifiedJulianDay() const { return (GetJulianDay() - 2400000.5); }

		/*----------------------------------------------------------------------
		*  @brief : 1970/1/1����̕b�����擾����
		/*----------------------------------------------------------------------*/
		__forceinline int64 ToUnixTimestampsSecond() const {return (_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND;}

		/*----------------------------------------------------------------------
		*  @brief : 1970/1/1����̕b�����擾����
		/*----------------------------------------------------------------------*/
		__forceinline double ToUnixTimestampsSecondDecimal () const { return double(_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND; }
		/*----------------------------------------------------------------------
		*  @brief : ���̓��t�̌ߑO0������̌o�ߎ��Ԃ��擾���܂��B
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan GetTimeOfDay() const { return TimeSpan(_ticks % TimeSpan::TICKS_PER_DAY); }
		
		/*----------------------------------------------------------------------
		*  @brief : �����E�X���O���S���I��ɂ����ɂ����擾����
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromJulianDay(const double julianDay)
		{
			return DateTime((int64)((julianDay - 1721425.5) * TimeSpan::TICKS_PER_DAY));
		}
		/*----------------------------------------------------------------------
		*  @brief : Unix���Ԃ���O���S���I��ɂ����ɂ����擾����
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromUnixTimestamp(const int64 unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(unixTime * TimeSpan::TICKS_PER_SECOND);
		}
		/*----------------------------------------------------------------------
		*  @brief : Unix���Ԃ���O���S���I��ɂ����ɂ����擾����
		/*----------------------------------------------------------------------*/
		__forceinline static DateTime CreateFromUnixTimestampDecimal(const double unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(int64(unixTime * TimeSpan::TICKS_PER_SECOND));
		}
		/*----------------------------------------------------------------------
		*  @brief : �������Ԃ�
		/*----------------------------------------------------------------------*/
		gu::string ToString() const;
		gu::string ToString(const char8* format) const;

		__forceinline DateTime operator +(const TimeSpan& right) const { return DateTime(_ticks + right.GetTicks()); }
		
		__forceinline DateTime& operator+=(const TimeSpan& right) { _ticks += right.GetTicks(); return *this; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵�����t�Ɨ^����ꂽ���ɂ��̊Ԃ̃^�C���X�p����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline TimeSpan operator -(const DateTime& right) const { return TimeSpan(_ticks - right._ticks); }

		__forceinline DateTime operator -(const TimeSpan& right) const { return DateTime(_ticks - right.GetTicks()); }

		__forceinline DateTime& operator -=(const TimeSpan& right) { _ticks -= right.GetTicks(); return *this; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵�����t�Ɠ������ǂ���, �Ⴄ���ǂ���
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