//////////////////////////////////////////////////////////////////////////////////
///  @file   VulkanGPUDepthStencilState.hpp
///  @brief  ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_DEPTH_STENICIL_STATE_HPP
#define VULKAN_GPU_DEPTH_STENICIL_STATE_HPP

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
	****************************************************************************/
	/* @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUDepthStencilState : public rhi::core::GPUDepthStencilState
	{
	public:
#pragma region Public Function
#pragma endregion

		/****************************************************************************
		**                Public Property
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
		**                Protected Property
		*****************************************************************************/
		VkPipelineDepthStencilStateCreateInfo _depthStencilDesc = {};
	};
}
#endif