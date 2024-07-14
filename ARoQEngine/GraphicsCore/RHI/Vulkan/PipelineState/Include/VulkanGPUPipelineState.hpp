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
	****************************************************************************/
	/* @class     GPUGraphicsPipelineState
	*  @brief     Graphics Pipeline State
	*****************************************************************************/
	class GPUGraphicsPipelineState : public rhi::core::GPUGraphicsPipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting(const gu::tstring& name) override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkPipeline GetPipeline() const noexcept { return _pipeline; }

		void SetName(const gu::tstring& name)override;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUGraphicsPipelineState() = default;
		
		~GPUGraphicsPipelineState();
		
		explicit GPUGraphicsPipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHIRenderPass>& renderPass,
			const gu::SharedPointer<core::RHIResourceLayout>& layout);
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		gu::DynamicArray<VkDynamicState> _dynamicStates = {};

		VkPipeline _pipeline = nullptr;

	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	****************************************************************************/
	/* @class     GPUComputePipelineState
	*  @brief     Compute pipeline state
	*****************************************************************************/
	class GPUComputePipelineState : public rhi::core::GPUComputePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting(const gu::tstring& name) override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetName(const gu::tstring& name) override;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUComputePipelineState() = default;
		
		~GPUComputePipelineState() = default;
		
		explicit GPUComputePipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHIResourceLayout>& layout) : core::GPUComputePipelineState(device, layout) {};
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkPipeline _pipeline = nullptr;
	
	};
}
#endif