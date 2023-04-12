//////////////////////////////////////////////////////////////////////////////////
//              @file   a.cpp
///             @brief  a
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/CascadeShadow.hpp"
#include "../../Model/Include/GameModel.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace gc::rendering;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
CascadeShadow::CascadeShadow(const LowLevelGraphicsEnginePtr& engine, const CascadeShadowDesc& desc)
{

}

CascadeShadow::~CascadeShadow()
{

}

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void CascadeShadow::Draw()
* 
*  @brief     Draw shadow map and draw shadow
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void CascadeShadow::Draw()
{
	/*-------------------------------------------------------------------
	-               Prepare the variables
	---------------------------------------------------------------------*/
	const auto device      = _engine->GetDevice();
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics);

	/*-------------------------------------------------------------------
	-               Draw shadow maps of the each resolution
	---------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-               Execute cascade shadow
	---------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-               Execute gaussian blur
	---------------------------------------------------------------------*/

}

/****************************************************************************
*							Add
*************************************************************************//**
*  @fn        void CascadeShadow::Add(const GameModelPtr& gameModel)
*
*  @brief     Add game models for the rendering shadow
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void CascadeShadow::Add(const GameModelPtr& gameModel)
{
	if (!gameModel) { return; }

	_gameModels.push_back(gameModel);
}
#pragma endregion Main Function

#pragma region SetUp Function

#pragma endregion SetUp Function