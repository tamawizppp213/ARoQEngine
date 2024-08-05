//////////////////////////////////////////////////////////////////////////////////
///             @file   GamePad.cpp
///             @brief  GamePad
///             @author Toide Yutaro
///             @date   2020_12_29(ver1.0), 2021_02_04(複数人実装, デバッグ)
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Input/Include/GamePad.hpp"
#include <Xinput.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static constexpr float SHORT_PLUS_MAX = 32767.0f;
static constexpr float SHORT_MINUS_MAX = 32768.0f;
static constexpr float USHORT_MAX = 65535.0f;
#pragma warning(disable : 4995)

//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
GamePad::GamePad()
{
	_maxPlayer = (int)GamePadID::TotalIdNum;
	_currentStates  .resize(_maxPlayer);
	_previousStates .resize(_maxPlayer);
	_vibrationStates.resize(_maxPlayer);

	for (int i = 0; i < _maxPlayer; ++i)
	{
		_currentStates[i].dwPacketNumber        = 0;
		_currentStates[i].Gamepad.sThumbLX      = 0;
		_currentStates[i].Gamepad.sThumbLY      = 0;
		_currentStates[i].Gamepad.sThumbRX      = 0;
		_currentStates[i].Gamepad.sThumbRY      = 0;
		_currentStates[i].Gamepad.bLeftTrigger  = 0;
		_currentStates[i].Gamepad.bRightTrigger = 0;
		_currentStates[i].Gamepad.wButtons      = 0;

		_previousStates[i].dwPacketNumber        = 0;
		_previousStates[i].Gamepad.sThumbLX      = 0;
		_previousStates[i].Gamepad.sThumbLY      = 0;
		_previousStates[i].Gamepad.sThumbRX      = 0;
		_previousStates[i].Gamepad.sThumbRY      = 0;
		_previousStates[i].Gamepad.bLeftTrigger  = 0;
		_previousStates[i].Gamepad.bRightTrigger = 0;
		_previousStates[i].Gamepad.wButtons      = 0;

		_vibrationStates[i].wLeftMotorSpeed  = 0;
		_vibrationStates[i].wRightMotorSpeed = 0;
	}

}
/****************************************************************************
*							Initialize
****************************************************************************/
/* @fn        bool GamePad::Initialize()
*  @brief     GamePad Initialize
*  @param[in] void
*  @return 　　bool
*****************************************************************************/
bool GamePad::Initialize()
{
	/*-------------------------------------------------------------------
	-               Check connect one controller
	---------------------------------------------------------------------*/
	DWORD dwResult = XInputGetState(0, &_currentStates[0]);

	if (dwResult != ERROR_SUCCESS)
	{
		OutputDebugStringW(L"Please connect at least one controller that supports xinput.\n");
		//MessageBox(NULL, L"Please connect at least one controller that supports xinput.", L"Warning", MB_ICONWARNING);
	}
	XInputGetState(0, &_previousStates[0]);

	/*-------------------------------------------------------------------
	-               Connect other controller
	---------------------------------------------------------------------*/
	for (int i = 1; i < _maxPlayer; ++i)
	{
		XInputGetState(i, &_currentStates[i]);
		XInputGetState(i, &_previousStates[i]);
	}

	return true;
}

/****************************************************************************
*							Update
****************************************************************************/
/* @fn        bool GamePad::Update()
*  @brief     GamePad Check Input
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GamePad::Update()
{
	for (int i = 0; i < _maxPlayer; ++i)
	{
		_previousStates[i] = _currentStates[i];
		XInputGetState(i, &_currentStates[i]);
	}
}

/****************************************************************************
*							Finalize
****************************************************************************/
/* @fn        bool GamePad::Finalize()
*  @brief     GamePad Finalize
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GamePad::Finalize()
{
	_currentStates.clear(); _currentStates.shrink_to_fit();
	_previousStates.clear(); _previousStates.shrink_to_fit();
	_vibrationStates.clear(); _vibrationStates.shrink_to_fit();
}

/****************************************************************************
*							IsEnabledXInput
****************************************************************************/
/* @fn        void GamePad::IsEnabledXInput(bool isEnabled)
*  @brief     Enable XInput (for vibration)
*  @param[in] bool isEnabled
*  @return 　　void
*****************************************************************************/
void GamePad::EnableXInput(bool isEnabled)
{
	XInputEnable(isEnabled);
}

