//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_COMMON_STATE_HPP
#define CORE_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/****************************************************************************
	*				  			    PlatformType
	*************************************************************************//**
	*  @class     PlatformType
	*  @brief     プラットフォームの種類を定義しています
	*****************************************************************************/
	enum class PlatformType
	{
		Windows,

	};

#pragma region Mouse
	/****************************************************************************
	*				  			    CursorType
	*************************************************************************//**
	*  @class     CursorType
	*  @brief     cursor look 
	*****************************************************************************/
	enum class CursorType
	{
		None,            // Causes no mouse cursor to be visible.
		Default,         // arrow cursor
		TextEditBeam,    // text edit beam
		ResizeLeftRight, // resize horizontal
		ResizeUpDown,    // resize vertical
		ResizeSouthEast, // resize diagonal
		ResizeSouthWest, // resize other diagonal
		CardinalCross,   // move item
		Crosshairs,      // target cross
		Hand,            // hand cursor
		Prohibit,        // prohibit
		Original,        // your original cursor
		CountOf,
	};

	/****************************************************************************
	*				  			  ActivationType
	*************************************************************************//**
	*  @class     ActivationType
	*  @brief     ウィンドウの起動時, 非起動時の瞬間に呼ばれるenum classです.
	*****************************************************************************/
	enum class ActivationType
	{
		InActive,      // 非アクティブ
		Activate,      // マウスクリック以外のメソッドによってアクティブ化
		ClickActivate, // マウスクリックでアクティブ化
	};

#pragma endregion Mouse

#pragma region Window
	/****************************************************************************
	*				  			  ActivationPolicy
	*************************************************************************//**
	*  @class     ActivationPolicy
	*  @brief     ウィンドウの表示時のアクティブ化の方針を決めるやつ
	*****************************************************************************/
	enum class ActivationPolicy
	{
		Never,        // 決してActive化しない
		Always,       // 表示したときに必ずwindowをActiveにする
		FirstOnlyShow // 最初の一回だけ
	};

	/****************************************************************************
	*				  			    WindowMode
	*************************************************************************//**
	*  @class     WindowMode
	*  @brief     window mode
	*****************************************************************************/
	enum class WindowMode
	{
		FullScreen,
		BorderLess,
		Windowed,
		CountOf
	};

	/****************************************************************************
	*				  			  CoreWindowDesc
	*************************************************************************//**
	*  @struct     CoreWindowDesc
	*  @brief     ウィンドウ用のディスクリプタです
	*****************************************************************************/
	struct CoreWindowDesc
	{
		float DesiredScreenPositionX  = 0.0f;  // 最初に望ましいとされるスクリーンの横の位置
		float DesiredScreenPositionY  = 0.0f;  // 最初に望ましいとされるスクリーンの縦の位置
		float DesiredScreenWidth      = 0.0f;  // 最初に望ましいとされる幅
		float DesiredScreenHeight     = 0.0f;  // 最初に望ましいとされる高さ
		wchar_t* Title                = L"PPP Game Window";
		bool  HasWindowOSWindowBorder = true; // OSのウィンドウの境界線を使用したい場合はtrue
		bool  IsVanillaWindow         = true;
		bool  SupportMaximize         = true;  // ウィンドウの最大化をするか  
		bool  SupportMinimize         = true;  // ウィンドウの最小化をするか
		bool  SupportResize           = true;  // ウィンドウのリサイズを可能にするか
		bool  AppearsInTaskbar        = false; // タスクバーにウィンドウを表示するか
		bool  UseTouchFeedback        = false; // uiボタン等のタッチ入力を可能にするか
		bool  ShouldPreserveAspectRatio = false; // アスペクト比を一定にするか
		ActivationPolicy ActivationPolicy = ActivationPolicy::FirstOnlyShow;
		WindowMode       WindowMode       = WindowMode::Windowed;
	};
	
	struct Rectangle
	{
		long Left   = 0;
		long Top    = 0;
		long Right  = 0;
		long Bottom = 0;
	};
#pragma endregion Window
}

#endif
