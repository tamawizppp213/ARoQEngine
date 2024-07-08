//////////////////////////////////////////////////////////////////////////////////
///             @file   CoreWindowMessageHandler.hpp
///             @brief  ウィンドウメッセージの扱い方を登録します. 
///             @author Toide Yutaro
///             @date   2023_09_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_WINDOW_MESSAGE_HANDLER_HPP
#define CORE_WINDOW_MESSAGE_HANDLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class CoreWindow;
	/****************************************************************************
	*				  			    CoreWindowMessageHandler
	****************************************************************************/
	/* @class     CoreWindowMessageHandler
	*  @brief     ウィンドウのメッセージを受け取った時に使用するHandlerクラス
	*****************************************************************************/
	class CoreWindowMessageHandler : public gu::NonCopyable
	{
	protected:
		using CoreWindowPtr = gu::SharedPointer<core::CoreWindow>;
	public:
		#pragma region Static Function

		#pragma endregion

		#pragma region Public Function
		/*----------------------------------------------------------------------
		*  @brief : This function is called by window activation changed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowActivationChanged([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]] const core::ActivationType type)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Windowがアクティブ化, 非アクティブ化された時に送られるメッセージの実装
		*  @param[in]  const bool isActive
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowApplicationActivationChanged([[maybe_unused]]const bool isActive)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Windowがアクティブ化, 非アクティブ化された時に送られるメッセージの実装
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowAction([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]]const core::WindowActionType action)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Windowが移動したときに送られるメッセージの実装
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowMoved([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]]const gu::int32 x, [[maybe_unused]]const gu::int32 y)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Windowがアクティブ化, 非アクティブ化された時に送られるメッセージの実装
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowResizing([[maybe_unused]] const CoreWindowPtr& window)
		{
			_isResizing = true;
			return true;
		}

		/*!***********************************************************************
		*  @brief      対象ウィンドウの一部を塗りつぶす要求を行う場合に送られるメッセージの実装
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnOSPainted([[maybe_unused]]const CoreWindowPtr& window)
		{
			return true;
		}

		/*----------------------------------------------------------------------
		*  @brief : This function is called by window closed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowClosed(const CoreWindowPtr& window)
		{
			return window ? true : false;
		}

		/*!***********************************************************************
		*  @brief      システム全体の設定を変更したとき, またはポリシー設定が変更されたときに全ての最上位ウィンドウに送信されるメッセージ
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool OnSettingChanged()
		{
			return true;
		}

		/*----------------------------------------------------------------------
		*  @brief : This function is called by the window size changed
		*----------------------------------------------------------------------*/
		virtual bool OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		CoreWindowMessageHandler();

		virtual ~CoreWindowMessageHandler();
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor


		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief リサイズ中*/
		bool _isResizing = false;
		#pragma endregion
		
	};
}
#endif
