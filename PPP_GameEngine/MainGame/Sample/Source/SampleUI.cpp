//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleUI.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIRenderer.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIImage.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIButton.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace gc::ui;
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleUI::SampleUI()
{

}
SampleUI::~SampleUI()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleUI::Initialize( const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleUI::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleUI::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::Update()
{
	Scene::Update();

	_renderer->AddFrameObjects({ _button }, _resourceView);
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleUI::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::Draw()
{
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	commandList->SetViewportAndScissor(
		core::Viewport   (0, 0, (float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight()),
		core::ScissorRect(0, 0, (long) Screen::GetScreenWidth(), (long) Screen::GetScreenHeight()));

	_renderer->Draw();

	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleUI::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::Terminate()
{
	_resourceView.reset();
	_resourceCache.reset();
}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleUI::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex());
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	copyCommandList    ->BeginRecording();
	graphicsCommandList->BeginRecording();

	/*-------------------------------------------------------------------
	-             SetUp Resources
	---------------------------------------------------------------------*/
	_button = std::make_shared<Button>(_gameInput.GetMouse());
	_button->CreateInNDCSpace({ 0,0,0 }, { 0.5f,0.5f });

	// Create Texture
	_resourceCache = std::make_shared<GPUResourceCache>(_engine->GetDevice(), graphicsCommandList);
	_resourceView  = _resourceCache->Load(L"Resources/BackGround2.png");
	_resourceCache->Load(L"Resources/Cubemap.jpg");

	// Create UI Renderer
	_renderer = std::make_unique<gc::ui::UIRenderer>(_engine);

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList    ->EndRecording();

	_engine->FlushCommandQueue(CommandListType::Graphics);
	_engine->FlushCommandQueue(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleUI::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::OnKeyboardInput()
{

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleUI::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::OnMouseInput()
{
	if (_button->IsPress())
	{
		std::cout << "Click!!" << std::endl;
	};
}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleUI::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleUI::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleUI::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function