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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
#include <stdexcept>
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	core::DescriptorHeapType Convert(core::ResourceViewType type)
	{
		switch (type)
		{
			case core::ResourceViewType::Buffer: 
			case core::ResourceViewType::ConstantBuffer: return core::DescriptorHeapType::CBV;
			case core::ResourceViewType::Texture: return core::DescriptorHeapType::SRV;
			case core::ResourceViewType::RWBuffer:
			case core::ResourceViewType::RWStructuredBuffer:
			case core::ResourceViewType::RWTexture: return core::DescriptorHeapType::UAV;
			case core::ResourceViewType::Sampler  : return core::DescriptorHeapType::SAMPLER;
			default:
			{
				throw std::runtime_error("failed to convert descriptor heap type");
			}
		}
	}
}
#pragma region Constructor and Destructor
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, customHeap)
{
	_buffer  = buffer;
	_texture = nullptr;
	SelectDescriptorHeap(type);
	CreateView();
	
}
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	:core::GPUResourceView(device, type, customHeap)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);

	_buffer  = nullptr;
	_texture = texture;

	SelectDescriptorHeap(type);
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
#pragma region Bind Function
void GPUResourceView::Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index)
{
	assert(commandList->GetType() == core::CommandListType::Graphics);

	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkHeap   = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_heap);
	const auto vkDescriptorSet = vkHeap->GetDescriptorSet(_heapOffset);
	
	/*-------------------------------------------------------------------
	-               Set up write descriptor
	---------------------------------------------------------------------*/
	VkWriteDescriptorSet writeDesc = {};
	writeDesc.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writeDesc.descriptorCount = 1;
	writeDesc.pNext           = nullptr;
	writeDesc.dstArrayElement = 0;
	writeDesc.dstBinding      = index;
	writeDesc.dstSet          = vkDescriptorSet;

	if (_imageView)
	{
		const auto vkTexture = std::static_pointer_cast<vulkan::GPUTexture>(_texture);

		/*-------------------------------------------------------------------
		-               Set up Image info
		---------------------------------------------------------------------*/
		VkDescriptorImageInfo imageInfo = {};
		imageInfo.imageView   = _imageView;
		imageInfo.sampler     = nullptr; //必要になりそう
		imageInfo.imageLayout = EnumConverter::Convert(vkTexture->GetResourceState());
		
		writeDesc.pImageInfo     = &imageInfo;
		writeDesc.descriptorType = EnumConverter::Convert(Convert(_resourceViewType));
	}
	else if (_bufferView)
	{
		const auto vkBuffer = std::static_pointer_cast<vulkan::GPUBuffer>(_buffer);
		/*-------------------------------------------------------------------
		-               Set up Buffer info
		---------------------------------------------------------------------*/
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = vkBuffer->GetBuffer();
		bufferInfo.range  = vkBuffer->GetTotalByteSize();
		bufferInfo.offset = 0;
		
		writeDesc.pBufferInfo = &bufferInfo;
		writeDesc.descriptorType = EnumConverter::Convert(Convert(_resourceViewType)); // UAVもくるから分からん
	}
	else
	{
		throw std::runtime_error("failed to bind resource");
	}
	/*-------------------------------------------------------------------
	-                   Update
	---------------------------------------------------------------------*/
	vkUpdateDescriptorSets(vkDevice->GetDevice(), 1, &writeDesc, 0, nullptr);
}
#pragma endregion Bind Function
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
	imageViewCreateInfo.sType      = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.format     = EnumConverter::Convert(vkImage->GetPixelFormat());
	imageViewCreateInfo.viewType   = EnumConverter::Convert(vkImage->GetResourceType());
	imageViewCreateInfo.image      = vkImage->GetImage();
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

/****************************************************************************
*                     SelectDescriptorHeap
*************************************************************************//**
*  @fn        const std::shared_ptr<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
*  @brief     Select DirectX12 Descriptor Heap. return custom heap or default heap
*  @param[in] const core::DescriptorHeapType type
*  @return 　　const std::shared_ptr<directX12::RHIDescriptorHeap>
*****************************************************************************/
const std::shared_ptr<vulkan::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
{
	// Set custom Heap
	if (_heap) { return std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_heap); }

	// Select default heap based on core::ResourceViewType
	std::shared_ptr<vulkan::RHIDescriptorHeap> defaultHeap = nullptr;
	switch (type)
	{
		case core::ResourceViewType::ConstantBuffer: { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::CBV)); break; }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer: { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break; }
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::UAV)); break; }
		case core::ResourceViewType::RenderTarget:          { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::RTV)); break; }
		case core::ResourceViewType::DepthStencil:          { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::DSV)); break; }
		case core::ResourceViewType::AccelerationStructure: { defaultHeap = std::static_pointer_cast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break; }
		default:
		{
			throw std::runtime_error("not support descriptor view. (vulkan api)");
		}
	}
	_heap = defaultHeap;

	return defaultHeap;
}
#pragma endregion Prepare View Function