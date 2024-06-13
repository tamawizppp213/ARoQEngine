//////////////////////////////////////////////////////////////////////////////////
///             @file   Keyboard.hpp
///             @brief  Keyboard
///             @author Toide Yutaro
///             @date   2020_12_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <dinput.h>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#define MAX_KEY_BUFFER (256)

//////////////////////////////////////////////////////////////////////////////////
//                         Keyboard, class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			Keyboard
****************************************************************************/
/* @class     Keyboard
*  @brief     Keyboard Input 
*****************************************************************************/
class Keyboard
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd);
	void Update();
	void Finalize();
	bool IsPress  (int keyCode);
	bool IsTrigger(int keyCode);
	bool IsRelease(int keyCode);
	/****************************************************************************
	**                Public Property
	*****************************************************************************/
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	Keyboard();
	~Keyboard() = default;
	Keyboard(const Keyboard&)            = default;
	Keyboard& operator=(const Keyboard&) = default;
	Keyboard(Keyboard&&)                 = default;
	Keyboard& operator=(Keyboard&&)      = default;
	
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	bool CreateHWND();
	bool CreateHInstance();
	bool CreateKeyboardDevice();
	bool CreateDataFormat();
	bool CreateCooperativeLevel();


	/****************************************************************************
	**                Private Property
	*****************************************************************************/
	LPDIRECTINPUT8       _dInput    = nullptr;
	LPDIRECTINPUTDEVICE8 _keyboard  = nullptr;
	HINSTANCE            _hInstance = nullptr;
	HWND                 _hwnd      = nullptr;
	
	BYTE _keyState[MAX_KEY_BUFFER] = {0};
	BYTE _keyStateTrigger[MAX_KEY_BUFFER] = {0};
	BYTE _keyStateRelease[MAX_KEY_BUFFER] = {0};

};

#endif