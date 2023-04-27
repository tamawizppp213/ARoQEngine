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
		std::shared_ptr<core::GPUInputAssemblyState> CreateInputAssemblyState(
			const std::vector<core::InputLayoutElement>& elements,
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList) override;

		/* @brief : Create and return rasterizer state pointer*/
		std::shared_ptr<core::GPURasterizerState> CreateRasterizerState(
			const core::RasterizerProperty& rasterizerProperty) override;

		/* @brief : Create and return depth stencil state pointer*/
		std::shared_ptr<core::GPUDepthStencilState> CreateDepthStencilState(
			const core::DepthStencilProperty& depthStencilProperty) override;

		/* @brief : Create and return shader state pointer*/
		std::shared_ptr<core::GPUShaderState> CreateShaderState() override;

		/* @brief : Create and return multiple blend states pointer. */
		std::shared_ptr<core::GPUBlendState> CreateBlendState(
			const std::vector<core::BlendProperty>& properties) override;

		/* @brief : Create and return blend state pointer*/
		std::shared_ptr<core::GPUBlendState> CreateSingleBlendState(
			const core::BlendProperty& blendProperty
		) override ;

		/* @brief : Create and return input assembly state pointer*/
		std::shared_ptr<core::GPUBlendState> CreateBlendState(const size_t numRenderTargets)override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUPipelineFactory() = default;

		explicit GPUPipelineFactory(const std::shared_ptr<core::RHIDevice>& device);

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