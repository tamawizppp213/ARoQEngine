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
}
#endif