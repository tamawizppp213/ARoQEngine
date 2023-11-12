//////////////////////////////////////////////////////////////////////////////////
///             @file   GUWeakPointer.hpp
///             @brief  temp
///             @author toide
///             @date   2023/11/11 15:08:24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_WEAK_POINTER_HPP
#define GU_WEAK_POINTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Private/Include/SharedReferencer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	template<class ElementType, SharedPointerThreadMode Mode>
	class SharedPointer;
	/****************************************************************************
	*				  			   GUWeakPointer
	*************************************************************************//**
	*  @class     GUWeakPointer
	*  @brief     参照カウントを増やさないポインタです
	*****************************************************************************/
	template<class ElementType, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	class WeakPointer
	{
	private:
		using BaseType = ElementType;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Release the pointer
		/*----------------------------------------------------------------------*/
		void Reset() { _pointer = nullptr; _referenceCount = nullptr; }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Returns the object referenced by this pointer
		/*----------------------------------------------------------------------*/
		[[nodiscard]] inline ElementType* Get() const { return _pointer; }

		/*----------------------------------------------------------------------
		*  @brief : Checks to see if this shared pointer is actually pointing to an object
		/*----------------------------------------------------------------------*/
		[[nodiscard]] inline bool IsValid() const { return _pointer != nullptr; }

		/*----------------------------------------------------------------------
		*  @brief : Has the reference count is 1
		/*----------------------------------------------------------------------*/
		inline bool IsUnique() const { return _referenceCount && _referenceCount->IsUnique(); }

		/*----------------------------------------------------------------------
		*  @brief : Return the reference count
		/*----------------------------------------------------------------------*/
		inline const int32 GetReferenceCount() const { return _referenceCount ? _referenceCount->GetReferenceCount() : 0; }

		[[nodiscard]] inline details::SharedReferencer<Mode>* GetRawSharedReferencer() const { return _referenceCount; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty weak pointer.
		/*----------------------------------------------------------------------*/
		WeakPointer() = default;

		WeakPointer(decltype(__nullptr)) : _pointer(nullptr), _referenceCount(nullptr) {};

		/*----------------------------------------------------------------------
		*  Constructs a weak pointer from the shared pointer.
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		WeakPointer(SharedPointer<OtherType, Mode>const& pointer) :
			_pointer(pointer._pointer), _referenceCount(pointer._referenceCount){ };
		
		template<class OtherType>
		WeakPointer& operator=(SharedPointer<OtherType, Mode>const& pointer)
		{
			_pointer = pointer._pointer; _referenceCount = pointer._referenceCount;
			return *this;
		};

		/*----------------------------------------------------------------------
		*  Constructs a weak pointer from a weak pointer of another type.
		*  This constructor is intended to allow derived - to - base conversions
		/*----------------------------------------------------------------------*/
		template<class DerivedType> requires std::is_base_of_v<BaseType, DerivedType>
		WeakPointer(WeakPointer<DerivedType, Mode>const& weakPointer)
			: _pointer(weakPointer._pointer), _referenceCount(weakPointer._referenceCount){ };

		/*----------------------------------------------------------------------
		*  Move Constructs a weak pointer from a weak pointer of another type.
		*  This constructor is intended to allow derived - to - base conversions
		/*----------------------------------------------------------------------*/
		template<class DerivedType> requires std::is_base_of_v<BaseType, DerivedType>
		WeakPointer(WeakPointer<DerivedType, Mode>&& weakPointer)
			: _pointer(weakPointer._pointer), _referenceCount(weakPointer._referenceCount)
		{
			weakPointer._pointer        = nullptr;
			weakPointer._referenceCount = nullptr;
		}

		virtual ~WeakPointer() = default;

		explicit WeakPointer(ElementType* pointer) : _pointer(pointer), _referenceCount(new details::SharedReferencer<Mode>()) {};

		WeakPointer(const WeakPointer& right) : _pointer(right._pointer), _referenceCount(right._referenceCount) {};

		WeakPointer& operator=(WeakPointer const& right)
		{
			if (this == right) { return *this; }

			_pointer        = right._pointer;
			_referenceCount = right._referenceCount;
			return *this;
		}

		WeakPointer(WeakPointer&& right) : _pointer(right._pointer), _referenceCount(right._referenceCount)
		{
			right._pointer = nullptr; right._referenceCount = nullptr;
		}

		WeakPointer& operator=(WeakPointer&& right) noexcept
		{
			_pointer = right._pointer; _referenceCount = right._referenceCount;
			right._pointer = nullptr; right._referenceCount = nullptr;
			return *this;
		}

		[[nodiscard]] inline operator bool() const { return _pointer != nullptr; }

		[[nodiscard]] inline ElementType* operator ->() const noexcept { return _pointer; }

		[[nodiscard]] inline ElementType& operator* () noexcept { return *_pointer; }

		[[nodiscard]] inline const ElementType& operator*() const noexcept { return *_pointer; }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		ElementType* _pointer = nullptr;

		details::SharedReferencer<Mode>* _referenceCount = nullptr;
	};
}


#endif