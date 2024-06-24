//////////////////////////////////////////////////////////////////////////////////
///             @file   GUObserverPointer.hpp
///             @brief  temp
///             @author toide
///             @date   2023/11/14 2:08:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_OBSERVER_POINTER_HPP
#define GU_OBSERVER_POINTER_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUReferenceController.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::smart_pointer
{
	/****************************************************************************
	*				  			   GUObserverPointer
	****************************************************************************/
	/* @class     GUObserverPointer
	*  @brief     Shared pointer and weak pointer's base class 
	*             Default deleter is DefaultDeleterClass
	*****************************************************************************/
	template<class ElementType, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	class ObserverPointerBase
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Returns the object referenced by this pointer
		*----------------------------------------------------------------------*/
		[[nodiscard]] __forceinline ElementType* Get() const { return _elementPointer; }

		/*----------------------------------------------------------------------
		*  @brief : Checks to see if this shared pointer is actually pointing to an object
		*----------------------------------------------------------------------*/
		[[nodiscard]] __forceinline bool IsValid() const { return _elementPointer != nullptr; }

		/*----------------------------------------------------------------------
		*  @brief : Has the reference count is 1
		*----------------------------------------------------------------------*/
		__forceinline bool IsUnique() const { return _referenceController && _referenceController->IsUnique(); }

		/*----------------------------------------------------------------------
		*  @brief : Return the reference count
		*----------------------------------------------------------------------*/
		__forceinline int32 GetReferenceCount() const { return _referenceController ? _referenceController->GetSharedReferenceCount() : 0; }

		/*----------------------------------------------------------------------
		*  @brief : Resource is valid .
		*----------------------------------------------------------------------*/
		__forceinline operator bool() const { return _elementPointer != nullptr; }

		/*----------------------------------------------------------------------
		*  @brief : Return the element pointer
		*----------------------------------------------------------------------*/
		[[nodiscard]] __forceinline ElementType* operator ->() const noexcept { return _elementPointer; }

		/*----------------------------------------------------------------------
		*  @brief : Return the element
		*----------------------------------------------------------------------*/
		[[nodiscard]] __forceinline ElementType& operator* () noexcept { return *_elementPointer; }

		[[nodiscard]] __forceinline const ElementType& operator*() const noexcept { return *_elementPointer; }
		
		[[nodiscard]] __forceinline ReferenceControllerBase<Mode>* GetRawReferenceController() const noexcept { return _referenceController; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty observer pointer
		*----------------------------------------------------------------------*/
		ObserverPointerBase() noexcept : _referenceController(nullptr), _elementPointer(nullptr) {};

		/*----------------------------------------------------------------------
		*  Constructs a new observer pointer using a resource pointer
		*----------------------------------------------------------------------*/
		explicit ObserverPointerBase(ElementType* elementPointer) : _elementPointer(elementPointer),
			_referenceController(new ReferenceController<ElementType, DefaultDeleter<ElementType>, Mode>(elementPointer)) {};

		/*----------------------------------------------------------------------
		*  Constructs a observer pointer using a resource pointer and referencecontroller
		*----------------------------------------------------------------------*/
		explicit ObserverPointerBase(ElementType* elementPointer, ReferenceControllerBase<Mode>* referenceController) : _elementPointer(elementPointer), _referenceController(referenceController) {};

		/*----------------------------------------------------------------------
		*  Constructs a new observer pointer using a changable resource pointer
		*----------------------------------------------------------------------*/
		template<class OtherType>
		explicit ObserverPointerBase(OtherType* elementPointer) : _elementPointer(elementPointer), _referenceController(new ReferenceController<ElementType, DefaultDeleter<ElementType>>(elementPointer)) {};

		/*----------------------------------------------------------------------
		*  Constructs a new observer pointer using a resource pointer and customize deleter
		*----------------------------------------------------------------------*/
		template<class Deleter>
		ObserverPointerBase(ElementType* elementPointer, Deleter deleter) : _elementPointer(elementPointer), _referenceController(new ReferenceController<ElementType, DefaultDeleter<ElementType>>(elementPointer)) {};

		/*----------------------------------------------------------------------
		*  Constructs a new observer pointer using a changable resource pointer and customize deleter
		*----------------------------------------------------------------------*/
		template<class OtherType, class Deleter>
		ObserverPointerBase(OtherType* elementPointer, Deleter deleter) : _elementPointer(elementPointer), _referenceController(new ReferenceController<ElementType, Deleter>(elementPointer)) {};

		/*----------------------------------------------------------------------
		*  Copy constructs
		*----------------------------------------------------------------------*/
		ObserverPointerBase(const ObserverPointerBase& pointer) : _elementPointer(pointer._elementPointer), _referenceController(pointer._referenceController) {};

		/*----------------------------------------------------------------------
		*  Changable copy constructs
		*----------------------------------------------------------------------*/
		template<class OtherType>
		ObserverPointerBase(const ObserverPointerBase<OtherType,Mode>& pointer) : _elementPointer(pointer._elementPointer), _referenceController(pointer._referenceController) {};

		/*----------------------------------------------------------------------
		*  Move constructs
		*----------------------------------------------------------------------*/
		ObserverPointerBase(ObserverPointerBase&& pointer) noexcept :
			_elementPointer(pointer._elementPointer), _referenceController(pointer._referenceController)
		{
			pointer._elementPointer      = nullptr;
			pointer._referenceController = nullptr;
		};

		/*----------------------------------------------------------------------
		*  Changable move constructs
		*----------------------------------------------------------------------*/
		template<class OtherType>
		ObserverPointerBase(ObserverPointerBase<OtherType,Mode>&& pointer) noexcept :
			_elementPointer(pointer._elementPointer), _referenceController(pointer._referenceController)
		{
			pointer._elementPointer      = nullptr;
			pointer._referenceController = nullptr;
		}

		~ObserverPointerBase() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Increment the shared reference count.
		*----------------------------------------------------------------------*/
		__forceinline void AddSharedReference() { if (_referenceController) { _referenceController->AddSharedReference(); } }

		/*----------------------------------------------------------------------
		*  @brief : Increment the weak + shared reference count.
		*----------------------------------------------------------------------*/
		__forceinline void AddObserverReference() { if (_referenceController) { _referenceController->AddObserverReference(); } }

		/*----------------------------------------------------------------------
		*  @brief : Decrement the shared reference count.
		*----------------------------------------------------------------------*/
		__forceinline void ReleaseSharedReference() 
		{
			if (_referenceController) 
			{ 
				_referenceController->ReleaseSharedReference(); 
				ReleaseObserverReference();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : Decrement the weak + shared reference count.
		*----------------------------------------------------------------------*/
		__forceinline void ReleaseObserverReference() 
		{
			if (_referenceController == nullptr) { return; }
			
			_referenceController->ReleaseObserverReference(); 

			if (_referenceController->GetSharedReferenceCount() == 0)
			{
				delete _referenceController;
				_referenceController = nullptr;
			}
		}
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/

		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		ReferenceControllerBase<Mode>* _referenceController = nullptr;

		ElementType* _elementPointer = nullptr;

	private:
		template<class OtherType, SharedPointerThreadMode>
		friend class ObserverPointerBase;
	};

}
#endif