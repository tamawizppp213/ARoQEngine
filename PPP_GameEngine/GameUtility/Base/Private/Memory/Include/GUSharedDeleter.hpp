//////////////////////////////////////////////////////////////////////////////////
///             @file   SharedDeleter.hpp
///             @brief  Deleter class
///             @author toide
///             @date   2023/11/13 19:46:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SHARED_DELETER_HPP
#define SHADER_DELETER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	namespace details
	{
		template<class ElementType>
		inline void SafeDelete(ElementType*& pointer)
		{
			if (pointer != nullptr)
			{
				delete pointer;
				pointer = nullptr;
			}
		}

		template<class ElementType>
		inline void SafeArrayDelete(ElementType*& pointer)
		{
			if (pointer != nullptr)
			{
				delete[] pointer;
				pointer = nullptr;
			}
		}
	}
	/****************************************************************************
	*				  			   SharedDeleter
	*************************************************************************//**
	*  @class     SharedDeleter
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType>
	class DefaultDeleter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// 関数呼び出し演算子のオーバーロード
		// pointer : 解放したいインスタンスへのポインタ
		void operator () (ElementType*& pointer)
		{
			details::SafeDelete<ElementType>(pointer);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DefaultDeleter() noexcept = default;

		~DefaultDeleter() = default;
	};

	/****************************************************************************
	*				  			   SharedDeleter
	*************************************************************************//**
	*  @class     SharedDeleter
	*  @brief     temp
	*****************************************************************************/
	template<class ElementType>
	class ArrayDeleter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// 関数呼び出し演算子のオーバーロード
		// pointer : 解放したいインスタンスへのポインタ
		void operator () (ElementType*& pointer)
		{
			details::SafeArrayDelete<ElementType>(pointer);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ArrayDeleter() noexcept = default;

		~ArrayDeleter() = default;
	};
}
#endif