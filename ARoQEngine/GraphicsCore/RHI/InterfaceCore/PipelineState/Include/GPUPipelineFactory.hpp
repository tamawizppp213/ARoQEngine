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
#include "../../Core/Include/RHICommonState.hpp"
#include "../../Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	class GPUPipelineFactory : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Create and return input assembly state pointer*/
		virtual gu::SharedPointer<GPUInputAssemblyState> CreateInputAssemblyState(
			const gu::DynamicArray<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList) = 0;

		/* @brief : Create and return rasterizer state pointer*/
		virtual gu::SharedPointer<GPURasterizerState> CreateRasterizerState(
			const core::RasterizerProperty& rasterizerProperty) = 0;

		/* @brief : Create and return depth stencil state pointer*/
		virtual gu::SharedPointer<GPUDepthStencilState> CreateDepthStencilState(
			const core::DepthStencilProperty& depthStencilProperty = core::DepthStencilProperty()) = 0;

		/* @brief : Create and return shader state pointer*/
		virtual gu::SharedPointer<GPUShaderState> CreateShaderState() = 0;

		/* @brief : Create and return multiple blend states pointer. */
		virtual gu::SharedPointer<GPUBlendState> CreateBlendState(
			const gu::DynamicArray<BlendProperty>& properties = { BlendProperty() }, 
			const bool alphaToCoverageEnable = false) = 0;

		/* @brief : Create and return blend state pointer*/
		virtual gu::SharedPointer<GPUBlendState> CreateSingleBlendState(
			const BlendProperty& blendProperty = BlendProperty(),
			const bool alphaToCoverageEnable = false
		) = 0;

		/* @brief : Create and return input assembly state pointer*/
		virtual gu::SharedPointer<GPUBlendState> CreateBlendState(const size_t numRenderTargets, const bool alphaToCoverageEnable) = 0;
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
		virtual ~GPUPipelineFactory() { if (_device) { _device.Reset(); } }
		
		explicit GPUPipelineFactory(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		GPUPipelineFactory() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

	};
}
#endif