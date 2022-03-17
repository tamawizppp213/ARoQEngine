//////////////////////////////////////////////////////////////////////////////////
///             @file   Memory.hpp
///             @brief  Memory
///             @author Toide Yutaro
///             @date   2022_03_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MEMORY_HPP
#define MEMORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	/****************************************************************************
	*				  			UniquePointer
	*************************************************************************//**
	*  @class     UniquePointer
	*  @brief     Simple unique pointer
	*****************************************************************************/
	template <typename T>
	class UniquePointer
	{
	public:
		using InterfaceType = T;
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T* Get    () const { return _pointer; }
		T* Release() noexcept { T* temp = _pointer; _pointer = nullptr; return temp; }
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		UniquePointer() = default;
		UniquePointer(decltype(__nullptr)) :_pointer(nullptr) {};
		explicit UniquePointer(T* pointer) : _pointer(pointer) {};
		~UniquePointer() { if (_pointer) { delete _pointer; } }

		UniquePointer(const UniquePointer&)            = delete;
		UniquePointer& operator=(const UniquePointer&) = delete;

		UniquePointer(UniquePointer&& right) : _pointer(right._pointer) { right._pointer = nullptr; }
		UniquePointer& operator=(UniquePointer&& right)  noexcept
		{
			if (&right == this) { return *this; }
			delete _pointer; _pointer = right._pointer; right._pointer = nullptr; return *this; 
		}

		T& operator * () const { return *_pointer; }
		T* operator ->() const { return _pointer; }
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		InterfaceType* _pointer;
		
	};
	template <typename T>
	class SharedPointer
	{
	public:
		using InterfaceType = T;
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		T* Get() const { return _pointer; }
		unsigned long RefCount() const { return *_refCount; }
		void AddRef();
		void Release();
		void Swap(SharedPointer<T>& right);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SharedPointer() = default;
		explicit SharedPointer(T* pointer) : _pointer(pointer), _refCount(new unsigned long(1)) {};
		SharedPointer(decltype(__nullptr)) :_pointer(nullptr), _refCount(nullptr) {};
		~SharedPointer() { Release(); }
		SharedPointer(const SharedPointer& right) : _pointer(right._pointer), _refCount(right._refCount) { AddRef(); }
		SharedPointer& operator=(const SharedPointer& right)
		{
			if (this == &right) { return *this; }
			Release();
			_pointer = right._pointer;
			_refCount = right._refCount;
			AddRef();
			return *this;
		}
		SharedPointer(SharedPointer&& right) :_pointer(right._pointer), _refCount(right._refCount)
		{
			right._pointer = nullptr; right._refCount = nullptr;
		}
		SharedPointer& operator=(SharedPointer&& right)
		{
			Release(); 
			_pointer = right._pointer; _refCount = right._refCount;
			right._pointer = nullptr; right._refCount = nullptr;
			return *this;
		}

		T& operator* () noexcept { return *_pointer; }
		T* operator ->() noexcept { return _pointer; }
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		InterfaceType* _pointer  = nullptr;
		unsigned long* _refCount = nullptr;
	};
#pragma region Unique Pointer
	template <typename T> UniquePointer<T> MakeUnique()
	{
		return UniquePointer<T>(new T());
	}
	template <typename T, typename Types> UniquePointer<T> MakeUnique(Types&& types)
	{
		return UniquePointer<T>(new T(static_cast<Types&&>(types)));
	}
#pragma endregion Unique Pointer
#pragma region Shared Pointer
	template<typename T> void SharedPointer<T>::AddRef()
	{
		if (_refCount) { ++(*_refCount); }
	}
	template<typename T> void SharedPointer<T>::Release()
	{
		if (_refCount == nullptr) { return; }
		if (--(*_refCount) == 0)
		{
			delete _pointer ; _pointer  = nullptr;
			delete _refCount; _refCount = nullptr;
		}
	}
	template<typename T> void SharedPointer<T>::Swap(SharedPointer<T>& right)
	{
		T* temp        = _pointer;
		pointer        = right._pointer;
		right._pointer = temp;
	}
	template <typename T> SharedPointer<T> MakeShared()
	{
		return SharedPointer<T>(new T());
	}
#pragma endregion Shared Pointer
}
#endif