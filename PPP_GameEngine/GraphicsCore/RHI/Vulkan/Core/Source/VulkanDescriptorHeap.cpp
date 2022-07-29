//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDescriptorHeap.cpp
///             @brief  Descriptor Heap
///             @author Toide Yutaro
///             @date   2022_07_17
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDescriptorHeap.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
	: core::RHIDescriptorHeap(device, heapType, maxDescriptorCount)
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	VkDescriptorPoolSize poolSize = {};
	poolSize.type            = EnumConverter::Convert(heapType);
	poolSize.descriptorCount = static_cast<std::uint32_t>(maxDescriptorCount);
	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext         = nullptr;
	createInfo.flags         = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	createInfo.poolSizeCount = 1;
	createInfo.pPoolSizes    = &poolSize;
	createInfo.maxSets       = MAX_SET_COUNT;
}

RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::DescriptorHeapType>& heapType, const std::vector<size_t>& maxDescriptorCount)
	: core::RHIDescriptorHeap(device, heapType, maxDescriptorCount)
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	std::vector<VkDescriptorPoolSize> poolSizes = {};
	poolSizes.resize(heapType.size());
	for (int i = 0; i < poolSizes.size(); ++i)
	{
		poolSizes[i].type            = EnumConverter::Convert(heapType[i]);
		poolSizes[i].descriptorCount = static_cast<std::uint32_t>(maxDescriptorCount[i]);
	}
	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext         = nullptr;
	createInfo.flags         = VkDescriptorPoolCreateFlagBits::VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	createInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size());
	createInfo.pPoolSizes    = poolSizes.data();
	createInfo.maxSets       = MAX_SET_COUNT;
}

RHIDescriptorHeap::~RHIDescriptorHeap()
{
	const auto vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	vkDestroyDescriptorPool(vkDevice, _descriptorPool, nullptr);
	
}