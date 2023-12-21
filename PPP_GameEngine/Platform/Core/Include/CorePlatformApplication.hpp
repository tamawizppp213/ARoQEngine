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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include <vector>
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
		
		/*---------------------------------------------------------------
		　　　　　@brief : 指定のウィンドウを実際に作成し, セットアップするまで行います (初期化時に呼び出し必要)
		-----------------------------------------------------------------*/
		virtual void SetUpWindow(const gu::SharedPointer<CoreWindow>& window, const CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr) = 0;

		/* @brief : This function pumps window message, when you are returned the true, you accept the message. */
		virtual bool PumpMessage() = 0;

		/* @brief : Is platform application is quited.*/
		virtual bool IsQuit() const = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static constexpr wchar_t* APPLICATION_NAME = L"PPP Game Window";

		virtual void* GetInstanceHandle() const noexcept = 0;

#pragma region Monitor
		/*---------------------------------------------------------------
		　　　　　@brief : return the monitor DPI
		-----------------------------------------------------------------*/
		virtual gu::int32 GetMonitorDPI(const MonitorInfo& monitorInfo) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : return the high dpi mode
		-----------------------------------------------------------------*/
		virtual bool EnableHighDPIAwareness() const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Acquire the dpi zoom factor at pixel point (x,y)
		-----------------------------------------------------------------*/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Return the work area without including the task bar
		-----------------------------------------------------------------*/
		virtual Rectangle GetWorkArea(const Rectangle& window) = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Acquire the monitor list 
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(std::vector<core::MonitorInfo>& monitorInfo) const = 0;

		/*---------------------------------------------------------------
		　　　　　@brief : Set application message handle
		-----------------------------------------------------------------*/
		virtual void SetMessageHandler(const gu::SharedPointer<CoreWindowMessageHandler>& messageHandler)
		{
			_messageHandler = messageHandler;
		}

#pragma endregion Monitor
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		virtual ~PlatformApplication();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		PlatformApplication();

		virtual bool SetHighDPIMode() = 0;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<CoreWindowMessageHandler> _messageHandler = nullptr;
	};
}
#endif