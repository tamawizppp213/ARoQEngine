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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	using KeyboardPtr = gu::SharedPointer<Keyboard>;
	using MousePtr    = gu::SharedPointer<Mouse>;
	using GamePadPtr  = gu::SharedPointer<GamePad>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Initialize(void* hInstance, void* hwnd);
	void Update();
	void Finalize();
	KeyboardPtr GetKeyboard() { return this->_keyboard; }
	MousePtr    GetMouse()    { return this->_mouse; }
	GamePadPtr  GetGamePad()  { return this->_gamePad; }
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
	KeyboardPtr _keyboard = nullptr;
	MousePtr    _mouse    = nullptr;
	GamePadPtr  _gamePad   = nullptr;
	LPDIRECTINPUT8 _dInput = nullptr;
	

};

#endif