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
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanResourceLayout.hpp"
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
RHIDescriptorHeap::RHIDescriptorHeap(const std::shared_ptr<core::RHIDevice>& device) : core::RHIDescriptorHeap(device){}

RHIDescriptorHeap::~RHIDescriptorHeap()
{
	const auto vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	if (_descriptorPool)
	{
		vkDestroyDescriptorPool(vkDevice, _descriptorPool, nullptr);
	}
}

#pragma region Public Function
/****************************************************************************
*                     Allocate
*************************************************************************//**
*  @fn        RHIDescriptorHeap::DescriptorID RHIDescriptorHeap::Allocate(const core::DescriptorHeapType heapType, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
*  @brief     Allocate view. Return descriptor index
*  @param[in] const core::DescriptorHeapType type
*  @param[in] const std::shared_ptr<core::RHIResourceLayout>& resourceLayout
*  @return 　　DescriptorID (std::uint32_t)
*****************************************************************************/
RHIDescriptorHeap::DescriptorID RHIDescriptorHeap::Allocate(const core::DescriptorHeapType heapType, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	/*-------------------------------------------------------------------
	-			     Check heap type
	---------------------------------------------------------------------*/
	if (_heapInfo.find(heapType) == _heapInfo.end()) { return INVALID_ID; }
	
	/*-------------------------------------------------------------------
	-			     Set up descriptor set layout
	---------------------------------------------------------------------*/
	VkDevice   vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	const auto vkLayout = std::static_pointer_cast<vulkan::RHIResourceLayout>(resourceLayout);

	VkDescriptorSetAllocateInfo allocateInfo = {};
	allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocateInfo.descriptorPool     = _descriptorPool;
	allocateInfo.descriptorSetCount = static_cast<std::uint32_t>(vkLayout->GetDescriptorSetLayouts().size());
	allocateInfo.pNext              = nullptr;
	allocateInfo.pSetLayouts        = vkLayout->GetDescriptorSetLayouts().data();
	
	/*-------------------------------------------------------------------
	-			     Allocate descriptor set
	---------------------------------------------------------------------*/
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
	VkResult result = vkAllocateDescriptorSets(vkDevice, &allocateInfo, &descriptorSet);
	switch (result)
	{
		case VK_SUCCESS: break;
		case VK_ERROR_FRAGMENTED_POOL:    { throw std::runtime_error("fragment pool error"); }
		case VK_ERROR_OUT_OF_POOL_MEMORY: { throw std::runtime_error("out of memory error"); }
		default: { throw std::runtime_error("unrecoverable error (allocate descriptor set function)"); }
	}

	return _resourceAllocator.IssueID(descriptorSet);
}
/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
*  @brief     Resize max view count size heap
*  @param[in] const core::DescriptorHeapType type
*  @param[in] const size_t viewCount
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Resize(const core::DescriptorHeapType heapType, const size_t viewCount)
{
	std::map<core::DescriptorHeapType, MaxDescriptorSize> heapInfo;
	heapInfo[heapType] = viewCount;
	Resize(heapInfo);
}
/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
*  @brief     Resize max view count size heap
*  @param[in] const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfo
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Resize(const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfos)
{
	VkDevice vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Get descriptor pool size
	---------------------------------------------------------------------*/
	std::vector<VkDescriptorPoolSize> poolSizes = {};
	size_t totalHeapCount = 0;
	for (auto& heapInfo : heapInfos)
	{
		VkDescriptorPoolSize poolSize = {};
		poolSize.type            = EnumConverter::Convert(heapInfo.first);
		poolSize.descriptorCount = static_cast<std::uint32_t>(heapInfo.second);
		poolSizes.emplace_back(poolSize);
		totalHeapCount += poolSize.descriptorCount;
	}

	/*-------------------------------------------------------------------
	-               Max descriptor size check
	---------------------------------------------------------------------*/
	if (_totalHeapCount > totalHeapCount) { return; }
	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	VkDescriptorPoolCreateInfo createInfo = {};
	createInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	createInfo.pNext         = nullptr;
	createInfo.flags         = 0;
	createInfo.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size());
	createInfo.pPoolSizes    = poolSizes.data();
	createInfo.maxSets       = static_cast<std::uint32_t>(totalHeapCount);

	if (vkCreateDescriptorPool(vkDevice, &createInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool");
	}
	

	_totalHeapCount = totalHeapCount;
	_resourceAllocator.SetDescriptorPool(_descriptorPool);
}

void RHIDescriptorHeap::Reset(const ResetFlag flag)
{
	_resourceAllocator.ResetID();

	if (flag != ResetFlag::All) { return; }

	VkDevice vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	/*-------------------------------------------------------------------
	-               Reset descriptor pool
	---------------------------------------------------------------------*/
	if (vkResetDescriptorPool(vkDevice, _descriptorPool, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to reset descriptor pool");
	}
	_totalHeapCount = 0;
}
#pragma endregion Public Function