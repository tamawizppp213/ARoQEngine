//////////////////////////////////////////////////////////////////////////////////
///             @file   GUUniquePointer.hpp
///             @brief  temp
///             @author toide
///             @date   2023/11/19 14:01:58
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_UNIQUE_POINTER_HPP
#define GU_UNIQUE_POINTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Memory/Include/GUSharedDeleter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUUniquePointer
	*************************************************************************//**
	*  @class     UniquePointer
	*  @brief     This class has the only one ownered resource.
	*****************************************************************************/
	template<class ElementType, class Deleter = gu::DefaultDeleter<ElementType>>
	class UniquePointer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*   @brief : Function to relinquish ownership of a resource
		/*----------------------------------------------------------------------*/
		ElementType* Release() noexcept;

		/*----------------------------------------------------------------------
		*   @brief : Reset the pointer
		/*----------------------------------------------------------------------*/
		__forceinline void Reset(ElementType* pointer = nullptr)
		{
			Deleter()(_elementPointer);
			_elementPointer = pointer;
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		[[nodiscard]] __forceinline ElementType* Get() noexcept { return _elementPointer; }

		[[nodiscard]] __forceinline const ElementType* Get() const noexcept { return _elementPointer; }

		[[nodiscard]] __forceinline ElementType* operator -> () const { return _elementPointer; }

		[[nodiscard]] __forceinline ElementType& operator * () { return *_elementPointer; }
		
		[[nodiscard]] __forceinline const ElementType& operator* () const noexcept { return *_elementPointer; }

		explicit operator bool() const noexcept { return _elementPointer != nullptr; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty unique pointer.
		/*----------------------------------------------------------------------*/
		UniquePointer() noexcept : _elementPointer(nullptr) {};

		UniquePointer(decltype(__nullptr)) : _elementPointer(nullptr) {};

		/*----------------------------------------------------------------------
		*  Destructor
		/*----------------------------------------------------------------------*/
		~UniquePointer()
		{
			Deleter()(_elementPointer);
		}

		/*----------------------------------------------------------------------
		*  Constructs a new unique pointer from the raw pointer
		/*----------------------------------------------------------------------*/
		explicit UniquePointer(ElementType* elementPointer) : _elementPointer(elementPointer) {};

		/*----------------------------------------------------------------------
		*  Move Constructs a unique pointer
		/*----------------------------------------------------------------------*/
		UniquePointer(UniquePointer&& right) noexcept : _elementPointer(right.Release()) {};

		UniquePointer& operator=(UniquePointer&& right) noexcept
		{
			this->Reset(right.Release());
			return (*this);
		}

		/*----------------------------------------------------------------------
		*  Move Constructs a unique pointer from the other type unique pointer
		/*----------------------------------------------------------------------*/
		template<class OtherType, class OtherDeleter>
		UniquePointer(UniquePointer<OtherType, OtherDeleter>&& right) noexcept
			: _elementPointer(right.Release()){ };

		template<class OtherType, class OtherDeleter>
		UniquePointer<ElementType, Deleter>& UniquePointer<ElementType, Deleter>::operator = (UniquePointer<OtherType, OtherDeleter>&& right)
		{
			this->Reset(right.Release());
			return *this;
		}

		/*----------------------------------------------------------------------
		*  Copy Constructs : delete
		/*----------------------------------------------------------------------*/
		UniquePointer(const UniquePointer&)            = delete;
		UniquePointer& operator=(const UniquePointer&) = delete;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		template<class OtherType, class OtherDeleter>
		friend class UniquePointer;

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		ElementType* _elementPointer = nullptr;
	};

	template<class ElementType, class Deleter = DefaultDeleter<ElementType>, class... Arguments>
	UniquePointer<ElementType, Deleter> MakeUnique(Arguments&&... arguments)
	{
		return UniquePointer<ElementType, Deleter>(new ElementType(arguments...));
	}

	template<class ElementType1, class Deleter1, class ElementType2, class Deleter2>
	bool operator == (const UniquePointer<ElementType1, Deleter1>& left, const UniquePointer<ElementType2, Deleter2>& right) noexcept
	{
		return left.Get() == right.Get();
	}

	template<class ElementType1, class Deleter1, class ElementType2, class Deleter2>
	bool operator != (const UniquePointer<ElementType1, Deleter1>& left, const UniquePointer<ElementType2, Deleter2>& right) noexcept
	{
		return left.Get() != right.Get();
	}

#pragma region Unique pointer implement
	template<class ElementType, class Deleter>
	ElementType* UniquePointer<ElementType, Deleter>::Release() noexcept
	{
		ElementType* pointer = _elementPointer;
		_elementPointer = nullptr;
		return pointer;
	}
#pragma endregion Unique pointer implement
}
#endif