//////////////////////////////////////////////////////////////////////////////////
///             @file   Mouse.hpp
///             @brief  Mouse interface
///             @author Toide Yutaro
///             @date   2022_03_13
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MOUSE_HPP
#define MOUSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <queue>
#include <dinput.h>
#include <Windows.h>
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
struct MousePosition : POINT
{

};

enum MouseButton
{
	LEFT        = 0,
	RIGHT       = 1,
	WHEEL       = 2,
	TOTAL_COUNT = 3
};
/****************************************************************************
*				  			Mouse
*************************************************************************//**
*  @class     Mouse
*  @brief     mouse
*****************************************************************************/
class Mouse
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd);
	void Update();
	void Finalize();
	bool IsPress  (int mouseButton);
	bool IsTrigger(int mouseButton);
	bool IsRelease(int mouseButton);
	MousePosition& GetMousePosition();
	gm::Float2 GetMouseVelocity();
	int GetMousePosition_X();
	int GetMousePosition_Y();

	/****************************************************************************
	**                Public Property
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Mouse();
	~Mouse() = default;

	Mouse(const Mouse&)            = default;
	Mouse& operator=(const Mouse&) = default;
	Mouse(Mouse&&)                 = default;
	Mouse& operator=(Mouse&&)      = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	bool CreateHWND();
	bool CreateHInstance();
	bool CreateMouseDevice();
	bool CreateDataFormat();
	bool CreateCooperativeLevel();

	/****************************************************************************
	**                Private Property
	*****************************************************************************/
	LPDIRECTINPUT8       _dInput    = nullptr;
	LPDIRECTINPUTDEVICE8 _mouse     = nullptr;
	HINSTANCE            _hInstance = nullptr;
	HWND                 _hwnd      = nullptr;
	DIMOUSESTATE2        _currentMouseState;
	DIMOUSESTATE2        _previousMouseState;
	MousePosition        _mousePosition;
};
#endif