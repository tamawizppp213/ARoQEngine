//////////////////////////////////////////////////////////////////////////////////
///             @file   GameWorldInfo.hpp
///             @brief  Game object information to exist in the game world. (World position etc...)
///             @author Toide Yutaro
///             @date   2023_02_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/GameWorldInfo.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc::core;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GameWorldInfo::GameWorldInfo(const LowLevelGraphicsEnginePtr& engine, const std::uint64_t maxInstanceCount)
	: _engine(engine), _maxInstanceCount(maxInstanceCount)
{
#ifdef _DEBUG
	Check(engine);
	Check(maxInstanceCount > 0);
#endif

	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Prepare constant and upload buffer
	---------------------------------------------------------------------*/
	const GameWorldConstant world = { .World = gm::Float4x4()};
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(GameWorldConstant), maxInstanceCount, MemoryHeap::Upload, ResourceState::Common, (void*)&world);
	
	_gameWorldConstants = device->CreateBuffer(bufferInfo);
	_resourceView       = device->CreateResourceView(ResourceViewType::ConstantBuffer, _gameWorldConstants);

}

GameWorldInfo::~GameWorldInfo()
{
	_resourceView.Reset();
	_gameWorldConstants.Reset();
	_engine.Reset();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
void GameWorldInfo::Bind(const GraphicsCommandListPtr& commandList, const std::uint32_t index)
{
	_resourceView->Bind(commandList, index);
}
#pragma endregion Main Function