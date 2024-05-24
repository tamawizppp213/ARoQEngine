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
	*************************************************************************//**
	*  @class     ICursor
	*  @brief     Cursor interface
	*****************************************************************************/
	class ICursor : public core::ICursor
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief : カーソルを表示します
		-----------------------------------------------------------------*/
		virtual void Show() override;

		/*---------------------------------------------------------------
			@brief :　カーソルを隠します
		-----------------------------------------------------------------*/
		virtual void Hide() override;

		/*---------------------------------------------------------------
			@brief : カーソルを領域内だけでしか動かせないようにします
		-----------------------------------------------------------------*/
		virtual bool Lock(core::Rectangle* rectangle) override;

		/*---------------------------------------------------------------
			@brief : .curのみ現在使用可能ですが, オリジナルのカーソルを作成します. 
			          コンストラクタでも設定可能なのでお好みで使用してください. 
		-----------------------------------------------------------------*/
		virtual void* CreateCursorFromFile(const char* filePath) override;

		/*---------------------------------------------------------------
			@brief : RGBAの構造体から直接アイコンを作成します. 
		-----------------------------------------------------------------*/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) override;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*---------------------------------------------------------------
				@brief : カーソルの位置を返します.
		-----------------------------------------------------------------*/
		virtual gm::Float2  GetPosition() const override;

		/*---------------------------------------------------------------
				@brief : カーソルの見た目の種類を返します
		-----------------------------------------------------------------*/
		virtual core::CursorType GetType() const override;

		/*---------------------------------------------------------------
			@brief : カーソルのサイズを取得します
		-----------------------------------------------------------------*/
		virtual void GetSize(gu::int32& width, gu::int32& height) const override;

		/*---------------------------------------------------------------
				@brief : カーソルのxyピクセル空間での位置を返します
		-----------------------------------------------------------------*/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const override;

		/*---------------------------------------------------------------
			@brief : カーソルの種類を決定します
		-----------------------------------------------------------------*/
		virtual void SetType(const core::CursorType type) override;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
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