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


	/****************************************************************************
	*				  			    CursorType
	*************************************************************************//**
	*  @class     CursorType
	*  @brief     cursor look 
	*****************************************************************************/
	enum class CursorType
	{
		None, 
		Default,
		TextEditBeam,
		ResizeLeftRight,
		ResizeUpDown,
		ResizeSouthEast,
		ResizeSouthWest,
		CardinalCross,
		Crosshairs,
		Hand,
		Prohibit,
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
		bool  HasWindowOSWindowBorder = false; // OSのウィンドウの境界線を使用したい場合はtrue
		bool  IsVanillaWindow         = false;
		bool  SupportMaximize         = true;  // ウィンドウの最大化をするか  
		bool  SupportMinimize         = true;  // ウィンドウの最小化をするか
		bool  SupportResize           = true;  // ウィンドウのリサイズを可能にするか
		bool  AppearsInTaskbar        = false; // タスクバーにウィンドウを表示するか
		bool  UseTouchFeedback        = false; // uiボタン等のタッチ入力を可能にするか
		ActivationPolicy ActivationPolicy = ActivationPolicy::FirstOnlyShow;
		WindowMode       WindowMode       = WindowMode::Windowed;
	};
}

#endif
