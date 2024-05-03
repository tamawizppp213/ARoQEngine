//////////////////////////////////////////////////////////////////////////////////
///             @file   GUInitializerList.hpp
///             @brief  {}を使って初期化できるためのクラス. 
///                     標準ライブラリを全く使用しない場合はinitializer_listを呼び出し, 
///                     そうでなければ自作クラスを使用する. (こちらの方が無駄なincludeを減らせます)
///             @author toide
///             @date   2024/02/08 22:18:27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_INITIALIZER_LIST_HPP
#define GU_INITIALIZER_LIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if __has_include (<initializer_list>)
#include <initializer_list>
#else
#include "GameUtility/Base/Include/GUType.hpp"
// stdでないと動かないため. 
namespace std 
{
	/****************************************************************************
	*				  			   GUInitializerList
	*************************************************************************//**
	*  @class     GUInitializerList
	*  @brief     temp
	*****************************************************************************/
	template<class T>
	class initializer_list
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		gu::uint64 size() const { return _length; }

		const T* begin() const { return _pointer; }

		const T* end() const { return _pointer + _length; }

		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		initializer_list() : _pointer(nullptr), _length(0) {}

	private:
		initializer_list(const T* pointer, gu::uint64 length) : _pointer(pointer), _length(length) {}

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		const T* _pointer = nullptr;

		gu::uint64 _length = 0;
	};
}
#endif
#endif