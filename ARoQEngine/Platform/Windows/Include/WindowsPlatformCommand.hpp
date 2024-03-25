//////////////////////////////////////////////////////////////////////////////////
///             @file   WindowsPlatformCommand.hpp
///             @brief  temp
///             @author toide
///             @date   2023/10/30 2:45:01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_COMMAND_HPP
#define WINDOWS_PLATFORM_COMMAND_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CorePlatformCommand.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   WindowsPlatformCommand
	*************************************************************************//**
	*  @class     WindowsPlatformCommand
	*  @brief     temp
	*****************************************************************************/
	class PlatformCommand : public platform::core::PlatformCommand
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
#pragma region Debugging Command
		bool UseDebugger() const override;

		/*----------------------------------------------------------------------
		*  @brief : �f�o�b�N�p�̃u���[�N�|�C���g��ݒu���܂�. 
		/*----------------------------------------------------------------------*/
		void PutDebugBreak() const override;

#pragma endregion Debugging Command

#pragma region Initialize and Finalize Windows.h
		/*----------------------------------------------------------------------
		*  @brief : COM���C�u�����̏�����
		/*----------------------------------------------------------------------*/
		static bool CoInitialize(const bool useMultiThread = false);

		/*----------------------------------------------------------------------
		*  @brief : COM���C�u�����̏I��
		/*----------------------------------------------------------------------*/
		static void CoUnInitialize();

#pragma endregion Initialize and Finalize Windows.h

		/*----------------------------------------------------------------------
		*  @brief : �I�u�W�F�N�g����肷��UUID�𔭍s���܂�.
		/*----------------------------------------------------------------------*/
		gu::UUID IssueUUID() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

}
#endif
#endif