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
using namespace engine;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
/* @brief : Text string constructor */
Text::Text(const CoordinateType type, const gu::SharedPointer<Font>& font, const StringInfo& info)
	: _font(font)
{
	if (!font->HasLoaded()) { throw std::runtime_error("Font isn't read. You should read font."); }

	/*-------------------------------------------------------------------
	-              Create sprite
	---------------------------------------------------------------------*/
	int callAddNewLineCount = 0;
	int xMoveCount = 0;
	for (int i = 0; i < info.String.size(); ++i, ++xMoveCount)
	{
		/*-------------------------------------------------------------------
		-              Add new line check
		---------------------------------------------------------------------*/
		bool addNewLine = info.String[i] == '\n';
		if (addNewLine) 
		{
			callAddNewLineCount++; 
			xMoveCount = -1; // reset xStart position, ++xMoveCount‚É‚æ‚Á‚Ä0‚É•ÏX‚³‚ê‚é‚½‚ß, -1‚É‚µ‚Ä‚é.
			continue; 
		}

		/*-------------------------------------------------------------------
		-          Rect center position
		---------------------------------------------------------------------*/
		Float3 centerPosition = Float3
		(
			info.StartPosition.x + (xMoveCount + 0.5f) * info.SizePerChar.x + xMoveCount * info.Space.x,
			info.StartPosition.y - info.SizePerChar.y - callAddNewLineCount * (info.SizePerChar.y + info.Space.y),
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
		auto image = Image();
		switch (type)
		{
			using enum CoordinateType;
			case Screen : { image.CreateInScreenSpace(centerPosition, info.SizePerChar, u, v, info.Color); break;}
			case NDC    : { image.CreateInNDCSpace   (centerPosition, info.SizePerChar, u, v, info.Color); break;}
			default: 
				throw std::runtime_error("Choice wrong type");
		}

		_images.Push(image);
	}
}

/* @brief : Text number constructor*/
Text::Text(const CoordinateType type, const gu::SharedPointer<Font>& font, const NumberInfo& info)
{
#ifdef _DEBUG
	Check(info.Digit >= 1);
#endif
	/*-------------------------------------------------------------------
	-              Prepare image buffer
	---------------------------------------------------------------------*/
	_images.Resize(info.Digit);

	/*-------------------------------------------------------------------
	-              Push_back the value of each digit.
	---------------------------------------------------------------------*/
	gu::DynamicArray<int> values(info.Digit);

	// push back digit value
	int tempValue = info.Number;
	for (int i = (int)info.Digit - 1; i >= 0; --i)
	{
		values[i] = (tempValue % 10);
		tempValue = tempValue / 10;
	}

	/*-------------------------------------------------------------------
	-              Create Sprite (DirectX Coordinates)
	---------------------------------------------------------------------*/
	for (int i = 0; i < (int)info.Digit; ++i)
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
			case Screen : { _images[i].CreateInScreenSpace(centerPosition, info.SizePerDigit, u, v, info.Color); break;}
			case NDC    : { _images[i].CreateInNDCSpace    (centerPosition, info.SizePerDigit, u, v, info.Color); break;}
			default: 
				throw std::runtime_error("Choice wrong type");
		}
	}
}

Text::~Text()
{
	_images.Clear(); _images.ShrinkToFit();
}
#pragma endregion Constructor and Destructor

#pragma region Property
const gu::SharedPointer<rhi::core::GPUResourceView> Text::GetFontView() const noexcept
{
	return _font->GetFontResourceView();
}
#pragma endregion Property