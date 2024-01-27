//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUBuffer.cpp
///             @brief  Buffer
///             @author Toide Yutaro
///             @date   2022_08_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUBuffer.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include "../../Core/Include/VulkanDevice.hpp"
#include "../../Core/Include/VulkanAdapter.hpp"
#include "../../Core/Include/VulkanInstance.hpp"
#include "../../Core/Include/VulkanCommandList.hpp"
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
GPUBuffer::GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::wstring& name)
	: core::GPUBuffer(device, metaData, name)
{
#ifdef _DEBUG
	assert(device);
#endif
	Prepare(_buffer, _memory, EnumConverter::Convert(_metaData.HeapType));
	SetName(name);
}

GPUBuffer::~GPUBuffer()
{
	VkDevice vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	if (_stagingMemory)      { vkFreeMemory(vkDevice, _stagingMemory, nullptr); }
	if (_memory)             { vkFreeMemory(vkDevice, _memory, nullptr); }
	if (_stagingBuffer) { vkDestroyBuffer(vkDevice, _stagingBuffer, nullptr); }
	if (_buffer) { vkDestroyBuffer(vkDevice, _buffer, nullptr); }
}

#pragma endregion Constructor and Destructor

#pragma region Map
void GPUBuffer::Pack(const void* data, const gu::SharedPointer<core::RHICommandList>& copyCommandList)
{
	const auto vkDevice = gu::StaticPointerCast<rhi::vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-           GPU only accessible 
	---------------------------------------------------------------------*/
	if ((!_metaData.IsCPUAccessible()))
	{
#if _DEBUG
		assert(copyCommandList->GetType() == core::CommandListType::Copy);
#endif
		/*-------------------------------------------------------------------
		-           Reset intermediate buffer
		---------------------------------------------------------------------*/
		if (_stagingMemory) { vkFreeMemory   (vkDevice, _stagingMemory, nullptr); }
		if (_stagingBuffer) { vkDestroyBuffer(vkDevice, _stagingBuffer, nullptr); }

		/*-------------------------------------------------------------------
		-           Create intermediate buffer
		---------------------------------------------------------------------*/
		Prepare(_stagingBuffer, _stagingMemory, EnumConverter::Convert(core::MemoryHeap::Upload));

		/*-------------------------------------------------------------------
		-           Create intermediate buffer
		---------------------------------------------------------------------*/
		if (vkMapMemory(vkDevice, _stagingMemory, 0, _metaData.ByteSize, 0, reinterpret_cast<void**>(&_stagingMappedData)) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to vk map memory.");
		};
		std::memcpy(_stagingMappedData, data, _metaData.ByteSize);
		vkUnmapMemory(vkDevice, _stagingMemory);

		/*-------------------------------------------------------------------
		-           Copy intermediate buffer to the main buffer
		---------------------------------------------------------------------*/
		VkBufferCopy copy = 
		{
			.srcOffset = 0, 
			.dstOffset = 0,
			.size      = _metaData.ByteSize
		};

		const auto vkCommandList = gu::StaticPointerCast<vulkan::RHICommandList>(copyCommandList)->GetCommandList();
		vkCmdCopyBuffer(vkCommandList, _stagingBuffer, _buffer, 1, &copy);

	}
	/*-------------------------------------------------------------------
	-           CPU accessible
	---------------------------------------------------------------------*/
	else if (_metaData.HeapType == core::MemoryHeap::Upload || _metaData.HeapType == core::MemoryHeap::Readback)
	{
		Update(data, GetElementCount());
	}
	/*-------------------------------------------------------------------
	-           Other
	---------------------------------------------------------------------*/
	else
	{
		throw std::runtime_error("Unknown memory heap type");
	}
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
#if _DEBUG
	assert(_metaData.IsCPUAccessible());
#endif

	VkDevice vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	if (vkMapMemory(vkDevice, _memory, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void**>(&_mappedData)) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to vk map memory.");
	};
}

/****************************************************************************
*                     CopyData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
* 
*  @brief     GPU copy to one element 
* 
*  @param[in] const void* dataPtr
* 
*  @param[in] const size_t elementIndex
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyData(const void* data, const size_t elementIndex)
{
#ifdef _DEBUG
	assert(elementIndex <= _metaData.Count);
#endif
	std::memcpy(&_mappedData[elementIndex * _metaData.Stride], data, _metaData.Stride);
}

/****************************************************************************
*                     CopyTotalData
*************************************************************************//**
*  @fn        void GPUBuffer::CopyData()
* 
*  @brief     GPU copy the specified range
* 
*  @param[in] void*  dataPtr
* 
*  @param[in] size_t dataLength
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset)
{
#ifdef _DEBUG
	assert(dataLength + indexOffset <= _metaData.Count);
#endif

	std::memcpy(&_mappedData[indexOffset * _metaData.Stride], data, _metaData.Stride * (size_t)dataLength);
}

/****************************************************************************
*                     CopyEnd
*************************************************************************//**
*  @fn        void GPUBuffer::CopyEnd()
* 
*  @brief     Call UnMap Function
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::CopyEnd()
{
	VkDevice vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	vkUnmapMemory(vkDevice, _memory);
}
#pragma endregion   Map
#pragma region Debug
/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void GPUBuffer::SetName(const gu::wstring& name)
* 
*  @brief     Set Buffer Name
* 
*  @param[in] const gu::wstring& name
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::SetName(const gu::wstring& name)
{
	const auto vkDevice  = gu::StaticPointerCast<vulkan::RHIDevice>(_device);

	vkDevice->SetVkResourceName(name, VK_OBJECT_TYPE_BUFFER, reinterpret_cast<std::uint64_t>(_buffer));
}
#pragma endregion Debug

#pragma region SetUp
/****************************************************************************
*                     Prepare
*************************************************************************//**
*  @fn        void GPUBuffer::Prepare()
* 
*  @brief     Create Buffer and Bind buffer memory
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void GPUBuffer::Prepare(VkBuffer& buffer, VkDeviceMemory& memory, VkMemoryPropertyFlags flags)
{
	VkDevice vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-           Create Buffer
	---------------------------------------------------------------------*/
	const VkBufferCreateInfo bufferInfo = 
	{
		.sType                 = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.pNext                 = nullptr,
		.flags                 = 0,
		.size                  = _metaData.ByteSize,
		.usage                 = EnumConverter::Convert(_metaData.ResourceUsage).first, // 役割
		.sharingMode           = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices   = nullptr
	};
	
	if (vkCreateBuffer(vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create buffer (vulkan api)");
	}

	/*-------------------------------------------------------------------
	-           Compute memory size
	---------------------------------------------------------------------*/
	VkMemoryRequirements memoryRequirement = {};
	vkGetBufferMemoryRequirements(vkDevice, buffer, &memoryRequirement);

	const VkMemoryAllocateInfo memoryInfo = 
	{
		.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext           = nullptr,
		.allocationSize  = memoryRequirement.size,
		.memoryTypeIndex = gu::StaticPointerCast<vulkan::RHIDevice>(_device)
		->GetMemoryTypeIndex(memoryRequirement.memoryTypeBits, flags)
	};
	
	/*-------------------------------------------------------------------
	-           Allocate memory
	---------------------------------------------------------------------*/
	if (vkAllocateMemory(vkDevice, &memoryInfo, nullptr, &memory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate memory (vulkan api)");
	}

	/*-------------------------------------------------------------------
	-           Bind memory to the VKBuffer
	---------------------------------------------------------------------*/
	if (vkBindBufferMemory(vkDevice, buffer, memory, 0) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to bind buffer memory. (vulkan api)");
	}

}
#pragma endregion SetUp