//////////////////////////////////////////////////////////////////////////////////
///             @file   GUMemory.hpp
///             @brief  Shared pointer
///             @author toide
///             @date   2023/11/08 23:39:22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_SHARED_POINTER_HPP
#define GU_SHARED_POINTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUWeakPointer.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"
#include "GameUtility/Base/Include/GUTypeCast.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			   GUSharedPointer
	*************************************************************************//**
	*  @class     GUSharedPointer
	*  @brief     if the reference count is 0, the resource ownered by this will destroy.
	*****************************************************************************/
	template<class ElementType,  SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	class SharedPointer : public details::smart_pointer::ObserverPointerBase<ElementType, Mode>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Release the pointer and decrement the reference counter.
		/*----------------------------------------------------------------------*/
		__forceinline void Reset() 
		{
			ReleaseSharedReference(); 
			_referenceController = nullptr;
			_elementPointer = nullptr;
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty shared pointer.
		/*----------------------------------------------------------------------*/
		SharedPointer() : details::smart_pointer::ObserverPointerBase<ElementType, Mode>() { };

		SharedPointer(decltype(__nullptr)) : details::smart_pointer::ObserverPointerBase<ElementType, Mode>() {};

		/*----------------------------------------------------------------------
		*  Destructor 
		/*----------------------------------------------------------------------*/
		~SharedPointer()
		{
			Reset();
		}

		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer 
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		explicit SharedPointer(OtherType* pointer) 
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer) {};
		
		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer
		/*----------------------------------------------------------------------*/
		explicit SharedPointer(ElementType* pointer) 
			:details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer) { };

		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer with the custom deleter
		/*----------------------------------------------------------------------*/
		template<class Deleter>
		SharedPointer(ElementType* pointer, Deleter deleter)
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer, deleter) {};

		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer with the custom deleter
		/*----------------------------------------------------------------------*/
		template<class OtherType, class Deleter>
		SharedPointer(OtherType* pointer, Deleter deleter) : details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer, deleter) {};

		/*----------------------------------------------------------------------
		*  Constructs a shared pointer from the weak pointer
		/*----------------------------------------------------------------------*/
		explicit SharedPointer(const WeakPointer<ElementType,Mode>& pointer) : details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddSharedReference();
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Copy constructs a shared pointer from the same type shared pointer,
		/*----------------------------------------------------------------------*/
		SharedPointer(const SharedPointer<ElementType,Mode>& pointer) 
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddSharedReference();
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Copy constructs a shared pointer from the other type shared pointer,
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		SharedPointer(const SharedPointer<OtherType, Mode>& pointer) 
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddSharedReference();
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Copy constructs a shared pointer from the other type shared pointer,
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		SharedPointer& operator=(const SharedPointer<OtherType, Mode>& right) noexcept 
		{
			_elementPointer      = right._elementPointer;
			_referenceController = right._referenceController;
			AddSharedReference();
			AddObserverReference();
			return *this;
		}

		SharedPointer& operator=(const SharedPointer& right) noexcept
		{
			_elementPointer      = right._elementPointer;
			_referenceController = right._referenceController;
			AddSharedReference();
			AddObserverReference();
			return *this;
		}

		/*----------------------------------------------------------------------
		*  Move Constructs a shared pointer
		/*----------------------------------------------------------------------*/
		SharedPointer(SharedPointer&& right) noexcept : 
			ObserverPointerBase(type::Forward<ObserverPointerBase>(right))
		{
			
		};

		SharedPointer& operator=(SharedPointer&& right) noexcept
		{
			_elementPointer = right._elementPointer; _referenceController = right._referenceController;
			right._elementPointer = nullptr; right._referenceController = nullptr;
			return *this;
		}

		/*----------------------------------------------------------------------
		*  for static_pointer_cast. 
		/*----------------------------------------------------------------------*/
		SharedPointer(ElementType* pointer, details::smart_pointer::ReferenceControllerBase<Mode>* referenceController)
			: details::smart_pointer::ObserverPointerBase<ElementType, Mode>(pointer, referenceController)
		{
			AddSharedReference();
			AddObserverReference();
		};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		template<class OtherType, SharedPointerThreadMode OtherMode>
		friend class WeakPointer;
		template<class OtherType, SharedPointerThreadMode OtherMode>
		friend class SharedPointer;
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};


	template<class ElementType1, class ElementType2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	bool operator == (const SharedPointer<ElementType1,Mode>& left, const SharedPointer<ElementType2, Mode>& right) noexcept
	{
		return left.Get() == right.Get();
	}

	template<class ElementType1, class ElementType2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	bool operator != (const SharedPointer<ElementType1, Mode>& left, const SharedPointer<ElementType2,Mode>& right) noexcept
	{
		return left.Get() != right.Get();
	}


	/****************************************************************************
	*				  			   EnableSharedFromThis
	*************************************************************************//**
	*  @class     EnableSharedFromThis
	*  @brief     自分自身のクラスからスマートポインタの参照を渡すときに使用します. 
	*             現在, MakeShared時, インタフェース部分に(platform::coreとかに継承)継承を入れると認識しない場合があります. 
	*             そのため, MakeSharedを実際に適用するクラス(platform::windowsに継承)に直接継承を入れるようにしてください.
	*****************************************************************************/
	template<class ElementType, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	class EnableSharedFromThis
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		[[nodiscard]] SharedPointer<ElementType, Mode> SharedFromThis()       
		{ 
			return SharedPointer<ElementType, Mode>(_weakPointer);
		}
		[[nodiscard]] const SharedPointer<ElementType, Mode> SharedFromThis() const
		{
			return SharedPointer<ElementType, Mode>(_weakPointer);
		}

		void SetWeakPointer(const SharedPointer<ElementType, Mode>& pointer)
		{
			_weakPointer = WeakPointer<ElementType, Mode>(pointer);
		}

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		EnableSharedFromThis()
		{

		}

		virtual ~EnableSharedFromThis()
		{
			_weakPointer = nullptr;
		}

		EnableSharedFromThis& operator=(EnableSharedFromThis const&) { return *this; }

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		WeakPointer<ElementType, Mode> _weakPointer = nullptr;
	};

