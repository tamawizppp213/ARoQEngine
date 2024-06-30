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
#include <Ole2.h>
#include <oleidl.h>
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	****************************************************************************/
	/* @brief     描画用のウィンドウを作成するクラス
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow, public IDropTarget
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      Applicationに登録しつつ, 指定したDescriptorでウィンドウを作成します.
		*  @param[in]  const gu::SharedPointer<core::PlatformApplication>& application
		*  @param[in]  const core::CoreWindowDesc& desc
		*  @param[in]  const gu::SharedPointer<core::CoreWindow>& parentWindow
		*  @return     bool
		**************************************************************************/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr);

		/*!***********************************************************************
		*  @brief      Native Windowを表示します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Show() override;

		/*!***********************************************************************
		*  @brief      Native Windowを非表示にします
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Hide() override;

		/*!***********************************************************************
		*  @brief      ウィンドウを最小化します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Minimize() override;

		/*!***********************************************************************
		*  @brief      ウィンドウを最大化します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Maximize() override;

		/*!***********************************************************************
		*  @brief      ウィンドウをアクティブにして表示する. 最小化・最大化されている場合は元のサイズと位置に復元される.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ReStore() override;

		/*!***********************************************************************
		*  @brief      ウィンドウを破棄します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Destroy() override;

		/*!***********************************************************************
		*  @brief      ウィンドウサイズを変更します
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @param[in]  const gu::int32 width
		*  @param[in]  const gu::int32 height
		*  @return     void
		**************************************************************************/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) override;
		
		/*!***********************************************************************
		*  @brief      既に設定されたウィンドウを最前面に表示する
		*  @param[in]  const bool 強制的に最前面に表示するか
		*  @return     void
		**************************************************************************/
		virtual void BringToFront(const bool forceFront = false) override;

		/*!***********************************************************************
		*  @brief      指定したx, y座標にウィンドウを移動させます
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void Move(const gu::int32 x, const gu::int32 y) override;

		/*!***********************************************************************
		*  @brief      ウィンドウを有効/無効化する. 無効化する場合入力を受け付けない.
		*  @param[in]  const bool 有効化
		*  @return     void
		**************************************************************************/
		virtual void Enable(const bool enable) override { ::EnableWindow(_hwnd, enable); }

		/*!***********************************************************************
		*  @brief      透過度を変更する
		*  @param[in]  const core::WindowTransparency 透過度
		*  @return     void
		**************************************************************************/
		virtual void OnTransparencySupportChanged(const core::WindowTransparency transparency) override;
		
		#pragma region IDropTarget
		/*---------------------------------------------------------------
		　　　　　@brief : オブジェクトがコントロールの境界内にドラッグされると発生するイベント
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragEnter(
			/* [unique][in] */ __RPC__in_opt IDataObject* dataObject,
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

		/*---------------------------------------------------------------
		　　　　　@brief : オブジェクトがコントロールの境界を越えてドラッグされると発生するイベント
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragOver(
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

		/*---------------------------------------------------------------
		　　　　　@brief : オブジェクトがコントロールの境界外にドラッグされたときに発生するイベント
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragLeave() override;

		/*---------------------------------------------------------------
		　　　　　@brief : オブジェクトがドロップするときに発生するイベント
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE Drop(
			/* [unique][in] */ __RPC__in_opt IDataObject* dataObject,
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

		#pragma endregion IDropTarget

		#pragma region IUnknown Interface
		/*!***********************************************************************
		*  @brief      インタフェース (Droptarget)のポインタを返す
		*  @param[in]  REFIID riid
		*  @param[out] ppvObject
		*  @return     HRESULT
		**************************************************************************/
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

		/*!***********************************************************************
		*  @brief      OLEの参照カウンタを一つ上げる
		*  @param[in]  void
		*  @return     ULONG 参照カウンタ
		**************************************************************************/
		virtual ULONG STDMETHODCALLTYPE AddRef() override;

		/*!***********************************************************************
		*  @brief      OLEの参照カウンタを一つ下げる
		*  @param[in]  void
		*  @return     ULONG 参照カウンタ
		**************************************************************************/
		virtual ULONG STDMETHODCALLTYPE Release() override;
		#pragma endregion IUnknown Interface
		#pragma endregion	

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      フルスクリーンをサポートしているかを調べます.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsFullscreenSupported() const override;

		/*!***********************************************************************
		*  @brief      現在作業中のウィンドウであるかを調べます.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsForegroundWindow() const override;

		/*!***********************************************************************
		*  @brief      最大化されているかを調べます
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		/*!***********************************************************************
		*  @brief      最小化されているかを調べます (アイコン状態になっているのか)
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsMinimized() const override { return !!::IsIconic(_hwnd); }

		/*!***********************************************************************
		*  @brief      ウィンドウが有効化されているか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsEnabled() override { return !!::IsWindowEnabled(_hwnd); }

		/*!***********************************************************************
		*  @brief      Window handlerを取得します
		*  @param[in]  void
		*  @return     HWND
		**************************************************************************/
		[[nodiscard]] __forceinline HWND GetHWND() const { return _hwnd; }

		/*!***********************************************************************
		*  @brief      手動でDPIを変更可能にするか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsManualManageDPIChanges() const override { return _useManualDPIChange; }

		/*!***********************************************************************
		*  @brief      手動でDPIを変更する
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) override { _useManualDPIChange = manualManageDPIChange; }

		/*!***********************************************************************
		*  @brief      ウィンドウのハンドルを取得します
		*  @param[in]  void
		*  @return     void*
		**************************************************************************/
		[[nodiscard]] __forceinline virtual void* GetWindowHandle() const noexcept override { return _hwnd; }

		/*!***********************************************************************
		*  @brief      Windowのアスペクト比を渡します.
		*  @param[in]  void
		*  @return     float
		**************************************************************************/
		__forceinline virtual float GetAspectRatio() const override { return _aspectRatio; }

		/*!***********************************************************************
		*  @brief      Windowの左上位置を取得します.
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @return     void
		**************************************************************************/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const override;

		/*!***********************************************************************
		*  @brief      Windowのボーダーサイズを取得します.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowBorderSize() const override;

		/*!***********************************************************************
		*  @brief      Windowのタイトルバーのサイズを取得します.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowTitleBarSize() const override;

		/*!***********************************************************************
		*  @brief      ウィンドウが復元されたときのサイズと位置を返す
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height);

		/*!***********************************************************************
		*  @brief      座標の下にウィンドウが存在する場合はtrueを返す
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const override;
		
		/*!***********************************************************************
		*  @brief       Window handlerを設定します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/*!***********************************************************************
		*  @brief      フルスクリーンやウィンドウモードといったWindowの状態を設定します
		*  @param[in]  const WindowMode ウィンドウモード
		*  @return     void
		**************************************************************************/
		virtual void SetWindowMode(const core::WindowMode windowMode) override;

		/*!***********************************************************************
		*  @brief      キャプションにテキストを設定する
		*  @param[in]  const gu::tchar* const text
		*  @return     void
		**************************************************************************/
		__forceinline virtual void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/*!***********************************************************************
		*  @brief      透過度を設定する
		*  @param[in]  const float 透過度
		*  @return     void
		**************************************************************************/
		virtual void SetOpacity(const float opacity) const;

		/*!***********************************************************************
		*  @brief      KeyboardFocusにこのウィンドウを設定する
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void SetKeyboardFocus() override;

		/*!***********************************************************************
		*  @brief      FullScreen情報を取得する
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const override;

		/*!***********************************************************************
		*  @brief      親ウィンドウが最小化されたときに呼び出される関数
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool OnParentWindowMinimized();

		/*!***********************************************************************
		*  @brief      親ウィンドウがRestoreされたときに呼び出される関数
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool OnParentWindowRestored();

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		CoreWindow();

		/*! @brief デストラクタ*/
		~CoreWindow();

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion

		#pragma region Protected Function

		/*!***********************************************************************
		*  @brief      タッチフィードバックを無効化する.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void DisableTouchFeedback();
		
		/*!***********************************************************************
		*  @brief      ウィンドウ領域の調整
		*  @param[in]  const gu::int32 幅
		*  @param[in]  const gu::int32 高さ
		*  @return     void
		**************************************************************************/
		void AdjustWindowRegion(const gu::int32 width, const gu::int32 height);

		#pragma endregion

		#pragma region Protected Property

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief Window handle*/
		HWND _hwnd = NULL;

		/*! @brief WindowのClient領域のアスペクト比*/
		float _aspectRatio = 0.0f;

		/*! @brief ウィンドウがまだ最初のShow() を呼び出していないか*/
		bool _isFirstTimeVisible = true;

		/*! @brief 初回の最小化*/
		bool _initiallyMinimized = false;

		/*! @brief 初回の最大*/
		bool _initiallyMaximized = false;

		/*! @brief 96DPI以上のDPI設定を有効化するか*/
		bool _enableHighDPIMode = false;

		// @brief : ディスプレイの拡大率です. 
		float _dpiScaleFactor = 1.0f;

		// @brief : ディスプレイの仮想的な幅と高さを設定します. 
		// 実際にリサイズしてGPUバッファのパフォーマンスに影響を与えるのではなく, 
		// Windows側で小さく見えるようにトリミングして描画させるために使用します.
		gu::int32 _virtualWindowWidth  = 0;
		gu::int32 _virtualWindowHeight = 0;

		// @brief : 任意のピクセルががウィンドウ内にあるかどうかを問い合わせるためのウィンドウ領域サイズを格納する
		gu::int32 _regionWidth  = -1;
		gu::int32 _regionHeight = -1;

		// @brief : フルスクリーン状態になる前のウィンドウの配置を決定する
		WINDOWPLACEMENT _previousFullScreenWindowPlacement = WINDOWPLACEMENT();

		// @brief : 親ウィンドウが最小化される前の配置情報
		WINDOWPLACEMENT _previousParentMinimizedWindowPlacement = WINDOWPLACEMENT();

		// @brief : 参照カウント
		gu::int32 _oleReferenceCount = 0;

		// @brief : DPIを手動で変更するか
		bool _useManualDPIChange = false;

		#pragma endregion
	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!***********************************************************************
		*  @brief      Regionオブジェクトの作成
		*  @param[in]  const bool useBorderWhenMaximizedは最大化しているときにも境界部分を使うか.
		*  @return     void
		**************************************************************************/
		HRGN CreateWindowRegionObject(const bool useBorderWhenMaximized) const;
		#pragma endregion

		#pragma region Private Property

		#pragma endregion
	};
}
#endif _WIN32
#endif 