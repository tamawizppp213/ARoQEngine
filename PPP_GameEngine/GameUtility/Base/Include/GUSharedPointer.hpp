//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSharedPointer.hpp
///             @brief  shared pointer
///             @author toide
///             @date   2023/11/08 23:39:22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_SHARED_POINTER_HPP
#define GU_SHARED_POINTER_HPP

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
	/****************************************************************************
	*				  			   GUSharedPointer
	*************************************************************************//**
	*  @class     GUSharedPointer
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType, SharedPointerThreadMode Mode = SharedPointerThreadMode::NotThreadSafe>
	class SharedPointer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Release the pointer and decrement the reference counter.
		/*----------------------------------------------------------------------*/
		void Reset();

		/*----------------------------------------------------------------------
		*  @brief : Add the reference count
		/*----------------------------------------------------------------------*/
		inline void AddReference() { _referenceCount->Add(); }

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
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SharedPointer() = default;

		SharedPointer(decltype(__nullptr)) : _pointer(nullptr), _referenceCount(nullptr) {};

		virtual ~SharedPointer() = default;

		explicit SharedPointer(ElementType* pointer) : _pointer(pointer), _referenceCount(new details::SharedReferencer<Mode>()) {};

		SharedPointer(const SharedPointer& right) : _pointer(right._pointer), _referenceCount(right._referenceCount)
		{
			AddReference();
		}

		SharedPointer& operator=(const SharedPointer& right);

		SharedPointer(SharedPointer&& right) : _pointer(right._pointer), _referenceCount(right._referenceCount)
		{
			right._pointer = nullptr; right._referenceCount = nullptr;
		}

		SharedPointer& operator=(SharedPointer&& right) noexcept
		{
			_pointer = right._pointer; _referenceCount = right._referenceCount;
			right._pointer = nullptr; right._referenceCount = nullptr;
			return *this;
		}

		[[nodiscard]] inline operator bool() const { return _pointer != nullptr; }

		[[nodiscard]] inline ElementType* operator ->() noexcept { return _pointer; }

		[[nodiscard]] inline ElementType& operator* () noexcept { return *_pointer; }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		ElementType* _pointer = nullptr;

		// Addreferenceのために外に逃がしておく
		details::SharedReferencer<Mode>* _referenceCount = nullptr;
	};

	/*----------------------------------------------------------------------
	*  @brief :  return the new shared pointer
	/*----------------------------------------------------------------------*/
	template<class ElementType, SharedPointerThreadMode Mode = SharedPointerThreadMode::NotThreadSafe, class... Arguments>
	SharedPointer<ElementType, Mode> MakeShared(Arguments... arguments)
	{
		return SharedPointer<ElementType, Mode>(new ElementType(arguments...));
	};

	/*----------------------------------------------------------------------
	*  @brief : ポインタを破棄して参照カウントを減らす
	/*----------------------------------------------------------------------*/
	template<class ElementType, SharedPointerThreadMode Mode>
	void SharedPointer<ElementType, Mode>::Reset()
	{
		if (_referenceCount == nullptr) { return; }

		_referenceCount->Release();
		
		if(_referenceCount->EnableDelete())
		{
			delete(_pointer);        _pointer = nullptr;
			delete(_referenceCount); _referenceCount = nullptr;
		}
		else
		{
			_pointer        = nullptr;
			_referenceCount = nullptr;
		}
	}

	template<class ElementType, SharedPointerThreadMode Mode>
	SharedPointer<ElementType, Mode>& SharedPointer<ElementType, Mode>::operator=(const SharedPointer<ElementType,Mode>& right)
	{
		if (this == right) { return *this; }

		_pointer        = right._pointer;
		_referenceCount = right._referenceCount;
		AddReference();
		return *this;
	}
}

#endif