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
#define DIRECTINPUT_VERSION 0x0800
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
	bool IsPress  (int keyCode) const;
	bool IsTrigger(int keyCode) const;
	bool IsRelease(int keyCode) const;
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
	bool CreateHWND() const;
	bool CreateHInstance() const;
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