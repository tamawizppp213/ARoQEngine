//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUBuffer.cpp
///             @brief  Buffer
///             @author Toide Yutaro
///             @date   2022_08_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
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
GPUBuffer::GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData)
	: core::GPUBuffer(device, metaData)
{
	Prepare();
}

GPUBuffer::~GPUBuffer()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	vkFreeMemory(vkDevice, _memory, nullptr);
	vkDestroyBuffer(vkDevice, _buffer, nullptr);
}
#pragma endregion Constructor and Destructor
#pragma region Map
void GPUBuffer::CopyStart()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	if (vkMapMemory(vkDevice, _memory, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void**>(_mappedData)) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to vk map memory.");
	};
}
void GPUBuffer::CopyData(int elementIndex, const void* data)
{
	std::memcpy(&_mappedData[elementIndex * _metaData.Stride], data, _metaData.Stride);
}
void GPUBuffer::CopyTotalData(const void* data, int dataLength)
{
	std::memcpy(&_mappedData[0], data, _metaData.Stride * (size_t)dataLength);
}
void GPUBuffer::CopyEnd()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	vkUnmapMemory(vkDevice, _memory);
}
#pragma endregion   Map
#pragma region SetUp
void GPUBuffer::Prepare()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	/*-------------------------------------------------------------------
	-           Create Buffer
	---------------------------------------------------------------------*/
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;
	bufferInfo.flags = 0;
	bufferInfo.usage = EnumConverter::Convert(_metaData.ResourceUsage).first;
	bufferInfo.size  = _metaData.ByteSize;
	bufferInfo.queueFamilyIndexCount = 0;
	bufferInfo.pQueueFamilyIndices   = nullptr;
	bufferInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
	if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer (vulkan api)");
	}
	/*-------------------------------------------------------------------
	-           Allocate memory
	---------------------------------------------------------------------*/
	VkMemoryRequirements memoryRequirement = {};
	vkGetBufferMemoryRequirements(vkDevice, _buffer, &memoryRequirement);

	VkMemoryAllocateInfo memoryInfo = {};
	memoryInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryInfo.pNext           = nullptr;
	memoryInfo.allocationSize  = memoryRequirement.size;
	memoryInfo.memoryTypeIndex = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetMemoryTypeIndex(memoryRequirement.memoryTypeBits, EnumConverter::Convert(_metaData.HeapType));
	if (vkAllocateMemory(vkDevice, &memoryInfo, nullptr, &_memory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate memory (vulkan api)");
	}

	/*-------------------------------------------------------------------
	-           Bind buffer memory
	---------------------------------------------------------------------*/
	if (vkBindBufferMemory(vkDevice, _buffer, _memory, 0) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to bind buffer memory. (vulkan api)");
	}

}
#pragma endregion SetUp