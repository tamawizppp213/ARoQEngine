//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUInputAssemblyState.hpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_INPUT_ASSEMBLY_STATE_HPP
#define VULKAN_GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUInputAssemblyState.hpp"
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
	*				  			GPUBlendState
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUInputAssemblyState : public rhi::core::GPUInputAssemblyState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkPipelineInputAssemblyStateCreateInfo& GetInputAssembly() const { return _inputAssembly; }
		
		const VkPipelineVertexInputStateCreateInfo&   GetVertexInput  () const { return _vertexInput; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUInputAssemblyState() = default;
		
		~GPUInputAssemblyState() = default;
		
		explicit GPUInputAssemblyState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::DynamicArray<core::InputLayoutElement>& elements,
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Set the vertex attribute information contained in each vertex stream.*/
		gu::DynamicArray<VkVertexInputAttributeDescription> _vertexAttributes = {};

		/* @brief : how each vertex stream is to be read*/
		gu::DynamicArray<VkVertexInputBindingDescription>   _vertexBinding    = {};

		VkPipelineInputAssemblyStateCreateInfo         _inputAssembly    = {};
		VkPipelineVertexInputStateCreateInfo           _vertexInput      = {};
	};
}
#endif