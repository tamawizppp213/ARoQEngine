//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_HPP
#define WINDOWS_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreWindow.hpp"
#include <Windows.h>
#include "GameUtility/Base/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    CoreWindow
	*************************************************************************//**
	*  @class     CoreWindow
	*  @brief     描画用のウィンドウを作成するクラス
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc);

		/* @brief : Visibleでなかった場合にウィンドウを表示します.*/
		bool Show() override;

		/* @brief : Visible出会った場合にウィンドウを隠します*/
		bool Hide() override;

		/* @brief : ウィンドウを最小化します*/
		bool Minimize() override;

		/* @brief : ウィンドウを最大化します.*/
		bool Maximize() override;

		/* @brief : ウィンドウをアクティブにして表示する。最小化・最大化されている場合は元のサイズと位置に復元される.*/
		bool ReStore() override;

		/* @brief : ウィンドウを破棄します*/
		bool Destroy() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void* GetWindowHandle() const noexcept override { return _hwnd; }

		float GetAspectRatio() const override { return _aspectRatio;  }

		HWND GetHWND() const { return _hwnd; }

		void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/* @brief : キャプションにテキストを追加します. */
		void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/* @brief : 現在作業中のウィンドウであるかを調べます. */
		bool IsForegroundWindow() const override;

		/* @brief : フルスクリーンをサポートしているかを調べます. */
		bool IsFullscreenSupported() const override;

		// @brief : 最大化されているかを調べます
		bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		// @brief : 最小化されているかを調べます (アイコン状態になっているか)
		bool IsMinimized() const override { return !!::IsIconic(_hwnd); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow();

		~CoreWindow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		float _aspectRatio = 0.0f;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;
	};
}
#endif _WIN32
#endif 