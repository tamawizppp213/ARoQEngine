//////////////////////////////////////////////////////////////////////////////////
///             @file   CoreCursor.hpp
///             @brief  カーソルのアプリケーションに対する表示, 非表示を行います
///             @author Toide Yutaro
///             @date   2023_09_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_CURSOR_HPP
#define CORE_CURSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct RGBA;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/****************************************************************************
	*				  			    ICursor
	*************************************************************************//**
	*  @class     ICursor
	*  @brief     カーソルの表示クラス
	*****************************************************************************/
	class ICursor : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief : Show the cursor pointer
		-----------------------------------------------------------------*/
		virtual void Show() = 0;

		/*---------------------------------------------------------------
			@brief : Hide the cursor pointer
		-----------------------------------------------------------------*/
		virtual void Hide() = 0;

		/*---------------------------------------------------------------
			@brief : Locked the cursor to the passed in bounds
		-----------------------------------------------------------------*/
		virtual bool Lock(Rectangle* rectangle) = 0;

		/*---------------------------------------------------------------
			@brief : Return the suport to create cursor from the file
			         currently .cur only usable.
		-----------------------------------------------------------------*/
		virtual void* CreateCursorFromFile([[maybe_unused]]const char* filePath) { return nullptr; }

		/*---------------------------------------------------------------
			@brief : Create new cursor icon from rgba buffer
		-----------------------------------------------------------------*/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) = 0;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief : Return the cursor position in (x, y) pixel
		-----------------------------------------------------------------*/
		virtual gm::Float2 GetPosition() const = 0;
		
		/*---------------------------------------------------------------
			@brief : Return the cursor look type
		-----------------------------------------------------------------*/
		virtual CursorType GetType() const = 0;

		/*---------------------------------------------------------------
			@brief : Return the cursor size
		-----------------------------------------------------------------*/
		virtual void GetSize(gu::int32& width, gu::int32& height) const = 0;

		/*---------------------------------------------------------------
			@brief : Set the cursor position in (x, y) pixel
		-----------------------------------------------------------------*/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const = 0;

		/*---------------------------------------------------------------
			@brief : Set the cursor look type
		-----------------------------------------------------------------*/
		virtual void SetType(const CursorType type) = 0;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit ICursor(const CursorType type) : _type(type) {};

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		CursorType _type = CursorType::Default;
	};
}
#endif
