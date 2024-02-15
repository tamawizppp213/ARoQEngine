//////////////////////////////////////////////////////////////////////////////////
//              @file   PPPEngine.cpp
///             @brief  各分野のエンジンをまとめ, スレッドを管理するクラス
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/EngineThreadManager.hpp"
#include "GameUtility/Thread/Public/Include/GUSemaphore.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::core;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
EngineThreadManager::EngineThreadManager()
{
	_threadPools.resize(static_cast<int>(ThreadPoolType::CountOf));
	_hasCompletedExecution.Resize(static_cast<int>(ThreadPoolType::CountOf));

	_threadPools[(int)ThreadPoolType::RenderMain] = gu::MakeShared<ThreadPool>(1);
	_threadPools[(int)ThreadPoolType::UpdateMain] = gu::MakeShared<ThreadPool>(1);

	_hasCompletedSemaphore = MakeShared<Semaphore>();
}

EngineThreadManager::~EngineThreadManager()
{
	_threadPools.clear();
	_threadPools.shrink_to_fit();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
bool EngineThreadManager::CallExecuteComplete(const ThreadPoolType type)
{
	if (_hasCompletedExecution[(int)type])
	{
		return false;
	}

	_hasCompletedExecution[(int)type] = true;
	_fenceValue++;
	_hasCompletedSemaphore->Signal(_fenceValue);
	return true;
}

void EngineThreadManager::ShutDown()
{
	_hasCompletedSemaphore->Wait(_threadPools.size());

	_threadPools.clear();
	_threadPools.shrink_to_fit();
}
#pragma endregion Main Function