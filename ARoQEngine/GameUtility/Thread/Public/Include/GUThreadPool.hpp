//////////////////////////////////////////////////////////////////////////////////
///             @file   ThreadPool.hpp
///             @brief  Thread pool executor
///                     Reference : https://contentsviewer.work/Master/Cpp/how-to-implement-a-thread-pool/article#cite-note-cpprefjp-how_to_use_cv
///                     �g���� : �O���[�o���֐��̔񓯊����s�̏ꍇ��, Submit(�֐���, �����̒l)
/// �@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@ �����o�֐��̔񓯊����s�̏ꍇ��, Submit(�����_��, �����łł��܂�. )
///             @date   2023_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <functional>
#include <thread>
#include <memory>
#include <atomic>
#include <mutex>
#include <queue>
#include <future>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			ThreadPool
	****************************************************************************/
	/* @class     ThreadPool
	*  @brief     Thread pool
	*****************************************************************************/
	class ThreadPool : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// �C�ӂ̕Ԃ�l, �����ɑΉ�������. 
		template<typename Function, typename... Arguments, typename Return = std::invoke_result_t<std::decay_t<Function>, std::decay_t<Arguments>...>>
		std::future<Return> Submit(Function&& function, const Arguments&&... arguments)
		{
			// packaged task��, �ʃX���b�h�ł̏���������҂�, ���̏������ʂ��擾����񓯊��������������邽�߂̃N���X. 
			auto task = std::make_shared<std::packaged_task<Return()>>([function, arguments...]()
				{
					return function(arguments...);
				});

			auto future = task->get_future();

			PushTask([task]() { (*task)(); });
			return future;
		}

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		std::uint32_t GetThreadCount() const { return _threadCount; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ThreadPool(const std::uint32_t threadCount = std::thread::hardware_concurrency());

		virtual ~ThreadPool();
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		template<typename Function>
		void PushTask(const Function& task);

		void ExecuteWork();

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		std::unique_ptr<std::thread[]> _threads;

		// A queue of tasks to be executed by the threads
		std::queue<std::function<void()>> _taskQueue;

		// a mutex to synchronize access to the task queue by different threads.
		mutable std::mutex _taskMutex = {};

		// An atomic variable indicating to the workers to keep running.
		// When set to false, the workers permanently stop working.
		std::atomic<bool> _isRunning = true;

		// The number of threads in the pool
		std::uint32_t _threadCount = 0;

		// A condition variable used to notify worker threads of state changes
		std::condition_variable _condition;
	};
}
template<typename Function>
void gu::ThreadPool::PushTask(const Function& task)
{
	{
		const std::lock_guard<std::mutex> lock(_taskMutex);

		if (!_isRunning) { throw std::runtime_error("Cannot schedule new task after shutdown"); }

		_taskQueue.push(std::function<void()>(task));
	}

	// �ҋ@���Ă���X���b�h��������ĂыN����. 
	_condition.notify_one();
}
#endif