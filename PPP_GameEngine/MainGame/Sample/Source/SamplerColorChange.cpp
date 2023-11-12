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
#include "GameCore/Rendering/Effect/Include/DepthOfField.hpp"
#include "GameCore/Rendering/Effect/Include/Mosaic.hpp"
#include "GameCore/Rendering/Effect/Include/Vignette.hpp"
#include "GameCore/Rendering/Effect/Include/WhiteBalance.hpp"
#include "GameCore/Rendering/Effect/Include/ScreenSpaceReflection.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameCore/Rendering/Debugger/Include/ScreenCapture.hpp"

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
namespace
{
	std::shared_ptr<gc::rendering::ScreenCapture> _capture = nullptr;
}
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
void SampleColorChange::Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer)
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
	_engine->BeginSwapchainRenderPass();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto frameIndex  = _engine->GetCurrentFrameIndex();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	const auto frameBuffer = _engine->GetFrameBuffer(frameIndex);

	commandList->SetViewportAndScissor(
		core::Viewport(0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		core::ScissorRect(0, 0, (long)Screen::GetScreenWidth(), (long)Screen::GetScreenHeight()));

	_skybox->Draw(_camera->GetResourceView());

	/*-------------------------------------------------------------------
	-             Use effects
	---------------------------------------------------------------------*/
	//_colorChanges[_colorIndex]->Draw();
	//_capture->Capture(_engine->GetFrameBuffer(frameIndex)->GetRenderTarget());
	
	if (_useWhiteBalance) { _whiteBalance->Draw(); }
	if (_useVignette) { _vignette->Draw(); }
	if (_useBlur)   { _gaussianBlur->Draw(frameBuffer); }
	if (_useMosaic) { _mosaic->Draw(); }
	

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
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy);
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics);
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
	_gaussianBlur = std::make_shared<GaussianBlur>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight(), true);

	_mosaic = std::make_shared<Mosaic>(_engine, 20.0f);

	const VignetteSettings vignetteSettings =
	{
		.Color = gm::Float4(1,0,0,1),
		.UVCenter = gm::Float2(0.5f, 0.5f),
		.Intensity = 1.0f,
		.Smoothness = 1.0f
	};
	_vignette = std::make_shared<Vignette>(_engine, vignetteSettings);

	_whiteBalance = std::make_shared<WhiteBalance>(_engine, 1.0f, 0.0f);
	_capture = std::make_shared<gc::rendering::ScreenCapture>(_engine, _gameInput.GetKeyboard());

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList->EndRecording();

	_engine->FlushGPUCommands(CommandListType::Copy);
	_engine->FlushGPUCommands(CommandListType::Graphics);
	
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
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_P))
	{
		_colorIndex = (_colorIndex + 1) % _colorChanges.size();
	}
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_O))
	{
		_useBlur = _useBlur ? false : true;
	}
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_I))
	{
		_useMosaic = _useMosaic ? false : true;
	}
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_U))
	{
		_useVignette = _useVignette ? false : true;
	}
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_Y))
	{
		_useWhiteBalance = _useWhiteBalance ? false : true;
	}
	if (_gameInput.GetKeyboard()->IsTrigger(DIK_T))
	{
		_useScreenSpaceReflection = _useScreenSpaceReflection ? false : true;
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
*  @fn        void SampleColorChange::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleColorChange::OnGamePadInput()
{

}

#pragma endregion Protected Function