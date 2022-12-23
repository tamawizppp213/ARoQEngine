//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleColorChange.hpp"
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GameCore/Core/Include/Camera.hpp"
#include "GameCore/Rendering/Effect/Include/ColorChange.hpp"
#include "GameCore/Rendering/Effect/Include/Blur.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
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
SampleColorChange::SampleColorChange()
{

}
SampleColorChange::~SampleColorChange()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleColorChange::Initialize( const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in]  const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleColorChange::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleColorChange::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::Update()
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
void SampleColorChange::Draw()
{
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	commandList->SetViewportAndScissor(
		core::Viewport(0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		core::ScissorRect(0, 0, (long)Screen::GetScreenWidth(), (long)Screen::GetScreenHeight()));

	_skybox->Draw(_camera->GetResourceView());
	_colorChanges[_colorIndex]->Draw();
	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleColorChange::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::Terminate()
{

}
#pragma endregion Public Function

#pragma region Protected Function

/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleColorChange::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy    , _engine->GetCurrentFrameIndex());
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	copyCommandList->BeginRecording();
	graphicsCommandList->BeginRecording();
	/*-------------------------------------------------------------------
	-           Camera
	---------------------------------------------------------------------*/
	_camera = std::make_shared<Camera>(_engine);
	_camera->SetPosition(0.0f, 10.0f, -20.0f);
	/*-------------------------------------------------------------------
	-           Skybox
	---------------------------------------------------------------------*/
	_skybox = std::make_shared<SkyDome>(_engine, L"Resources/grasscube1024.dds");
	/*-------------------------------------------------------------------
	-           Color Changes
	---------------------------------------------------------------------*/
	_colorChanges.resize(5);
	for (size_t i = 0; i < _colorChanges.size(); ++i)
	{
		_colorChanges[i] = std::make_shared<ColorChange>((ColorChangeType)(i + 1), _engine);
	}

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList->EndRecording();

	_engine->FlushCommandQueue(CommandListType::Graphics);
	_engine->FlushCommandQueue(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleColorChange::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::OnKeyboardInput()
{
	const float deltaTime = _gameTimer->DeltaTime();
	const float speed = 0.25f;
	/*-------------------------------------------------------------------
	-           Keyboad Input W (Move Camera )
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard().IsPress(DIK_W))
	{
		_camera->Walk(speed * deltaTime);
	}
	/*-------------------------------------------------------------------
	-           Keyboad Input S (Move Camera)
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard().IsPress(DIK_S))
	{
		_camera->Walk(-speed * deltaTime);
	}
	if (_gameInput.GetKeyboard().IsTrigger(DIK_P))
	{
		_colorIndex = (_colorIndex + 1) % _colorChanges.size();
	}
}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleColorChange::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::OnMouseInput()
{
	/*-------------------------------------------------------------------
	-           Mouse Input Left Button
	---------------------------------------------------------------------*/
	if (_gameInput.GetMouse().IsPress(MouseButton::LEFT))
	{
		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse().GetMouseVelocity().x)); // 0.25f: 感度設定したい
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse().GetMouseVelocity().y));

		_camera->RotatePitch(dy);
		_camera->RotateWorldY(dx);
	}
}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleColorChange::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::OnGamePadInput()
{

}

#pragma endregion Protected Function