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
	Check(_device);
	Check(_renderPass);
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
		if (_renderTargets[i]->GetDimension() != ResourceDimension::Texture2D) { throw "Wrong render target dimension"; }
		if (!gu::HasAnyFlags(_renderTargets[i]->GetUsage(), core::TextureCreateFlags::RenderTargetable))
		{ 
			throw "Wrong resource usage"; 
		}
	}

	if (_depthStencil)
	{
		if (_depthStencil->GetDimension() != ResourceDimension::Texture2D ) { throw "Wrong depthStencil dimension"; }
		if (!gu::HasAnyFlags(_depthStencil->GetUsage(), TextureCreateFlags::DepthStencilTargetable)) { throw "Wrong resource usage"; }
	}
}
#pragma endregion Prepare
#pragma region Property
Viewport RHIFrameBuffer::GetFullViewport(const gu::uint32 index) const noexcept
{
	Check(index < _renderTargets.Size());
	return
	{
		0.0f, 0.0f,
		(float)_renderTargets[index]->GetWidth(0),
		(float)_renderTargets[index]->GetHeight(0),
		0.0f, 1.0f
	};
}
ScissorRect RHIFrameBuffer::GetFullScissorRect(const gu::uint32 index) const noexcept
{
	Check(index < _renderTargets.Size());
	return 
	{
		0,0, 
		(long)_renderTargets[index]->GetWidth(0),
		(long)_renderTargets[index]->GetHeight(0)
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

void RHIFrameBuffer::SetRenderTarget(const TexturePtr& texture, const gu::uint32 index)
{
	Check(index < _renderTargets.Size());
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