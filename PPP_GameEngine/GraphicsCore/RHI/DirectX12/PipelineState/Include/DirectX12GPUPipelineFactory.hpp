//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
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
		std::shared_ptr<core::GPUInputAssemblyState> CreateInputAssemblyState(
			const std::vector<core::InputLayoutElement>& elements,
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList) override;

		std::shared_ptr<core::GPURasterizerState> CreateRasterizerState(
			const core::FrontFace   frontFace   = core::FrontFace::Clockwise,
			const core::CullingMode cullingMode = core::CullingMode::None,
			const core::FillMode    fillMode    = core::FillMode::Solid,
			const bool depthClamp = true) override;

		std::shared_ptr<core::GPUDepthStencilState> CreateDepthStencilState(
			const bool                 depthEnable = true,
			const bool                 depthWriteEnable = true,
			const bool                 stencilEnable = false,
			const core::CompareOperator      depthOperator = core::CompareOperator::LessEqual,
			const core::StencilOperatorInfo& front = core::StencilOperatorInfo(),
			const core::StencilOperatorInfo& back  = core::StencilOperatorInfo()) override;

		std::shared_ptr<core::GPUShaderState> CreateShaderState() override;

		std::shared_ptr<core::GPUBlendState> CreateBlendState(
			const std::vector<core::BlendProperty>& properties) override;

		std::shared_ptr<core::GPUBlendState> CreateSingleBlendState(
			const core::BlendProperty& blendProperty
		) override ;

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