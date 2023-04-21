//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleCollisionDetection.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleCollisionDetection::SampleCollisionDetection()
{

}
SampleCollisionDetection::~SampleCollisionDetection()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleCollisionDetection::Initialize( const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleCollisionDetection::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::Update()
{
	Scene::Update();
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleCollisionDetection::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::Draw()
{
	_engine->BeginDrawFrame();
	_engine->BeginSwapchainRenderPass();

	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleCollisionDetection::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::Terminate()
{

}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleCollisionDetection::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::LoadMaterials()
{

}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleCollisionDetection::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::OnKeyboardInput()
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
void SampleCollisionDetection::OnMouseInput()
{

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleCollisionDetection::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleCollisionDetection::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleCollisionDetection::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function