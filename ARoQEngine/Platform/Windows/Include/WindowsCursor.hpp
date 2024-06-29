//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_CURSOR_HPP
#define WINDOWS_CURSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreCursor.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::windows
{
	/****************************************************************************
	*				  			    ICursor
	****************************************************************************/
	/* @class     ICursor
	*  @brief     Cursor interface
	*****************************************************************************/
	class ICursor : public core::ICursor
	{
	public:
#pragma region Public Function
		/*!***********************************************************************
		*  @brief      カーソルを表示します
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Show() override;

		/*!***********************************************************************
		*  @brief      カーソルを非表示にします
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Hide() override;

		/*!***********************************************************************
		*  @brief      指定したピクセル領域内でのみカーソルを動かせるようにします
		*  @param[in]  Rectangle* 領域
		*  @return     bool
		**************************************************************************/
		virtual bool Lock(core::Rectangle* rectangle) override;

		/*!***********************************************************************
		*  @brief      .curのみ現在使用可能ですが, オリジナルのカーソルを作成します.コンストラクタでも設定可能なのでお好みで使用してください.
		*  @param[in]  const char* filePath ファイルパス
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromFile(const char* filePath) override;

		/*!***********************************************************************
		*  @brief      RGBAのバッファに基づいて直接カーソルを作成します
		*  @param[in]  const gm::RGBA*  ピクセル
		*  @param[in]  const gu::int32  幅
		*  @param[in]  const gu::int32  高さ
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) override;
		
		#pragma endregion

		#pragma region Public Property

		/*!***********************************************************************
		*  @brief      (x, y)ピクセルからカーソル位置を取得します
		*  @param[in]  void
		*  @return     gm::Float2
		**************************************************************************/
		virtual gm::Float2  GetPosition() const override;

		/*!***********************************************************************
		*  @brief      カーソルの種類を返します
		*  @param[in]  void
		*  @return     CursorType
		**************************************************************************/
		virtual core::CursorType GetType() const override;

		/*!***********************************************************************
		*  @brief      カーソルのサイズを返します
		*  @param[out] gu::int32& 幅
		*  @param[out] gu::int32& 高さ
		*  @return     void
		**************************************************************************/
		virtual void GetSize(gu::int32& width, gu::int32& height) const override;

		/*!***********************************************************************
		*  @brief      (x, y)ピクセルからカーソル位置を設定します
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const override;

		/*!***********************************************************************
		*  @brief      カーソルの種類を設定します
		*  @param[in]  const CursorType type
		*  @return     void
		**************************************************************************/
		virtual void SetType(const core::CursorType type) override;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		explicit ICursor(const core::CursorType type);

		explicit ICursor(const char* filePath);

		~ICursor();


	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		HCURSOR _cursor = NULL;
	};
}
#endif
#endif