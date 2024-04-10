//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUBuffer.cpp
///             @brief  Buffer
///             @author Toide Yutaro
///             @date   2022_08_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUResourceView.hpp"
#include "../Include/VulkanGPUBuffer.hpp"
#include "../Include/VulkanGPUTexture.hpp"
#include "../../Core/Include/VulkanDescriptorHeap.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include "../../Core/Include/VulkanDevice.hpp"
#include "../../Core/Include/VulkanAdapter.hpp"
#include "../../Core/Include/VulkanCommandList.hpp"
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
GPUResourceView::GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUBuffer>& buffer, 
	const gu::uint32 mipSlice, const gu::uint32 planeSlice,
	const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, mipSlice, planeSlice, customHeap)
{
	_buffer  = buffer;
	_texture = nullptr;
	SelectDescriptorHeap(type);
	CreateView();
	
}
GPUResourceView::GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUTexture>& texture, 
	const gu::uint32 mipSlice, const gu::uint32 planeSlice,
	const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
	:core::GPUResourceView(device, type, mipSlice, planeSlice, customHeap)
{
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device);

	_buffer  = nullptr;
	_texture = texture;

	SelectDescriptorHeap(type);
	CreateView();
}

GPUResourceView::~GPUResourceView()
{
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
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
/****************************************************************************
*                     Bind
*************************************************************************//**
*  @fn        void GPUResourceView::Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const std::uint32_t index)
*
*  @brief     Bind resource layout array index to the command list.
			  index : resource layout array index

*  @param[in] const gu::SharedPointer<core::RHICommandList>& commandList pointer
*  @param[in] const std::uint32_t resource layout array index.
*
*  @return 　　void
*****************************************************************************/
void GPUResourceView::Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const std::uint32_t index, const gu::SharedPointer<core::RHIResourceLayout>& layout)
{
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	const auto vkHeap   = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_heap);

	if (_heapOffset == INVALID_ID)
	{
		_heapOffset = static_cast<int>(vkHeap->Allocate(_heapType, layout));
	}
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
		const auto vkTexture = gu::StaticPointerCast<vulkan::GPUTexture>(_texture);

		/*-------------------------------------------------------------------
		-               Set up Image info
		---------------------------------------------------------------------*/
		const VkDescriptorImageInfo imageInfo = 
		{
			.sampler     = nullptr,
			.imageView   = _imageView,
			.imageLayout = EnumConverter::Convert(vkTexture->GetBarrierState())
		};
		
		writeDesc.pImageInfo     = &imageInfo;
		writeDesc.descriptorType = EnumConverter::Convert(Convert(_resourceViewType));
	}
	else if (_calledCreateBufferView || _bufferView)
	{
		const auto vkBuffer = gu::StaticPointerCast<vulkan::GPUBuffer>(_buffer);
		/*-------------------------------------------------------------------
		-               Set up Buffer info
		---------------------------------------------------------------------*/
		const VkDescriptorBufferInfo bufferInfo = 
		{
			.buffer = vkBuffer->GetBuffer(),
			.offset = 0,
			.range  = vkBuffer->GetTotalByteSize()
		};
		
		writeDesc.pBufferInfo = &bufferInfo;
		writeDesc.descriptorType = EnumConverter::Convert(Convert(_resourceViewType)); // UAVもくるから分からん
	}
	else
	{
		throw std::runtime_error("failed to bind resource");
	}
	/*-------------------------------------------------------------------
	-                   Update descriptor set 
	---------------------------------------------------------------------*/
	vkUpdateDescriptorSets(vkDevice->GetDevice(), 1, &writeDesc, 0, nullptr);
}
#pragma endregion Bind Function
#pragma region Prepare View Function 
/****************************************************************************
*                     CreateView
*************************************************************************//**
*  @fn        void GPUResourceView::CreateView()
*
*  @brief     Create view
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateView()
{
	switch (_resourceViewType)
	{
		case core::ResourceViewType::ConstantBuffer:
		case core::ResourceViewType::StructuredBuffer:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWStructuredBuffer:
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

/****************************************************************************
*                     CreateImageView
*************************************************************************//**
*  @fn        void GPUResourceView::CreateImageView()
*
*  @brief     Create image view
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateImageView()
{
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	const auto vkImage  = gu::StaticPointerCast<vulkan::GPUTexture>(_texture);

	const VkImageViewCreateInfo imageViewCreateInfo = 
	{
		.sType            = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.pNext            = nullptr,
		.flags            = 0,
		.image            = vkImage->GetImage(),
		.viewType         = EnumConverter::Convert(vkImage->GetResourceType()),
		.format           = EnumConverter::Convert(vkImage->GetPixelFormat()),
		.components       = VkComponentMapping(VkComponentSwizzle::VK_COMPONENT_SWIZZLE_R, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_G, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_B, VkComponentSwizzle::VK_COMPONENT_SWIZZLE_A),
		.subresourceRange = VkImageSubresourceRange
		(
			GetImageAspectFlags(imageViewCreateInfo.format),        // aspect mask
			0,                                                      // base mip levels
			static_cast<std::uint32_t>(vkImage->GetMipMapLevels()), // level count
			0,                                                      // base array layer
			static_cast<std::uint32_t>(vkImage->GetArrayLength())   // layer count
		)
	};

	if (vkCreateImageView(vkDevice->GetDevice(), &imageViewCreateInfo, nullptr, &_imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer view (vulkan api)");
	}

	if (_resourceViewType == core::ResourceViewType::RWTexture)
	{
		_heapType = rhi::core::DescriptorHeapType::UAV;
	}
	else
	{
		_heapType = rhi::core::DescriptorHeapType::SRV;
	}
}

/****************************************************************************
*                     CreateBufferView
*************************************************************************//**
*  @fn        void GPUResourceView::CreateBufferView()
*
*  @brief     Create buffer view
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateBufferView()
{
#ifdef _DEBUG
	assert(_buffer->IsBuffer());
#endif

	if (_buffer->GetMetaData().Format != core::PixelFormat::Unknown)
	{
		
		const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
		const auto vkBuffer = gu::StaticPointerCast<vulkan::GPUBuffer>(_buffer);

		const VkBufferViewCreateInfo bufferViewCreateInfo =
		{
			.sType  = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO,
			.pNext  = nullptr,
			.flags  = 0,
			.buffer = vkBuffer->GetBuffer(),
			.format = EnumConverter::Convert(_buffer->GetMetaData().Format),
			.offset = 0,
			.range  = vkBuffer->GetTotalByteSize()
		};

		if (vkCreateBufferView(vkDevice->GetDevice(), &bufferViewCreateInfo, nullptr, &_bufferView) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create buffer view (vulkan api)");
		}

	}
	else
	{
		_calledCreateBufferView = true;
	}

	if (_resourceViewType == core::ResourceViewType::RWBuffer || _resourceViewType == core::ResourceViewType::RWStructuredBuffer)
	{
		_heapType = rhi::core::DescriptorHeapType::UAV;
	}
	else
	{
		_heapType = rhi::core::DescriptorHeapType::CBV;
	}
}

/****************************************************************************
*                     GetImageAspectFlags
*************************************************************************//**
*  @fn        VkImageAspectFlags GPUResourceView::GetImageAspectFlags(const VkFormat format)
*
*  @brief     Return the image aspect flags in accroding to the Vkformat.
*
*  @param[in] const VkFormat format
*
*  @return 　　VkImageAspectFlags
*****************************************************************************/
VkImageAspectFlags GPUResourceView::GetImageAspectFlags(const VkFormat format)
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
*  @fn        const gu::SharedPointer<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
* 
*  @brief     Select DirectX12 Descriptor Heap. return custom heap or default heap
* 
*  @param[in] const core::DescriptorHeapType type
* 
*  @return 　　const gu::SharedPointer<directX12::RHIDescriptorHeap>
*****************************************************************************/
const gu::SharedPointer<vulkan::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
{
	// Set custom Heap
	if (_heap) { return gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_heap); }

	// Select default heap based on core::ResourceViewType
	gu::SharedPointer<vulkan::RHIDescriptorHeap> defaultHeap = nullptr;
	switch (type)
	{
		case core::ResourceViewType::ConstantBuffer:        { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::CBV)); break; }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer:      { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break; }
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::UAV)); break; }
		case core::ResourceViewType::RenderTarget:          { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::RTV)); break; }
		case core::ResourceViewType::DepthStencil:          { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::DSV)); break; }
		case core::ResourceViewType::AccelerationStructure: { defaultHeap = gu::StaticPointerCast<vulkan::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break; }
		default:
		{
			throw std::runtime_error("not support descriptor view. (vulkan api)");
		}
	}

	_heap = defaultHeap;
	return defaultHeap;
}
#pragma endregion Prepare View Function