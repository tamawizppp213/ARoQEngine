//////////////////////////////////////////////////////////////////////////////////
///             @file   Mouse.cpp
///             @brief  Mouse
///             @author Toide Yutaro
///             @date   2020_12_28
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Input/Include/Mouse.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
Mouse::Mouse()
{
	_currentMouseState.lX = 0;
	_currentMouseState.lY = 0;
	_currentMouseState.lZ = 0;
	_previousMouseState.lX = 0;
	_previousMouseState.lY = 0;
	_previousMouseState.lZ = 0;
	_mousePosition.x = 0;
	_mousePosition.y = 0;
}

/****************************************************************************
*							Initialize
****************************************************************************/
/* @fn        bool Mouse::Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd)
*  @brief     Mouse Initialize
*  @param[in] LPDIRECTINPUT8 dInput
*  @param[in] HINSTANCE hInstace
*  @param[in] HWND hwnd
*  @return 　　bool
*****************************************************************************/
bool Mouse::Initialize(LPDIRECTINPUT8 dInput, HINSTANCE hInstance, HWND hwnd)
{
	_dInput = dInput;
	_hInstance = hInstance;
	_hwnd = hwnd;

	bool result = true;

	result = CreateHWND();
	result = CreateHInstance();
	result = CreateMouseDevice();
	result = CreateDataFormat();
	result = CreateCooperativeLevel();

	_mouse->Acquire();

	return result;

}

/****************************************************************************
*							Update
****************************************************************************/
/* @fn        bool Mouse::Update()
*  @brief     Check Mouse Input
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Mouse::Update()
{
	// Save the mouse information before updating.
	_previousMouseState = _currentMouseState;

	// Update the mouse information
	if (SUCCEEDED(_mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &_currentMouseState)))
	{

	}
	else
	{
		_mouse->Acquire();
	}
}

/****************************************************************************
*							Finalize
****************************************************************************/
/* @fn        void Mouse::Finalize()
*  @brief     Finish Mouse Device
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void Mouse::Finalize()
{
	if (_mouse != nullptr)
	{
		_mouse->Unacquire();
		_mouse->Release();
		_mouse = nullptr;
	}
}

/****************************************************************************
*							IsPress
****************************************************************************/
/* @fn        void Mouse::IsPress()
*  @brief     Detect pressing button
*  @param[in] mouseButton
*  @return 　　void
*****************************************************************************/
bool Mouse::IsPress(int mouseButton) const
{
	return (_currentMouseState.rgbButtons[mouseButton] & 0x80) ? true : false;
}

/****************************************************************************
*							IsTrigger
****************************************************************************/
/* @fn        void Mouse::IsTrigger()
*  @brief     Detect trigger button
*  @param[in] mouseButton
*  @return 　　void
*****************************************************************************/
bool Mouse::IsTrigger(int mouseButton) const
{
	if (!(_previousMouseState.rgbButtons[mouseButton] & 0x80) &&
		(_currentMouseState.rgbButtons[mouseButton] & 0x80))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsRelease
****************************************************************************/
/* @fn        void Mouse::IsRelease()
*  @brief     Detect release button
*  @param[in] mouseButton
*  @return 　　void
*****************************************************************************/
bool Mouse::IsRelease(int mouseButton) const
{
	if ((_previousMouseState.rgbButtons[mouseButton] & 0x80) &&
		!(_currentMouseState.rgbButtons[mouseButton] & 0x80))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							GetMousePosition
****************************************************************************/
/* @fn        MousePosition& Mouse::GetMousePosition
*  @brief     Get Mouse Position
*  @param[in] void
*  @return 　　MousePosition&
*****************************************************************************/
MousePosition& Mouse::GetMousePosition()
{
	GetCursorPos(&_mousePosition);
	ScreenToClient(_hwnd, &_mousePosition);
	return _mousePosition;
}

/****************************************************************************
*							GetMousePosition_X
****************************************************************************/
/* @fn        int Mouse::GetMousePosition_X
*  @brief     Get Mouse Position X
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int Mouse::GetMousePosition_X()
{
	GetCursorPos(&_mousePosition);
	ScreenToClient(_hwnd, &_mousePosition);
	return static_cast<int>(_mousePosition.x);
}

/****************************************************************************
*							GetMousePosition_Y
****************************************************************************/
/* @fn        int Mouse::GetMousePosition_Y
*  @brief     Get Mouse Position Y
*  @param[in] void
*  @return 　　int
*****************************************************************************/
int Mouse::GetMousePosition_Y()
{
	GetCursorPos(&_mousePosition);
	ScreenToClient(_hwnd, &_mousePosition);
	return static_cast<int>(_mousePosition.y);
}

/****************************************************************************
*							GetMouseVelocity
****************************************************************************/
/* @fn        DirectX::XMFLOAT2 Mouse::GetMouseVelocity
*  @brief     GetMouseVelocity
*  @param[in] void
*  @return 　　XMFLOAT2
*****************************************************************************/
gm::Float2 Mouse::GetMouseVelocity() const
{
	gm::Float2 velocity={};
	velocity.x = (float)_currentMouseState.lX;
	velocity.y = (float)_currentMouseState.lY;
	return velocity;
}
#pragma endregion Public Function

#pragma region Private Function
bool Mouse::CreateHWND() const
{
	if (_hwnd == nullptr)
	{
		MessageBox(NULL, L"hwnd is nullptr", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Mouse::CreateHInstance() const
{
	if (_hInstance == nullptr)
	{
		MessageBox(_hwnd, L"dInput is nullptr.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Mouse::CreateMouseDevice()
{
	if (FAILED(_dInput->CreateDevice(GUID_SysMouse, &_mouse, NULL)))
	{
		MessageBox(_hwnd, L"can't crate keyboard device.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Mouse::CreateDataFormat()
{
	if (FAILED(_mouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(_hwnd, L"can't create data format.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;
}

bool Mouse::CreateCooperativeLevel()
{
	if (FAILED(_mouse->SetCooperativeLevel(_hwnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(_hwnd, L"can't create cooperative level.", L"Warning", MB_ICONWARNING);
		return false;
	}
	return true;

}
#pragma endregion Private Function