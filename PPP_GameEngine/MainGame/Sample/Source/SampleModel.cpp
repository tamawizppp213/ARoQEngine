//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleModel.hpp"
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GameCore/Core/Include/Camera.hpp"
#include "GameCore/Rendering/Model/Include/GameModel.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameCore/Rendering/Core/BasePass/Include/BasePassZPrepass.hpp"
#include "GameCore/Rendering/Core/BasePass/Include/BasePassGBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;
using namespace gc;
using namespace gc::core;

std::shared_ptr<basepass::ZPrepass> zprepass = nullptr;
std::shared_ptr<basepass::GBuffer> gbuffer = nullptr;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleModel::SampleModel()
{

}
SampleModel::~SampleModel()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleModel::Initialize(const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] GameTimer* gameTimer
*  @return �@�@void
*****************************************************************************/
void SampleModel::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleModel::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::Update()
{
	Scene::Update();
	_camera->Update(_gameTimer);
	_model->Update(_gameTimer->DeltaTime());
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleModel::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::Draw()
{
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	commandList->SetViewportAndScissor(
		rhi::core::Viewport(0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		rhi::core::ScissorRect(0, 0, (long)Screen::GetScreenWidth(), (long)Screen::GetScreenHeight()));
	
	/*zprepass->Draw(_camera->GetResourceView());
	gbuffer->Draw(_camera->GetResourceView());*/
	_engine->BeginSwapchainRenderPass();
	_skybox->Draw(_camera->GetResourceView());
	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleModel::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::Terminate()
{
	
}
#pragma endregion Public Function

#pragma region Protected Function

/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleModel::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList = _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex());
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
	-           Model
	---------------------------------------------------------------------*/
	_model = GameObject::Create<GameModel>(_engine);
	_model->Load(L"Resources/YYB Hatsune Miku/YYB Hatsune Miku_10th_v1.02.pmx");
	_model->SetDebugColor(gm::Float4(1, 0, 0, 1));

	zprepass = std::make_shared<basepass::ZPrepass>(_engine, Screen::GetScreenWidth(), Screen::GetScreenHeight());
	zprepass->Add(_model);
	gbuffer = std::make_shared<basepass::GBuffer>(_engine);
	gbuffer->Add(_model);

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
*  @fn        void SampleModel::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::OnKeyboardInput()
{
	const float deltaTime = _gameTimer->DeltaTime();
	const float speed = 15.0f;
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
	if(_gameInput.GetKeyboard()->IsPress(DIK_D))
	{
		_camera->Strafe(speed * deltaTime);
	}
	/*-------------------------------------------------------------------
	-           Keyboad Input S (Move Camera)
	---------------------------------------------------------------------*/
	if (_gameInput.GetKeyboard()->IsPress(DIK_A))
	{
		_camera->Strafe(-speed * deltaTime);
	}

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleModel::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::OnMouseInput()
{
	/*-------------------------------------------------------------------
	-           Mouse Input Left Button
	---------------------------------------------------------------------*/
	if (_gameInput.GetMouse()->IsPress(MouseButton::LEFT))
	{
		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().x)); // 0.25f: ���x�ݒ肵����
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().y));

		_camera->RotatePitch(dy);
		_camera->RotateWorldY(dx);
	}
}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleModel::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void SampleModel::OnGamePadInput()
{

}

#pragma endregion Protected Function