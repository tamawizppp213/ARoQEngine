//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUPipelineFactory.hpp
///             @brief  GPU Pipeline Factory
///             @author Toide Yutaro
///             @date   2022_07_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_PIPELINE_FACTORY_HPP
#define VULKAN_GPU_PIPELINE_FACTORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIPipelineState
	****************************************************************************/
	/* @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUPipelineFactory : public core::GPUPipelineFactory
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		gu::SharedPointer<core::GPUInputAssemblyState> CreateInputAssemblyState(
			const gu::DynamicArray<core::InputLayoutElement>& elements,
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList) override;

		gu::SharedPointer<core::GPURasterizerState> CreateRasterizerState(
			const core::RasterizerProperty& rasterizerProperty) override;

		gu::SharedPointer<core::GPUDepthStencilState> CreateDepthStencilState(
			const core::DepthStencilProperty& depthStencilProperty) override;

		gu::SharedPointer<core::GPUShaderState> CreateShaderState() override;

		gu::SharedPointer<core::GPUBlendState> CreateBlendState(
			const gu::DynamicArray<core::BlendProperty>& properties = { core::BlendProperty() }, 
			const bool alphaToCoverageEnable = false) override;

		gu::SharedPointer<core::GPUBlendState> CreateSingleBlendState(
			const core::BlendProperty& blendProperty = core::BlendProperty(),
			const bool alphaToCoverageEnable = false
		) override;

		virtual gu::SharedPointer<core::GPUBlendState> CreateBlendState(const size_t numRenderTargets, const bool alphaToCoverageEnables = false)override;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUPipelineFactory() = default;

		explicit GPUPipelineFactory(const gu::SharedPointer<core::RHIDevice>& device);

		~GPUPipelineFactory() = default;
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/

	};
}
#endif