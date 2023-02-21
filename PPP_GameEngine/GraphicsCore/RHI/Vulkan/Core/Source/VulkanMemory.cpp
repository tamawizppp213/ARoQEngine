//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanMemory.cpp
///             @brief  CPU-GPU synchronization
///             @author Toide Yutaro
///             @date   2023_02_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanMemory.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanEnumConverter.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIMemory::RHIMemory( const std::shared_ptr<core::RHIDevice>& device, const core::MemoryHeap heapType,
	const std::uint64_t size, std::uint32_t typeBits)
	: core::RHIMemory(device, heapType, size)
{
	VkDevice vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-           Set memory allocation information
	---------------------------------------------------------------------*/
	const VkMemoryAllocateInfo memoryInfo = 
	{
		.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext           = nullptr,
		.allocationSize  = size,
		.memoryTypeIndex = std::static_pointer_cast<vulkan::RHIDevice>(_device)
		->GetMemoryTypeIndex(typeBits, EnumConverter::Convert(_memoryType))
	};

	/*-------------------------------------------------------------------
	-           Allocate memory
	---------------------------------------------------------------------*/
	if (vkAllocateMemory(vkDevice, &memoryInfo, nullptr, &_memory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate memory (vulkan api)");
	}
}

#pragma endregion Constructor and Destructor