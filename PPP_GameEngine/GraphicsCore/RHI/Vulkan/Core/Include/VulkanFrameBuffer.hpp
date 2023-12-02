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
		
		VkExtent2D    GetExtent2D() const noexcept { return VkExtent2D(static_cast<std::uint32_t>(_width), static_cast<std::uint32_t>(_height)); }
		
		size_t GetWidth () const noexcept { return _width; }
		
		size_t GetHeight() const noexcept { return _height; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIFrameBuffer() = default;
		
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass,  const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr);
		
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>&renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr);
		
		~RHIFrameBuffer();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkFramebuffer _frameBuffer = nullptr;
		size_t _width  = 0;
		size_t _height = 0;
	private:
		void Prepare();
	};
}
#endif