/****************************************************************************
*							IsPressButton
****************************************************************************/
/* @fn        bool GamePad::IsPressButton(GamePadInput button, int gamePadId)
*  @brief     Detect pressing button
*  @param[in] int button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressButton(GamePadInput button, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	return (_currentStates[gamePadId].Gamepad.wButtons & (int)button) ? true : false;
}

/****************************************************************************
*							IsTriggerButton
****************************************************************************/
/* @fn        bool GamePad::IsTriggerButton(GamePadInput button, int gamePadId)
*  @brief     Detect trigger button
*  @param[in] int button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsTriggerButton(GamePadInput button, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	if (!(_previousStates[gamePadId].Gamepad.wButtons & (int)button) &&
		(_currentStates[gamePadId].Gamepad.wButtons   & (int)button))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsReleaseButton
****************************************************************************/
/* @fn        bool GamePad::IsReleaseButton(GamePadInput button, int gamePadId)
*  @brief     Detect release button
*  @param[in] int button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsReleaseButton(GamePadInput button, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	if ((_previousStates[gamePadId].Gamepad.wButtons & (int)button) &&
		!(_currentStates[gamePadId].Gamepad.wButtons & (int)button))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsPressLeftButton
****************************************************************************/
/* @fn        bool GamePad::IsPressLeftButton(int gamePadId)
*  @brief     Detect press Left button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressLeftButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE axis = _currentStates[gamePadId].Gamepad.bLeftTrigger;
	return (axis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

/****************************************************************************
*							IsTriggerLeftButton
****************************************************************************/
/* @fn        bool GamePad::IsTriggerLeftButton(int gamePadId)
*  @brief     Detect trigger Left button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsTriggerLeftButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE previousAxis    = _previousStates[gamePadId].Gamepad.bLeftTrigger;
	BYTE currentAxis     = _currentStates[gamePadId] .Gamepad.bLeftTrigger;
	bool previousTrigger = (previousAxis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
	bool currentTrigger  = (currentAxis  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;

	if ((!previousTrigger) && (currentTrigger))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsReleaseLeftButton
****************************************************************************/
/* @fn        bool GamePad::IsReleaseLeftButton(int gamePadId)
*  @brief     Detect release Left button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsReleaseLeftButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE previousAxis    = _previousStates[gamePadId].Gamepad.bLeftTrigger;
	BYTE currentAxis     = _currentStates[gamePadId].Gamepad.bLeftTrigger;
	bool previousTrigger = (previousAxis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
	bool currentTrigger  = (currentAxis  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;

	if ((previousTrigger) && (!currentTrigger))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsPressRightButton
****************************************************************************/
/* @fn        bool GamePad::IsPressRightButton(int gamePadId)
*  @brief     Detect press Right button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressRightButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE axis = _currentStates[gamePadId].Gamepad.bRightTrigger;
	return (axis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
}

/****************************************************************************
*							IsTriggerRightButton
****************************************************************************/
/* @fn        bool GamePad::IsTriggerRightButton(int gamePadId)
*  @brief     Detect trigger Right button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsTriggerRightButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE previousAxis    = _previousStates[gamePadId].Gamepad.bRightTrigger;
	BYTE currentAxis     = _currentStates[gamePadId].Gamepad.bRightTrigger;
	bool previousTrigger = (previousAxis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
	bool currentTrigger  = (currentAxis  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;

	if ((!previousTrigger) && (currentTrigger))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							IsReleaseRightButton
****************************************************************************/
/* @fn        bool GamePad::IsReleaseRightButton(int gamePadId)
*  @brief     Detect release Right button
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsReleaseRightButton(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	BYTE previousAxis    = _previousStates[gamePadId].Gamepad.bRightTrigger;
	BYTE currentAxis     = _currentStates[gamePadId] .Gamepad.bRightTrigger;
	bool previousTrigger = (previousAxis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;
	bool currentTrigger  = (currentAxis  > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? true : false;

	if ((previousTrigger) && (!currentTrigger))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							LeftButtonValue
****************************************************************************/
/* @fn        bool GamePad::LeftButtonValue(int gamePadId)
*  @brief     Return Left Button Value(Threshold ～ 100) : not pressing 0
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
int GamePad::LeftButtonValue(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0; }

	BYTE axis = _currentStates[gamePadId].Gamepad.bLeftTrigger;
	if (axis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		float result = axis * 100.0f / 255.0f;
		return static_cast<int>(result);
	}
	return 0;
}

/****************************************************************************
*							RightButtonValue
****************************************************************************/
/* @fn        bool GamePad::RightButtonValue(int gamePadId)
*  @brief     Return Right Button Value(Threshold ～ 100) : not pressing 0
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
int GamePad::RightButtonValue(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0; }

	BYTE axis = _currentStates[gamePadId].Gamepad.bRightTrigger;
	if (axis > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		float result = axis * 100.0f / 255.0f;
		return static_cast<int>(result);
	}
	return 0;
}

/****************************************************************************
*							IsPressLStick
****************************************************************************/
/* @fn        bool GamePad::IsPressLStick(int gamePadId)
*  @brief     Detect press LStick
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressLStick(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	short x = _currentStates[gamePadId].Gamepad.sThumbLX;
	short y = _currentStates[gamePadId].Gamepad.sThumbLY;

	if ((x <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ||
		(x >=  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		return true;
	}

	if ((y <= -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) ||
		(y >=  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
	{
		return true;
	}
	
	return false;
}

/****************************************************************************
*							IsPressRStick
****************************************************************************/
/* @fn        bool GamePad::IsPressRStick(int gamePadId)
*  @brief     Detect press RStick
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressRStick(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	short x = _currentStates[gamePadId].Gamepad.sThumbRX;
	short y = _currentStates[gamePadId].Gamepad.sThumbRY;

	if ((x <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ||
		(x >=  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return true;
	}

	if ((y <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) ||
		(y >=  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
	{
		return true;
	}

	return false;
}

/****************************************************************************
*							IsPressLStick
****************************************************************************/
/* @fn        bool GamePad::IsPressLStick(float threshold, int gamePadId)
*  @brief     Detect press LStick
*  @param[in] float threshold(0.0～100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressLStick(float threshold, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	short x = _currentStates[gamePadId].Gamepad.sThumbLX;
	short y = _currentStates[gamePadId].Gamepad.sThumbLY;
	short deadZone = static_cast<short>(threshold / 100.0f * SHORT_PLUS_MAX);

	if (x <= (-deadZone) || (deadZone <= x))
	{
		return true;
	}

	if ((y <= -deadZone) || (deadZone <= y))
	{
		return true;
	}

	return false;
}

/****************************************************************************
*							IsPressRStick
****************************************************************************/
/* @fn        bool GamePad::IsPressRStick(int threshold, int gamePadId)
*  @brief     Detect press RStick
*  @param[in] float threshold(0.0～100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　bool
*****************************************************************************/
bool GamePad::IsPressRStick(float threshold, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return false; }

	short x = _currentStates[gamePadId].Gamepad.sThumbLX;
	short y = _currentStates[gamePadId].Gamepad.sThumbLY;
	short deadZone = static_cast<short>(threshold / 100.0f * SHORT_PLUS_MAX);

	if (x <= (-deadZone) || (deadZone <= x))
	{
		return true;
	}

	if ((y <= -deadZone) || (deadZone <= y))
	{
		return true;
	}
	return false;
}

