//////////////////////////////////////////////////////////////////////////////////
///             @file   Text.hpp
///             @brief  Text object (string, number)
///                     現在では, 一行分の大きな自作テクスチャ画像を読み込んで, UV値に合わせて表示しています. 
///                     今後の予定では,　ttfファイルを読み込んでBitmapに変換, その後テクスチャを読み込んで
/// 　　　　　　　　　　　　　各文字を表示できるようにしたいと考えています. (現状では工数がかなりかかるためやってません)
///                     行う場合は破壊的変更が生じることが考えられます. 
///             @author Toide Yutaro
///             @date   2022_12_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TEXT_HPP
#define TEXT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "UIImage.hpp"
#include <vector>
#include <string>
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUResourceView;
}
namespace gc::ui
{
	class Font;

	struct StringInfo
	{
		std::string String;
		gm::Float2  SizePerChar;
		gm::Float3  StartPosition; // rect lect upper point
		gm::Float2  Space = { 0.0f, 0.0f };
		gm::Float4  Color = gm::Float4(1,1,1,1);
	};

	struct NumberInfo
	{
		std::uint32_t Number;
		std::uint32_t Digit = 1;
		gm::Float2    SizePerDigit;
		gm::Float3    StartPosition;
		float         Space = 0.0f;
		gm::Float4    Color = gm::Float4(1,1,1,1);
	};

	/****************************************************************************
	*				  			    Text
	*************************************************************************//**
	*  @class     Text
	*  @brief     現在では, 一行分の大きな自作テクスチャ画像を読み込んで, UV値に合わせて表示しています. 
	              今後の予定では,　ttfファイルを読み込んでBitmapに変換, その後テクスチャを読み込んで
    　　　　　　　　　各文字を表示できるようにしたいと考えています. (現状では工数がかなりかかるためやってません)
			      行う場合は破壊的変更が生じることが考えられます. 
	*****************************************************************************/
	class Text 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Get text image list*/
		const std::vector<Image>& GetTextImages() const { return _images; }

		const gu::SharedPointer<Font> GetFont() const noexcept { return _font; }

		const gu::SharedPointer<rhi::core::GPUResourceView> GetFontView() const noexcept;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/* @brief : Text string constructor */
		Text(const CoordinateType type, const gu::SharedPointer<Font>& font, const StringInfo& stringInfo);
		
		/* @brief : Text number constructor*/
		Text(const CoordinateType type, const gu::SharedPointer<Font>& font, const NumberInfo& numberInfo);

		~Text();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<Image>  _images = {};

		gu::SharedPointer<Font> _font = nullptr;

		static constexpr std::int32_t ASCII_START_CHAR = 32;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
	};
}
#endif