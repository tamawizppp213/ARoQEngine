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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDescriptorHeap.hpp"
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
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, customHeap)
{
	_buffer  = buffer;
	_texture = nullptr;
	CreateView();
	
}
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	:core::GPUResourceView(device, type, customHeap)
{
	_buffer  = nullptr;
	_texture = texture;
	CreateView();
}

GPUResourceView::~GPUResourceView()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	if (_bufferView)
	{
		vkDestroyBufferView(vkDevice, _bufferView, nullptr);
	}
	if (_imageView)
	{
		vkDestroyImageView(vkDevice, _imageView, nullptr);
	}
}
#pragma endregion Constructor and Destructor

#pragma region Prepare View Function 
void GPUResourceView::CreateView()
{
	switch (_resourceViewType)
	{
		case core::ResourceViewType::ConstantBuffer:
		case core::ResourceViewType::StructuredBuffer:
		case core::ResourceViewType::RWStructuredBuffer:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::RWBuffer:
		{
			CreateBufferView();
			break;
		}
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RenderTarget:
		case core::ResourceViewType::DepthStencil:
		{
			CreateImageView();
			break;
		}
		case core::ResourceViewType::AccelerationStructure:
		{

		}
		default:
		{
			throw std::runtime_error("not support descriptor view. (directX12 api)");
		}
	}
}
void GPUResourceView::CreateImageView()
{
	const auto vkDevice = static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkImage  = static_pointer_cast<vulkan::GPUTexture>(_texture);

	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.format   = EnumConverter::Convert(vkImage->GetPixelFormat());
	imageViewCreateInfo.viewType = EnumConverter::Convert(vkImage->GetResourceType());
	imageViewCreateInfo.image    = vkImage->GetImage();
	imageViewCreateInfo.components = VkComponentMapping(VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A);
	imageViewCreateInfo.subresourceRange.baseMipLevel   = 0;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	imageViewCreateInfo.subresourceRange.layerCount     = static_cast<std::uint32_t>(vkImage->GetArrayLength());
	imageViewCreateInfo.subresourceRange.levelCount     = static_cast<std::uint32_t>(vkImage->GetMipMapLevels());
	imageViewCreateInfo.subresourceRange.aspectMask     = GetImageAspectFlags(imageViewCreateInfo.format);

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

VkImageAspectFlags GPUResourceView::GetImageAspectFlags(VkFormat format)
{
	switch (format)
	{
		case VkFormat::VK_FORMAT_D32_SFLOAT_S8_UINT:
		case VkFormat::VK_FORMAT_D24_UNORM_S8_UINT:
		case VkFormat::VK_FORMAT_D16_UNORM_S8_UINT:
			return VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT | VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT;
		case VkFormat::VK_FORMAT_D16_UNORM:
		case VkFormat::VK_FORMAT_D32_SFLOAT:
		case VkFormat::VK_FORMAT_X8_D24_UNORM_PACK32:
			return VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT;
		case VkFormat::VK_FORMAT_S8_UINT:
			return VkImageAspectFlagBits::VK_IMAGE_ASPECT_STENCIL_BIT;
		default:
			return VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
		
	}
}
#pragma endregion Prepare View Function