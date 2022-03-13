//////////////////////////////////////////////////////////////////////////////////
///             @file   GameInput.hpp
///             @brief  GameInput (Singleton)
///             @author Toide Yutaro
///             @date   2020_11_
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAMEINPUT_HPP
#define GAMEINPUT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GamePad.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                        Geme Input
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			GameInput
*************************************************************************//**
*  @class     GameInput
*  @brief     GameInput
*****************************************************************************/
class GameInput
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(HINSTANCE hInstance, HWND hwnd);
	void Update();
	void Finalize();
	Keyboard& GetKeyboard() { return this->_keyboard; }
	Mouse&    GetMouse()    { return this->_mouse; }
	GamePad&  GetGamePad()  { return this->_gamePad; }
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	// Singleton 
	static GameInput& Instance()
	{
		static GameInput gameInput;
		return gameInput;
	}
	GameInput(const GameInput&)            = delete;
	GameInput& operator=(const GameInput&) = delete;
	GameInput(GameInput&&)                 = delete;
	GameInput& operator=(GameInput&&)      = delete;
	
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	GameInput() = default;
	~GameInput() = default;
	bool DInputInitialize(HINSTANCE hInstance);
	void DInputFinalize();

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	Keyboard _keyboard;
	Mouse    _mouse;
	GamePad _gamePad;
	LPDIRECTINPUT8 _dInput = nullptr;
	

};

#endif