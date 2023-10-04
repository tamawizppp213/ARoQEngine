//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleURP.hpp"
#include "GameCore/Rendering/EnvironmentMap/Include/SkyDome.hpp"
#include "GameCore/Core/Include/Camera.hpp"
#include "GameCore/Rendering/Model/Include/GameModel.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameCore/Rendering/Core/Renderer/Include/UniversalRenderPipeline.hpp"
#include "GameCore/Rendering/Light/External/Include/IESProfiler.hpp"
#include "GameCore/Rendering/Light/Include/SceneLightBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#include "GameCore/Rendering/Model/Include/MaterialType.hpp"
#include "GameCore/Rendering/Model/Include/Material.hpp"
#include "MainGame/Sample/Include/SampleColorChange.hpp"
#include "MainGame/Core/Include/SceneManager.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;
using namespace gc;
using namespace gc::core;
using namespace gc::rendering;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleURP::SampleURP()
{

}
SampleURP::~SampleURP()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleURP::Initialize(const GameTimerPtr& gameTimer)
* 
*  @brief     Initialize scene
* 
*  @param[in] GameTimer* gameTimer
* 
*  @return    void
*****************************************************************************/
void SampleURP::Initialize(const std::shared_ptr<PPPEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}

/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleURP::Update()
* 
*  @brief     Update Scene
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void SampleURP::Update()
{
	Scene::Update();
	_camera->Update(_gameTimer);
	_model->Update(_gameTimer->DeltaTime());
	_floor->Update(_gameTimer->DeltaTime());

	const DirectionalLightData directionalLight = 
	{
		.Direction = gm::Float3(1.0f, 1.0f, 1.0f),
		.Brightness = 2.5f,
		.Color      = gm::Float3(1,1,1),
		.IsUse      = true
	};
	_renderer->SetLight<DirectionalLightData>(LightType::Directional, 0, directionalLight);
}

/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleURP::Draw()
* 
*  @brief     Draw Scene]]
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void SampleURP::Draw()
{
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);
	commandList->SetViewportAndScissor(
		rhi::core::Viewport(0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		rhi::core::ScissorRect(0, 0, (long)Screen::GetScreenWidth(), (long)Screen::GetScreenHeight()));

	_renderer->Draw();
	_skybox  ->Draw(_camera->GetResourceView());
	_engine  ->EndDrawFrame();
}

/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleURP::Terminate()
* 
*  @brief     Terminate Scene
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void SampleURP::Terminate()
{

}
#pragma endregion Public Function

#pragma region Protected Function

/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleURP::LoadMaterials(GameTimer* gameTimer)
* 
*  @brief     Load Materials
* 
*  @param[in] void
* 
*  @return Å@Å@void
*****************************************************************************/
void SampleURP::LoadMaterials()
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
	-           Model
	---------------------------------------------------------------------*/
	_model = GameObject::Create<GameModel>(_engine);
	_model->Load(L"Resources/YYB Hatsune Miku/YYB Hatsune Miku_10th_v1.02.pmx");
	_model->SetDebugColor(gm::Float4(1, 0, 0, 1));

	PBRMaterial pbrMaterial = {};
	pbrMaterial.Diffuse = gm::Float4(1.0f, 1.0f, 1.0f, 1);
	pbrMaterial.Roughness = 1.0f;
	const auto material = std::make_shared<Material>(_engine, GPUBufferMetaData::ConstantBuffer(sizeof(PBRMaterial), 1));
	material->PackMaterial(&pbrMaterial);

	_floor = GameObject::Create<GameModel>(_engine);
	_floor->Load(PrimitiveMeshType::Grid, material);
	_floor->SetScale(100.0f, 100.0f, 100.0f);

	/*-------------------------------------------------------------------
	-           Universal Rendering Pipeline
	---------------------------------------------------------------------*/
	_renderer = std::make_shared<gc::URP>(_engine, _gameTimer);
	_renderer->SetSceneView(_camera->GetResourceView());
	_renderer->Add(Forward, _model);
	_renderer->Add(Forward, _floor);
	_pppEngine->SetRenderingPipeline(_renderer);

	IESProfiler profiler;
	profiler.Load(L"Resources/Test.IES");


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
*  @fn        void SampleURP::OnKeyboardInput()
* 
*  @brief     KeyboardInput
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
bool isOn = false;
void SampleURP::OnKeyboardInput()
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
	if (_gameInput.GetKeyboard()->IsPress(DIK_D))
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

	if (_gameInput.GetKeyboard()->IsTrigger(DIK_Q))
	{
		isOn = !isOn;
		if(isOn){ _engine->OnResize(2800, 1000); }
		else{ _engine->OnResize(1920, 1080); }
	}
	//std::cout << Screen::GetScreenWidth() << std::endl;
}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleURP::OnMouseInput()
* 
*  @brief     MouseInput
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void SampleURP::OnMouseInput()
{
	/*-------------------------------------------------------------------
	-           Mouse Input Left Button
	---------------------------------------------------------------------*/
	if (_gameInput.GetMouse()->IsPress(MouseButton::LEFT))
	{
		float dx = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().x)); // 0.25f: ä¥ìxê›íËÇµÇΩÇ¢
		float dy = DirectX::XMConvertToRadians(0.25f * static_cast<float>(_gameInput.GetMouse()->GetMouseVelocity().y));

		_camera->RotatePitch(dy);
		_camera->RotateWorldY(dx);
	}

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleModel::OnGamePadInput()
* 
*  @brief     GamePadInput
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void SampleURP::OnGamePadInput()
{

}

#pragma endregion Protected Function