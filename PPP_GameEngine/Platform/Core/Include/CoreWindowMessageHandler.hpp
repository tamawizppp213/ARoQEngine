//////////////////////////////////////////////////////////////////////////////////
///             @file   CoreWindowMessageHandler.hpp
///             @brief  �E�B���h�E���b�Z�[�W�̈�������o�^���܂�. 
///             @author Toide Yutaro
///             @date   2023_09_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_WINDOW_MESSAGE_HANDLER_HPP
#define CORE_WINDOW_MESSAGE_HANDLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <cstdint>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class CoreWindow;
	/****************************************************************************
	*				  			    CoreWindowMessageHandler
	*************************************************************************//**
	*  @class     CoreWindowMessageHandler
	*  @brief     �E�B���h�E�̃��b�Z�[�W���󂯎�������Ɏg�p����Handler�N���X
	*****************************************************************************/
	class CoreWindowMessageHandler : public NonCopyable
	{
	protected:
		using CoreWindowPtr = std::shared_ptr<core::CoreWindow>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : This function is called by window activation changed*/
		virtual bool OnWindowActivationChanged(const CoreWindowPtr& window, const core::ActivationType type) = 0;

		virtual bool OnWindowClosed(const CoreWindowPtr& window) = 0;
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
		CoreWindowMessageHandler() = default;

		virtual ~CoreWindowMessageHandler() = default;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		
	};
}
#endif
