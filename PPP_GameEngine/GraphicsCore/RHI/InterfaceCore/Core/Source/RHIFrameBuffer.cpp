//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIFrameBuffer.hpp
///             @brief  Frame Buffer (Render Target and Depth Stencil) 
///             @author Toide Yutaro
///             @date   2022_08_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil)
	: _device(device), _renderTargets(renderTargets), _depthStencil(depthStencil)
{
	Prepare();
}
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil)
	: _device(device), _renderTargets(std::vector<std::shared_ptr<GPUTexture>>{renderTarget}), _depthStencil(depthStencil)
{
	Prepare();
}
#pragma endregion Constructor and Destructor
#pragma region Prepare
void RHIFrameBuffer::Prepare()
{
	for (int i = 0; i < _renderTargets.size(); ++i)
	{
		if (_renderTargets[i] == nullptr) { continue; }
		if (_renderTargets[i]->GetDimension() != ResourceDimension::Dimension2D) { throw std::runtime_error("Wrong render target dimension"); }
		if (_renderTargets[i]->GetUsage() != ResourceUsage::RenderTarget) { throw std::runtime_error("Wrong resource usage"); }
	}

	if (_depthStencil != nullptr)
	{
		if (_depthStencil->GetDimension() != ResourceDimension::Dimension2D) { throw std::runtime_error("Wrong depthStencil dimension"); }
		if (_depthStencil->GetUsage() != ResourceUsage::DepthStencil) { throw std::runtime_error("Wrong resource usage"); }
	}
}
#pragma endregion Prepare
#pragma region Property
Viewport RHIFrameBuffer::GetFullViewport(const size_t index) const noexcept
{
	return
	{
		0.0f, 0.0f,
		(float)_renderTargets[index]->GetWidth(_renderTargets[index]->GetMipMapLevels()),
		(float)_renderTargets[index]->GetHeight(_renderTargets[index]->GetMipMapLevels()),
		0.0f, 1.0f
	};
}
ScissorRect RHIFrameBuffer::GetFullScissorRect(const size_t index) const noexcept
{
	return 
	{
		0,0, 
		(long)_renderTargets[index]->GetWidth(_renderTargets[index]->GetMipMapLevels()),
		(long)_renderTargets[index]->GetHeight(_renderTargets[index]->GetMipMapLevels())
	};
}
#pragma endregion Property