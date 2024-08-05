//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSemaphore.cpp
///             @brief  �����X���b�h�ł̓������s�����߂̃N���X
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
*  @brief     �J�E���^�[���w��̒l�ɍX�V���܂�
*
*  @param[in] const std::uint64_t �X���b�h�������񍐂��邽�߂ɕK�v�ȑ҂��߂̒l
*
*  @return    void
*****************************************************************************/
void Semaphore::Signal(const std::uint64_t value)
{
	// �֐��𔲂���܂Ō��݂̃X���b�h���u���b�L���O
	std::unique_lock<std::mutex> lock(_lockMutex);
	
	// �l�̍X�V
	_completedValue = value;

	// �X���b�h����ĂыN����
	_conditionVariable.notify_one();
}

/****************************************************************************
*                     Wait
****************************************************************************/
/* @fn        void Semaphore::Wait(const std::uint64_t value)
*
*  @brief     ���݂̃X���b�h�ɑ΂���, �w��̒l�ɂȂ�܂ő҂��܂�. 
*
*  @param[in] const std::uint64_t �X���b�h���������������������邽�߂̒l
*  @param[in] const gu::Optional<int> �^�C���A�E�g�ɕK�v��millsecond�̎���
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