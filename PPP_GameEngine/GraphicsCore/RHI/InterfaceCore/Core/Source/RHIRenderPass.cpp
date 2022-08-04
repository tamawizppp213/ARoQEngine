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
RHIRenderPass::RHIRenderPass(const std::shared_ptr<RHIDevice>& device, const std::vector<Attachment>& colors, const std::optional<Attachment>& depth)
	: _device(device), _colorAttachments(colors), _depthAttachment(depth),_depthClearValue(ClearValue())
{
	std::uint32_t maxSample = 1;
	for (int i = 0; i < _colorAttachments.size(); ++i)
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_colorAttachments[i].SampleCount));
	}

	if (_depthAttachment.has_value())
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_depthAttachment.value().SampleCount));
	}

	_maxSample = static_cast<core::MultiSample>(maxSample);
}
RHIRenderPass::RHIRenderPass(const std::shared_ptr<RHIDevice>& device, const Attachment& color, const std::optional<Attachment>& depth)
	: _device(device), _colorAttachments(std::vector<Attachment>{color}), _depthAttachment(depth)
{
	std::uint32_t maxSample = 1;
	for (int i = 0; i < _colorAttachments.size(); ++i)
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_colorAttachments[i].SampleCount));
	}

	if (_depthAttachment.has_value())
	{
		maxSample = std::max(maxSample, static_cast<std::uint32_t>(_depthAttachment.value().SampleCount));
	}

	_maxSample = static_cast<core::MultiSample>(maxSample);
}

bool RHIRenderPass::Compatible(const std::shared_ptr<RHIFrameBuffer>& frameBuffer) const
{
	// the number of color attachments should greater than the number of render targets
			// the depth attachment can not be null when the depth stencil is existed.
	if (_colorAttachments.size() < frameBuffer->GetRenderTargetSize()) return false;
	if (!_depthAttachment.has_value() && frameBuffer->GetDepthStencil() != nullptr) return false;

	return true;
}