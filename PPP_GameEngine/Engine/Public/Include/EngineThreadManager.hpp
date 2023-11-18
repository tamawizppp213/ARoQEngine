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
		UpdateMain,   // ���W�X�V���̏������s�����C���X���b�h. ��Ƀ��[�v���܂�
		RenderMain,   // �`��p�C�v���C���𔭍s���郁�C���X���b�h. ��Ƀ��[�v���܂�
		UpdateWorker, // �I�u�W�F�N�g�̍X�V�Ȃǂ��s���T�u�X���b�h. �^�X�N�Ɏg�p���܂�
		RenderWorker, // �R�}���h���X�g�ɋl�ߍ��ނȂǂ̃T�u�X���b�h. �^�X�N�Ɏg�p���܂�
		CountOf
	};

	/****************************************************************************
	*				  			    EngineThreadManager
	*************************************************************************//**
	*  @class     EngineThreadManager
	*  @brief     �Q�[���Ɏg�p����X���b�h���Ǘ�����N���X.
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