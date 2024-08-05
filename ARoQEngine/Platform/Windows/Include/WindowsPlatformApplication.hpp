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
	****************************************************************************/
	/* @brief     ウィンドウを管理し, メッセージループを管理するクラス
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication, public gu::EnableSharedFromThis<PlatformApplication>
	{
	public:
		#pragma region Public Function

		/*!***********************************************************************
		*  @brief      新規のウィンドウインスタンスを作成します. ここではセットアップは行いません
		*  @param[in]  void
		*  @return     SharedPointer<core::CoreWindow> ウィンドウ
		**************************************************************************/
		gu::SharedPointer<core::CoreWindow> MakeWindow() override;

		/*---------------------------------------------------------------
		　　@brief : 新規でコマンドをまとめたクラスのインスタンスを作成します.
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::PlatformCommand> MakeCommand() override;

		/*!***********************************************************************
		*  @brief      指定のウィンドウを実際に作成し, セットアップするまで行います (初期化時に呼び出し必要)
		*  @param[in]  const gu::SharedPointer<CoreWindow>& window
		*  @param[in]  const CoreWindowDesc& windowの設定項目
		*  @param[in]  const gu::SharedPointer<CoreWindow>& 親のウィンドウ
		*  @return     SharedPointer<core::CoreWindow> ウィンドウ
		**************************************************************************/
		void SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow) override;

		/*---------------------------------------------------------------
		　　@brief : メッセージを出します. この関数は仮想キーメッセージが受け取られ, 
		            それをメッセージ形式に変換した時にtrueを返します
		-----------------------------------------------------------------*/
		bool PumpMessage() override;

		/*!***********************************************************************
		*  @brief      アプリケーションが終了したかを検知します.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool IsQuit() const override { return _isApplicationQuited; };

		#pragma endregion
		
		#pragma region Public Property

		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }

		#pragma region Monitor 
		/*!***********************************************************************
		*  @brief      モニターのdot per inchを取得する.  https://zenn.dev/tenka/articles/windows_display_monitor_dpi
		*  @param[in]  const core::MonitorInfo& monitor
		*  @return     gu::int32 DPI
		**************************************************************************/
		virtual gu::int32 GetMonitorDPI(const core::MonitorInfo& monitorInfo) const override;

		/*!***********************************************************************
		*  @brief      高DPIモードになっているかどうか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool EnableHighDPIAwareness() const override { return _enableHighDPIMode; }

		/*!***********************************************************************
		*  @brief      あるピクセル位置でのDPIの拡大率を取得する
		*  @param[in]  const float x
		*  @param[in]  const float y
		*  @return     float 拡大率
		**************************************************************************/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const override;

		/*!***********************************************************************
		*  @brief      タスクバーなどを無視した作業領域を返します.
		*  @param[in]  const core::Rectangle& ウィンドウ
		*  @return     core::Rectangle 作業領域
		**************************************************************************/
		virtual core::Rectangle GetWorkArea(const core::Rectangle& window) override;

		/*---------------------------------------------------------------
		　　　　　@brief : モニターの情報を取得する
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const override;

		#pragma endregion Monitor

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		PlatformApplication();

		~PlatformApplication();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!***********************************************************************
		*  @brief      Windowsのイベントを実行するコールバック関数
		*  @param[in]  HWND  ウィンドウハンドル
		*  @param[in]  UINT  メッセージ
		*  @param[in]  WPARAM 
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		static LRESULT CALLBACK StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      Windowsのイベントを実行する実態
		*  @param[in]  HWND  ウィンドウハンドル
		*  @param[in]  UINT  メッセージ
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		virtual LRESULT ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/*!***********************************************************************
		*  @brief      96dpi以上のdpiを設定するために使用します.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool SetHighDPIMode() override;

		#pragma endregion

		#pragma region Protected Property
		HINSTANCE _instanceHandle = NULL;

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		/*!***********************************************************************
		*  @brief 	   モニターの情報を取得するためのコールバック関数
		*  @param[in]  HMONITOR monitor : モニターのハンドル
		*  @param[in]  HDC monitorDC : モニターのデバイスコンテキスト
		*  @param[in]  LPRECT rect : モニターの矩形
		*  @param[in]  LPARAM userData : ユーザーデータ
		*  @return     bool : ウィンドウクラスの登録に成功したかどうか
		**************************************************************************/
		static BOOL CALLBACK MonitorEnumProcedure(HMONITOR monitor, [[maybe_unused]]HDC monitorDC, [[maybe_unused]]LPRECT rect, LPARAM userData);
		
		/*!***********************************************************************
		*  @brief      Windowアプリケーションで新しいウィンドウクラスを登録します
		*  @param[in]  void
		*  @return     bool : ウィンドウクラスの登録に成功したかどうか
		**************************************************************************/
		bool RegisterWindowClass() const;

		/*!***********************************************************************
		*  @brief      ウィンドウの各メッセージを処理する実体(即時処理を行いたい処理を記述します)
		*  @param[in]  HWND ウィンドウハンドル
		*  @param[in]  UINT メッセージ
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		LRESULT ProcessImmediateWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      遅延処理を実行するためのウィンドウメッセージを登録します
		*  @param[in]  HWND ウィンドウハンドル
		*  @param[in]  UINT メッセージ
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		LRESULT PushDeferredWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      ウィンドウの各メッセージを処理する実体 (遅延してまとめて処理しても問題無いような処理を記述します.)
		*  @param[in]  const DeferredMessage Windows専用のメッセージ
		*  @return     LRESULT
		**************************************************************************/
		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();

		/*!***********************************************************************
		*  @brief      接続されているマウスの個数を取得します. 
		*  @param[in]  void
		*  @return     gu:int32 
		**************************************************************************/
		gu::int32 GetConnectedMiceCount();

		#pragma endregion
		
		#pragma region Private Property
		gu::DynamicArray<gu::SharedPointer<windows::CoreWindow>> _windows = {};

		gu::DynamicArray<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		/*! @brief 変更中のウィンドウの幅*/
		gu::uint32 _resizingWidth = 0;
		gu::uint32 _resizingHeight = 0;

		// @brief : メインループの外で実行するときにtrueになります
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : アプリケーションが終了したときにtrueになる
		bool _isApplicationQuited = false;

		/*! @brief マウスを使用することでウィンドウを強制的にアクティブ状態にするか*/
		bool _isForceActivateByMouse = false;

		/*! @brief サイズ変更もしくは移動中*/
		bool _inModalSizeLoop = false;

		/*! @brief リサイズ中*/
		bool _isResizing = false;

		bool _enableHighDPIMode = true;

		/*! @brief 接続されているマウスが存在するか*/
		bool _isConnectedMouse = false;
		#pragma endregion
	};
}
#endif
#endif