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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <cstdint>
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
	****************************************************************************/
	/* @class     CoreWindowMessageHandler
	*  @brief     �E�B���h�E�̃��b�Z�[�W���󂯎�������Ɏg�p����Handler�N���X
	*****************************************************************************/
	class CoreWindowMessageHandler : public gu::NonCopyable
	{
	protected:
		using CoreWindowPtr = gu::SharedPointer<core::CoreWindow>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : This function is called by window activation changed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowActivationChanged([[maybe_unused]]const CoreWindowPtr& window, [[maybe_unused]]const core::ActivationType type) = 0;

		/*----------------------------------------------------------------------
		*  @brief : This function is called by window closed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowClosed(const CoreWindowPtr& window) = 0;

		/*----------------------------------------------------------------------
		*  @brief : This function is called by the window size changed
		*----------------------------------------------------------------------*/
		virtual bool OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height) = 0;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		virtual ~CoreWindowMessageHandler();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		CoreWindowMessageHandler();

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		
	};
}
#endif
