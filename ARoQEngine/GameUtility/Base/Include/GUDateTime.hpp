//////////////////////////////////////////////////////////////////////////////////
/// @file   GUDateTime.hpp
/// @brief  ���t�Ɋւ���Utility�N���X�ł�. @n 
///         ���ݎ����̎擾���s���܂�.
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
	*  @brief      �j��
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
	*  @brief      ��
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
	/*  @brief     ���t�⎞�����擾����N���X�ł�. @n
	*              ���̃N���X�́A�O���S���I���1��1���ߑO0��00��00�b����12��31���ߑO0��59��59�b999999�܂ł̓��t�Ǝ�����\���܂�@n
	*              Now���g���Č��ݎ���������DateTime���쐬��, ToString()�œ��t�⎞���̕������Ԃ����Ƃ��o���܂�. @n
	*              ���l�f�[�^���擾�������ꍇ��Get...�Ŏ擾�\�ł�. 
	*  @note       �ŏ��P�ʂ�0.1��s�ł�
	*****************************************************************************/
	struct DateTime
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      ���[�J��PC�ɑ΂��錻�݂̃^�C���]�[��(�w��n�悪��������)���l�����Č��ݎ�����Ԃ��܂�.
		*  @param[in]  void
		*  @return     DateTime : ���ݎ���������_ticks���������\����
		**************************************************************************/
		static DateTime Now();

		/*!***********************************************************************
		*  @brief      ���[�J��PC�̃^�C���]�[�����l�������ɐ��E�W������Ԃ��܂�.
		*  @param[in]  void
		*  @return     DateTime : ���ݎ���������_ticks���������\����
		**************************************************************************/
		static DateTime UTCNow();

		/*!***********************************************************************
		*  @brief      ���݂̓��ɂ����擾���܂�. DateTime�̊e���������͑S��0�ƂȂ�܂�.
		*  @param[in]  void
		*  @return     DateTime : ���ݎ���������_ticks���������\����
		**************************************************************************/
		static DateTime Today() { return Now().GetDate(); }

		/*!***********************************************************************
		*  @brief      �N�ƌ����炻�̌��ɂ����������Ԃ��܂�. 
		*  @param[in]  const int32 : ����
		*  @param[in]  const month : ��
		*  @return     int32 : 4����30���Ȃǂ�, �w�茎�ɑ΂������
		**************************************************************************/
		static int32 DaysInMonth(const int32 year, const int32 month);

		/*!***********************************************************************
		*  @brief      �w��N���������邩��Ԃ��܂�
		*  @param[in]  const int32 : ����
		*  @return     int32 : 366�� or 365��
		**************************************************************************/
		static int32 DaysInYear(const int32 year){ return IsLeapYear(year) ? 366 : 365; }

		/*!***********************************************************************
		*  @brief      ���邤�N���ǂ���
		*  @param[in]  const int32 : ����
		*  @return     bool : true�ł���΂��邤�N
		**************************************************************************/
		static bool IsLeapYear(const int32 year);

		/*!***********************************************************************
		*  @brief     �w����������ꂼ�ꐳ�����͈͓��ɂ��邩�𒲂ׂ܂� 
		*  @param[in] const int32 : ���� (1�`9999)
		*  @param[in] const int32 : �� (1�`12)
		*  @param[in] const int32 : �� (1-DayInMonth)
		*  @param[in] const int32 : ���� (0�`23)
		*  @param[in] const int32 : �� (0�`59)
		*  @param[in] const int32 : �b (0�`59)
		*  @param[in] const int32 : �~���b (0�`999)
		*  @return     bool : true�ł���ΑS�Đ������͈͓�
		**************************************************************************/
		static bool Validate(const int32 year, const int32 month, const int32 day,
			const int32 hour, const int32 minute, const int32 second, const int32 millisecond);
		
		#pragma endregion Public Function

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      ���ɂ��݂̂��擾����. �����̕�����00:00:00.000�ɐ؂�̂Ă���
		*  @return     DateTime : ���ݎ���������_ticks���������\����
		**************************************************************************/
		__forceinline DateTime GetDate() const { return DateTime(_ticks - (_ticks % TimeSpan::TICKS_PER_DAY)); }

		/*!***********************************************************************
		*  @brief      ���ɂ������ꂼ���int32�^�Ɋ���U��
		*  @param[out] int32 �N
		*  @param[out] int32 ��
		*  @param[out] int32 ��
		*  @return     void
		**************************************************************************/
		void GetDate(int32& year, int32& month, int32& day) const;

		/*!***********************************************************************
		*  @brief      ��N�̒��ł̌o�ߓ������擾���� 
		*  @return     int32 : ��N�̂����̌o�ߓ���
		**************************************************************************/
		int32 GetDayOfYear() const;

		/*!***********************************************************************
		*  @brief      ��DateTime�ɂ�����j�����擾����
		*  @return     DayOfWeek : �j��
		**************************************************************************/
		__forceinline DayOfWeek GetDayOfWeek() const { return static_cast<DayOfWeek>((_ticks / TimeSpan::TICKS_PER_DAY) % 7); }

		/*!***********************************************************************
		*  @brief      ��������Ԃ�
		*  @return     MonthOfYear : ��
		**************************************************************************/
		__forceinline MonthOfYear GetMonthOfYear() const { return static_cast<MonthOfYear>(GetMonth()); }

		/*!***********************************************************************
		*  @brief      ���ɂ���1�`31�܂ł͈̔͂Ŏ擾����
		*  @return     int32 : ��
		**************************************************************************/
		int32 GetDay() const;

		/*!***********************************************************************
		*  @brief      ���ɂ���1�`31�܂ł͈̔͂Ŏ擾����
		*  @return     int32 : ��
		**************************************************************************/
		int32 GetMonth() const;

		/*!***********************************************************************
		*  @brief      ����̂����̌��ݎ��Ԃ��擾���� (0�`23)
		*  @return     int32 : ���݂̎��� (0�`23)
		**************************************************************************/
		__forceinline int32 GetHour() const { return (int32)((_ticks / TimeSpan::TICKS_PER_HOUR) % 24); }

		/*!***********************************************************************
		*  @brief      ����̂����̌��ݎ��Ԃ��擾���� (0�`11)
		*  @return     int32 : ����
		**************************************************************************/
		__forceinline int32 GetHour12() const { auto hour = GetHour(); return hour > 12 ? hour - 12 : hour; }

		/*!***********************************************************************
		*  @brief      �����擾���� (0�`59)
		*  @return     int32 : �� (0�`59)
		**************************************************************************/
		__forceinline int32 GetMinute() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MINUTE) % 60); }

		/*!***********************************************************************
		*  @brief       �b���擾���� (0�`59)
		*  @return     int32 : �b (0�`59)
		**************************************************************************/
		__forceinline int32 GetSecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_SECOND) % 60); }

		/*!***********************************************************************
		*  @brief      millisecond���擾���� (0�`59)
		*  @return     int32 : �~���b (0�`59)
		**************************************************************************/
		__forceinline int32 GetMillisecond() const { return (int32)((_ticks / TimeSpan::TICKS_PER_MILLISECOND) % 1000); }

		/*!***********************************************************************
		*  @brief      �����̓��t���擾����Ticks���擾����
		*  @return     int64 : �o�ߎ��� (0.1��s�P��)
		**************************************************************************/
		__forceinline int64 GetTicks() const { return _ticks; }

		/*!***********************************************************************
		*  @brief      �ߑO�����ǂ�����Ԃ��܂�
		**************************************************************************/
		__forceinline bool IsAM() const { return GetHour() < 12; }

		/*!***********************************************************************
		*  @brief      �ߌォ�ǂ�����Ԃ��܂�
		**************************************************************************/
		__forceinline bool IsPM() const { return GetHour() >= 12; }

		/*!***********************************************************************
		*  @brief      ���ݎ��������Ƃ�, �����E�X��(�I���O4713�N1/1����̓���)���擾����
		**************************************************************************/
		__forceinline double GetJulianDay() const {return 1721425.5 + double(_ticks / TimeSpan::TICKS_PER_DAY) + GetTimeOfDay().GetTotalDays();}

		/*!***********************************************************************
		*  @brief      ���ݎ��������Ƃ�, �C�������E�X�����擾���� @n
		*              http://manome.world.coocan.jp/astrology/julian.html
		*              1858�N11/17��0�����_�̎���
		**************************************************************************/
		__forceinline double GetModifiedJulianDay() const { return (GetJulianDay() - 2400000.5); }

		/*!***********************************************************************
		*  @brief    1970/1/1����̕b�����擾���� (int64�^)
		**************************************************************************/
		__forceinline int64 ToUnixTimestampsSecond() const {return (_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND;}

		/*!***********************************************************************
		*  @brief    1970/1/1����̕b�����擾���� (double�^)
		**************************************************************************/
		__forceinline double ToUnixTimestampsSecondDecimal () const { return double(_ticks - DateTime(1970, 1, 1)._ticks) / TimeSpan::TICKS_PER_SECOND; }
		
		/*!***********************************************************************
		*  @brief    �ߑO0������̌o�ߎ��Ԃ��擾���܂��B
		**************************************************************************/
		__forceinline TimeSpan GetTimeOfDay() const { return TimeSpan(_ticks % TimeSpan::TICKS_PER_DAY); }
		
		/*!***********************************************************************
		*  @brief    �����E�X���O���S���I��ɂ����ɂ����擾����
		**************************************************************************/
		__forceinline static DateTime CreateFromJulianDay(const double julianDay)
		{
			return DateTime((int64)((julianDay - 1721425.5) * TimeSpan::TICKS_PER_DAY));
		}

		/*!***********************************************************************
		*  @brief    Unix���Ԃ���O���S���I��ɂ����ɂ����擾����
		**************************************************************************/
		__forceinline static DateTime CreateFromUnixTimestamp(const int64 unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(unixTime * TimeSpan::TICKS_PER_SECOND);
		}

		/*!***********************************************************************
		*  @brief    Unix���Ԃ���O���S���I��ɂ����ɂ����擾����
		**************************************************************************/
		__forceinline static DateTime CreateFromUnixTimestampDecimal(const double unixTime)
		{
			return DateTime(1970, 1, 1) + TimeSpan(int64(unixTime * TimeSpan::TICKS_PER_SECOND));
		}

		/*!***********************************************************************
		*  @brief   �������%Y.%m.%d-%H.%M.%S�̃t�H�[�}�b�g�ŕԂ�
		**************************************************************************/
		gu::string ToString() const;
		gu::string ToString(const char* format) const;

		__forceinline DateTime operator +(const TimeSpan& right) const { return DateTime(_ticks + right.GetTicks()); }
		
		__forceinline DateTime& operator+=(const TimeSpan& right) { _ticks += right.GetTicks(); return *this; }

		/*!***********************************************************************
		*  @brief   �w�肵�����t�Ɨ^����ꂽ���ɂ��̊Ԃ̃^�C���X�p����Ԃ�
		**************************************************************************/
		__forceinline TimeSpan operator -(const DateTime& right) const { return TimeSpan(_ticks - right._ticks); }

		__forceinline DateTime operator -(const TimeSpan& right) const { return DateTime(_ticks - right.GetTicks()); }

		__forceinline DateTime& operator -=(const TimeSpan& right) { _ticks -= right.GetTicks(); return *this; }

		#pragma endregion Public Property

		#pragma region Public Operator 
		/*----------------------------------------------------------------------
		*  @brief : �w�肵�����t�Ɠ������ǂ���, �Ⴄ���ǂ���
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