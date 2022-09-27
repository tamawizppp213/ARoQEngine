//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIRenderPass.hpp
///             @brief  Render Pass (for use vulkan)
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RENDER_PASS_HPP
#define RHI_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
#include <optional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIFrameBuffer;
	/****************************************************************************
	*				  			RHIRenderPass
	*************************************************************************//**
	*  @class     RHIRenderPass
	*  @brief     Render pass:  Use before rendering
	*****************************************************************************/
	class RHIRenderPass : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetClearValue(const std::optional<ClearValue>& color, const std::optional<ClearValue>& depth = std::nullopt)
		{
			_colorClearValues = color.has_value() ? std::vector<ClearValue>{color.value()} : std::vector<ClearValue>{};
			_depthClearValue  = depth;
		}
		void SetClearValue(const std::vector  <ClearValue>& colors, const std::optional<ClearValue>& depth = std::nullopt)
		{
			_colorClearValues = colors;
			_depthClearValue  = depth;
		}
		bool Compatible(const std::shared_ptr<RHIFrameBuffer>& frameBuffer) const;
		
		/* @brief : Return multi sample count*/
		MultiSample GetMaxSample() const noexcept { return _maxSample; }
		/* @brief : Return clear color value*/
		const std::vector<ClearValue>& GetClearColor() const noexcept { return _colorClearValues; }
		/* @brief : Return clear depth color value*/
		std::optional<ClearValue> GetDepthClear() const noexcept { return _depthClearValue; }
		/* @brief : Return Get color attachment*/
		std::optional<Attachment> GetColorAttachment(const size_t index = 0) const { return _colorAttachments[index]; }
		/* @brief : Return Get Depth attachment*/
		std::optional<Attachment> GetDepthAttachment() const noexcept { return _depthAttachment; }
		/* @brief : Return color attachment size*/
		size_t GetColorAttachmentSize() const noexcept { return _colorAttachments.size(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIRenderPass() = default;
		virtual ~RHIRenderPass()
		{
			_depthClearValue.reset();
			_depthAttachment.reset();
			_colorClearValues.clear(); _colorClearValues.shrink_to_fit();
			_colorAttachments.clear(); _colorAttachments.shrink_to_fit();
		}
		explicit RHIRenderPass(const std::shared_ptr<RHIDevice>& device, const std::vector<Attachment>& colors, const std::optional<Attachment>& depth = std::nullopt);
		explicit RHIRenderPass(const std::shared_ptr<RHIDevice>& device, const Attachment& color, const std::optional<Attachment>& depth = std::nullopt);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;

		std::vector<Attachment>    _colorAttachments = {};
		std::vector<ClearValue>    _colorClearValues = {};

		std::optional<Attachment> _depthAttachment;
		std::optional<ClearValue> _depthClearValue;

		MultiSample _maxSample = MultiSample::Count1;
	};
}
#endif