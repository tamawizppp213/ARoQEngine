//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAtomic.hpp
///  @brief  実行順序を保証するためのアトミック操作を提供します.@n
///  @author toide
///  @date   2024/06/22 22:03:08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ATOMIC_HPP
#define GU_ATOMIC_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreAtomicOperator.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			 MemoryOrder
	*************************************************************************//**
	/*  @brief    実行順序を保証するためのアトミック操作を提供します.@n
	*****************************************************************************/
	enum class MemoryOrder : uint8
	{
		Relaxed                = 0, //!< アトミック性のみを保証し, バリアやフェンスは使用しないため, コンパイラやハードウェアによって並べ替えられる可能性がある
		Consume                = 1, //!< 同じスレッドのほかの読み取りや書き込みが, この読み取りより前に並べ替えされないことを保証 
		Acquire                = 2, //!< この読み取りが完了すると, この読み取りより前に書き込まれたすべての読み取りや書き込みが完了することを保証
		Release                = 3, //!< この書き込みが完了すると, この書き込みより後に書き込まれたすべての読み取りや書き込みが完了することを保証
		AcquireRelease         = 4, //!< AcquireとReleaseの両方を提供
		SequentiallyConsistent = 5  //!< この読み取りや書き込みが完了すると, この読み取りや書き込みより前に書き込まれたすべての読み取りや書き込みが完了することを保証
	};

	/****************************************************************************
	*				  			   Atomic
	*************************************************************************//**
	/*  @brief    実行順序を保証するためのアトミック操作を提供します.@n
	*****************************************************************************/
	template<typename T>
	class Atomic : public NonCopyAndMove
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     値を取得します
		*  @attention 現在Windowsのみ対応しています
		*  @param[in] MemoryOrder order : メモリの実行順序を指定します
		*  @return    T
		*************************************************************************/
		T Load(const MemoryOrder order = MemoryOrder::SequentiallyConsistent) const;

		/*!**********************************************************************
		*  @brief     値を設定します
		*  @attention 現在Windowsのみ対応しています
		*  @param[in] T value
		*  @param[in] MemoryOrder order : メモリの実行順序を指定します
		*  @return    void
		*************************************************************************/
		void Store(const T value, const MemoryOrder order = MemoryOrder::SequentiallyConsistent);

		/*!**********************************************************************
		*  @brief     castをしたうえで値を設定します
		*  @attention 現在Windowsのみ対応しています
		*  @param[in] const OtherType value
		*  @param[in] MemoryOrder order : メモリの実行順序を指定します
		*  @return    void
		*************************************************************************/
		template<typename Other>
		__forceinline void Store(const Other value)
		{
			platform::core::AtomicOperator::Write(&_value, static_cast<T>(value));
		}

		/*!**********************************************************************
		*  @brief     値を加算します. 
		*  @param[in] const T value
		*  @return    T
		*************************************************************************/
		__forceinline T Add(const T value) 
		{
			return platform::core::AtomicOperator::Add(&_value, value);
		}

		/*!**********************************************************************
		*  @brief     値を減算します.
		*  @param[in] const T value
		*  @return    T
		* ************************************************************************/
		__forceinline T Subtract(const T value)
		{
			return platform::core::AtomicOperator::Subtract(&_value, value);
		}

		/*!**********************************************************************
		*  @brief     And演算を行います.
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T And(const T operand)
		{
			return platform::core::AtomicOperator::And(&_value, operand);
		}

		/*!**********************************************************************
		*  @brief     Or演算を行います.
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T Or(const T operand)
		{
			return platform::core::AtomicOperator::Or(&_value, operand);
		}

		/*!**********************************************************************
		*  @brief     Xor演算を行います.
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T Xor(const T operand)
		{
			return platform::core::AtomicOperator::Xor(&_value, operand);
		}

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		/*! @brief インクリメント*/
		__forceinline T operator++()
		{
			return platform::core::AtomicOperator::Increment(&_value);
		}

		/*! @brief デクリメント*/
		__forceinline T operator--()
		{
			return platform::core::AtomicOperator::Decrement(&_value);
		}

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

		/*! @brief Atomic操作用の値*/
		T _value = T();
		
		#pragma endregion 

	};

	template<typename T>
	T Atomic<T>::Load(const MemoryOrder order) const
	{
		switch (order)
		{
			case MemoryOrder::Relaxed:
			case MemoryOrder::SequentiallyConsistent:
			case MemoryOrder::Consume:
			case MemoryOrder::Acquire:
			{
				_value = platform::core::AtomicOperator::Read(&_value);
				break;
			}
			default:
			{
				throw "Invalid MemoryOrder";
			}
		}

		return _value;
	}

	template<typename T>
	void Atomic<T>::Store(const T value, const MemoryOrder order)
	{
		switch (order)
		{
			case MemoryOrder::Relaxed:
			case MemoryOrder::SequentiallyConsistent:
			case MemoryOrder::Release:
			{
				platform::core::AtomicOperator::Write(&_value, value);
				break;
			}
			default:
			{
				throw "Invalid MemoryOrder";
			}
		}
	}
}

#endif