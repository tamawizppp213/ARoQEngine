//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12BlendState.hpp
///             @brief  DirectX12BlendState.hpp
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_BLEND_STATE_HPP
#define VULKAN_GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUBlendState.hpp"
#include <vulkan/vulkan.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUBlendState : public rhi::core::GPUBlendState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkPipelineColorBlendStateCreateInfo& GetBlendState() const { return _blendDesc; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBlendState() = default;
		~GPUBlendState();
		explicit GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::vector<rhi::core::BlendProperty>& blendProperties);
		explicit GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<VkPipelineColorBlendAttachmentState> _attachments = {};
		VkPipelineColorBlendStateCreateInfo _blendDesc = {};
	};
}
#endif