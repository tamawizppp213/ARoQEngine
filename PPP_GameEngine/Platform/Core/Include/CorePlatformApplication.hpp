//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_PLATFORM_APPLICATION_HPP
#define CORE_PLATFORM_APPLICATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class CoreWindowMessageHandler;
	class CoreWindow;

	/****************************************************************************
	*				  			    PlatformApplication
	*************************************************************************//**
	*  @class     PlatformApplication
	*  @brief     This class is the window list manager.
	*****************************************************************************/
	class PlatformApplication : public NonCopyable, public std::enable_shared_from_this<PlatformApplication>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static std::shared_ptr<PlatformApplication> Create(const core::PlatformType type);

		/* @brief : This application creator is determined in according to platform macro*/
		static std::shared_ptr<PlatformApplication> Create();

		virtual std::shared_ptr<CoreWindow> MakeWindow() = 0;
		
		virtual void SetUpWindow(const std::shared_ptr<CoreWindow>& window, const CoreWindowDesc& desc) = 0;

		/* @brief : This function pumps window message, when you are returned the true, you accept the message. */
		virtual bool PumpMessage() = 0;

		/* @brief : Is platform application is quited.*/
		virtual bool IsQuit() const = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static constexpr wchar_t* APPLICATION_NAME = L"PPP Game Window";

		virtual void* GetInstanceHandle() const noexcept = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication() = default;

		virtual ~PlatformApplication() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<CoreWindowMessageHandler> _messageHandler = nullptr;
	};
}
#endif