//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSemaphore.cpp
///             @brief  複数スレッドでの同期を行うためのクラス
///             @author Toide Yutaro
///             @date   2023_09_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUSemaphore.hpp"
#include <mutex>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     Signal
****************************************************************************/
/* @fn        void Semaphore::Signal(const std::uint64_t value)
*
*  @brief     カウンターを指定の値に更新します
*
*  @param[in] const std::uint64_t スレッドが完了報告するために必要な待つための値
*
*  @return    void
*****************************************************************************/
void Semaphore::Signal(const std::uint64_t value)
{
	// 関数を抜けるまで現在のスレッドをブロッキング
	std::unique_lock<std::mutex> lock(_lockMutex);
	
	// 値の更新
	_completedValue = value;

	// スレッドを一つ呼び起こす
	_conditionVariable.notify_one();
}

/****************************************************************************
*                     Wait
****************************************************************************/
/* @fn        void Semaphore::Wait(const std::uint64_t value)
*
*  @brief     現在のスレッドに対して, 指定の値になるまで待ちます. 
*
*  @param[in] const std::uint64_t スレッドが完了したかを検査するための値
*  @param[in] const gu::Optional<int> タイムアウトに必要なmillsecondの時間
*
*  @return    void
*****************************************************************************/
void Semaphore::Wait(const std::uint64_t value, const gu::Optional<int> timeoutMilliSeconds )
{
	std::unique_lock<std::mutex> lock(_lockMutex);
	
	if (timeoutMilliSeconds.HasValue())
	{
		_conditionVariable.wait_for(lock, std::chrono::milliseconds(timeoutMilliSeconds.Value()), [&]() { return value <= _completedValue; });
	}
	else
	{
		_conditionVariable.wait(lock, [&]() { return value <= _completedValue; });
	}

}
#pragma endregion Main Function