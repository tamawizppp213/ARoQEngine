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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	class ThreadPool;
	class Semaphore;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::core
{
	enum class ThreadPoolType
	{
		UpdateMain,   // ���W�X�V���̏������s�����C���X���b�h. ��Ƀ��[�v���܂�
		RenderMain,   // �`��p�C�v���C���𔭍s���郁�C���X���b�h. ��Ƀ��[�v���܂�
		//UpdateWorker, // �I�u�W�F�N�g�̍X�V�Ȃǂ��s���T�u�X���b�h. �^�X�N�Ɏg�p���܂�
		//RenderWorker, // �R�}���h���X�g�ɋl�ߍ��ނȂǂ̃T�u�X���b�h. �^�X�N�Ɏg�p���܂�
		CountOf
	};

	/****************************************************************************
	*				  			    EngineThreadManager
	****************************************************************************/
	/* @class     EngineThreadManager
	*  @brief     �Q�[���Ɏg�p����X���b�h���Ǘ�����N���X.
	*****************************************************************************/
	class EngineThreadManager final : public gu::NonCopyable
	{
	private :
		using ThreadPoolPtr = gu::SharedPointer<gu::ThreadPool>;
		using SemaphorePtr  = gu::SharedPointer<gu::Semaphore>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool CallExecuteComplete(const ThreadPoolType type);

		void ShutDown();

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		const ThreadPoolPtr GetThreadPool(const ThreadPoolType type) { return _threadPools[(int)type]; }
		const ThreadPoolPtr GetUpdateMainThread() { return _threadPools[(int)ThreadPoolType::UpdateMain]; }
		const ThreadPoolPtr GetRenderMainThread() { return _threadPools[(int)ThreadPoolType::RenderMain]; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		EngineThreadManager();

		virtual ~EngineThreadManager();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		gu::DynamicArray<ThreadPoolPtr> _threadPools = {};

		gu::DynamicArray<bool> _hasCompletedExecution = {};
		SemaphorePtr      _hasCompletedSemaphore = nullptr;
		gu::uint64        _fenceValue = 0;
	};
}
#endif