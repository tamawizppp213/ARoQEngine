//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleRectangle.hpp"
#include "GameCore/Rendering/Sprite/Include/UIRenderer.hpp"
#include "GameCore/Rendering/Sprite/Include/Image.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleRectangle::SampleRectangle()
{

}
SampleRectangle::~SampleRectangle()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleRectangle::Initialize(GameTimer* gameTimer)
*  @brief     Initialize scene
*  @param[in] GameTimer* gameTimer
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, GameTimer* gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleRectangle::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Update()
{
	Scene::Update();
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleRectangle::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Draw()
{
	/*-------------------------------------------------------------------
	-             Start frame
	---------------------------------------------------------------------*/
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	/*-------------------------------------------------------------------
	-             End frame
	---------------------------------------------------------------------*/
	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleRectangle::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::Terminate()
{

}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleRectangle::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::LoadMaterials()
{

}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnKeyboardInput()
{

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnMouseInput()
{

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleRectangle::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleRectangle::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleRectangle::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function