//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleText.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIRenderer.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIText.hpp"
#include "GameCore/Rendering/UI/Public/Include/UIFont.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"

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
SampleText::SampleText()
{

}
SampleText::~SampleText()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleEmpty::Initialize(const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleText::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleEmpty::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::Update()
{
	Scene::Update();

	/*-------------------------------------------------------------------
	-            Update Text Color
	---------------------------------------------------------------------*/
	StringInfo info = 
	{   .String = "Text\nSample", .SizePerChar = {0.1f, 0.167f}, 
		.StartPosition = {0.5f * gm::Cos(_gameTimer->TotalTime()) - 0.5f, 0.5f * gm::Sin(_gameTimer->TotalTime()), 1.0f},
		.Space = {0.0f, 0.0f},
		.Color = {gm::Sin(_gameTimer->TotalTime()),gm::Cos(_gameTimer->TotalTime()),1,1}
	};
	_text = std::make_shared<Text>(CoordinateType::NDC, _font, info);

	_renderer->AddFrameObjects(_text->GetTextImages(), _text->GetFontView());
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleEmpty::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::Draw()
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
*  @fn        void SampleEmpty::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::Terminate()
{
	_resourceView.reset();
	_resourceCache.reset();
}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleEmpty::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::LoadMaterials()
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
	_font = std::make_shared<Font>(_engine, L"Resources/Font/GennokakuEnglish.png", gm::Float2(35.0f, 64.0f), 3325.0f);
	// Create image sprite
	StringInfo info = { .String = "Text Sample", .SizePerChar = {0.1f, 0.15f}, .StartPosition = {-0.6f, 0.0f, 1.0f}, .Space = 0.0f, .Color = {1,1,1,1} };
	_text = std::make_shared<Text>(CoordinateType::NDC, _font, info);

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
*  @fn        void SampleEmpty::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::OnKeyboardInput()
{

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::OnMouseInput()
{

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleEmpty::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleText::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function