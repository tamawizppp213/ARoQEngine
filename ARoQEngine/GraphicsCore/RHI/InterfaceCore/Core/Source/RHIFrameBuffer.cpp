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
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RenderPass.hpp"
#include <stdexcept>
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil)
	: _device(device), _renderPass(renderPass), _renderTargets(renderTargets), _depthStencil(depthStencil)
{
	assert(_device);
	assert(_renderPass);
	CheckResourceFormat();
}

RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil)
	: _device(device), _renderPass(renderPass), _renderTargets(gu::DynamicArray<gu::SharedPointer<GPUTexture>>{renderTarget}), _depthStencil(depthStencil)
{
	CheckResourceFormat();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	if (_depthStencilSRV) { _depthStencilSRV.Reset(); }
	if (_depthStencilView){ _depthStencilView.Reset();}
	if (_depthStencil) { _depthStencil.Reset(); }

	_renderTargetViews.Clear(); _renderTargetViews.ShrinkToFit();
	_renderTargetUAVs.Clear(); _renderTargetUAVs.ShrinkToFit();
	_renderTargetSRVs.Clear(); _renderTargetSRVs.ShrinkToFit();
	_renderTargets.Clear(); _renderTargets.ShrinkToFit();

	if (_renderPass) { _renderPass.Reset(); }
	if (_device) { _device.Reset(); }
}

#pragma endregion Constructor and Destructor
#pragma region Prepare
void RHIFrameBuffer::CheckResourceFormat()
{
	for (int i = 0; i < _renderTargets.Size(); ++i)
	{
		if (!_renderTargets[i]) { continue; }
		if (_renderTargets[i]->GetDimension() != ResourceDimension::Dimension2D) { throw std::runtime_error("Wrong render target dimension"); }
		if (!gu::HasAnyFlags(_renderTargets[i]->GetUsage(), core::ResourceUsage::RenderTarget))
		{ 
			throw std::runtime_error("Wrong resource usage"); 
		}
	}

	if (_depthStencil)
	{
		if (_depthStencil->GetDimension() != ResourceDimension::Dimension2D ) { throw std::runtime_error("Wrong depthStencil dimension"); }
		if (!gu::HasAnyFlags(_depthStencil->GetUsage(), ResourceUsage::DepthStencil)) { throw std::runtime_error("Wrong resource usage"); }
	}
}
#pragma endregion Prepare
#pragma region Property
Viewport RHIFrameBuffer::GetFullViewport(const size_t index) const noexcept
{
	assert(index < _renderTargets.Size());
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
	assert(index < _renderTargets.Size());
	return 
	{
		0,0, 
		(long)_renderTargets[index]->GetWidth(_renderTargets[index]->GetMipMapLevels()),
		(long)_renderTargets[index]->GetHeight(_renderTargets[index]->GetMipMapLevels())
	};
}

void RHIFrameBuffer::SetRenderTargets(const gu::DynamicArray<TexturePtr>& textures)
{
	_renderTargets = textures;
	for (size_t i = 0; i < _renderTargets.Size(); ++i)
	{
		_renderTargetViews[i]->SetTexture(textures[i]);
		_renderTargetSRVs[i]->SetTexture(textures[i]);
		_renderTargetUAVs[i]->SetTexture(textures[i]);
	}
}

void RHIFrameBuffer::SetRenderTarget(const TexturePtr& texture, const size_t index)
{
	assert(index < _renderTargets.Size());
	_renderTargetViews[index]->SetTexture(texture);
	_renderTargetSRVs[index]->SetTexture(texture);
	_renderTargetUAVs[index]->SetTexture(texture);
}

void RHIFrameBuffer::SetDepthStencil(const TexturePtr& texture)
{
	_depthStencil = texture;
	_depthStencilView->SetTexture(texture);
	_depthStencilSRV ->SetTexture(texture);
}
#pragma endregion Property