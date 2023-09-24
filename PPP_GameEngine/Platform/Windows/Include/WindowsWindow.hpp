//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_HPP
#define WINDOWS_WINDOW_HPP

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CoreWindow.hpp"
#include <Windows.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Create(const std::shared_ptr<core::PlatformApplication>& application, const core::CoreWindowDesc& desc);

		/* @brief : Visibleでなかった場合にウィンドウを表示します.*/
		bool Show() override;

		/* @brief : Visible出会った場合にウィンドウを隠します*/
		bool Hide() override;

		bool Minimize() override;

		bool Maximize() override;

		/* @brief : ウィンドウをアクティブにして表示する。最小化・最大化されている場合は元のサイズと位置に復元される.*/
		bool ReStore() override;

		/* @brief : ウィンドウを破棄します*/
		bool Destroy() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HWND GetHWND() const { return _hwnd; }

		void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/* @brief : 現在作業中のウィンドウであるかを調べます. */
		bool IsForegroundWindow() const override;

		/* @brief : フルスクリーンをサポートしているかを調べます. */
		bool IsFullscreenSupported() const override;
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
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;
	};
}
#endif _WIN32
#endif 