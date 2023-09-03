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
#include <Windows.h>
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
		void InitializeWindow(const std::shared_ptr<core::CoreWindow>& window) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetInstanceHandle() const noexcept { return _instanceHandle; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief : Event driven windows function*/
		static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		static LRESULT WindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool RegisterWindowClass();

		
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::shared_ptr<windows::CoreWindow> _window = nullptr;

	};
}
#endif
#endif