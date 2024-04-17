//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTimeSpan.hpp
///             @brief  ���Ԍv���p�̃N���X�ł�. @n
///                     2�_�Ԃ̓����̍���\������ۂɎg�p����N���X�ł���, @n
/// �@�@�@�@�@�@�@�@�@�@�@�@�@ 2000�N1��1��12:00:00��2000�N1��2��18:00:00��TimeSpan��30.0 * TICHS_PER_HOUR @n
///                     TimeSpan�̒P�ʂ�tick�ł���, 0.1��s�P�ʂŌv������܂�.  @n
///                     TimeSpan�͍���\�����邾���Ȃ̂�, ���ۂ̓��t�̎擾��DateTime���g�p���Ă�������. @n
///                     DateTime�Ƒg�ݍ��킹��, ���t�Ǝ����Ɋ�Â����Z���s�����Ƃ��o���܂�. 
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
	*  @brief     ���Ԍv���p�̃N���X�ł�. @n
	              2�_�Ԃ̓����̍���\������ۂɎg�p����N���X�ł���, 2000�N1��1��12:00:00��2000�N1��2��18:00:00��TimeSpan��30.0 * TICKS_PER_HOUR @n
	              TimeSpan�̒P�ʂ�tick�ł���, 0.1��s�P�ʂŌv������܂�. @n
				  TimeSpan�͍���\�����邾���Ȃ̂�, ���ۂ̓��t�̎擾��DateTime���g�p���Ă�������. @n 
				  DateTime�Ƒg�ݍ��킹��, ���t�Ǝ����Ɋ�Â����Z���s�����Ƃ��o���܂�. 
	*****************************************************************************/
	struct TimeSpan
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  ����P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalDays        () const { return ((double)_ticks / TICKS_PER_DAY); }

		/*!**********************************************************************
		*  @brief  ����P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalHours       () const { return ((double)_ticks / TICKS_PER_HOUR); }

		/*!**********************************************************************
		*  @brief  ����P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalMinutes     () const { return ((double)_ticks / TICKS_PER_MINUTE); }

		/*!**********************************************************************
		*  @brief  �b��P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalSeconds     () const { return ((double)_ticks / TICKS_PER_SECOND);}

		/*!**********************************************************************
		*  @brief  �~���b��P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalMilliseconds() const { return ((double)_ticks / TICKS_PER_MILLISECOND); }

		/*!**********************************************************************
		*  @brief  �}�C�N���b��P�ʂƂ��Č�Tick��double�^�ŕԋp���܂�
		*************************************************************************/
		__forceinline double GetTotalMicroseconds() const { return ((double)_ticks / TICKS_PER_MICROSECOND); };

		/*!**********************************************************************
		*  @brief  �b�����w�肵��Timespan�\���̂��쐬���܂�
		*************************************************************************/
		static TimeSpan CreateFromSeconds(const double seconds);

		/*!**********************************************************************
		*  @brief  �������w�肵��Timespan�\���̂��쐬���܂�
		*************************************************************************/
		static TimeSpan CreateFromMinutes(const double minutes);

		/*!**********************************************************************
		*  @brief  ���Ԑ����w�肵��Timespan�\���̂��쐬���܂�
		*************************************************************************/
		static TimeSpan CreateFromHours  (const double hours);

		/*!**********************************************************************
		*  @brief  �������w�肵��Timespan�\���̂��쐬���܂�
		*************************************************************************/
		static TimeSpan CreateFromDays   (const double days);
		#pragma endregion 

		#pragma region Public Member Variables
		static constexpr int64 MAX_TICKS             =  9223372036854775807;      //!< TimeSpan�Ōv���\�ȍő�(����)��tick = 0.1��s�P�� 
		static constexpr int64 MIN_TICKS             = -9223372036854775807 - 1;  //!< TimeSpan�Ōv���\�ȍŏ�(�ߋ�)��tick
		static constexpr int64 NANOSECONDS_PER_TICK  = 100;                       //!< 1 tick = 0.1��s�P�ʂŌv��
		static constexpr int64 TICKS_PER_WEEK        = 6048000000000;             //!< 1�T�Ԃɂ�����tick��
		static constexpr int64 TICKS_PER_DAY         = 864000000000;              //!< 1���ɂ�����tick��
		static constexpr int64 TICKS_PER_HOUR        = 36000000000;               //!< 1���Ԃɂ�����tick��
		static constexpr int64 TICKS_PER_MINUTE      = 600000000;                 //!< 1���ɂ�����tick��
		static constexpr int64 TICKS_PER_SECOND      = 10000000;                  //!< 1�b�ɂ�����tick��
		static constexpr int64 TICKS_PER_MILLISECOND = 10000;                     //!< 1ms�ɂ�����tick��
		static constexpr int64 TICKS_PER_MICROSECOND = 10;                        //!< 1��s�ɂ�����tick��
		static constexpr int64 TICKS_PER_YEAR        = 365 * TICKS_PER_DAY;       //!< 1�N�ɂ�����tick��

		/*!**********************************************************************
		*  @brief  Tick�̂����}�C�N���b�������擾���܂�
		*************************************************************************/
		__forceinline int32  GetFractionMicrosecond() const { return (int32)((_ticks % TICKS_PER_SECOND) / TICKS_PER_MICROSECOND); }
		
		/*!**********************************************************************
		*  @brief  Tick�̂����~���b�������擾���܂�
		*************************************************************************/
		__forceinline int32  GetFractionMillisecond() const { return (int32)((_ticks % TICKS_PER_SECOND) / TICKS_PER_MILLISECOND);}

		/*!**********************************************************************
		*  @brief  Tick�̂����i�m�b�������擾���܂�
		*************************************************************************/
		__forceinline int32  GetFractionNanosecond () const { return (int32)((_ticks % TICKS_PER_SECOND) * NANOSECONDS_PER_TICK);}
		
		/*!**********************************************************************
		*  @brief  Tick�̂����b���������擾���܂�
		*************************************************************************/
		__forceinline int32  GetFractionTicks() const { return (int32)(_ticks % TICKS_PER_SECOND); }

		/*!**********************************************************************
		*  @brief  Tick�̂��������������擾���܂�
		*************************************************************************/
		__forceinline int32  GetDays   () const { return (int32)(_ticks / TICKS_PER_DAY); }

		/*!**********************************************************************
		*  @brief  Tick�̂������Ԑ��������擾���܂�
		*************************************************************************/
		__forceinline int32  GetHours  () const { return (int32)((_ticks / TICKS_PER_HOUR  ) % 24); }

		/*!**********************************************************************
		*  @brief  Tick�̂������P�ʂ̕������擾���܂�
		*************************************************************************/
		__forceinline int32  GetMinutes() const { return (int32)((_ticks / TICKS_PER_MINUTE) % 60); }

		/*!**********************************************************************
		*  @brief  Tick�̂����b���������擾���܂�
		*************************************************************************/
		__forceinline int32  GetSeconds() const { return (int32)((_ticks / TICKS_PER_SECOND) % 60); }

		/*!**********************************************************************
		*  @brief  1 tick = 0.1��s�P�ʂł̐��f�[�^��n���܂�. 
		*************************************************************************/
		__forceinline int64  GetTicks  () const { return _ticks; }

		/*!**********************************************************************
		*  @brief  Tick�̐�Βl���擾��, Timespan�̍������擾���܂�
		*************************************************************************/
		__forceinline TimeSpan GetDuration() const { return TimeSpan(_ticks >= 0 ? _ticks : -_ticks); }

		/*!**********************************************************************
		*  @brief  Tick��0���ǂ�����ݒ肵�܂�
		*************************************************************************/
		__forceinline bool IsZero() const { return (_ticks == 0); }

		/*!**********************************************************************
		*  @brief  p[d.]hh:mm:ss.fff�@���̃^�C���X�p���̕�����\�����f�t�H���g�̃t�H�[�}�b�g�ŕԂ�
		*************************************************************************/
		gu::string ToString() const;

		/*!**********************************************************************
		*  @brief  p[d.]hh:mm:ss.fff�@���̃^�C���X�p���̕�����\�����f�t�H���g�̃t�H�[�}�b�g�ŕԂ�
		*************************************************************************/
		gu::string ToString(const gu::char8* format) const;

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		TimeSpan() : _ticks(0) {};

		/*! @brief ����Tick���w�肷��R���X�g���N�^*/
		explicit TimeSpan(const int64 ticks);

		/*! @brief ����, ��, �b��, �[���͖�������R���X�g���N�^*/
		TimeSpan(const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(0, hours, minutes, seconds, 0);
		}
		
		/*! @brief ����, ����, ��, �b�Œ[���͖�������R���X�g���N�^*/
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds)
		{
			Assign(days, hours, minutes, seconds, 0);
		}
		
		/*! @brief ����, ����, ��, �b, �i�m�b�Ŗ�������R���X�g���N�^*/
		TimeSpan(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano)
		{
			Assign(days, hours, minutes, seconds, fractionNano);
		}

		#pragma endregion 
		
		#pragma region Public Operator Function

		/*! @brief �^����ꂽ�^�C���X�p�������̃^�C���X�p���ɉ��Z�������ʂ�Ԃ�*/
		__forceinline TimeSpan operator+(const TimeSpan& other) const { return TimeSpan(_ticks + other._ticks); }
		
		/*! @brief �^����ꂽ�^�C���X�p�������̃^�C���X�p���ɉ��Z�������ʂ�Ԃ�*/
		__forceinline TimeSpan& operator+=(const TimeSpan& right) { _ticks += right._ticks; return *this; }

		/*! @brief �^����ꂽ�^�C���X�p�������̃^�C���X�p���Ɍ��Z�������ʂ�Ԃ�*/
		__forceinline TimeSpan operator-() const { return TimeSpan(-_ticks); }

		/*! @brief ���̃^�C���X�p������^����ꂽ�^�C���X�p�������������ʂ�Ԃ�*/
		__forceinline TimeSpan operator-(const TimeSpan& right) const { return TimeSpan(_ticks - right._ticks); }

		/*! @brief ���̃^�C���X�p������^����ꂽ�^�C���X�p�������������ʂ�Ԃ�*/
		__forceinline TimeSpan& operator-=(const TimeSpan& right) { _ticks -= right._ticks; return *this; }

		/*! @brief ���̃^�C���X�p���Ɨ^����ꂽ�X�J���[���|�����킹�����ʂ�Ԃ�*/
		__forceinline TimeSpan operator*(const double scalar) const
		{
			return TimeSpan((int64)((double)_ticks * scalar));
		}

		/*! @brief ���̃^�C���X�p���Ɨ^����ꂽ�X�J���[���|�����킹�����ʂ�Ԃ�*/
		__forceinline TimeSpan& operator*=(const double scalar)
		{
			_ticks = (int64)((double)_ticks * scalar);
			return *this;
		}

		/*! @brief ���̃^�C���X�p����^����ꂽ�X�J���[�Ŋ��������ʂ�Ԃ�*/
		__forceinline TimeSpan operator / (const double scalar) const { return TimeSpan((int64)((double)_ticks / scalar)); }

		/*! @brief ���̃^�C���X�p����^����ꂽ�X�J���[�Ŋ��������ʂ�Ԃ�*/
		__forceinline TimeSpan& operator/=(const double scalar)
		{
			_ticks = (int64)((double)_ticks / scalar);
			return *this;
		}

		/*! @brief ���̃^�C���X�p���ƕʂ̃^�C���X�p���Ƃ̗]��̌v�Z���ʂ�Ԃ�*/
		__forceinline TimeSpan operator% (const TimeSpan& right) const { return TimeSpan(_ticks % right._ticks); }
		
		/*! @brief ���̃^�C���X�p���ƕʂ̃^�C���X�p���Ƃ̗]��̌v�Z���ʂ�Ԃ�*/
		__forceinline TimeSpan& operator%= (const TimeSpan& right)
		{
			_ticks = _ticks % right._ticks;
			return *this;
		}

		/*! @brief ���҂����������ǂ�����Ԃ��܂�*/
		bool operator==(const TimeSpan& right) const { return (_ticks == right._ticks); }

		/*! @brief ���҂��������Ȃ����ǂ�����Ԃ��܂�*/
		bool operator!=(const TimeSpan& right) const { return (_ticks != right._ticks); }

		/*! @brief Tick���傫�����ǂ�����Ԃ��܂�*/
		bool operator> (const TimeSpan& right) const { return (_ticks >  right._ticks); }

		/*! @brief Tick��right�ȏ�ǂ�����Ԃ��܂�*/
		bool operator>=(const TimeSpan& right) const { return (_ticks >= right._ticks); }

		/*! @brief Tick��right�����ǂ�����Ԃ��܂�*/
		bool operator< (const TimeSpan& right) const { return (_ticks <  right._ticks); }

		/*! @brief Tick��right�ȉ��ǂ�����Ԃ��܂�*/
		bool operator<=(const TimeSpan& right) const { return (_ticks <= right._ticks); }

		#pragma endregion


	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief  Tick�̊��蓖��
		*************************************************************************/
		void Assign(const int32 days, const int32 hours, const int32 minutes, const int32 seconds, const int32 fractionNano);
		
		#pragma endregion
	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Member Variables
		/*! @brief 0.1��s�P�ʂŎw�肷�鎞�Ԃł�. 1 tick = 0.1 ��s*/
		int64 _ticks = 0;
		#pragma endregion
	};

}
#endif