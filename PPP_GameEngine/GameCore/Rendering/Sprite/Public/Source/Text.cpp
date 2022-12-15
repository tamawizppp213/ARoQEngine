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
#include "../Include/Text.hpp"
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
Text::Text(const CoordinateType type, const StringInfo& info)
{
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
			info.StartPosition.x + (i * 0.5f) * info.SizePerChar.x * i * info.Space,
			info.StartPosition.y - info.SizePerChar.y,
			info.StartPosition.z
		);

		/*-------------------------------------------------------------------
		-          Texture uv value
		---------------------------------------------------------------------*/
		Float2 u = Float2(
			//(string[i]     - ASCII_START_CHAR) * (fontInfo->PixelSizePerChar.x / fontInfo->ImagePixelWidth)
			//(string[i] + 1 - ASCII_START_CHAR) * (fontInfo->PixelSizePerChar.x / fontInfo->ImagePixelWidth)
		);

		Float2 v = Float2(0.0f, 1.0f);

		/*-------------------------------------------------------------------
		-          Create a sprite in the selected coordinate space
		---------------------------------------------------------------------*/
		switch (type)
		{
			using enum CoordinateType;
			case Screen : { _images[i].CreateInScreenSpace(centerPosition, info.SizePerChar, u, v, info.Color); }
			case NDC    : { _images[i].CreateInNDCSpace   (centerPosition, info.SizePerChar, u, v, info.Color); }
			default: 
				throw std::runtime_error("Choice wrong type");
		}
	}
}

/* @brief : Text number constructor*/
Text::Text(const CoordinateType type, const NumberInfo& info)
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
			//values[i]       * (fontInfo->PixelSizePerChar.x / fontInfo->ImagePixelWidth),
			//(values[i] + 1) * (fontInfo->PixelSizePerChar.x / fontInfo->ImagePixelWidth)
		);

		Float2 v = Float2(0.0f, 1.0f);

		/*-------------------------------------------------------------------
		-          Create a sprite in the selected coordinate space
		---------------------------------------------------------------------*/
		switch (type)
		{
			using enum CoordinateType;
			case Screen : { _images[i].CreateInScreenSpace(centerPosition, info.SizePerDigit, u, v, info.Color); }
			case NDC    : { _images[i].CreateInNDCSpace   (centerPosition, info.SizePerDigit, u, v, info.Color); }
			default: 
				throw std::runtime_error("Choice wrong type");
		}
	}
}
#pragma endregion Constructor and Destructor