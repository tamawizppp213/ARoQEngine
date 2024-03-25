//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_MESSAGE_HANDLER_HPP
#define WINDOWS_WINDOW_MESSAGE_HANDLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  		CoreWindowMessageHandler
	*************************************************************************//**
	*  @class     CoreWindowMessageHandler
	*  @brief     Cursor interface
	*****************************************************************************/
	class CoreWindowMessageHandler : public core::CoreWindowMessageHandler
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool OnWindowActivationChanged(const CoreWindowPtr& window, const core::ActivationType type)override;

		bool OnWindowClosed(const CoreWindowPtr& window) override;

		bool OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindowMessageHandler();

		~CoreWindowMessageHandler();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _isResizing = false;
	};
}
#endif
#endif