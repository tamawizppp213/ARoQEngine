//////////////////////////////////////////////////////////////////////////////////
///             @file   GamePad.hppÅ@(current 1Player only)
///             @brief  GamePad
///             @author Toide Yutaro
///             @date   2020_12_29: , 2021_02_04(ï°êîêlé¿ëï)
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
#include <Xinput.h>
#include <vector>


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4369)
/****************************************************************************
*				  			GamePadInput
*************************************************************************//**
*  @enum class     GamePadInput 
*  @brief     GamePad
*****************************************************************************/
enum class GamePadInput
{
	DPAD_UP    = 0x0001,
	DPAD_DOWN  = 0x0002,
	DPAD_LEFT  = 0x0004,
	DPAD_RIGHT = 0x0008,

	START = 0x0010,
	BACK  = 0x0020,
	LB    = 0x0100,
	RB    = 0x0200,

	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,

	CONNECTED = 0x80000000,
};

/****************************************************************************
*				  			GamePadInput
*************************************************************************//**
*  @enum class GamePadID
*  @brief      GamePad Player Num
*****************************************************************************/
enum class GamePadID
{
	GamePadId_1P,
	GamePadId_2P,
	GamePadId_3P,
	GamePadId_4P,
	TotalIdNum
};

/****************************************************************************
*				  			StickValue
*************************************************************************//**
*  @struct    StickValue
*  @brief     Stick Value (X, Y)
*****************************************************************************/
struct StickValue
{
	float XAxis;
	float YAxis;
};

//////////////////////////////////////////////////////////////////////////////////
//                          GamePad Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region GamePad
/****************************************************************************
*				  			Gamepad
*************************************************************************//**
*  @class     GamePad
*  @brief     GamePad
*****************************************************************************/
class GamePad
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize();
	void Update();
	void Finalize();

#pragma region XINPUT
	void EnableXInput(bool isEnabled);
#pragma endregion XINPUT

#pragma region BUTTON
	bool IsPressButton  (GamePadInput button, int gamePadId = 0);
	bool IsTriggerButton(GamePadInput button, int gamePadId = 0);
	bool IsReleaseButton(GamePadInput button, int gamePadId = 0);
	bool IsPressLeftButton   (int gamePadId = 0);
	bool IsTriggerLeftButton (int gamePadId = 0);
	bool IsReleaseLeftButton (int gamePadId = 0);
	bool IsPressRightButton  (int gamePadId = 0);
	bool IsTriggerRightButton(int gamePadId = 0);
	bool IsReleaseRightButton(int gamePadId = 0);
	int LeftButtonValue (int gamePadId = 0); // 0 Å` 100
	int RightButtonValue(int gamePadId = 0);// 0 Å` 100
#pragma endregion BUTTON

#pragma region STICK
	bool IsPressLStick(int gamePadId = 0);
	bool IsPressRStick(int gamePadId = 0);
	bool IsPressLStick(float threshold, int gamePadId = 0);
	bool IsPressRStick(float threshold, int gamePadId = 0);
	float LStick_X(int gamePadId = 0); // 0 Å` 100
	float LStick_Y(int gamePadId = 0); // 0 Å` 100
	float RStick_X(int gamePadId = 0); // 0 Å` 100 
	float RStick_Y(int gamePadId = 0); // 0 Å` 100
	StickValue LStick(int gamePadId = 0);
	StickValue RStick(int gamePadId = 0);
#pragma endregion STICK

#pragma region VIBRATION
	void SetVibration(float leftMotor, float rightMotor, int gamePadId = 0);
#pragma endregion VIBRATION

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GamePad();
	~GamePad() = default;
	GamePad(const GamePad&)            = default;
	GamePad& operator=(const GamePad&) = default;
	GamePad(GamePad&&)                 = default;
	GamePad& operator=(GamePad&&)      = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	std::vector<XINPUT_STATE>     _currentStates;
	std::vector<XINPUT_STATE>     _previousStates;
	std::vector<XINPUT_VIBRATION> _vibrationStates;
	int _maxPlayer;
	
};

#endif
