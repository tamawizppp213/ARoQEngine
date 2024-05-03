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
	*************************************************************************//**
	*  @class     CoreWindow
	*  @brief     描画用のウィンドウを作成するクラス
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow, public IDropTarget
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : Applicationに登録しつつ, 指定したDescriptorでウィンドウを作成します. 
		-----------------------------------------------------------------*/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr);

		/*---------------------------------------------------------------
		　　　　　@brief : Visibleでなかった場合にウィンドウを表示します
		-----------------------------------------------------------------*/
		virtual bool Show() override;

		/*---------------------------------------------------------------
		　　　　　@brief : Visibleであった場合にウィンドウを隠します
		-----------------------------------------------------------------*/
		virtual bool Hide() override;

		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウを最小化します
		-----------------------------------------------------------------*/
		virtual bool Minimize() override;

		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウを最大化します.
		-----------------------------------------------------------------*/
		virtual bool Maximize() override;

		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウをアクティブにして表示する
			            最小化・最大化されている場合は元のサイズと位置に復元される.
		-----------------------------------------------------------------*/
		virtual bool ReStore() override;

		/*---------------------------------------------------------------
		　　　　　@brief :  ウィンドウを破棄します
		-----------------------------------------------------------------*/
		virtual bool Destroy() override;

		/*---------------------------------------------------------------
		　　　　　@brief :  ウィンドウサイズを変更します
		-----------------------------------------------------------------*/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) override;
		
		/*---------------------------------------------------------------
		　　　　　@brief :  既に設定されたウィンドウを最前面に表示する
		-----------------------------------------------------------------*/
		virtual void BringToFront(const bool forceFront = false) override;

		/*---------------------------------------------------------------
		　　　　　@brief : 指定したx, y座標にウィンドウを移動させます
		-----------------------------------------------------------------*/
		virtual void Move(const gu::int32 x, const gu::int32 y) override;

		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウを有効/無効化する. 無効化する場合入力を受け付けない.
		-----------------------------------------------------------------*/
		virtual void Enable(const bool enable) override { ::EnableWindow(_hwnd, enable); }

		/*---------------------------------------------------------------
		　　　　　@brief : 透過度を変更する	
		-----------------------------------------------------------------*/
		void OnTransparenySupportChanged(const core::WindowTransparency transparency);
		
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
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

		virtual ULONG STDMETHODCALLTYPE AddRef() override;

		virtual ULONG STDMETHODCALLTYPE Release() override;
#pragma endregion IUnknown Interface
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : Windows専用じゃない場合のWindow Handlerの受け渡しです.
		-----------------------------------------------------------------*/
		[[nodiscard]] __forceinline virtual void* GetWindowHandle() const noexcept override { return _hwnd; }

		/*---------------------------------------------------------------
		　　　　　@brief : Windowのアスペクト比を渡します.
		-----------------------------------------------------------------*/
		__forceinline virtual float GetAspectRatio() const override { return _aspectRatio;  }

		/*---------------------------------------------------------------
		　　　　　@brief : Window handlerを取得します
		-----------------------------------------------------------------*/
		[[nodiscard]] __forceinline HWND GetHWND() const { return _hwnd; }

		/*---------------------------------------------------------------
		　　　　　@brief : return the border size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowBorderSize() const override;

		/*---------------------------------------------------------------
		　　　　　@brief : return the title bar size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowTitleBarSize() const override;

		/*---------------------------------------------------------------
		　　　　　@brief : windowの現在の左上の位置を返します.
		-----------------------------------------------------------------*/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const override;
		/*---------------------------------------------------------------
		　　　　　@brief : return the title bar size
		-----------------------------------------------------------------*/
		bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height);

		/*---------------------------------------------------------------
		　　　　　@brief : 指定した位置がウィンドウ中に存在するかを判定します.
		-----------------------------------------------------------------*/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const override;
		
		/*---------------------------------------------------------------
		　　　　　@brief : Window handlerを設定します
		-----------------------------------------------------------------*/
		__forceinline void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/*---------------------------------------------------------------
		　　　　　@brief : キャプションにテキストを追加します.
		-----------------------------------------------------------------*/
		__forceinline virtual void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/*---------------------------------------------------------------
		　　　　　@brief : 現在作業中のウィンドウであるかを調べます.
		-----------------------------------------------------------------*/
		virtual bool IsForegroundWindow() const override;

		/*---------------------------------------------------------------
		　　　　　@brief : フルスクリーンをサポートしているかを調べます. 
		-----------------------------------------------------------------*/
		virtual bool IsFullscreenSupported() const override;

		/*---------------------------------------------------------------
		　　　　　@brief : 最大化されているかを調べます
		-----------------------------------------------------------------*/
		__forceinline virtual bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		/*---------------------------------------------------------------
		　　　　　@brief : 最小化されているかを調べます (アイコン状態になっているか)
		-----------------------------------------------------------------*/
		__forceinline virtual bool IsMinimized() const override { return !!::IsIconic(_hwnd); }
		
		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウが有効化されているか
		-----------------------------------------------------------------*/
		virtual bool IsEnabled() override { return !!::IsWindowEnabled(_hwnd); }

		/*---------------------------------------------------------------
		　　　　　@brief : 手動でDPIを変更可能にするか
		-----------------------------------------------------------------*/
		virtual bool IsManualManageDPIChanges() const override { return _useManualDPIChange; }

		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) override { _useManualDPIChange = manualManageDPIChange; }
		/*---------------------------------------------------------------
		　　　　　@brief : Return the rectangle of the winow is associated with
		-----------------------------------------------------------------*/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const override;

		/*---------------------------------------------------------------
		　　　　　@brief : window modeを設定する
		-----------------------------------------------------------------*/
		virtual void SetWindowMode(const core::WindowMode windowMode) override;

		/*---------------------------------------------------------------
		　　　　　@brief : 透過度を設定する
		-----------------------------------------------------------------*/
		virtual void SetOpacity(const float opacity) const;

		/*---------------------------------------------------------------
		　　　　　@brief : KeyboardFocusにこのウィンドウを設定する
		-----------------------------------------------------------------*/
		virtual void SetKeyboardFocus() override;

		/*---------------------------------------------------------------
		　　　　　@brief : 親ウィンドウが最小化されたときに呼び出される関数
		-----------------------------------------------------------------*/
		bool OnParentWindowMinimized();

		/*---------------------------------------------------------------
		　　　　　@brief : 親ウィンドウがRestoreされたときに呼び出される関数
		-----------------------------------------------------------------*/
		bool OnParentWindowRestored();

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow();

		~CoreWindow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : タッチフィードバックを無効化する.
		-----------------------------------------------------------------*/
		void DisableTouchFeedback();

		/*---------------------------------------------------------------
		　　　　　@brief : ウィンドウ領域の調整
		-----------------------------------------------------------------*/
		void AdjustWindowRegion(const gu::int32 width, const gu::int32 height);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		float _aspectRatio = 0.0f;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;

		// @brief : ディスプレイの拡大率です. 
		float _dpiScaleFactor = 1.0f;

		bool _enableHighDPIMode = false;

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

		// @brief : DPIを手動で変更するか
		bool _useManualDPIChange = false;

		// @brief : 参照カウント
		gu::int32 _oleReferenceCount = 0;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : Regionオブジェクトの作成]
			 const bool useBorderWhenMaximizedは最大化しているときにも境界部分を使うか.
		-----------------------------------------------------------------*/
		HRGN CreateWindowRegionObject(const bool useBorderWhenMaximized) const;
	};
}
#endif _WIN32
#endif 