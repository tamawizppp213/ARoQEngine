//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleSky.hpp"
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GameCore/Core/Include/Camera.hpp"
#include "GameUtility/Base/Include/Screen.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;
using namespace gc;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleSky::SampleSky()
{

}
SampleSky::~SampleSky()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleSky::Initialize(const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] GameTimer* gameTimer
*  @return 　　void
*****************************************************************************/
void SampleSky::Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleSky::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::Update()
{
	Scene::Update();
	_camera->Update(_gameTimer);
	
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleSky::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::Draw()
{
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	commandList->SetViewportAndScissor(
		core::Viewport   (0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		core::ScissorRect(0, 0, (long) Screen::GetScreenWidth(), (long) Screen::GetScreenHeight()));

	_skybox->Draw(_camera->GetResourceView());
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleSky::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::Terminate()
{
	
}
#pragma endregion Public Function

#pragma region Protected Function

/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleSky::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);
	copyCommandList    ->BeginRecording();
	graphicsCommandList->BeginRecording();
	/*-------------------------------------------------------------------
	-           Camera
	---------------------------------------------------------------------*/
	_camera = gu::MakeShared<Camera>(_engine);
	_camera->SetPosition(0.0f, 10.0f, -20.0f);
	/*-------------------------------------------------------------------
	-           Skybox
	---------------------------------------------------------------------*/
	_skybox = gu::MakeShared<SkyDome>(_engine, L"Resources/grasscube1024.dds");

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList->EndRecording();

	_engine->FlushGPUCommands(CommandListType::Graphics);
	_engine->FlushGPUCommands(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleSky::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::OnKeyboardInput()
{
	const float deltaTime = _gameTimer->DeltaTime();
	const float speed     = 0.25f;
	/*-------------------------------------------------------------------
	-           Keyboad Input W (Move Camera )
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_W))
	{
		_camera->Walk(speed * deltaTime);
	}
	/*-------------------------------------------------------------------
	-           Keyboad Input S (Move Camera)
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_S))
	{
		_camera->Walk(-speed * deltaTime);
	}

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleSky::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::OnMouseInput()
{
	/*-------------------------------------------------------------------
	-           Mouse Input Left Button
	---------------------------------------------------------------------*/
	if (_gameInput.GetMouse()->IsPress(MouseButton::LEFT))
	{
		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().x)); // 0.25f: 感度設定したい
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().y));

		_camera->RotatePitch(dy);
		_camera->RotateWorldY(dx);
	}
}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleSky::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleSky::OnGamePadInput()
{

}

#pragma endregion Protected Function