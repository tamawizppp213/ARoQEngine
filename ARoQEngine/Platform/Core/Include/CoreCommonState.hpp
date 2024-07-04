//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreCommonState.hpp
///  @brief  Platform共通に使用する定義群です
///  @author Toide Yutaro
///  @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_COMMON_STATE_HPP
#define CORE_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

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
	****************************************************************************/
	/* @brief プラットフォームの種類を定義しています
	*****************************************************************************/
	enum class PlatformType
	{
		Windows, //!< Windows

	};

	#pragma region Mouse
	/****************************************************************************
	*				  			    CursorType
	****************************************************************************/
	/* @brief  マウスカーソルの種類を定義しています 
	*****************************************************************************/
	enum class CursorType
	{
		None,            //!< Causes no mouse cursor to be visible.
		Default,         //!< arrow cursor
		TextEditBeam,    //!< text edit beam
		ResizeLeftRight, //!< resize horizontal
		ResizeUpDown,    //!< resize vertical
		ResizeSouthEast, //!< resize diagonal
		ResizeSouthWest, //!< resize other diagonal
		CardinalCross,   //!< move item
		Crosshairs,      //!< target cross
		Hand,            //!< hand cursor
		Prohibit,        //!< prohibit
		Original,        //!< your original cursor
		CountOf,         //!< cursor count
	};

	/****************************************************************************
	*				  			  ActivationType
	****************************************************************************/
	/* @brief     ウィンドウの起動時, 非起動時の瞬間に呼ばれるenum classです.
	*****************************************************************************/
	enum class ActivationType
	{
		InActivate,    //!< 非アクティブ
		Activate,      //!< マウスクリック以外のメソッドによってアクティブ化
		ClickActivate, //!< マウスクリックでアクティブ化
	};

	#pragma endregion Mouse

	#pragma region Window
	/****************************************************************************
	*				  	WindowActionType
	****************************************************************************/
	/* @brief  ウィンドウの表示アクションの設定
	*****************************************************************************/
	enum class WindowActionType
	{
		None                 = 0, //!< 何もしない
		Maximize             = 1, //!< 最大化
		Minimize             = 2, //!< 最小化
		Restore              = 3, //!< ウィンドウをアクティブにして表示する. 最小化・最大化されている場合は元のサイズと位置に復元される.
		ClickedNonClientArea = 4, //!< ウィンドウ領域外のクリック
 	};

	/****************************************************************************
	*				  			  ActivationPolicy
	****************************************************************************/
	/* @brief     ウィンドウの表示時のアクティブ化の方針を決めるやつ
	*****************************************************************************/
	enum class ActivationPolicy
	{
		Never,        //!< 決してActive化しない
		Always,       //!< 表示したときに必ずwindowをActiveにする
		FirstOnlyShow //!< 最初の一回だけ
	};

	/****************************************************************************
	*				  			    WindowMode
	****************************************************************************/
	/* @brief フルスクリーンやウィンドウモードといったWindowの状態を定義しています
	*****************************************************************************/
	enum class WindowMode
	{ 
		FullScreen, //!< フルスクリーン
		BorderLess, //!< ボーダーレス
		Windowed,   //!< ウィンドウモード
		CountOf     //!< ウィンドウモードの数
	};

	/****************************************************************************
	*				  	   EngineWindowType
	****************************************************************************/
	/* @brief engineで使用するwindowの種類
	*****************************************************************************/
	enum class EngineWindowType
	{
		Normal, //!< 通常のウィンドウ (Editor)
		Menu,   //!< メニューウィンドウ
		Game,   //!< ゲームウィンドウ (ゲーム画面)
	};

	/****************************************************************************
	*				  			    WindowMode
	****************************************************************************/
	/* @brief  ウィンドウの透過指定
	*****************************************************************************/
	enum class WindowTransparency
	{
		None,      //!< 透過度をサポートしない
		PerWindow, //!< ウィンドウ・レベルで透過度をサポートしていることを示す値（1つの不透明度がウィンドウ全体に適用される
		PerPixel,  //!< ピクセル単位のアルファ混合透過をサポートしていることを示す値。
	};

	/****************************************************************************
	*				  			  CoreWindowDesc
	****************************************************************************/
	/* @brief ウィンドウ用のディスクリプタです
	*****************************************************************************/
	struct CoreWindowDesc
	{
		float DesiredScreenPositionX  = 0.0f;  //!< 最初に望ましいとされるスクリーンの横の位置
		float DesiredScreenPositionY  = 0.0f;  //!< 最初に望ましいとされるスクリーンの縦の位置
		float DesiredScreenWidth      = 0.0f;  //!< 最初に望ましいとされる幅
		float DesiredScreenHeight     = 0.0f;  //!< 最初に望ましいとされる高さ
		float ExpectedMaxWidth        = -1.0f; //!< ウィンドウの予想最大幅 (-1は無効)
		float ExpectedMaxHeight       = -1.0f; //!< ウィンドウの予想最大幅 (-1は無効)
		int   CornerRadius            = 0;     //!< 角の丸み
		float Opacity                 = 1.0f;  //!< ウィンドウの透明度
		wchar_t* Title                = L"PPP Game Window"; //!< ウィンドウのタイトル
		bool  HasWindowOSWindowBorder = true;  //!< OSのウィンドウの境界線を使用したい場合はtrue
		bool  IsVanillaWindow         = true;  //!< 特別な目的に使用するか.
		bool  SupportMaximize         = true;  //!< ウィンドウの最大化をするか  
		bool  SupportMinimize         = true;  //!< ウィンドウの最小化をするか
		bool  SupportResize           = true;  //!< ウィンドウのリサイズを可能にするか
		bool  AppearsInTaskbar        = false; //!< タスクバーにウィンドウを表示するか
		bool  IsTopmostWindow         = false; //!< 何よりも最前面にウィンドウを表示したい場合に使用します.
		bool  AcceptInput             = true;  //!< interactiveにするか
		bool  UseTouchFeedback        = false; //!< uiボタン等のタッチ入力を可能にするか
		bool  ShouldPreserveAspectRatio = false; //!< アスペクト比を一定にするか
		bool  SizeWillChangeOften       = false; //!< 頻繁にサイズ調整が入るか
		bool  HasCloseButton            = true;  //!< 閉じるボタンを必要とするか
		ActivationPolicy ActivationPolicy   = ActivationPolicy::FirstOnlyShow;
		WindowMode         WindowMode       = WindowMode::Windowed;
		WindowTransparency TransparencyType = WindowTransparency::PerWindow;
		EngineWindowType  EngineWindowType  = EngineWindowType::Normal;
	};
	
	struct Rectangle
	{
		long Left   = 0;
		long Top    = 0;
		long Right  = 0;
		long Bottom = 0;
	};

	/****************************************************************************
	*				  			 MonitorInfo
	****************************************************************************/
	/* @brief 使用しているモニターの情報を取得します
	*****************************************************************************/
	struct MonitorInfo
	{
		gu::tchar* Name             = nullptr;  //!< モニターの名前
		gu::tchar* ID               = nullptr;  //!< IDの名前
		gu::int32  NativeWidth      = 0;        //!< width
		gu::int32  NativeHeight     = 0;        //!< height
		gu::int32  DPI              = 0;        //!< Dots per inch (1インチあたりにドットが何個あるか)
		Rectangle  DisplayRectangle = {};       //!< モニター全体のRectangle
		Rectangle  WorkArea         = {};       //!< タスクバーとかを含まない作業領域自体のRectangle
		bool       IsPrimary        = false;    //!< プライマリーモニターか
	};

#pragma endregion Window
}

#endif
