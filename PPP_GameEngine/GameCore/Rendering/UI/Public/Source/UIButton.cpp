//////////////////////////////////////////////////////////////////////////////////
///             @file   UIButton.hpp
///             @brief  Button object
///             @author Toide Yutaro
///             @date   2023_01_07
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/UIButton.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
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
Button::Button(const std::shared_ptr<Mouse>& mouse)
	: _mouse(mouse)
{

}

Button::~Button()
{

}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*					OnClick
*************************************************************************//**
*  @fn        bool Button::OnClick(const MouseButton mouseButton) const 
*
*  @brief     Trigger click check in the ui button.
*
*  @param[in] void
*
*  @return 　　bool (true : click the button, false: not clicking)
*****************************************************************************/
bool Button::OnClick(const MouseButton mouseButton) const 
{
	if (!_mouse) { return false; }

	/*-------------------------------------------------------------------
	-              Enable button check
	---------------------------------------------------------------------*/
	if (!_isInteractive) { return false; }

	/*-------------------------------------------------------------------
	-              Mouse Press Check
	---------------------------------------------------------------------*/
	if (!_mouse->IsTrigger(mouseButton)) { return false; }

	
	return DetectClick();
}

/****************************************************************************
*					IsPress
*************************************************************************//**
*  @fn        bool Button::IsPress(const MouseButton mouseButton) const
*
*  @brief     Press check in the ui button.
*
*  @param[in] void
*
*  @return 　　bool (true : pressing the button, false: not pressing)
*****************************************************************************/
bool Button::IsPress(const MouseButton mouseButton) const
{
	if (!_mouse) { return false; }

	/*-------------------------------------------------------------------
	-              Enable button check
	---------------------------------------------------------------------*/
	if (!_isInteractive) { return false; }

	/*-------------------------------------------------------------------
	-              Mouse Press Check
	---------------------------------------------------------------------*/
	if (!_mouse->IsPress(mouseButton)) { return false; }

	return DetectClick();
}

/****************************************************************************
*					DetectClick
*************************************************************************//**
*  @fn        bool Button::DetectClick() const
*
*  @brief     Detect click (ndc space)
*
*  @param[in] void
*
*  @return 　　bool (true: clicking in the rectangle)
*****************************************************************************/
bool Button::DetectClick() const
{
	/*-------------------------------------------------------------------
	-              Convert Screen Space into NDC Space
	---------------------------------------------------------------------*/
	const float cursolX      = static_cast<float>(_mouse->GetMousePosition_X());
	const float cursolY      = static_cast<float>(_mouse->GetMousePosition_Y());
	const float screenWidth  = static_cast<float>(Screen::GetScreenWidth());
	const float screenHeight = static_cast<float>(Screen::GetScreenHeight());
	
	float ndcCursolX = static_cast<float>(cursolX - 0.5f * screenWidth ) / screenWidth  * (+2.0f);
	float ndcCursolY = static_cast<float>(cursolY - 0.5f * screenHeight) / screenHeight * (-2.0f);

	/*-------------------------------------------------------------------
	-              Exceed value check
	---------------------------------------------------------------------*/
	if (ndcCursolX >  1.0f) { ndcCursolX = 1.0f; }
	if (ndcCursolX < -1.0f) { ndcCursolX = -1.0f; }
	if (ndcCursolY >  1.0f) { ndcCursolY = 1.0f; }
	if (ndcCursolX < -1.0f) { ndcCursolY = -1.0f; }

	/*-------------------------------------------------------------------
	-              Click detection
	---------------------------------------------------------------------*/
	if (ndcCursolX < _positionNDC.x - _size.x / 2 || _positionNDC.x + _size.x / 2 < ndcCursolX) { return false; }
	if (ndcCursolY < _positionNDC.y - _size.y / 2 || _positionNDC.y + _size.y / 2 < ndcCursolY) { return false; }

	return true;
}
