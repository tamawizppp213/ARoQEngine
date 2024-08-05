//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsAtomics.hpp
///  @brief  ���s������ۏ؂��邽�߂̃A�g�~�b�N�����񋟂��܂�.@n
///  @author toide
///  @date   2024/06/22 12:39:28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_ATOMIC_HPP
#define WINDOWS_ATOMIC_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{

	/****************************************************************************
	*				  			   WindowsAtomics
	*************************************************************************//**
	/*  @brief  ���s������ۏ؂��邽�߂̃A�g�~�b�N�����񋟂��܂�.@n
	*****************************************************************************/
	class CoreAtomicOperator : public gu::NonCopyAndMove
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     Read/Write�o���A��ݒ肵�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void ReadWriteBarrier();

		/*!**********************************************************************
		*  @brief     Read�o���A��ݒ肵�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void ReadBarrier();

		/*!**********************************************************************
		*  @brief     Read�o���A��ݒ肵�܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		static void WriteBarrier();


		/*!**********************************************************************
		*  @brief     int8�^���A�g�~�b�N�ɃC���N�������g���܂�.
		*  @param[in] volatile gu::int8* value : �C���N�������g�������l
		*  @return    gu::int8 : �C���N�������g��̒l
		*************************************************************************/
		static gu::int8 Increment(volatile gu::int8* value);

		/*!**********************************************************************
		*  @brief     int16�^���A�g�~�b�N�ɃC���N�������g���܂�.
		*  @param[in] volatile gu::int16* value : �C���N�������g�������l
		*  @return    gu::int16 : �C���N�������g��̒l
		*************************************************************************/
		static gu::int16 Increment(volatile gu::int16* value);

		/*!**********************************************************************
		*  @brief     int32�^���A�g�~�b�N�ɃC���N�������g���܂�.
		*  @param[in] volatile gu::int32* value : �C���N�������g�������l
		*  @return    gu::int32 : �C���N�������g��̒l
		*************************************************************************/
		static gu::int32 Increment(volatile gu::int32* value);

		/*!**********************************************************************
		*  @brief     int64�^���A�g�~�b�N�ɃC���N�������g���܂�.
		*  @param[in] volatile gu::int64* value : �C���N�������g�������l
		*  @return    gu::int64 : �C���N�������g��̒l
		*************************************************************************/
		static gu::int64 Increment(volatile gu::int64* value);

		/*!**********************************************************************
		*  @brief     int8�^���A�g�~�b�N�Ƀf�N�������g���܂�.
		*  @param[in] volatile gu::int8* value : �f�N�������g�������l
		*  @return    gu::int8 : �f�N�������g��̒l
		* ************************************************************************/
		static gu::int8 Decrement(volatile gu::int8* value);

		/*!**********************************************************************
		*  @brief     int16�^���A�g�~�b�N�Ƀf�N�������g���܂�.
		*  @param[in] volatile gu::int16* value : �f�N�������g�������l
		*  @return    gu::int16 : �f�N�������g��̒l
		* ************************************************************************/
		static gu::int16 Decrement(volatile gu::int16* value);

		/*!**********************************************************************
		*  @brief     int32�^���A�g�~�b�N�Ƀf�N�������g���܂�.
		*  @param[in] volatile gu::int32* value : �f�N�������g�������l
		*  @return    gu::int32 : �f�N�������g��̒l
		* ************************************************************************/
		static gu::int32 Decrement(volatile gu::int32* value);

		/*!**********************************************************************
		*  @brief     int64�^���A�g�~�b�N�Ƀf�N�������g���܂�.
		*  @param[in] volatile gu::int64* value : �f�N�������g�������l
		*  @return    gu::int64 : �f�N�������g��̒l
		* ************************************************************************/
		static gu::int64 Decrement(volatile gu::int64* value);

		/*!**********************************************************************
		*  @brief     int8�^���A�g�~�b�N�ɉ��Z���܂�.
		*  @param[in] volatile gu::int8* value : ���Z�������l
		*  @param[in] gu::int8 addValue : ���Z�������l
		*  @return    gu::int8 : ���Z��̒l
		* ************************************************************************/
		static gu::int8 Add(volatile gu::int8* value, gu::int8 addValue);
		
		/*!**********************************************************************
		*  @brief     int16�^���A�g�~�b�N�ɉ��Z���܂�.
		*  @param[in] volatile gu::int16* value : ���Z�������l
		*  @param[in] gu::int16 addValue : ���Z�������l
		*  @return    gu::int16 : ���Z��̒l
		* ************************************************************************/
		static gu::int16 Add(volatile gu::int16* value, gu::int16 addValue);

		/*!**********************************************************************
		*  @brief     int32�^���A�g�~�b�N�ɉ��Z���܂�.
		*  @param[in] volatile gu::int32* value : ���Z�������l
		*  @param[in] gu::int32 addValue : ���Z�������l
		*  @return    gu::int32 : ���Z��̒l
		* ************************************************************************/
		static gu::int32 Add(volatile gu::int32* value, gu::int32 addValue);

		/*!**********************************************************************
		*  @brief     int64�^���A�g�~�b�N�ɉ��Z���܂�.
		*  @param[in] volatile gu::int64* value : ���Z�������l
		*  @param[in] gu::int64 addValue : ���Z�������l
		*  @return    gu::int64 : ���Z��̒l
		* ************************************************************************/
		static gu::int64 Add(volatile gu::int64* value, gu::int64 addValue);

		/*!**********************************************************************
		*  @brief     int8�^���A�g�~�b�N�Ɍ��Z���܂�.
		*  @param[in] volatile gu::int8* value : ���Z�������l
		*  @param[in] gu::int8 subValue : ���Z�������l
		*  @return    gu::int8 : ���Z��̒l
		* ************************************************************************/
		static gu::int8 Subtract(volatile gu::int8* value, gu::int8 subValue);

		/*!**********************************************************************
		*  @brief     int16�^���A�g�~�b�N�Ɍ��Z���܂�.
		*  @param[in] volatile gu::int16* value : ���Z�������l
		*  @param[in] gu::int16 subValue : ���Z�������l
		*  @return    gu::int16 : ���Z��̒l
		* ************************************************************************/
		static gu::int16 Subtract(volatile gu::int16* value, gu::int16 subValue);

		/*!**********************************************************************
		*  @brief     int32�^���A�g�~�b�N�Ɍ��Z���܂�.
		*  @param[in] volatile gu::int32* value : ���Z�������l
		*  @param[in] gu::int32 subValue : ���Z�������l
		*  @return    gu::int32 : ���Z��̒l
		* ************************************************************************/
		static gu::int32 Subtract(volatile gu::int32* value, gu::int32 subValue);

		/*!**********************************************************************
		*  @brief     int64�^���A�g�~�b�N�Ɍ��Z���܂�.
		* @param[in] volatile gu::int64* value : ���Z�������l
		* @param[in] gu::int64 subValue : ���Z�������l
		* @return    gu::int64 : ���Z��̒l
		* ************************************************************************/
		static gu::int64 Subtract(volatile gu::int64* value, gu::int64 subValue);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int8* value : ��r�������l
		*  @param[in]    gu::int8 exchangeValue : �����������l
		*  @param[in]    gu::int8 comparandValue : ��r�������l
		*  @return       gu::int8 : �����O�̒l
		* ************************************************************************/
		static gu::int8 CompareExchange(volatile gu::int8* value, gu::int8 exchangeValue, gu::int8 comparandValue);

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int16* value : ��r�������l
		*  @param[in]    gu::int16 exchangeValue : �����������l
		*  @param[in]    gu::int16 comparandValue : ��r�������l
		*  @return       gu::int16 : �����O�̒l
		* ************************************************************************/
		static gu::int16 CompareExchange(volatile gu::int16* value, gu::int16 exchangeValue, gu::int16 comparandValue);

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int32* value : ��r�������l
		*  @param[in]    gu::int32 exchangeValue : �����������l
		*  @param[in]    gu::int32 comparandValue : ��r�������l
		*  @return       gu::int32 : �����O�̒l
		* ************************************************************************/
		static gu::int32 CompareExchange(volatile gu::int32* value, gu::int32 exchangeValue, gu::int32 comparandValue);

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int64* value : ��r�������l
		*  @param[in]    gu::int64 exchangeValue : �����������l
		*  @param[in]    gu::int64 comparandValue : ��r�������l
		*  @return       gu::int64 : �����O�̒l
		* ************************************************************************/
		static gu::int64 CompareExchange(volatile gu::int64* value, gu::int64 exchangeValue, gu::int64 comparandValue);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int8* value : ��r�������l
		*  @param[in]    gu::int8 exchangeValue : �����������l
		*  @return       gu::int8 : �����O�̒l
		* ************************************************************************/
		static gu::int8 Exchange(volatile gu::int8* value, gu::int8 exchangeValue);

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int16* value : ��r�������l
		*  @param[in]    gu::int16 exchangeValue : �����������l
		*  @return       gu::int16 : �����O�̒l
		* ************************************************************************/
		static gu::int16 Exchange(volatile gu::int16* value, gu::int16 exchangeValue);

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int32* value : ��r�������l
		*  @param[in]    gu::int32 exchangeValue : �����������l
		*  @return       gu::int32 : �����O�̒l
		* ************************************************************************/
		static gu::int32 Exchange(volatile gu::int32* value, gu::int32 exchangeValue);

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N�ɔ�r���Č������܂�.
		*  @param[inout] volatile gu::int64* value : ��r�������l
		*  @param[in]    gu::int64 exchangeValue : �����������l
		*  @return       gu::int64 : �����O�̒l
		* ************************************************************************/
		static gu::int64 Exchange(volatile gu::int64* value, gu::int64 exchangeValue);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N��And���Z���܂�.
		*  @param[inout] volatile gu::int8* destination : ���͒l��And���Z�̌���
		*  @param[in]    gu::int8 value : And���Z�������l
		*  @return       gu::int8 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int8 And(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N��And���Z���܂�.
		*  @param[inout] volatile gu::int16* destination : ���͒l��And���Z�̌���
		*  @param[in]    gu::int16 value : And���Z�������l
		*  @return       gu::int16 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int16 And(volatile gu::int16* destination, gu::int16 value);
		
		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N��And���Z���܂�.
		*  @param[inout] volatile gu::int32* destination : ���͒l��And���Z�̌���
		*  @param[in]    gu::int32 value : And���Z�������l
		*  @return       gu::int32 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int32 And(volatile gu::int32* destination, gu::int32 value);
		
		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N��And���Z���܂�.
		*  @param[inout] volatile gu::int64* destination : ���͒l��And���Z�̌���
		*  @param[in]    gu::int64 value : And���Z�������l
		*  @return       gu::int64 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int64 And(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N��Or���Z���܂�.
		*  @param[inout] volatile gu::int8* destination : ���͒l��Or���Z�̌���
		*  @param[in]    gu::int8 value : Or���Z�������l
		*  @return       gu::int8 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int8 Or(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N��Or���Z���܂�.
		*  @param[inout] volatile gu::int16* destination : ���͒l��Or���Z�̌���
		*  @param[in]    gu::int16 value : Or���Z�������l
		*  @return       gu::int16 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int16 Or(volatile gu::int16* destination, gu::int16 value);

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N��Or���Z���܂�.
		*  @param[inout] volatile gu::int32* destination : ���͒l��Or���Z�̌���
		*  @param[in]    gu::int32 value : Or���Z�������l
		*  @return       gu::int32 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int32 Or(volatile gu::int32* destination, gu::int32 value);

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N��Or���Z���܂�.
		*  @param[inout] volatile gu::int64* destination : ���͒l��Or���Z�̌���
		*  @param[in]    gu::int64 value : Or���Z�������l
		*  @return       gu::int64 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int64 Or(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N��Xor���Z���܂�.
		*  @param[inout] volatile gu::int8* destination : ���͒l��Xor���Z�̌���
		*  @param[in]    gu::int8 value : Xor���Z�������l
		*  @return       gu::int8 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int8 Xor(volatile gu::int8* destination, gu::int8 value);

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N��Xor���Z���܂�.
		*  @param[inout] volatile gu::int16* destination : ���͒l��Xor���Z�̌���
		*  @param[in]    gu::int16 value : Xor���Z�������l
		*  @return       gu::int16 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int16 Xor(volatile gu::int16* destination, gu::int16 value);

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N��Xor���Z���܂�.
		*  @param[inout] volatile gu::int32* destination : ���͒l��Xor���Z�̌���
		*  @param[in]    gu::int32 value : Xor���Z�������l
		*  @return       gu::int32 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int32 Xor(volatile gu::int32* destination, gu::int32 value);

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N��Xor���Z���܂�.
		*  @param[inout] volatile gu::int64* destination : ���͒l��Xor���Z�̌���
		*  @param[in]    gu::int64 value : Xor���Z�������l
		*  @return       gu::int64 : ���X�̓��͒l
		* ************************************************************************/
		static gu::int64 Xor(volatile gu::int64* destination, gu::int64 value);

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N�ɓǂݎ��܂�
		* @param[in]     volatile const gu::int8* value : �ǂݎ�肽���l
		* @return        gu::int8 : �ǂݎ�����l
		* ************************************************************************/
		static gu::int8 Read(volatile const gu::int8* value)
		{
			return CompareExchange((gu::int8*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N�ɓǂݎ��܂�
		*  @param[in]    volatile const gu::int16* value : �ǂݎ�肽���l
		*  @return       gu::int16 : �ǂݎ�����l
		* ************************************************************************/
		static gu::int16 Read(volatile const gu::int16* value)
		{
			return CompareExchange((gu::int16*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N�ɓǂݎ��܂�
		*  @param[in]    volatile const gu::int32* value : �ǂݎ�肽���l
		*  @return       gu::int32 : �ǂݎ�����l
		* ************************************************************************/
		static gu::int32 Read(volatile const gu::int32* value)
		{
			return CompareExchange((gu::int32*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N�ɓǂݎ��܂�
		*  @param[in]    volatile const gu::int64* value : �ǂݎ�肽���l
		*  @return       gu::int64 : �ǂݎ�����l
		* ************************************************************************/
		static gu::int64 Read(volatile const gu::int64* value)
		{
			return CompareExchange((gu::int64*)value, 0, 0);
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int8�^��ǂݎ��܂�
		*  @param[in]    volatile const gu::int8* value : �ǂݎ�肽���l
		*  @return       gu::int8 : �ǂݎ�����l
		* ************************************************************************/
		__forceinline static gu::int8 ReadRelaxed(volatile const gu::int8* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int16�^��ǂݎ��܂�
		*  @param[in]    volatile const gu::int16* value : �ǂݎ�肽���l
		*  @return       gu::int16 : �ǂݎ�����l
		* ************************************************************************/
		__forceinline static gu::int16 ReadRelaxed(volatile const gu::int16* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int32�^��ǂݎ��܂�
		*  @param[in]    volatile const gu::int32* value : �ǂݎ�肽���l
		*  @return       gu::int32 : �ǂݎ�����l
		* ************************************************************************/
		__forceinline static gu::int32 ReadRelaxed(volatile const gu::int32* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int64�^��ǂݎ��܂�
		*  @param[in]    volatile const gu::int64* value : �ǂݎ�肽���l
		*  @return       gu::int64 : �ǂݎ�����l
		* ************************************************************************/
		__forceinline static gu::int64 ReadRelaxed(volatile const gu::int64* value)
		{
			return *value;
		}

		/*!**********************************************************************
		*  @brief        int8�^���A�g�~�b�N�ɏ������݂܂�
		*  @param[inout] volatile gu::int8* destination : �������ݐ�
		*  @param[in]    gu::int8 value : �������݂����l
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int8* destination, const gu::int8 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int16�^���A�g�~�b�N�ɏ������݂܂�
		*  @param[inout] volatile gu::int16* destination : �������ݐ�
		*  @param[in]    gu::int16 value : �������݂����l
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int16* destination, const gu::int16 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int32�^���A�g�~�b�N�ɏ������݂܂�
		*  @param[inout] volatile gu::int32* destination : �������ݐ�
		*  @param[in]    gu::int32 value : �������݂����l
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int32* destination, const gu::int32 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        int64�^���A�g�~�b�N�ɏ������݂܂�
		*  @param[inout] volatile gu::int64* destination : �������ݐ�
		*  @param[in]    gu::int64 value : �������݂����l
		* ************************************************************************/
		__forceinline static void Write(volatile gu::int64* destination, const gu::int64 value)
		{
			Exchange(destination, value);
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int8�^���������݂܂�
		*  @param[inout] volatile gu::int8* destination : �������ݐ�
		*  @param[in]    gu::int8 value : �������݂����l
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int8* destination, const gu::int8 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int16�^���������݂܂�
		*  @param[inout] volatile gu::int16* destination : �������ݐ�
		*  @param[in]    gu::int16 value : �������݂����l
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int16* destination, const gu::int16 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int32�^���������݂܂�
		*  @param[inout] volatile gu::int32* destination : �������ݐ�
		*  @param[in]    gu::int32 value : �������݂����l
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int32* destination, const gu::int32 value)
		{
			*destination = value;
		}

		/*!**********************************************************************
		*  @brief        �A�g�~�b�N������s�킸��int64�^���������݂܂�
		*  @param[inout] volatile gu::int64* destination : �������ݐ�
		*  @param[in]    gu::int64 value : �������݂����l
		* ************************************************************************/
		__forceinline static void WriteRelaxed(volatile gu::int64* destination, const gu::int64 value)
		{
			*destination = value;
		}


		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif