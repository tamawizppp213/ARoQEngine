//////////////////////////////////////////////////////////////////////////////////
//              Title:  ThreadPool.cpp
//            Content:  Thread pool executor
//             Author:  Toide Yutaro
//             Create:  2023_07_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Public/Include/GUThreadPool.hpp"
#include <assert.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor
ThreadPool::ThreadPool(const std::uint32_t threadCount) : _threadCount(threadCount)
{
	assert(("threadCount needs to over 1.", _threadCount > 0));

	// Limit max thread count
	if (_threadCount > std::thread::hardware_concurrency())
	{
		_threadCount = std::thread::hardware_concurrency();
	}

	// create threads
	_threads = std::make_unique<std::thread[]>(_threadCount);
	for (std::uint32_t i = 0; i < _threadCount; ++i)
	{
		_threads[i] = std::thread(&ThreadPool::ExecuteWork, this);
	}
}

ThreadPool::~ThreadPool()
{
	// Lock task queue to prevent adding new task
	{
		std::lock_guard<std::mutex> lock(_taskMutex);
		_isRunning = false;
	}

	// Wake up all waiting threads so that they may exist.
	_condition.notify_all();

	for (std::uint32_t i = 0; i < _threadCount; ++i)
	{
		_threads[i].join();
	}
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     ExecuteWork
****************************************************************************/
/* @fn        void ThreadPool::ExecuteWork()
*
*  @brief     A worker function to be assigned to each thread in the pool,
*             Continuously pops tasks out of the queue and execute them, as long as the atomic variable running is set to true.
*
*  @param[in] void
*
*  @return �@  void
*****************************************************************************/
void ThreadPool::ExecuteWork()
{
	while (true)
	{
		std::function<void()> task;
		{
			// Task queue����łȂ�. �������� Thread Pool���I������܂őҋ@����
			std::unique_lock<std::mutex> lock(_taskMutex);

			// ���������邽�߂̏������ł������Ƃ�notify_one, notify_all�ɂ���Ēʒm�����܂ŃX���b�h��ҋ@����.
			// pred���w�肷�邱�Ƃ�, pred(�Ăяo����true�ɂȂ�܂őҋ@�𑱍s����. )
			_condition.wait(lock, [&] { return !_taskQueue.empty() || !_isRunning; });

			// Thread pool���I������, Task queue����̎��̓��[�v�𔲂��� ->�@�X���b�h���I������.
			if (!_isRunning && _taskQueue.empty()) { return; }

			// Task queue����Task�����o��.
			task = std::move(_taskQueue.front());
			_taskQueue.pop();
		}

		task();
	}
}

#pragma endregion Main Function