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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
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
#pragma region Constructor and Destructor
GPUBuffer::GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData)
	: core::GPUBuffer(device, metaData)
{
	Prepare();
}

GPUBuffer::~GPUBuffer()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	vkFreeMemory   (vkDevice, _memory, nullptr);
	vkDestroyBuffer(vkDevice, _buffer, nullptr);
}
#pragma endregion Constructor and Destructor

#pragma region Map
void GPUBuffer::Pack(const void* data, const std::shared_ptr<core::RHICommandList>& copyCommandList)
{
	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device);
	Update(data, GetElementCount());
}
/****************************************************************************
*                     CopyStart
*************************************************************************//**
*  @fn        void GPUBuffer::CopyStart()
*  @brief     Call Map Function
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyStart()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	if (vkMapMemory(vkDevice, _memory, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void**>(&_mappedData)) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to vk map memory.");
	};
}
/****************************************************************************
*                     CopyData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
*  @brief     GPU copy to one element 
*  @param[in] const void* dataPtr
*  @param[in] const size_t elementIndex
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyData(const void* data, const size_t elementIndex)
{
	std::memcpy(&_mappedData[elementIndex * _metaData.Stride], data, _metaData.Stride);
}
/****************************************************************************
*                     CopyTotalData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
*  @brief     GPU copy the specified range
*  @param[in] void*  dataPtr
*  @param[in] size_t dataLength
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset)
{
	assert(dataLength + indexOffset <= _metaData.Count);
	std::memcpy(&_mappedData[indexOffset * _metaData.Stride], data, _metaData.Stride * (size_t)dataLength);
}
/****************************************************************************
*                     CopyEnd
*************************************************************************//**
*  @fn        void GPUBuffer::CopyEnd()
*  @brief     Call UnMap Function
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyEnd()
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	vkUnmapMemory(vkDevice, _memory);
}
#pragma endregion   Map
#pragma region Debug
/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void GPUBuffer::SetName(const std::wstring& name)
*  @brief     Set Buffer Name
*  @param[in] const std::wstring& name
*  @return 　　void
*****************************************************************************/
void GPUBuffer::SetName(const std::wstring& name)
{
	std::string utf8Name = unicode::ToUtf8String(name);

	/*-------------------------------------------------------------------
	-          Set Object Name Info
	---------------------------------------------------------------------*/
	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;  // structure type
	info.pNext        = nullptr;                                             // no extension
	info.objectType   = VK_OBJECT_TYPE_BUFFER;                               // object type : buffer
	info.objectHandle = reinterpret_cast<std::uint64_t>(_buffer);            // vkBuffer to std::uint64_t
	info.pObjectName  = utf8Name.c_str();      

	/*-------------------------------------------------------------------
	-          GetInstance
	---------------------------------------------------------------------*/
	const auto vkDevice  = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(vkDevice->GetDisplayAdapter());
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance())->GetVkInstance();

	/*-------------------------------------------------------------------
	-          SetName
	---------------------------------------------------------------------*/
	auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr( vkInstance, "vkSetDebugUtilsObjectNameEXT");
	if (func(vkDevice->GetDevice(), &info) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set buffer name");
	}
}
#pragma endregion Debug

#pragma region SetUp
/****************************************************************************
*                     Prepare
*************************************************************************//**
*  @fn        void GPUBuffer::Prepare()
*  @brief     Create Buffer and Bind buffer memory
*  @param[in] void
*  @return 　　void
*****************************************************************************/
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
	bufferInfo.sharingMode           = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
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