//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanRenderPass.hpp
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
//                               Class
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

		gu::DynamicArray<VkClearValue> GetVkClearValues() const;

		void SetName(const gu::tstring& name) override;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIRenderPass() = default;

		~RHIRenderPass();
		
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth = {});
		
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const core::Attachment& color, const gu::Optional<core::Attachment>& depth = {});
	
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