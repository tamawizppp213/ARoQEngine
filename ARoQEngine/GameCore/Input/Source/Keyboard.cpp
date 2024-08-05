//////////////////////////////////////////////////////////////////////////////////
///             @file   Keyboard.cpp
///             @brief  Keyboard
///             @author Toide Yutaro
///             @date   2020_12_27
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Input/Include/Keyboard.hpp"
#include <queue>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Public Function
Keyboard::Keyboard()
{
	
}

/****************************************************************************
*							Initialize
****************************************************************************/
/* @fn        bool Keyboard::Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd)
*  @brief     Keyboard Initialize
*  @param[in] LPDIRECTINPUT8 dInput
*  @param[in] HINSTANCE hInstace
*  @param[in] HWND hwnd
*  @return 　　bool
*****************************************************************************/
bool Keyboard::Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd)
{
	_dInput    = dInput;
	_hInstance = hInstance;
	_hwnd      = hwnd;

	bool result = true;

	result = CreateHWND();
	result = CreateHInstance();
	result = CreateKeyboardDevice();
	result = CreateDataFormat();
	result = CreateCooperativeLevel();

	_keyboard->Acquire();

	return result;

}

/****************************************************************************
*							Update
****************************************************************************/
/* @fn        bool Keyboard::Update()
*  @brief     Check Keyboard Input
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Keyboard::Update()
{
	BYTE keyState[MAX_KEY_BUFFER]={};

	if (SUCCEEDED(_keyboard->GetDeviceState(sizeof(keyState), keyState)))
	{
		for (int input = 0; input < MAX_KEY_BUFFER; ++input)
		{
			_keyStateTrigger[input] = (_keyState[input] ^ keyState[input]) & keyState[input];
			_keyStateRelease[input] = (_keyState[input] ^ keyState[input]) & _keyState[input];

			_keyState[input] = keyState[input];
		}
	}
	else
	{
		_keyboard->Acquire();
	}
}

/****************************************************************************
*							Finalize
****************************************************************************/
/* @fn        void Keyboard::Finalize()
*  @brief     Finish Keyboard Device
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Keyboard::Finalize()
{
	if (_keyboard != nullptr)
	{
		_keyboard->Unacquire();
		_keyboard->Release();
		_keyboard = nullptr;
	}
}

/****************************************************************************
*							IsPress
****************************************************************************/
/* @fn        void Keyboard::IsPress()
*  @brief     Detect pressing button
*  @param[in] keyCode
*  @return 　　bool
*****************************************************************************/
bool Keyboard::IsPress(int keyCode) const
{
	return (_keyState[keyCode] & 0x80) ? true : false;
}

/****************************************************************************
*							IsTrigger
****************************************************************************/
/* @fn        void Keyboard::IsTrigger()
*  @brief     Detect trigger button
*  @param[in] keyCode
*  @return 　　bool
*****************************************************************************/
bool Keyboard::IsTrigger(int keyCode) const
{
	return (_keyStateTrigger[keyCode] & 0x80) ? true : false;
}

/****************************************************************************
*							IsRelease
****************************************************************************/
/* @fn        void Keyboard::IsRelease()
*  @brief     Detect release button
*  @param[in] keyCode
*  @return 　　bool
*****************************************************************************/
bool Keyboard::IsRelease(int keyCode) const
{
	return (_keyStateRelease[keyCode] & 0x80) ? true : false;
}
#pragma endregion Public Function

#pragma region Private Function
bool Keyboard::CreateHWND() const
{
	if (_hwnd == nullptr)
	{
		MessageBox(NULL, L"hwnd is nullptr", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Keyboard::CreateHInstance() const
{
	if (_hInstance == nullptr)
	{
		MessageBox(_hwnd, L"dInput is nullptr.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Keyboard::CreateKeyboardDevice()
{
	if (FAILED(_dInput->CreateDevice(GUID_SysKeyboard, &_keyboard, NULL)))
	{
		MessageBox(_hwnd, L"can't crate keyboard device.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Keyboard::CreateDataFormat()
{
	if (FAILED(_keyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(_hwnd, L"can't create data format.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Keyboard::CreateCooperativeLevel()
{
	if (FAILED(_keyboard->SetCooperativeLevel(_hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(_hwnd, L"can't create cooperative level.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;

}
#pragma endregion Private Function