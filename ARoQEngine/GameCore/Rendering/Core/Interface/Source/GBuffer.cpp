//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Core/Interface/Include/GBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GBufferBase::GBufferBase(const LowLevelGraphicsEnginePtr& engine, const GBufferDesc& desc, [[maybe_unused]]const gu::tstring& addName)
	: _engine(engine), _desc(desc)
{
	
}

GBufferBase::~GBufferBase()
{
	_frameBuffers.Clear();
	_frameBuffers.ShrinkToFit();
	Clear();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void GBufferBase::Add(const GameModelPtr& model)
{
	_gameModels.Push(model);
}

void GBufferBase::Clear()
{
	_gameModels.Clear();
	_gameModels.ShrinkToFit();
}

void GBufferBase::Clear(const GameModelPtr& model)
{
	_gameModels.Remove(model);
}

GBufferBase::TexturePtr GBufferBase::GetRenderedTexture(const std::uint32_t index) const noexcept
{
	const auto& frameIndex = _engine->GetCurrentFrameIndex();
	const auto& frameBuffer = _frameBuffers[frameIndex];
	if (index >= frameBuffer->GetRenderTargets().Size()) { return nullptr; }

	return frameBuffer->GetRenderTargets()[index];
}

GBufferBase::GPUResourceViewPtr GBufferBase::GetRenderedTextureView(const std::uint32_t index) const noexcept
{
	const auto& frameIndex = _engine->GetCurrentFrameIndex();
	const auto& frameBuffer = _frameBuffers[frameIndex];
	if (index >= frameBuffer->GetRenderTargets().Size()) { return nullptr; }

	return frameBuffer->GetRenderTargetSRV(index);
}
#pragma endregion Main Function
