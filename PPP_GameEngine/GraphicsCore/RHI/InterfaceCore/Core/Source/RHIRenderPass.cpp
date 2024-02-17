//////////////////////////////////////////////////////////////////////////////////
///             @file   GPURenderPass.hpp
///             @brief  Render pass (root signature) 
///             @author Toide Yutaro
///             @date   2022_08_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIRenderPass::RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const gu::DynamicArray<Attachment>& colors, const std::optional<Attachment>& depth)
	: _device(device), _colorAttachments(colors), _depthAttachment(depth), _depthClearValue(ClearValue())
{
	_colorClearValues = gu::DynamicArray<ClearValue>(colors.Size(), ClearValue());

	std::uint32_t maxSample = 1;
	for (int i = 0; i < _colorAttachments.Size(); ++i)
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_colorAttachments[i].SampleCount));
	}

	if (_depthAttachment.has_value())
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_depthAttachment.value().SampleCount));
	}

	_maxSample = static_cast<core::MultiSample>(maxSample);
}
RHIRenderPass::RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const Attachment& color, const std::optional<Attachment>& depth)
	: _device(device), _colorAttachments(gu::DynamicArray<Attachment>{color}), _depthAttachment(depth)
{
	_colorClearValues = gu::DynamicArray<ClearValue>(1, ClearValue());

	std::uint32_t maxSample = 1;
	for (int i = 0; i < _colorAttachments.Size(); ++i)
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_colorAttachments[i].SampleCount));
	}

	if (_depthAttachment.has_value())
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_depthAttachment.value().SampleCount));
	}

	_maxSample = static_cast<core::MultiSample>(maxSample);
}

bool RHIRenderPass::Compatible(const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) const
{
	// the number of color attachments should greater than the number of render targets
			// the depth attachment can not be null when the depth stencil is existed.
	if (_colorAttachments.Size() < frameBuffer->GetRenderTargetSize()) return false;
	if (!_depthAttachment.has_value() && frameBuffer->GetDepthStencil()) return false;

	return true;
}