#pragma region Shared Pointer Implement
	/*----------------------------------------------------------------------
	*  Cast function
	/*----------------------------------------------------------------------*/
	template<class Element1, class Element2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	[[nodiscard]] SharedPointer<Element1, Mode> StaticPointerCast(const SharedPointer<Element2, Mode>& element)
	{
		return SharedPointer<Element1, SHARED_POINTER_DEFAULT_THREAD_MODE>(static_cast<Element1*>(element.Get()), element.GetRawReferenceController());
	}
	template<class Element1, class Element2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	[[nodiscard]] SharedPointer<Element1, Mode> ConstPointerCast(const SharedPointer<Element2, Mode>& element)
	{
		return SharedPointer<Element1, Mode>(const_cast<Element1*>(element.Get()), element.GetRawReferenceController());
	}
	template<class Element1, class Element2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	[[nodiscard]] SharedPointer<Element1, Mode> ReinterpretPointerCast(const SharedPointer<Element2,Mode>& element)
	{
		return SharedPointer<Element1,Mode>(reinterpret_cast<Element1*>(element.Get()), element.GetRawReferenceController());
	}
	template<class Element1, class Element2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	[[nodiscard]] SharedPointer<Element1,Mode> DynamicPointerCast(const SharedPointer<Element2,Mode>& element)
	{
		return SharedPointer<Element1,Mode>(dynamic_cast<Element1*>(element.Get()), element.GetRawReferenceController());
	}

	/*----------------------------------------------------------------------
	*  @brief :  return the new shared pointer
	/*----------------------------------------------------------------------*/
	template<class ElementType, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE, class... Arguments>
	SharedPointer<ElementType, Mode> MakeShared(Arguments&&... arguments)
	{
		SharedPointer<ElementType, Mode> pointer(new ElementType(type::Forward<Arguments>(arguments)...));
		
		// EnableSharedFromThisをサポートする場合, weak_pointerを設定する
		if constexpr(gu::type::IS_DERIVED_OF<ElementType, gu::EnableSharedFromThis<ElementType, Mode>>)
		{
			pointer->SetWeakPointer(pointer);
		}

		return pointer;
	};

#pragma endregion Shared Pointer Implement


}

#endif