/****************************************************************************
*							LStick_X
****************************************************************************/
/* @fn        float GamePad::LStick_X(int gamePadId)
*  @brief     Return LStick_X Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
float GamePad::LStick_X(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0.0f; }

	short  y      = _currentStates[gamePadId].Gamepad.sThumbLX;
	bool   isPlus = (y >= 0) ? true : false;
	float  result = isPlus ? (static_cast<float>(y) / SHORT_PLUS_MAX) * 100.0f : (static_cast<float>(y) / SHORT_MINUS_MAX) * 100.0f;
	return result;
}

/****************************************************************************
*							LStick_Y
****************************************************************************/
/* @fn        float GamePad::LStick_Y(int gamePadId)
*  @brief     Return LStick_Y Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
float GamePad::LStick_Y(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0.0f; }

	short  y      = _currentStates[gamePadId].Gamepad.sThumbLY;
	bool   isPlus = (y >= 0) ? true : false;
	float  result = isPlus ? (static_cast<float>(y) / SHORT_PLUS_MAX) * 100.0f : (static_cast<float>(y) / SHORT_MINUS_MAX) * 100.0f;
	return result;
}

/****************************************************************************
*							RStick_X
****************************************************************************/
/* @fn        float GamePad::RStick_X(int gamePadId)
*  @brief     Return RStick_X Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
float GamePad::RStick_X(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0.0f; }

	short  y = _currentStates[gamePadId].Gamepad.sThumbRX;
	bool   isPlus = (y >= 0) ? true : false;
	float  result = isPlus ? (static_cast<float>(y) / SHORT_PLUS_MAX) * 100.0f : (static_cast<float>(y) / SHORT_MINUS_MAX) * 100.0f;
	return result;
}

/****************************************************************************
*							RStick_Y
****************************************************************************/
/* @fn        float GamePad::RStick_Y(int gamePadId)
*  @brief     Return RStick_Y Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　float
*****************************************************************************/
float GamePad::RStick_Y(int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId) { return 0.0f; }

	short  y      = _currentStates[gamePadId].Gamepad.sThumbRY;
	bool   isPlus = (y >= 0) ? true : false;
	float  result = isPlus ? (static_cast<float>(y) / SHORT_PLUS_MAX) * 100.0f : (static_cast<float>(y) / SHORT_MINUS_MAX) * 100.0f; 
	return result;
}

