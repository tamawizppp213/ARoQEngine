//////////////////////////////////////////////////////////////////////////////////
///             @file   ThreadPool.hpp
///             @brief  Thread pool executor
///                     Reference : https://contentsviewer.work/Master/Cpp/how-to-implement-a-thread-pool/article#cite-note-cpprefjp-how_to_use_cv
///                     使い方 : グローバル関数の非同期実行の場合は, Submit(関数名, 引数の値)
/// 　　　　　　　　　　　　　　　　　　 メンバ関数の非同期実行の場合は, Submit(ラムダ式, 引数でできます. )
///             @date   2023_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef THREAD_HPP
#define THREAD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../../Base/Include/ClassUtility.hpp"
#include "../GameUtility/Base/Include/ClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			Thread
	*************************************************************************//**
	*  @class     Thread
	*  @brief     Thread
	*****************************************************************************/
	class Thread : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

	};
}
#endif