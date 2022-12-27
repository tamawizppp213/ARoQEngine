//////////////////////////////////////////////////////////////////////////////////
///             @file   Text.cpp
///             @brief  Text object (string, number)
///             @author Toide Yutaro
///             @date   2022_12_14
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/UIText.hpp"
#include "../Include/UIFont.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::ui;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
/* @brief : Text string constructor */
Text::Text(const CoordinateType type, const std::shared_ptr<Font>& font, const StringInfo& info)
	: _font(font)
{
	if (!font->HasLoaded()) { throw std::runtime_error("Font isn't read. You should read font."); }

	/*-------------------------------------------------------------------
	-              Prepare image buffer
	---------------------------------------------------------------------*/
	_images.resize(info.String.size());

	/*-------------------------------------------------------------------
	-              Create sprite
	---------------------------------------------------------------------*/
	for (int i = 0; i < _images.size(); ++i)
	{
		/*-------------------------------------------------------------------
		-          Rect center position
		---------------------------------------------------------------------*/
		Float3 centerPosition = Float3
		(
			info.StartPosition.x + (i + 0.5f) * info.SizePerChar.x + i * info.Space,
			info.StartPosition.y - info.SizePerChar.y,
			info.StartPosition.z
		);

		/*-------------------------------------------------------------------
		-          Texture uv value
		---------------------------------------------------------------------*/
		Float2 u = Float2(
			(info.String[i]     - ASCII_START_CHAR) * (font->GetPixelPerChar().x / font->GetImagePixelWidth()),
			(info.String[i] + 1 - ASCII_START_CHAR) * (font->GetPixelPerChar().x / font->GetImagePixelWidth())
		);

		Float2 v = Float2(0.0f, 1.0f);

		/*-------------------------------------------------------------------
		-          Create a sprite in the selected coordinate space
		---------------------------------------------------------------------*/
		switch (type)
		{
			using enum CoordinateType;
			case Screen : { _images[i].CreateInScreenSpace(centerPosition, info.SizePerChar, u, v, info.Color); break;}
			case NDC    : { _images[i].CreateInNDCSpace   (centerPosition, info.SizePerChar, u, v, info.Color); break;}
			default: 
				throw std::runtime_error("Choice wrong type");
		}
	}
}

/* @brief : Text number constructor*/
Text::Text(const CoordinateType type, const std::shared_ptr<Font>& font, const NumberInfo& info)
{
	/*-------------------------------------------------------------------
	-              Prepare image buffer
	---------------------------------------------------------------------*/
	_images.resize(info.Digit);

	/*-------------------------------------------------------------------
	-              Push_back the value of each digit.
	---------------------------------------------------------------------*/
	std::vector<int> values(info.Digit);

	// push back digit value
	int tempValue = info.Number;
	for (int i = info.Digit - 1; i >= 0; --i)
	{
		values[i] = (tempValue % 10);
		tempValue = tempValue / 10;
	}

	/*-------------------------------------------------------------------
	-              Create Sprite (DirectX Coordinates)
	---------------------------------------------------------------------*/
	for (int i = 0; i < info.Digit; ++i)
	{
		Float3 centerPosition = Float3(
			info.StartPosition.x + (i + 0.5f) * info.SizePerDigit.x + i * info.Space,
			info.StartPosition.y - info.SizePerDigit.y,
			info.StartPosition.z);

		Float2 u = Float2(
			(values[i]    ) * (font->GetPixelPerChar().x / font->GetImagePixelWidth()),
			(values[i] + 1) * (font->GetPixelPerChar().x / font->GetImagePixelWidth())
		);

		Float2 v = Float2(0.0f, 1.0f);

		/*-------------------------------------------------------------------
		-          Create a sprite in the selected coordinate space
		---------------------------------------------------------------------*/
		switch (type)
		{
			using enum CoordinateType;
			case Screen : { _images[i].CreateInScreenSpace(centerPosition, info.SizePerDigit, u, v, info.Color); }
			case NDC    : { _images[i].CreateInNDCSpace    (centerPosition, info.SizePerDigit, u, v, info.Color); }
			default: 
				throw std::runtime_error("Choice wrong type");
		}
	}
}

Text::~Text()
{
	_images.clear(); _images.shrink_to_fit();
}
#pragma endregion Constructor and Destructor

#pragma region Property
const std::shared_ptr<rhi::core::GPUResourceView> Text::GetFontView() const noexcept
{
	return _font->GetFontResourceView();
}
#pragma endregion Property