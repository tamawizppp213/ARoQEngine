//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_APPLICATION_HPP
#define WINDOWS_PLATFORM_APPLICATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../Private/Include/WindowsDeferredMessage.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	class CoreWindow;
	/****************************************************************************
	*				  			    PlatformApplication
	*************************************************************************//**
	*  @class     PlatformApplication
	*  @brief     ウィンドウを管理し, メッセージループを管理するクラス
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication, public gu::EnableSharedFromThis<PlatformApplication>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　@brief : 新規のウィンドウインスタンスを作成します. ここではセットアップは行いません
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::CoreWindow> MakeWindow() override;

		/*---------------------------------------------------------------
		　　@brief : 新規でコマンドをまとめたクラスのインスタンスを作成します.
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::PlatformCommand> MakeCommand() override;

		/*---------------------------------------------------------------
		　　@brief : 指定のウィンドウを実際に作成するまで行います. セットアップも行います
		-----------------------------------------------------------------*/
		void SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow) override;

		/*---------------------------------------------------------------
		　　@brief : メッセージを出します. この関数は仮想キーメッセージが受け取られ, 
		            それをメッセージ形式に変換した時にtrueを返します
		-----------------------------------------------------------------*/
		bool PumpMessage() override;

		/*---------------------------------------------------------------
		　　@brief : アプリケーションが終了したかを検知します.
		-----------------------------------------------------------------*/
		bool IsQuit() const override { return _isApplicationQuited; };
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }

#pragma region Monitor 
		/*---------------------------------------------------------------
		　　　　　@brief : モニターのdisplay per inchを取得する.
		-----------------------------------------------------------------*/
		virtual gu::int32 GetMonitorDPI(const core::MonitorInfo& monitorInfo) const override;

		/*---------------------------------------------------------------
		　　　　　@brief : 高DPIモードになっているかどうか
		-----------------------------------------------------------------*/
		virtual bool EnableHighDPIAwareness() const override { return _enableHighDPIMode; }

		/*---------------------------------------------------------------
		　　　　　@brief : あるピクセル位置でのDPIの拡大率を取得する
		-----------------------------------------------------------------*/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const override;

		/*---------------------------------------------------------------
		　　　　　@brief : タスクバーなどを無視した作業領域を返します.
		-----------------------------------------------------------------*/
		virtual core::Rectangle GetWorkArea(const core::Rectangle& window) override;

		/*---------------------------------------------------------------
		　　　　　@brief : モニターの情報を取得する
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(std::vector<core::MonitorInfo>& monitorInfo) const override;

#pragma endregion Monitor
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication();

		~PlatformApplication();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief : Event driven windows function*/
		static LRESULT CALLBACK StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual LRESULT ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/*---------------------------------------------------------------
		　　　　　@brief : 96dpi以上のdpiを設定するために使用します.
		-----------------------------------------------------------------*/
		virtual bool SetHighDPIMode() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		static BOOL CALLBACK MonitorEnumProcedure(HMONITOR monitor, HDC monitorDC, LPRECT rect, LPARAM userData);
		
		bool RegisterWindowClass();

		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::vector<gu::SharedPointer<windows::CoreWindow>> _windows = {};

		std::vector<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		// @brief : メインループの外で実行するときにtrueになります
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : アプリケーションが終了したときにtrueになる
		bool _isApplicationQuited = false;

		bool _enableHighDPIMode = true;
	};
}
#endif
#endif