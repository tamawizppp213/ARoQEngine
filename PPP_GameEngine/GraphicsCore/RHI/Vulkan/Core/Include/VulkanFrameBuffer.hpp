//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanFrameBuffer.hpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_08_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_FRAME_BUFFER_HPP
#define VULKAN_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             FrameBuffer
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIRenderPass;
}
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     Fence
	*****************************************************************************/
	class RHIFrameBuffer : public rhi::core::RHIFrameBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkFramebuffer GetFrameBuffer() const noexcept { return _frameBuffer; }
		size_t GetWidth () const noexcept { return _width; }
		size_t GetHeight() const noexcept { return _height; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFrameBuffer() = default;
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr);
		explicit RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr);
		~RHIFrameBuffer();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkFramebuffer                  _frameBuffer = nullptr;
		VkImageView                    _depthStencilView = nullptr;
		std::vector<VkImageView>       _renderTargetView = { nullptr };
		std::shared_ptr<core::RHIRenderPass> _renderPass = nullptr;
		size_t _width = 0;
		size_t _height = 0;
	private:
		void Prepare();
	};
}
#endif