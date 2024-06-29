//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAtomic.hpp
///  @brief  ���s������ۏ؂��邽�߂̃A�g�~�b�N�����񋟂��܂�.@n
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
	****************************************************************************/
	/*  @brief    ���s������ۏ؂��邽�߂̃A�g�~�b�N�����񋟂��܂�.@n
	*****************************************************************************/
	enum class MemoryOrder : uint8
	{
		Relaxed                = 0, //!< �A�g�~�b�N���݂̂�ۏ؂�, �o���A��t�F���X�͎g�p���Ȃ�����, �R���p�C����n�[�h�E�F�A�ɂ���ĕ��בւ�����\��������
		Consume                = 1, //!< �����X���b�h�̂ق��̓ǂݎ��⏑�����݂�, ���̓ǂݎ����O�ɕ��בւ�����Ȃ����Ƃ�ۏ� 
		Acquire                = 2, //!< ���̓ǂݎ�肪���������, ���̓ǂݎ����O�ɏ������܂ꂽ���ׂĂ̓ǂݎ��⏑�����݂��������邱�Ƃ�ۏ�
		Release                = 3, //!< ���̏������݂����������, ���̏������݂���ɏ������܂ꂽ���ׂĂ̓ǂݎ��⏑�����݂��������邱�Ƃ�ۏ�
		AcquireRelease         = 4, //!< Acquire��Release�̗������
		SequentiallyConsistent = 5  //!< ���̓ǂݎ��⏑�����݂����������, ���̓ǂݎ��⏑�����݂��O�ɏ������܂ꂽ���ׂĂ̓ǂݎ��⏑�����݂��������邱�Ƃ�ۏ�
	};

	/****************************************************************************
	*				  			   Atomic
	*************************************************************************//**
	/*  @brief    ���s������ۏ؂��邽�߂̃A�g�~�b�N�����񋟂��܂�.@n
	*****************************************************************************/
	template<typename T>
	class Atomic : public NonCopyAndMove
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �l���擾���܂�
		*  @attention ����Windows�̂ݑΉ����Ă��܂�
		*  @param[in] MemoryOrder order : �������̎��s�������w�肵�܂�
		*  @return    T
		*************************************************************************/
		T Load(const MemoryOrder order = MemoryOrder::SequentiallyConsistent);

		/*!**********************************************************************
		*  @brief     �l��ݒ肵�܂�
		*  @attention ����Windows�̂ݑΉ����Ă��܂�
		*  @param[in] T value
		*  @param[in] MemoryOrder order : �������̎��s�������w�肵�܂�
		*  @return    void
		*************************************************************************/
		void Store(const T value, const MemoryOrder order = MemoryOrder::SequentiallyConsistent);

		/*!**********************************************************************
		*  @brief     �l�����Z���܂�. 
		*  @attention ����Windows�̂ݑΉ����Ă��܂�
		*  @param[in] const T value
		*  @return    T
		*************************************************************************/
		__forceinline T Add(const T value, [[maybe_unused]] const MemoryOrder order = MemoryOrder::SequentiallyConsistent) 
		{
			return platform::core::AtomicOperator::Add(&_value, value);
		}

		/*!**********************************************************************
		*  @brief     �l�����Z���܂�.
		*  @attention ����Windows�̂ݑΉ����Ă��܂�
		*  @param[in] const T value
		*  @return    T
		* ************************************************************************/
		__forceinline T Subtract(const T value, [[maybe_unused]] const MemoryOrder order = MemoryOrder::SequentiallyConsistent)
		{
			return platform::core::AtomicOperator::Subtract(&_value, value);
		}

		/*!**********************************************************************
		*  @brief     And���Z���s���܂�.
		*  @attention ����Windows�̂ݑΉ����Ă��܂�
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T And(const T operand, [[maybe_unused]] const MemoryOrder order = MemoryOrder::SequentiallyConsistent)
		{
			return platform::core::AtomicOperator::And(&_value, operand);
		}

		/*!**********************************************************************
		*  @brief     Or���Z���s���܂�.
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T Or(const T operand, [[maybe_unused]] const MemoryOrder order = MemoryOrder::SequentiallyConsistent)
		{
			return platform::core::AtomicOperator::Or(&_value, operand);
		}

		/*!**********************************************************************
		*  @brief     Xor���Z���s���܂�.
		*  @param[in] const T operand
		*  @return    T
		* ************************************************************************/
		__forceinline T Xor(const T operand, [[maybe_unused]] const MemoryOrder order = MemoryOrder::SequentiallyConsistent)
		{
			return platform::core::AtomicOperator::Xor(&_value, operand);
		}

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		/*! @brief ���*/
		__forceinline T operator=(const T value) volatile noexcept
		{
			Store(value);
			return _value;
		}

		/*! @brief ��������*/
		__forceinline bool operator==(const T value) const noexcept
		{
			return _value == value;
		}

		/*! @brief �������Ȃ���*/
		__forceinline bool operator!=(const T value) const noexcept
		{
			return _value != value;
		}

		/*! @brief �傫����*/
		__forceinline bool operator>(const T value) const noexcept
		{
			return _value > value;
		}

		/*! @brief �傫������������*/
		__forceinline bool operator>=(const T value) const noexcept
		{
			return _value >= value;
		}

		/*! @brief ��������*/
		__forceinline bool operator<(const T value) const noexcept
		{
			return _value < value;
		}

		/*! @brief ����������������*/
		__forceinline bool operator<=(const T value) const noexcept
		{
			return _value <= value;
		}


		/*! @brief �C���N�������g*/
		__forceinline T operator++()
		{
			return platform::core::AtomicOperator::Increment(&_value);
		}

		/*! @brief �f�N�������g*/
		__forceinline T operator--()
		{
			return platform::core::AtomicOperator::Decrement(&_value);
		}

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		
		/*! @brief �l��ݒ�*/
		__forceinline Atomic(const T value)
		{
			Store(value);
		}

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		__forceinline Atomic()
		{
			Store(0);
		}

		/*! @brief �f�X�g���N�^*/
		__forceinline ~Atomic() noexcept = default;

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

		/*! @brief Atomic����p�̒l*/
		T _value = T();
		
		#pragma endregion 

	};

	template<typename T>
	T Atomic<T>::Load(const MemoryOrder order)
	{
		T result;

		switch (order)
		{
			case MemoryOrder::Relaxed:
			case MemoryOrder::SequentiallyConsistent:
			case MemoryOrder::Consume:
			case MemoryOrder::Acquire:
			{
				result = platform::core::AtomicOperator::Read(&_value);
				break;
			}
			default:
			{
				throw "Invalid MemoryOrder";
			}
		}

		return result;
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