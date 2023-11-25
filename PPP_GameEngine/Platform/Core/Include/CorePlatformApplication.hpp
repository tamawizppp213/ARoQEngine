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
#include "GameUtility/Base/Include/GUMemory.hpp"
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
	class PlatformCommand;

	/****************************************************************************
	*				  			    PlatformApplication
	*************************************************************************//**
	*  @class     PlatformApplication
	*  @brief     This class is the window list manager.
	*****************************************************************************/
	class PlatformApplication 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : This application creator is determined in according to platform macro*/
		static gu::SharedPointer<PlatformApplication> Create();

		virtual gu::SharedPointer<CoreWindow> MakeWindow() = 0;

		virtual gu::SharedPointer<PlatformCommand> MakeCommand() = 0;
		
		virtual void SetUpWindow(const gu::SharedPointer<CoreWindow>& window, const CoreWindowDesc& desc) = 0;

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
		virtual ~PlatformApplication();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		PlatformApplication();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<CoreWindowMessageHandler> _messageHandler = nullptr;
	};
}
#endif