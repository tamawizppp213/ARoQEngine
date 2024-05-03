//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUPipelineFactory.cpp
///             @brief  Pipeline each stage creator
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_PIPELINE_FACTORY_HPP
#define DIRECTX12_PIPELINE_FACTORY_HPP

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
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUPipelineFactory : public core::GPUPipelineFactory
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Create and return input assembly state pointer*/
		gu::SharedPointer<core::GPUInputAssemblyState> CreateInputAssemblyState(
			const gu::DynamicArray<core::InputLayoutElement>& elements,
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList) override;

		/* @brief : Create and return rasterizer state pointer*/
		gu::SharedPointer<core::GPURasterizerState> CreateRasterizerState(
			const core::RasterizerProperty& rasterizerProperty) override;

		/* @brief : Create and return depth stencil state pointer*/
		gu::SharedPointer<core::GPUDepthStencilState> CreateDepthStencilState(
			const core::DepthStencilProperty& depthStencilProperty) override;

		/* @brief : Create and return shader state pointer*/
		gu::SharedPointer<core::GPUShaderState> CreateShaderState() override;

		/* @brief : Create and return multiple blend states pointer. */
		gu::SharedPointer<core::GPUBlendState> CreateBlendState(
			const gu::DynamicArray<core::BlendProperty>& properties,
			const bool alphaToCoverageEnable = false) override;

		/* @brief : Create and return blend state pointer*/
		gu::SharedPointer<core::GPUBlendState> CreateSingleBlendState(
			const core::BlendProperty& blendProperty,
			const bool alphaToCoverageEnable
		) override ;

		/* @brief : Create and return input assembly state pointer*/
		gu::SharedPointer<core::GPUBlendState> CreateBlendState(const size_t numRenderTargets, const bool alphaToCoverageEnable = false)override;
		/****************************************************************************
		**                Public Member Variables
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
		**                Protected Member Variables
		*****************************************************************************/

	};
}
#endif