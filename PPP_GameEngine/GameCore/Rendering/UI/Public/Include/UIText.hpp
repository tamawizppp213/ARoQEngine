//////////////////////////////////////////////////////////////////////////////////
///             @file   Text.hpp
///             @brief  Text object (string, number)
///                     ���݂ł�, ��s���̑傫�Ȏ���e�N�X�`���摜��ǂݍ����, UV�l�ɍ��킹�ĕ\�����Ă��܂�. 
///                     ����̗\��ł�,�@ttf�t�@�C����ǂݍ����Bitmap�ɕϊ�, ���̌�e�N�X�`����ǂݍ����
/// �@�@�@�@�@�@�@�@�@�@�@�@�@�e������\���ł���悤�ɂ������ƍl���Ă��܂�. (����ł͍H�������Ȃ肩���邽�߂���Ă܂���)
///                     �s���ꍇ�͔j��I�ύX�������邱�Ƃ��l�����܂�. 
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
	*  @brief     ���݂ł�, ��s���̑傫�Ȏ���e�N�X�`���摜��ǂݍ����, UV�l�ɍ��킹�ĕ\�����Ă��܂�. 
	              ����̗\��ł�,�@ttf�t�@�C����ǂݍ����Bitmap�ɕϊ�, ���̌�e�N�X�`����ǂݍ����
    �@�@�@�@�@�@�@�@�@�e������\���ł���悤�ɂ������ƍl���Ă��܂�. (����ł͍H�������Ȃ肩���邽�߂���Ă܂���)
			      �s���ꍇ�͔j��I�ύX�������邱�Ƃ��l�����܂�. 
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