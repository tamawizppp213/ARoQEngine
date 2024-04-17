//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTimeSpan.hpp
///             @brief  時間計測用のクラスです. @n
///                     2点間の日時の差を表現する際に使用するクラスであり, @n
/// 　　　　　　　　　　　　　 2000年1月1日12:00:00と2000年1月2日18:00:00のTimeSpanは30.0 * TICHS_PER_HOUR @n
///                     TimeSpanの単位はtickであり, 0.1μs単位で計測されます.  @n
///                     TimeSpanは差を表現するだけなので, 実際の日付の取得はDateTimeを使用してください. @n
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
	*  @brief     時間計測用のクラスです. @n
	              2点間の日時の差を表現する際に使用するクラスであり, 2000年1月1日12:00:00と2000年1月2日18:00:00のTimeSpanは30.0 * TICKS_PER_HOUR @n
	              TimeSpanの単位はtickであり, 0.1μs単位で計測されます. @n
				  TimeSpanは差を表現するだけなので, 実際の日付の取得はDateTimeを使用してください. @n 
				  DateTimeと組み合わせて, 日付と時刻に基づく演算を行うことが出来ます. 
	*****************************************************************************/
	struct TimeSpan
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  日を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalDays        () const { return ((double)_ticks / TICKS_PER_DAY); }

		/*!**********************************************************************
		*  @brief  時を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalHours       () const { return ((double)_ticks / TICKS_PER_HOUR); }

		/*!**********************************************************************
		*  @brief  分を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalMinutes     () const { return ((double)_ticks / TICKS_PER_MINUTE); }

		/*!**********************************************************************
		*  @brief  秒を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalSeconds     () const { return ((double)_ticks / TICKS_PER_SECOND);}

		/*!**********************************************************************
		*  @brief  ミリ秒を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalMilliseconds() const { return ((double)_ticks / TICKS_PER_MILLISECOND); }

		/*!**********************************************************************
		*  @brief  マイクロ秒を単位として現Tickをdouble型で返却します
		*************************************************************************/
		__forceinline double GetTotalMicroseconds() const { return ((double)_ticks / TICKS_PER_MICROSECOND); };

		/*!**********************************************************************
		*  @brief  秒数を指定してTimespan構造体を作成します
		*************************************************************************/
		static TimeSpan CreateFromSeconds(const double seconds);

		/*!**********************************************************************
		*  @brief  分数を指定してTimespan構造体を作成します
		*************************************************************************/
		static TimeSpan CreateFromMinutes(const double minutes);

		/*!**********************************************************************
		*  @brief  時間数を指定してTimespan構造体を作成します
		*************************************************************************/
		static TimeSpan CreateFromHours  (const double hours);

		/*!**********************************************************************
		*  @brief  日数を指定してTimespan構造体を作成します
		*************************************************************************/
		static TimeSpan CreateFromDays   (const double days);
		#pragma endregion 

		#pragma region Public Member Variables
		static constexpr int64 MAX_TICKS             =  9223372036854775807;      //!< TimeSpanで計測可能な最大(未来)のtick = 0.1μs単位 
		static constexpr int64 MIN_TICKS             = -9223372036854775807 - 1;  //!< TimeSpanで計測可能な最小(過去)のtick
		static constexpr int64 NANOSECONDS_PER_TICK  = 100;                       //!< 1 tick = 0.1μs単位で計測
		static constexpr int64 TICKS_PER_WEEK        = 6048000000000;             //!< 1週間にかかるtick数
		static constexpr int64 TICKS_PER_DAY         = 864000000000;              //!< 1日にかかるtick数
		static constexpr int64 TICKS_PER_HOUR        = 36000000000;               //!< 1時間にかかるtick数
		static constexpr int64 TICKS_PER_MINUTE      = 600000000;                 //!< 1分にかかるtick数
		static constexpr int64 TICKS_PER_SECOND      = 10000000;                  //!< 1秒にかかるtick数
		static constexpr int64 TICKS_PER_MILLISECOND = 10000;                     //!< 1msにかかるtick数
		static constexpr int64 TICKS_PER_MICROSECOND = 10;                        //!< 1μsにかかるtick数
		static constexpr int64 TICKS_PER_YEAR        = 365 * TICKS_PER_DAY;       //!< 1年にかかるtick数

		/*!**********************************************************************
		*  @brief  Tickのうちマイクロ秒部分を取得します
		*************************************************************************/
		__forceinline int32  GetFractionMicrosecond() const { return (int32)((_ticks % TICKS_PER_SECOND) / TICKS_PER_MICROSECOND); }
		
		/*!**********************************************************************
		*  @brief  Tickのうちミリ秒部分を取得します
		*************************************************************************/
		__forceinline int32  GetFractionMillisecond() const { return (int32)((_ticks % TICKS_PER_SECOND) / TICKS_PER_MILLISECOND);}

		/*!**********************************************************************
		*  @brief  Tickのうちナノ秒部分を取得します
		*************************************************************************/
		__forceinline int32  GetFractionNanosecond () const { return (int32)((_ticks % TICKS_PER_SECOND) * NANOSECONDS_PER_TICK);}
		
		/*!**********************************************************************
		*  @brief  Tickのうち秒数部分を取得します
		*************************************************************************/
		__forceinline int32  GetFractionTicks() const { return (int32)(_ticks % TICKS_PER_SECOND); }

		/*!**********************************************************************
		*  @brief  Tickのうち日数部分を取得します
		*************************************************************************/
		__forceinline int32  GetDays   () const { return (int32)(_ticks / TICKS_PER_DAY); }

		/*!**********************************************************************
		*  @brief  Tickのうち時間数部分を取得します
		*************************************************************************/
		__forceinline int32  GetHours  () const { return (int32)((_ticks / TICKS_PER_HOUR  ) % 24); }

		/*!**********************************************************************
		*  @brief  Tickのうち分単位の部分を取得します
		*************************************************************************/
		__forceinline int32  GetMinutes() const { return (int32)((_ticks / TICKS_PER_MINUTE) % 60); }

		/*!**********************************************************************
		*  @brief  Tickのうち秒数部分を取得します
		*************************************************************************/
		__forceinline int32  GetSeconds() const { return (int32)((_ticks / TICKS_PER_SECOND) % 60); }

		/*!**********************************************************************
		*  @brief  1 tick = 0.1μs単位での生データを渡します. 
		*************************************************************************/
		__forceinline int64  GetTicks  () const { return _ticks; }

		/*!**********************************************************************
		*  @brief  Tickの絶対値を取得し, Timespanの差分を取得します
		*************************************************************************/
		__forceinline TimeSpan GetDuration() const { return TimeSpan(_ticks >= 0 ? _ticks : -_ticks); }

		/*!**********************************************************************
		*  @brief  Tickが0かどうかを設定します
		*************************************************************************/
		__forceinline bool IsZero() const { return (_ticks == 0); }

		/*!**********************************************************************
		*  @brief  p[d.]hh:mm:ss.fff　このタイムスパンの文字列表現をデフォルトのフォーマットで返す
		*************************************************************************/
		gu::string ToString() const;

		/*!**********************************************************************
		*  @brief  p[d.]hh:mm:ss.fff　このタイムスパンの文字列表現をデフォルトのフォーマットで返す
		*************************************************************************/
		gu::string ToString(const gu::char8* format) const;

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		TimeSpan() : _ticks(0) {};

		/*! @brief 直接Tickを指定するコンストラクタ*/
		explicit TimeSpan(const int64 ticks);

		/*! @brief 時間, 分, 秒で, 端数は無視するコンストラクタ*/
		TimeSpan(const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(0, hours, minutes, seconds, 0);
		}
		
		/*! @brief 日数, 時間, 分, 秒で端数は無視するコンストラクタ*/
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(days, hours, minutes, seconds, 0);
		}
		
		/*! @brief 日数, 時間, 分, 秒, ナノ秒で無視するコンストラクタ*/
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano)
		{
			Assign(days, hours, minutes, seconds, fractionNano);
		}

		#pragma endregion 
		
		#pragma region Public Operator Function

		/*! @brief 与えられたタイムスパンをこのタイムスパンに加算した結果を返す*/
		__forceinline TimeSpan operator+(const TimeSpan& other) const { return TimeSpan(_ticks + other._ticks); }
		
		/*! @brief 与えられたタイムスパンをこのタイムスパンに加算した結果を返す*/
		__forceinline TimeSpan& operator+=(const TimeSpan& right) { _ticks += right._ticks; return *this; }

		/*! @brief 与えられたタイムスパンをこのタイムスパンに減算した結果を返す*/
		__forceinline TimeSpan operator-() const { return TimeSpan(-_ticks); }

		/*! @brief このタイムスパンから与えられたタイムスパンを引いた結果を返す*/
		__forceinline TimeSpan operator-(const TimeSpan& right) const { return TimeSpan(_ticks - right._ticks); }

		/*! @brief このタイムスパンから与えられたタイムスパンを引いた結果を返す*/
		__forceinline TimeSpan& operator-=(const TimeSpan& right) { _ticks -= right._ticks; return *this; }

		/*! @brief このタイムスパンと与えられたスカラーを掛け合わせた結果を返す*/
		__forceinline TimeSpan operator*(const double scalar) const
		{
			return TimeSpan((int64)((double)_ticks * scalar));
		}

		/*! @brief このタイムスパンと与えられたスカラーを掛け合わせた結果を返す*/
		__forceinline TimeSpan& operator*=(const double scalar)
		{
			_ticks = (int64)((double)_ticks * scalar);
			return *this;
		}

		/*! @brief このタイムスパンを与えられたスカラーで割った結果を返す*/
		__forceinline TimeSpan operator / (const double scalar) const { return TimeSpan((int64)((double)_ticks / scalar)); }

		/*! @brief このタイムスパンを与えられたスカラーで割った結果を返す*/
		__forceinline TimeSpan& operator/=(const double scalar)
		{
			_ticks = (int64)((double)_ticks / scalar);
			return *this;
		}

		/*! @brief このタイムスパンと別のタイムスパンとの余りの計算結果を返す*/
		__forceinline TimeSpan operator% (const TimeSpan& right) const { return TimeSpan(_ticks % right._ticks); }
		
		/*! @brief このタイムスパンと別のタイムスパンとの余りの計算結果を返す*/
		__forceinline TimeSpan& operator%= (const TimeSpan& right)
		{
			_ticks = _ticks % right._ticks;
			return *this;
		}

		/*! @brief 両者が等しいかどうかを返します*/
		bool operator==(const TimeSpan& right) const { return (_ticks == right._ticks); }

		/*! @brief 両者が等しくないかどうかを返します*/
		bool operator!=(const TimeSpan& right) const { return (_ticks != right._ticks); }

		/*! @brief Tickが大きいかどうかを返します*/
		bool operator> (const TimeSpan& right) const { return (_ticks >  right._ticks); }

		/*! @brief Tickがright以上どうかを返します*/
		bool operator>=(const TimeSpan& right) const { return (_ticks >= right._ticks); }

		/*! @brief Tickがright未満どうかを返します*/
		bool operator< (const TimeSpan& right) const { return (_ticks <  right._ticks); }

		/*! @brief Tickがright以下どうかを返します*/
		bool operator<=(const TimeSpan& right) const { return (_ticks <= right._ticks); }

		#pragma endregion


	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief  Tickの割り当て
		*************************************************************************/
		void Assign(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano);
		
		#pragma endregion
	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Member Variables
		/*! @brief 0.1μs単位で指定する時間です. 1 tick = 0.1 μs*/
		int64 _ticks = 0;
		#pragma endregion
	};

}
#endif