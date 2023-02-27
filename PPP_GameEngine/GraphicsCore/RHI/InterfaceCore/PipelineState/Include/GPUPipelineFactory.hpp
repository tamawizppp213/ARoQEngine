//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUPipelineFactory.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_PIPELINE_FACTORY_HPP
#define GPU_PIPELINE_FACTORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class GPUInputAssemblyState;
	class GPURasterizerState;
	class GPUDepthStencilState;
	class GPUShaderState;
	class GPUBlendState;
	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUPipelineFactory : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual std::shared_ptr<GPUInputAssemblyState> CreateInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList) = 0;

		virtual std::shared_ptr<GPURasterizerState> CreateRasterizerState(
			const core::RasterizerProperty& rasterizerProperty) = 0;

		virtual std::shared_ptr<GPUDepthStencilState> CreateDepthStencilState(
			const core::DepthStencilProperty& depthStencilProperty = core::DepthStencilProperty()) = 0;

		virtual std::shared_ptr<GPUShaderState> CreateShaderState() = 0;

		virtual std::shared_ptr<GPUBlendState> CreateBlendState(
			const std::vector<BlendProperty>& properties = { BlendProperty() }) = 0;

		virtual std::shared_ptr<GPUBlendState> CreateSingleBlendState(
			const BlendProperty& blendProperty = BlendProperty()
		) = 0;

		virtual std::shared_ptr<GPUBlendState> CreateBlendState(const size_t numRenderTargets) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		virtual ~GPUPipelineFactory() = default;
		
		explicit GPUPipelineFactory(const std::shared_ptr<RHIDevice>& device) : _device(device) {};
		
		GPUPipelineFactory() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;

	};
}
#endif