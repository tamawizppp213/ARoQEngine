//////////////////////////////////////////////////////////////////////////////////
///  @file   CoreCursor.hpp
///  @brief  カーソルのアプリケーションに対する表示, 非表示を行います
///  @author Toide Yutaro
///  @date   2023_09_02
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
	****************************************************************************/
	/*  @brief カーソルのアプリケーションに対する表示, 非表示を行います
	*****************************************************************************/
	class ICursor : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      カーソルを表示します
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Show() = 0;

		/*!***********************************************************************
		*  @brief      カーソルを非表示にします
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void Hide() = 0;

		/*!***********************************************************************
		*  @brief      指定したピクセル領域内でのみカーソルを動かせるようにします
		*  @param[in]  Rectangle* 領域
		*  @return     bool
		**************************************************************************/
		virtual bool Lock(Rectangle* rectangle) = 0;

		/*!***********************************************************************
		*  @brief      .curのみ現在使用可能ですが, オリジナルのカーソルを作成します.コンストラクタでも設定可能なのでお好みで使用してください. 
		*  @param[in]  const char* filePath ファイルパス
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromFile([[maybe_unused]] const char* filePath) = 0;

		/*!***********************************************************************
		*  @brief      RGBAのバッファに基づいて直接カーソルを作成します
		*  @param[in]  const gm::RGBA*  ピクセル
		*  @param[in]  const gu::int32  幅
		*  @param[in]  const gu::int32  高さ
		*  @return     void*
		**************************************************************************/
		virtual void* CreateCursorFromRGBABuffer(const gm::RGBA* pixels, const gu::int32 width, const gu::int32 height) = 0;

		#pragma endregion

		#pragma region Public Property

		/*!***********************************************************************
		*  @brief      (x, y)ピクセルからカーソル位置を取得します
		*  @param[in]  void
		*  @return     gm::Float2
		**************************************************************************/
		virtual gm::Float2 GetPosition() const = 0;
		
		/*!***********************************************************************
		*  @brief      カーソルの種類を返します
		*  @param[in]  void
		*  @return     CursorType
		**************************************************************************/
		virtual CursorType GetType() const = 0;

		/*!***********************************************************************
		*  @brief      カーソルのサイズを返します
		*  @param[out] gu::int32& 幅
		*  @param[out] gu::int32& 高さ
		*  @return     void
		**************************************************************************/
		virtual void GetSize(gu::int32& width, gu::int32& height) const = 0;

		/*!***********************************************************************
		*  @brief      (x, y)ピクセルからカーソル位置を設定します
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void SetPosition(const gu::int32 x, const gu::int32 y) const = 0;

		/*!***********************************************************************
		*  @brief      カーソルの種類を設定します
		*  @param[in]  const CursorType type
		*  @return     void
		**************************************************************************/
		virtual void SetType(const CursorType type) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		
		/*! @brief カーソル種類で初期化*/
		explicit ICursor(const CursorType type) : _type(type) {};

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property

		/*! @brief カーソルの種類*/
		CursorType _type = CursorType::Default;

		#pragma endregion
	};
}
#endif
