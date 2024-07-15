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
#include "../Include/UISlider.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace gm;
using namespace rhi::core;

namespace
{
	std::wstring defaultSliderPath = L"Resources/Preset/DefaultSlider.png";
	std::wstring fillColorPath     = L"Resources/Preset/NullAlbedoMap.png";
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
Slider::Slider()
{
	_resourceList.resize(2);
}

Slider::~Slider()
{
	_functionList.clear(); _functionList.shrink_to_fit();
}

Slider::Slider(const float value, const float maxValue, const float minValue, const float stepSize)
	: _value(value), _maxValue(maxValue), _minValue(minValue), _stepSize(stepSize)
{
	_resourceList.resize(2);
}
#pragma endregion Construcor and Destructor 

#pragma region Main Function
void Slider::CreateInScreenSpace(const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4& fillColor)
{
	if (!_initialized) { InitializeSliderCanvas(false, position, rectSize, fillColor); }

	RecreateSliderUI();
}

void Slider::CreateInNDCSpace(const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4& fillColor)
{
	if (!_initialized) { InitializeSliderCanvas(true, position, rectSize, fillColor); }

	// Color 部分
	RecreateSliderUI();
}

/****************************************************************************
*					Add
****************************************************************************/
/* @fn        void Slider::Add(const float difference)
*
*  @brief     Add the difference to the current slider value
		      value + difference > maxValue -> maxValue is set
		      value + difference < minValue -> minValue is set.
*
*  @param[in] const float difference
*
*  @return 　　void
*****************************************************************************/
void Slider::Add(const float difference)
{
	/*-------------------------------------------------------------------
	-             Add
	---------------------------------------------------------------------*/
	float value = _value + difference;

	/*-------------------------------------------------------------------
	-             Exceed check
	---------------------------------------------------------------------*/
	if (value >= _maxValue) { value = _maxValue; }
	if (value <= _minValue) { value = _minValue; }

	/*-------------------------------------------------------------------
	-             Execute function list
	---------------------------------------------------------------------*/
	if (!OnValueChanged(value)) { return; }
	_value = value;
}

/****************************************************************************
*					Step
****************************************************************************/
/* @fn        void Slider::Step(const bool isPlus)
*
*  @brief     Add the stepsize to the current slider value
*
*  @param[in] const bool isPlus (true : add, false: minus)
*
*  @return 　　void
*****************************************************************************/
void Slider::Step(const bool isPlus)
{
	float stepValue = isPlus ? _stepSize : -_stepSize;
	Add(stepValue);

}

/****************************************************************************
*					OnValueChanged
****************************************************************************/
/* @fn        bool Slider::OnValueChanged(const float newValue)
*
*  @brief     if slider value is changed, execute registered functions and recreate slider UI
*
*  @param[in] const float newValue
*
*  @return 　　bool (true: changed, false: not changed)
*****************************************************************************/
bool Slider::OnValueChanged(const float newValue)
{
	if (newValue == _value) { return false; }

	/*-------------------------------------------------------------------
	-             Recreate slider UI
	---------------------------------------------------------------------*/
	RecreateSliderUI();

	/*-------------------------------------------------------------------
	-             Execute function list
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < _functionList.size(); ++i)
	{
		(*_functionList[i])(newValue);
	}
	return true;
}

/****************************************************************************
*					RecreateSliderUI
****************************************************************************/
/* @fn        void Slider::RecreateSliderUI()
*
*  @brief     Recreate slider ui in NDC space
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void Slider::RecreateSliderUI()
{
	/*-------------------------------------------------------------------
	-             Calculate slider position
	---------------------------------------------------------------------*/
	const float scale               = (_value - _minValue) / (_maxValue - _minValue);
	const gm::Float2 rectSize       = gm::Float2(scale * _fullRectSize.x, _fullRectSize.y);
	const gm::Float3 centerPosition = gm::Float3(_leftEdgePosition.x + rectSize.x / 2.0f, _leftEdgePosition.y, _leftEdgePosition.z);
	
	/*-------------------------------------------------------------------
	-             Create in NDC space
	---------------------------------------------------------------------*/
	_resourceList[Color].Image.CreateInNDCSpace(centerPosition, rectSize, { 0, 1 }, { 0, 1 }, _fillColor, 0.0f);
}
#pragma endregion Main Function

#pragma region Property
/****************************************************************************
*					SetValue
****************************************************************************/
/* @fn        void Slider::SetValue(float value)
*
*  @brief     Set slider value determined by myself.
*
*  @param[in] float value (minValue <= value <= maxValue)
*
*  @return 　　void
*****************************************************************************/
void Slider::SetValue(float value)
{
	if (value >= _maxValue) { value = _maxValue; }
	if (value <= _minValue) { value = _minValue; }
	if (!OnValueChanged(value)) { return; };
	_value = value;
}

/****************************************************************************
*					SetFillColor
****************************************************************************/
/* @fn        void Slider::SetFillColor(const gm::Float4& fillColor)
*
*  @brief     Set fill color 
*
*  @param[in] const gm::Float4& fillColor
*
*  @return 　　void
*****************************************************************************/
void Slider::SetFillColor(const gm::Float4& fillColor)
{
	_fillColor = fillColor;
	RecreateSliderUI();
}

/****************************************************************************
*					InitializeSliderCanvas
****************************************************************************/
/* @fn        bool Slider::InitializeSliderCanvas(const bool isNDC, const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4 fillColor)
*
*  @brief     Set up the slider backGround image.
*
*  @param[in] const bool isNDC, 
*  @param[in] const gm::Float3& position, 
*  @param[in] const gm::Float2& rectSize, 
*  @param[in] const gm::Float4 fillColor
*
*  @return 　　bool (true : finish initialization, false : has already finished)
*****************************************************************************/
bool Slider::InitializeSliderCanvas(const bool isNDC, const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4 fillColor)
{
	if (!_initialized)
	{
		_fillColor        = fillColor;
		_initialized      = true;

		if (isNDC)
		{
			_leftEdgePosition = gm::Float3(position.x - rectSize.x / 2.0f, position.y, position.z);
			_fullRectSize     = rectSize;
			_resourceList[BackGround].Image.CreateInNDCSpace(position, rectSize, { 0, 1 }, { 0, 1 }, { 1,1,1,1 }, 0.0f);
		}
		else
		{
			_leftEdgePosition = gm::Float3(position.x - rectSize.x / (2.0f * Screen::GetScreenWidth()), position.y / (Screen::GetScreenHeight()), position.z);
			_fullRectSize     = gm::Float2(rectSize.x / Screen::GetScreenWidth(), rectSize.y / Screen::GetScreenHeight());
			_resourceList[BackGround].Image.CreateInScreenSpace(position, rectSize, { 0, 1 }, { 0, 1 }, { 1,1,1,1 }, 0.0f);
		}

		return true;
	}
	else
	{
		return false;
	}
}
#pragma endregion Property