/****************************************************************************
*							LStick
****************************************************************************/
/* @fn        float GamePad::LStick(int gamePadId)
*  @brief     Return LStick Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　StickValue
*****************************************************************************/
StickValue GamePad::LStick(int gamePadId)
{
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId){ ::OutputDebugString(L"Error! An unspecified id was entered.");}

	StickValue stickValue={};
	stickValue.XAxis = LStick_X(gamePadId);
	stickValue.YAxis = LStick_Y(gamePadId);
	return stickValue;
}

/****************************************************************************
*							RStick
****************************************************************************/
/* @fn        float GamePad::RStick(int gamePadId)
*  @brief     Return RStick Value (0.0f ～ 100.0f)
*  @param[in] int gamePadId (Max 4 people)
*  @return 　　StickValue
*****************************************************************************/
StickValue GamePad::RStick(int gamePadId)
{
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId){ ::OutputDebugString(L"Error! An unspecified id was entered.");}

	StickValue stickValue={};
	stickValue.XAxis = RStick_X(gamePadId);
	stickValue.YAxis = RStick_Y(gamePadId);
	return stickValue;
}

/****************************************************************************
*							SetVibration
****************************************************************************/
/* @fn        void GamePad::SetVibration(float leftMotor, float rightMotor, int gamePadId)
*  @brief     Set Motor Vibration(0.0f ～ 1.0f)
*  @param[in] leftMoter (0.0f ～ 1.0f)
*  @param[in] rightMoter(0.0f ～ 1.0f)
*  @param[in] gamePadId (Max 4 people)
*  @return 　　void
*****************************************************************************/
void GamePad::SetVibration(float leftMotor, float rightMotor, int gamePadId)
{
	// Range check for gamepad id.
	if (gamePadId < 0 || (int)GamePadID::TotalIdNum < gamePadId)
	{
		::OutputDebugString(L"Error! An unspecified id was entered.");
		return;
	}

	if (leftMotor  > 1.0f) { leftMotor  = 1.0f; }
	if (leftMotor  < 0.0f) { leftMotor  = 0.0f; }
	if (rightMotor > 1.0f) { rightMotor = 1.0f; }
	if (rightMotor < 0.0f) { rightMotor = 0.0f; }

	leftMotor  = leftMotor  * USHORT_MAX;
	rightMotor = rightMotor * USHORT_MAX;

	_vibrationStates[gamePadId].wLeftMotorSpeed  = static_cast<unsigned short>(leftMotor);
	_vibrationStates[gamePadId].wRightMotorSpeed = static_cast<unsigned short>(rightMotor);

	XInputSetState(gamePadId, &_vibrationStates[gamePadId]);
}
#pragma endregion Public Function

#pragma region Private Function
#pragma endregion Private Function