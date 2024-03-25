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
#include "GameUtility/Base/Private/Memory/Include/GUObserverPointer.hpp"

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
	class WeakPointer : public details::smart_pointer::ObserverPointerBase<ElementType, Mode>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Release the observer pointer
		/*----------------------------------------------------------------------*/
		__forceinline void Reset() { ReleaseObserverReference(); }


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty weak pointer.
		/*----------------------------------------------------------------------*/
		WeakPointer() : details::smart_pointer::ObserverPointerBase<ElementType, Mode>() {};

		WeakPointer(decltype(__nullptr)) : details::smart_pointer::ObserverPointerBase<ElementType, Mode>() {};

		/*----------------------------------------------------------------------
		*  Constructs a weak pointer from the shared pointer.
		/*----------------------------------------------------------------------*/
		explicit WeakPointer(const SharedPointer<ElementType, Mode>& pointer)
			: ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Constructs a weak pointer from the changable shared pointer.
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		explicit WeakPointer(const SharedPointer<OtherType, Mode>& pointer)
			:details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer) { AddObserverReference(); }

		/*----------------------------------------------------------------------
		*  Copy constructs have the same weak pointer type
		/*----------------------------------------------------------------------*/
		WeakPointer(const WeakPointer& pointer)
			:details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer) { AddObserverReference(); }

		WeakPointer& operator = (const WeakPointer& pointer);
		
		/*----------------------------------------------------------------------
		*  Copy constructs have the other pointer type
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		WeakPointer(const WeakPointer<OtherType>& pointer)
			: ObserverPointerBase<ElementType, Mode>(pointer) { AddObserverReference(); }

		template<class OtherType>
		WeakPointer& operator = (const WeakPointer<OtherType>& pointer)
		{
			_elementPointer      = pointer._elementPointer;
			_referenceController = pointer._referenceController;
			AddObserverReference();
			return *this;
		}

		/*----------------------------------------------------------------------
		*  Move constructs have the same weak pointer type
		/*----------------------------------------------------------------------*/
		WeakPointer(WeakPointer&& pointer) 
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer) {  }

		/*----------------------------------------------------------------------
		*  Move constructs have the other weak pointer type
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		WeakPointer(WeakPointer<OtherType>&& pointer) 
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer){}

		~WeakPointer(){ ReleaseObserverReference(); }

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/

	};

	template<class ElementType, SharedPointerThreadMode Mode>
	WeakPointer<ElementType, Mode>& WeakPointer<ElementType, Mode>::operator=(const WeakPointer<ElementType, Mode>& pointer)
	{
		if (*this == pointer) { return *this; }

		_elementPointer      = pointer._elementPointer;
		_referenceController = pointer._referenceController;
		AddObserverReference();
		return *this;
	}

}


#endif