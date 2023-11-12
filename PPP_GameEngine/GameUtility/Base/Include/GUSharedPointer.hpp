//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSharedPointer.hpp
///             @brief  shared pointer やることstatic_pointer_castとshared_from_this
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
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
	class SharedPointer
	{
	private:
		using BaseType = ElementType;
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
		[[nodiscard]] __forceinline ElementType* Get() const { return _pointer; }

		/*----------------------------------------------------------------------
		*  @brief : Checks to see if this shared pointer is actually pointing to an object
		/*----------------------------------------------------------------------*/
		[[nodiscard]] __forceinline bool IsValid() const { return _pointer != nullptr; }

		/*----------------------------------------------------------------------
		*  @brief : Has the reference count is 1
		/*----------------------------------------------------------------------*/
		__forceinline bool IsUnique() const { return _referenceCount && _referenceCount->IsUnique(); }

		/*----------------------------------------------------------------------
		*  @brief : Return the reference count 
		/*----------------------------------------------------------------------*/
		__forceinline const int32 GetReferenceCount() const { return _referenceCount ? _referenceCount->GetReferenceCount() : 0; }
		
		[[nodiscard]] __forceinline details::SharedReferencer<Mode>* GetRawSharedReferencer() const { return _referenceCount; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  Constructs an empty shared pointer.
		/*----------------------------------------------------------------------*/
		SharedPointer() = default;

		SharedPointer(decltype(__nullptr)) : _pointer(nullptr), _referenceCount(nullptr) {};

		virtual ~SharedPointer() 
		{
			Reset(); 
		}

		explicit SharedPointer(ElementType* pointer) : _pointer(pointer), _referenceCount(new details::SharedReferencer<Mode>()) {};

		SharedPointer(const SharedPointer& right) : _pointer(right._pointer), _referenceCount(right._referenceCount)
		{
			AddReference();
		}

		SharedPointer& operator=(const SharedPointer& right);

		/*----------------------------------------------------------------------
		*  Move Constructs a weak pointer from a weak pointer of another type.
		*  This constructor is intended to allow derived - to - base conversions
		/*----------------------------------------------------------------------*/
		SharedPointer(SharedPointer&& right) noexcept : _pointer(right._pointer), _referenceCount(right._referenceCount)
		{
			right._pointer = nullptr; right._referenceCount = nullptr;
		}

		SharedPointer& operator=(SharedPointer&& right) noexcept
		{
			_pointer = right._pointer; _referenceCount = right._referenceCount;
			right._pointer = nullptr; right._referenceCount = nullptr;
			return *this;
		}

		/*----------------------------------------------------------------------
		*  Move Constructs a weak pointer from a weak pointer of another type.
		*  This constructor is intended to allow derived - to - base conversions
		/*----------------------------------------------------------------------*/
		template<class DerivedType> requires std::is_base_of_v<BaseType, DerivedType>
		SharedPointer(SharedPointer<DerivedType, Mode>&& sharedPointer)
			: _pointer(sharedPointer.Get()), _referenceCount(sharedPointer.GetRawSharedReferencer())
		{
			sharedPointer._pointer = nullptr;
			sharedPointer._referenceCount = nullptr;
		}

		/*----------------------------------------------------------------------
		*  for static_pointer_cast. 
		/*----------------------------------------------------------------------*/
		SharedPointer(ElementType* pointer, details::SharedReferencer<Mode>* reference)
			: _pointer(pointer), _referenceCount(reference)
		{
			AddReference();

		};

		template<class OtherType>
		SharedPointer(WeakPointer<OtherType, Mode>& weakPointer)
		{
			_pointer = (ElementType*)weakPointer.Get();
			_referenceCount = weakPointer.GetRawSharedReferencer();
			AddReference();
		}
		template<class OtherType>
		SharedPointer(const WeakPointer<OtherType, Mode>& weakPointer)
		{
			_pointer = (ElementType*)weakPointer.Get();
			_referenceCount = weakPointer.GetRawSharedReferencer();
			AddReference();
		}

		[[nodiscard]] inline operator bool() const { return _pointer != nullptr; }

		[[nodiscard]] inline ElementType* operator ->() const noexcept { return _pointer; }

		[[nodiscard]] inline ElementType& operator* () noexcept { return *_pointer; }

		[[nodiscard]] inline const ElementType& operator*() const noexcept { return *_pointer; }
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
		ElementType* _pointer = nullptr;

		// Addreferenceのために外に逃がしておく
		details::SharedReferencer<Mode>* _referenceCount = nullptr;
	};


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
	template<class Element1, class Element2>
	[[nodiscard]] SharedPointer<Element1> StaticPointerCast(const SharedPointer<Element2>& element)
	{
		return SharedPointer<Element1>(static_cast<Element1*>(element.Get()), element.GetRawSharedReferencer());
	}
	template<class Element1, class Element2>
	[[nodiscard]] SharedPointer<Element1> ConstPointerCast(const SharedPointer<Element2>& element)
	{
		return SharedPointer<Element1>(const_cast<Element1*>(element.Get()), element.GetRawSharedReferencer());
	}
	template<class Element1, class Element2>
	[[nodiscard]] SharedPointer<Element1> ReinterpretPointerCast(const SharedPointer<Element2>& element)
	{
		return SharedPointer<Element1>(reinterpret_cast<Element1*>(element.Get()), element.GetRawSharedReferencer());
	}
	template<class Element1, class Element2>
	[[nodiscard]] SharedPointer<Element1> DynamicPointerCast(const SharedPointer<Element2>& element)
	{
		return SharedPointer<Element1>(dynamic_cast<Element1*>(element.Get()), element.GetRawSharedReferencer());
	}

	/*----------------------------------------------------------------------
	*  @brief :  return the new shared pointer
	/*----------------------------------------------------------------------*/
	template<class ElementType, SharedPointerThreadMode Mode = SharedPointerThreadMode::NotThreadSafe, class... Arguments>
	SharedPointer<ElementType, Mode> MakeShared(Arguments... arguments)
	{
		auto pointer = SharedPointer<ElementType, Mode>(new ElementType(arguments...));
		if constexpr(std::derived_from<ElementType,gu::EnableSharedFromThis<ElementType,Mode>>)
		{
			pointer->SetWeakPointer(pointer);
		}
		return pointer;
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
		if (*this == right) { return *this; }

		_pointer        = right._pointer;
		_referenceCount = right._referenceCount;
		AddReference();
		return *this;
	}

#pragma endregion Shared Pointer Implement


}

#endif