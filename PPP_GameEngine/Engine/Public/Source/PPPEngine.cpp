//////////////////////////////////////////////////////////////////////////////////
//              @file   PPPEngine.cpp
///             @brief  �e����̃G���W�����܂Ƃ�, �X���b�h���Ǘ�����N���X
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/PPPEngine.hpp"

// platform
#include "Platform/Core/Include/CorePlatformApplication.hpp"
#include "Platform/Core/Include/CoreWindow.hpp"

// renderer
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"

// input
#include "GameCore/Input/Include/GameInput.hpp"

// timer
#include "GameUtility/Base/Include/GameTimer.hpp"

#include "MainGame/Core/Include/GameManager.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::setting;
using namespace engine::core;
using namespace platform::core;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
void PPPEngine::StartUp(const StartUpParameters& setting)
{
	/*---------------------------------------------------------------
					  Timer
	-----------------------------------------------------------------*/
	_mainThreadTimer = std::make_shared<GameTimer>();

	/*---------------------------------------------------------------
					  Platform Application�̍쐬
	-----------------------------------------------------------------*/
	_platformApplication = PlatformApplication::Create(PlatformType::Windows);

	/*---------------------------------------------------------------
					  �E�B���h�E�N���X�̍쐬
	-----------------------------------------------------------------*/
	_mainWindow = _platformApplication->MakeWindow();
	_platformApplication->SetUpWindow(_mainWindow, setting.WindowSettings);

	if (!_mainWindow->Show()) { printf("failed to show window\n"); }

	/*---------------------------------------------------------------
					  �����_�����O�G���W���̍쐬
	-----------------------------------------------------------------*/
	_graphicsEngine = std::make_shared<LowLevelGraphicsEngine>();
	_graphicsEngine->StartUp(setting.GraphicsSettings.APIversion, _mainWindow->GetWindowHandle(), _platformApplication->GetInstanceHandle());

	/*---------------------------------------------------------------
					  Input�̍쐬
	-----------------------------------------------------------------*/
	GameInput::Instance().Initialize(_platformApplication->GetInstanceHandle(), _mainWindow->GetWindowHandle());

	/*---------------------------------------------------------------
					  �X���b�h�̊Ǘ�
	-----------------------------------------------------------------*/
	_engineThreadManager = std::make_unique<EngineThreadManager>();
}

void PPPEngine::ExecuteMainThread()
{
	_mainThreadTimer->Reset();
	/*---------------------------------------------------------------
						Main Loop
	-----------------------------------------------------------------*/
	while (!_platformApplication->IsQuit())
	{
		if (!_platformApplication->PumpMessage())
		{
			_mainThreadTimer->Tick();
			if (!_isApplicationPaused)
			{
				_mainThreadTimer->AverageFrame(_mainWindow->GetWindowHandle());
				GameInput::Instance().Update();
				GameManager::Instance().GameMain(); // ����ύX���܂�. 
			}
		}
	}
}

void PPPEngine::ShutDown()
{
	// ���̓f�o�C�X�̔j��
	GameInput::Instance().Finalize();
	
	// �O���t�B�b�N�G���W���̔j��
	_graphicsEngine->ShutDown();

	// �X���b�h�̔j��
	_engineThreadManager.reset();

}
#pragma endregion Main Function