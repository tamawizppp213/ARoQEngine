//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RenderPass.hpp
///             @brief  Render pass
///             @author Toide Yutaro
///             @date   2022_08_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RENDER_PASS_HPP
#define VULKAN_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIRenderPass
	*************************************************************************//**
	*  @class     RHIRenderPass
	*  @brief     RenderPass 
	*****************************************************************************/
	class RHIRenderPass : public  rhi::core::RHIRenderPass
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkRenderPass GetRenderPass() const noexcept { return _renderPass; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIRenderPass() = default;
		~RHIRenderPass();
		explicit RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth = std::nullopt);
		explicit RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const core::Attachment& color, const std::optional<core::Attachment>& depth = std::nullopt);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkRenderPass _renderPass = nullptr;
	private:
		void Prepare();
	};
}
#endif