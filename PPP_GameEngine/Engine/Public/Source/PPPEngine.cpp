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
#include "Platform//Core/Include/CorePlatformCommand.hpp"

// renderer
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Rendering/Core/Renderer/Include/RenderPipeline.hpp"
#include "GameCore/Core/Include/Camera.hpp"

// input
#include "GameCore/Input/Include/GameInput.hpp"

// timer
#include "GameUtility/Base/Include/GameTimer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

#include "MainGame/Core/Include/GameManager.hpp"
#include "MainGame/Core/Include/SceneManager.hpp"
#include "Platform/Windows/Include/WindowsCursor.hpp"

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
PPPEngine::~PPPEngine()
{
	ShutDown();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
void PPPEngine::StartUp(const StartUpParameters& setting)
{
	_hasShutdown = false;

	StartUpParameter = setting;

	/*---------------------------------------------------------------
					  Timer
	-----------------------------------------------------------------*/
	_mainThreadTimer = gu::MakeShared<GameTimer>();

	/*---------------------------------------------------------------
					  Platform Application�̍쐬
	-----------------------------------------------------------------*/
	_platformApplication = PlatformApplication::Create();

	/*---------------------------------------------------------------
					  �E�B���h�E�N���X�̍쐬
	-----------------------------------------------------------------*/
	_mainWindow = _platformApplication->MakeWindow();
	_platformApplication->SetUpWindow(_mainWindow, setting.WindowSettings);

	if (!_mainWindow->Show()) { printf("failed to show window\n"); }
	
	/*---------------------------------------------------------------
					  Platform command�N���X�̍쐬
	-----------------------------------------------------------------*/
	_platformCommand = _platformApplication->MakeCommand();

	/*---------------------------------------------------------------
					  �����_�����O�G���W���̍쐬
	-----------------------------------------------------------------*/
	_graphicsEngine = gu::MakeShared<LowLevelGraphicsEngine>();
	_graphicsEngine->StartUp(StartUpParameter.GraphicsSettings.APIversion, _mainWindow->GetWindowHandle(), _platformApplication->GetInstanceHandle());

	/*---------------------------------------------------------------
					  Input�̍쐬
	-----------------------------------------------------------------*/
	GameInput::Instance().Initialize(_platformApplication->GetInstanceHandle(), _mainWindow->GetWindowHandle());

}

void PPPEngine::Run()
{
	/*---------------------------------------------------------------
					  �X���b�h�̊Ǘ�
	-----------------------------------------------------------------*/
	_engineThreadManager = gu::MakeShared<EngineThreadManager>();
	_isStoppedAllThreads.store(false); // false����������

	_engineThreadManager->GetUpdateMainThread()->Submit([&]() { this->ExecuteUpdateThread(); });
	_engineThreadManager->GetRenderMainThread()->Submit([&]() { this->ExecuteRenderThread(); });
}

/****************************************************************************
*                     ExecuteMainThread
*************************************************************************//**
*  @fn        void PPPEngine::ExecuteMainThread()
*
*  @brief     Message loop�����Ă܂�.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
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
				// ����V���O���X���b�h��.
				GameManager::Instance().GameUpdateMain();
			}
		}

	}
	_isStoppedAllThreads.store(true);

	// �S�ẴX���b�h�ɑ΂�����s�����҂�
	_engineThreadManager->ShutDown();

}

void PPPEngine::ExecuteUpdateThread()
{
	while (!_isStoppedAllThreads)
	{
		
	}

	_engineThreadManager->CallExecuteComplete(ThreadPoolType::UpdateMain);
	printf("update finish\n");
}

/****************************************************************************
*                     ExecuteRenderThread
*************************************************************************//**
*  @fn        void PPPEngine::ExecuteRenderThread()
*
*  @brief     �`�惋�[�v�ł�
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PPPEngine::ExecuteRenderThread()
{
	/*---------------------------------------------------------------
					  �`�惋�[�v
	-----------------------------------------------------------------*/
	while (!_isStoppedAllThreads)
	{
		// �J�n���� : �R�}���h�o�b�t�@��Open�ɂ���
		_graphicsEngine->BeginDrawFrame();

		// ���������_�[�p�X�̐ݒ�
		_graphicsEngine->BeginSwapchainRenderPass();
		
		// �V�[���̕`��
		GameManager::Instance().GameDrawMain();

		// �I������, �`��R�}���h�����s����
		_graphicsEngine->EndDrawFrame();
	}


	_engineThreadManager->CallExecuteComplete(ThreadPoolType::RenderMain);
	printf("draw finish\n");
}

void PPPEngine::ShutDown()
{
	if (_hasShutdown) { return; }

	// �X���b�h�̔j��
	_engineThreadManager.Reset();

	// ���̓f�o�C�X�̔j��
	GameInput::Instance().Finalize();
	
	// �O���t�B�b�N�G���W���̔j��
	_graphicsEngine->ShutDown();


	_mainWindow.Reset();
	_platformCommand.Reset();
	_platformApplication.Reset();

}
#pragma endregion Main Function