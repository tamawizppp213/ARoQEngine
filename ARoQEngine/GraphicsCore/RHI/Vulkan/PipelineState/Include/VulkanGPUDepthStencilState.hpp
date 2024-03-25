//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPURasterizerState.hpp
///             @brief  DirectX12GPURasterizerState.hpp
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RASTERIZER_STATE_HPP
#define DIRECTX12_GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUDepthStencilState.hpp"
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
	class GPUDepthStencilState : public rhi::core::GPUDepthStencilState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkPipelineDepthStencilStateCreateInfo& GetDepthStencilState() const noexcept { return _depthStencilDesc; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::DepthStencilProperty& depthStencilProperty
		);

		~GPUDepthStencilState() = default;

		GPUDepthStencilState()  = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkPipelineDepthStencilStateCreateInfo _depthStencilDesc = {};
	};
}
#endif