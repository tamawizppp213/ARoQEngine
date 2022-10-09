//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUBuffer.cpp
///             @brief  Buffer
///             @author Toide Yutaro
///             @date   2022_08_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUResourceView.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Prepare View Function 
void GPUResourceView::CreateView()
{

}
void GPUResourceView::CreateImageView()
{
	const auto vkDevice = static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkImage  = static_pointer_cast<vulkan::GPUTexture>(_texture);

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.format   = EnumConverter::Convert(vkImage->GetPixelFormat());
	imageViewCreateInfo.viewType = EnumConverter::Convert(vkImage->GetResourceType());
	imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount     = static_cast<std::uint32_t>(vkImage->GetArrayLength());
	imageViewCreateInfo.subresourceRange.levelCount     = static_cast<std::uint32_t>(vkImage->GetMipMapLevels());
	imageViewCreateInfo.subresourceRange.aspectMask     = 0;

	if (imageViewCreateInfo.subresourceRange.aspectMask & (VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT | VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT))
	{
		if (true)
		{

		}
		else
		{

		}
	}

	if (vkCreateImageView(vkDevice->GetDevice(), &imageViewCreateInfo, nullptr, &_imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer view (vulkan api)");
	}
}
void GPUResourceView::CreateBufferView()
{
	const auto vkDevice = static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkBuffer = static_pointer_cast<vulkan::GPUBuffer>(_buffer);

	VkBufferViewCreateInfo bufferViewCreateInfo = {};
	bufferViewCreateInfo.buffer = vkBuffer->GetBuffer();
	bufferViewCreateInfo.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
	bufferViewCreateInfo.pNext  = nullptr;
	bufferViewCreateInfo.flags  = 0;
	bufferViewCreateInfo.format = VK_FORMAT_UNDEFINED;
	bufferViewCreateInfo.offset = 0;
	bufferViewCreateInfo.range  = vkBuffer->GetTotalByteSize();

	if (vkCreateBufferView(vkDevice->GetDevice(), &bufferViewCreateInfo, nullptr, &_bufferView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer view (vulkan api)");
	}
}
#pragma endregion Prepare View Function