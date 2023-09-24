//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_APPLICATION_HPP
#define WINDOWS_PLATFORM_APPLICATION_HPP

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../Private/Include/WindowsDeferredMessage.hpp"
#include <vector>
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
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		std::shared_ptr<core::CoreWindow> MakeWindow() override;

		void SetUpWindow(const std::shared_ptr<core::CoreWindow>& window, const core::CoreWindowDesc& desc) override;

		/* @brief : メッセージを出します. この関数は仮想キーメッセージが受け取られ, それをメッセージ形式に変換した時にtrueを返します*/
		bool PumpMessage() override;

		/* @brief : アプリケーションが終了したかを検知します.*/
		bool IsQuit() const override { return _isApplicationQuited; };
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }
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
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool RegisterWindowClass();

		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::vector<std::shared_ptr<windows::CoreWindow>> _windows = {};

		std::vector<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		// @brief : メインループの外で実行するときにtrueになります
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : アプリケーションが終了したときにtrueになる
		bool _isApplicationQuited = false;
	};
}
#endif
#endif