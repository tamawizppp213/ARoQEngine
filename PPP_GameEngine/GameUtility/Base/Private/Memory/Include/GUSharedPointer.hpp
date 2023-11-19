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
	class SharedPointer : public ObserverPointerBase<ElementType, Mode>
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
		SharedPointer() : ObserverPointerBase<ElementType, Mode>() { };

		SharedPointer(decltype(__nullptr)) : ObserverPointerBase<ElementType, Mode>() {};

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
			: ObserverPointerBase<ElementType, Mode>(pointer) {};
		
		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer
		/*----------------------------------------------------------------------*/
		explicit SharedPointer(ElementType* pointer) 
			:ObserverPointerBase<ElementType, Mode>(pointer) { };

		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer with the custom deleter
		/*----------------------------------------------------------------------*/
		template<class Deleter>
		SharedPointer(ElementType* pointer, Deleter deleter)
			: ObserverPointerBase<ElementType, Mode>(pointer, deleter) {};

		/*----------------------------------------------------------------------
		*  Constructs a new shared pointer from the raw pointer with the custom deleter
		/*----------------------------------------------------------------------*/
		template<class OtherType, class Deleter>
		SharedPointer(OtherType* pointer, Deleter deleter) : ObserverPointerBase<ElementType, Mode>(pointer, deleter) {};

		/*----------------------------------------------------------------------
		*  Constructs a shared pointer from the weak pointer
		/*----------------------------------------------------------------------*/
		explicit SharedPointer(const WeakPointer<ElementType,Mode>& pointer) : ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddSharedReference();
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Copy constructs a shared pointer from the same type shared pointer,
		/*----------------------------------------------------------------------*/
		SharedPointer(const SharedPointer<ElementType,Mode>& pointer) 
			: ObserverPointerBase<ElementType, Mode>(pointer)
		{
			AddSharedReference();
			AddObserverReference();
		}

		/*----------------------------------------------------------------------
		*  Copy constructs a shared pointer from the other type shared pointer,
		/*----------------------------------------------------------------------*/
		template<class OtherType>
		SharedPointer(const SharedPointer<OtherType, Mode>& pointer) 
			: ObserverPointerBase<ElementType, Mode>(pointer)
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
		SharedPointer(SharedPointer&& right) noexcept 
		{
			_elementPointer = right._elementPointer; _referenceController = right._referenceController;
			right._elementPointer = nullptr; right._referenceController = nullptr;
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
		SharedPointer(ElementType* pointer, details::ReferenceControllerBase<Mode>* referenceController)
			: ObserverPointerBase<ElementType, Mode>(pointer, referenceController)
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
	const bool operator == (const SharedPointer<ElementType1,Mode>& left, const SharedPointer<ElementType2, Mode>& right) noexcept
	{
		return left.Get() == right.Get();
	}

	template<class ElementType1, class ElementType2, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	const bool operator != (const SharedPointer<ElementType1, Mode>& left, const SharedPointer<ElementType2,Mode>& right) noexcept
	{
		return left.Get() != right.Get();
	}


	/****************************************************************************
	*				  			   EnableSharedFromThis
	*************************************************************************//**
	*  @class     EnableSharedFromThis
	*  @brief     �������g�̃N���X����X�}�[�g�|�C���^�̎Q�Ƃ�n���Ƃ��Ɏg�p���܂�. 
	*             ����, MakeShared��, �C���^�t�F�[�X������(platform::core�Ƃ��Ɍp��)�p��������ƔF�����Ȃ��ꍇ������܂�. 
	*             ���̂���, MakeShared�����ۂɓK�p����N���X(platform::windows�Ɍp��)�ɒ��ڌp��������悤�ɂ��Ă�������.
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

		~EnableSharedFromThis() = default;

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
	SharedPointer<ElementType, Mode> MakeShared(Arguments... arguments)
	{
		auto pointer = SharedPointer<ElementType, Mode>(new ElementType(arguments...));
		if constexpr(std::derived_from<ElementType,gu::EnableSharedFromThis<ElementType,Mode>>)
		{
			pointer->SetWeakPointer(pointer);
		}
		return pointer;
	};

#pragma endregion Shared Pointer Implement


}

#endif