//////////////////////////////////////////////////////////////////////////////////
///             @file   PPPEngine.hpp
///             @brief  Engine
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ENGINE_THREAD_MANAGER_HPP
#define ENGINE_THREAD_MANAGER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Thread/Public/Include/GUThreadPool.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	class ThreadPool;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::core
{
	enum class ThreadPoolType
	{
		UpdateMain,   // 座標更新等の処理を行うメインスレッド. 常にループします
		RenderMain,   // 描画パイプラインを発行するメインスレッド. 常にループします
		UpdateWorker, // オブジェクトの更新などを行うサブスレッド. タスクに使用します
		RenderWorker, // コマンドリストに詰め込むなどのサブスレッド. タスクに使用します
		CountOf
	};

	/****************************************************************************
	*				  			    EngineThreadManager
	*************************************************************************//**
	*  @class     EngineThreadManager
	*  @brief     ゲームに使用するスレッドを管理するクラス.
	*****************************************************************************/
	class EngineThreadManager final : public NonCopyable
	{
	private :
		using ThreadPoolPtr = gu::SharedPointer<gu::ThreadPool>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const ThreadPoolPtr GetThreadPool(const ThreadPoolType type) { return _threadPools[(int)type]; }
		const ThreadPoolPtr GetUpdateMainThread() { return _threadPools[(int)ThreadPoolType::UpdateMain]; }
		const ThreadPoolPtr GetRenderMainThread() { return _threadPools[(int)ThreadPoolType::RenderMain]; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		EngineThreadManager();

		~EngineThreadManager();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<ThreadPoolPtr> _threadPools = {};
	};
}
#endif