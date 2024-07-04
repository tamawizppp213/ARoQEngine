//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GENERAL_WINDOW_HPP
#define GENERAL_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class PlatformApplication;

	/****************************************************************************
	*				  			    CoreWindow
	****************************************************************************/
	/* @brief    オブジェクトを表示するためのウィンドウを作成します
	*****************************************************************************/
	class CoreWindow : public gu::NonCopyable
	{
	public:
		#pragma region Static 
		struct EmbeddedWindowHandle
		{
			/*! @brief 埋め込みモードを使用するか*/
			bool UseEmbeddedMode = false;

			/*! @brief ウィンドウハンドル*/
			void* WindowHandle = nullptr;

			/*! @brief 上書き可能か*/
			bool IsOverride = true;

			/*! @brief 幅*/
			gu::int32 Width = 0;

			/*! @brief 高さ*/
			gu::int32 Height = 0;
		};

		static EmbeddedWindowHandle EmbeddedWindow;

		#pragma endregion
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      Native Windowを表示します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Show() = 0;

		/*!***********************************************************************
		*  @brief      Native Windowを非表示にします
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Hide() = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウを最小化します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Minimize() = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウを最大化します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Maximize() = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウをアクティブにして表示する. 最小化・最大化されている場合は元のサイズと位置に復元される.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ReStore() = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウを破棄します
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Destroy() = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウサイズを変更します
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @param[in]  const gu::int32 width
		*  @param[in]  const gu::int32 height
		*  @return     void
		**************************************************************************/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) = 0;
		
		/*!***********************************************************************
		*  @brief      既に設定されたウィンドウを最前面に表示する
		*  @param[in]  const bool 強制的に最前面に表示するか
		*  @return     void
		**************************************************************************/
		virtual void BringToFront(const bool forceFront = false) = 0;

		/*!***********************************************************************
		*  @brief      指定したx, y座標にウィンドウを移動させます
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void Move(const gu::int32 x, const gu::int32 y) = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウを有効/無効化する. 無効化する場合入力を受け付けない.
		*  @param[in]  const bool 有効化
		*  @return     void
		**************************************************************************/
		virtual void Enable(const bool enable) = 0;

		/*!***********************************************************************
		*  @brief      透過度を変更する
		*  @param[in]  const core::WindowTransparency 透過度
		*  @return     void
		**************************************************************************/
		virtual void OnTransparencySupportChanged(const core::WindowTransparency transparency) = 0;

		#pragma endregion 

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      フルスクリーンをサポートしているかを調べます. 
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsFullscreenSupported() const = 0;

		/*!***********************************************************************
		*  @brief      現在作業中のウィンドウであるかを調べます.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsForegroundWindow() const = 0;

		/*!***********************************************************************
		*  @brief      最大化されているかを調べます
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsMaximized() const = 0;

		/*!***********************************************************************
		*  @brief      最小化されているかを調べます
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsMinimized() const = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウが有効化されているか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsEnabled() = 0;

		/*!***********************************************************************
		*  @brief      手動でDPIを変更可能にするか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsManualManageDPIChanges() const = 0;

		/*!***********************************************************************
		*  @brief      手動でDPIを変更する
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウのハンドルを取得します
		*  @param[in]  void
		*  @return     void*
		**************************************************************************/
		[[nodiscard]] virtual void* GetWindowHandle() const noexcept = 0;

		/*!***********************************************************************
		*  @brief      Windowのアスペクト比を渡します.
		*  @param[in]  void
		*  @return     float
		**************************************************************************/
		virtual float GetAspectRatio() const = 0;

		/*!***********************************************************************
		*  @brief      Windowのモードを取得します.
		*  @param[in]  void
		*  @return     WindowMode
		**************************************************************************/
		WindowMode GetWindowMode() const { return _windowDesc.WindowMode; };

		/*!***********************************************************************
		*  @brief      Windowの左上位置を取得します.
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @return     void
		**************************************************************************/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const = 0;

		/*!***********************************************************************
		*  @brief      Windowのボーダーサイズを取得します.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowBorderSize() const = 0;

		/*!***********************************************************************
		*  @brief      Windowのタイトルバーのサイズを取得します.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowTitleBarSize() const = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウが復元されたときのサイズと位置を返す
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) = 0;

		/*!***********************************************************************
		*  @brief      座標の下にウィンドウが存在する場合はtrueを返す
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const = 0;

		/*!***********************************************************************
		*  @brief      ウィンドウが有効化されているか
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool IsVisible() const { return _isVisible; }
	
		/*!***********************************************************************
		*  @brief      ウィンドウモードを設定する
		*  @param[in]  const WindowMode ウィンドウモード
		*  @return     void
		**************************************************************************/
		virtual void SetWindowMode(const WindowMode windowMode) = 0;

		/*!***********************************************************************
		*  @brief      キャプションにテキストを設定する
		*  @param[in]  const gu::tchar* const text
		*  @return     void
		**************************************************************************/
		virtual void SetText(const gu::tchar* const text) = 0;

		/*!***********************************************************************
		*  @brief      透過度を設定する
		*  @param[in]  const float 透過度
		*  @return     void
		**************************************************************************/
		virtual void SetOpacity(const float opacity) const = 0;

		/*!***********************************************************************
		*  @brief      KeyboardFocusにこのウィンドウを設定する
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void SetKeyboardFocus() = 0;

		/*!***********************************************************************
		*  @brief      FullScreen情報を取得する
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const = 0;

		/*!***********************************************************************
		*  @brief      Window作成のための初期設定を返します. 
		*  @param[in]  void
		*  @return     const CoreWindowDesc& 
		**************************************************************************/
		__forceinline const CoreWindowDesc& GetDesc() const { return _windowDesc; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief デフォルトコンストラクタ*/
		CoreWindow();

		/*! @brief デストラクタ*/
		~CoreWindow();

		/*! @brief WindowModeで初期化*/
		CoreWindow(const WindowMode windowMode);

		/*! @brief CoreWindowDescで初期化*/
		CoreWindow(const CoreWindowDesc& desc);
		#pragma endregion
	protected:

		#pragma region Protected Constructor and Destructor
		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief Windowの設定*/
		CoreWindowDesc _windowDesc = {};

		/*! @brief Platform Application*/
		gu::SharedPointer<PlatformApplication> _application = nullptr;

		/*! @brief ウィンドウが表示されているか*/
		bool _isVisible = false;

		#pragma endregion
	};
}
#endif