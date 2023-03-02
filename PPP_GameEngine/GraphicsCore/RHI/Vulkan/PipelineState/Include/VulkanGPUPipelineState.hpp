//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUPipelineState.hpp
///             @brief  PipelineState.hpp
///             @author Toide Yutaro
///             @date   2022_08_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_PIPELINE_STATE_HPP
#define VULKAN_GPU_PIPELINE_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
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
	*				  			GPUGraphicsPipelineState
	*************************************************************************//**
	*  @class     GPUGraphicsPipelineState
	*  @brief     Graphics Pipeline State
	*****************************************************************************/
	class GPUGraphicsPipelineState : public rhi::core::GPUGraphicsPipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkPipeline GetPipeline() const noexcept { return _pipeline; }

		void SetName(const std::wstring& name)override {}
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUGraphicsPipelineState() = default;
		
		~GPUGraphicsPipelineState();
		
		explicit GPUGraphicsPipelineState(
			const std::shared_ptr<core::RHIDevice>& device,
			const std::shared_ptr<core::RHIRenderPass>& renderPass,
			const std::shared_ptr<core::RHIResourceLayout>& layout);
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<VkDynamicState> _dynamicStates = {};
		VkPipeline _pipeline = nullptr;

	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	*************************************************************************//**
	*  @class     GPUComputePipelineState
	*  @brief     Compute pipeline state
	*****************************************************************************/
	class GPUComputePipelineState : public rhi::core::GPUComputePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting() override{};

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetName(const std::wstring& name) const override {};
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUComputePipelineState() = default;
		
		~GPUComputePipelineState() = default;
		
		explicit GPUComputePipelineState(
			const std::shared_ptr<core::RHIDevice>& device,
			const std::shared_ptr<core::RHIResourceLayout>& layout = nullptr) : core::GPUComputePipelineState(device, layout) {};
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	
	};
}
#endif