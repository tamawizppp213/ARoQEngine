//////////////////////////////////////////////////////////////////////////////////
///             @file   GameInput.cpp
///             @brief  GameInput
///             @author Toide Yutaro
///             @date   2020_11_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Input/Include/GameInput.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "xinput.lib")
//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
bool GameInput::Initialize(void* hInstance, void* hwnd)
{
	bool result = true;

	_keyboard = std::make_shared<Keyboard>();
	_mouse    = std::make_shared<Mouse>();
	_gamePad  = std::make_shared<GamePad>();

	result = DInputInitialize((HINSTANCE)hInstance);
	result = _keyboard->Initialize(_dInput, (HINSTANCE)hInstance, (HWND)hwnd);
	result = _mouse   ->Initialize(_dInput, (HINSTANCE)hInstance, (HWND)hwnd);
	result = _gamePad ->Initialize();

	return result;
}

void GameInput::Update()
{
	_keyboard->Update();
	_mouse   ->Update();
	_gamePad ->Update();
}

void GameInput::Finalize()
{
	_gamePad ->Finalize();
	_mouse   ->Finalize();
	_keyboard->Finalize();
	DInputFinalize();
}

bool GameInput::DInputInitialize(HINSTANCE hInstance)
{
	if (_dInput == nullptr)
	{
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&_dInput, NULL)))
		{
			return false;
		}
		
	}

	return true;
}

void GameInput::DInputFinalize()
{
	if (_dInput != nullptr)
	{
		_dInput->Release();
		_dInput = nullptr;
	}
}