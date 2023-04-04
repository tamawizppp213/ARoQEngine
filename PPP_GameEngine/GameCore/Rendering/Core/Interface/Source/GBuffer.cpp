//////////////////////////////////////////////////////////////////////////////////
//              @file   BasePassGBuffer.cpp
///             @brief  GBuffers
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::rendering;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GBuffer::GBuffer(const LowLevelGraphicsEnginePtr& engine, const GBufferDesc& desc, const std::wstring& addName)
	: _engine(engine), _desc(desc)
{
	
}

GBuffer::~GBuffer()
{
	_frameBuffers.clear();
	_frameBuffers.shrink_to_fit();
	Clear();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
void GBuffer::Add(const GameModelPtr& model)
{
	_gameModels.push_back(model);
}

void GBuffer::Clear()
{
	_gameModels.clear();
	_gameModels.shrink_to_fit();
}

void GBuffer::Clear(const GameModelPtr& model)
{
	std::erase(_gameModels, model);
}

GBuffer::TexturePtr GBuffer::GetRenderedTexture(const std::uint32_t index) const noexcept
{
	const auto frameIndex = _engine->GetCurrentFrameIndex();
	const auto frameBuffer = _frameBuffers[frameIndex];
	if (index >= frameBuffer->GetRenderTargets().size()) { return nullptr; }

	return frameBuffer->GetRenderTargets()[index];
}

GBuffer::GPUResourceViewPtr GBuffer::GetRenderedTextureView(const std::uint32_t index) const noexcept
{
	const auto frameIndex = _engine->GetCurrentFrameIndex();
	const auto frameBuffer = _frameBuffers[frameIndex];
	if (index >= frameBuffer->GetRenderTargets().size()) { return nullptr; }

	return frameBuffer->GetRenderTargetSRV(index);
}
#pragma endregion Main Function
