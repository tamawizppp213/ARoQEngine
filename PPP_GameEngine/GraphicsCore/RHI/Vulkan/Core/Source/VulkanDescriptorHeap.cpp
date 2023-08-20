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
* 
*  @brief     Allocate view. Return descriptor index
* 
*  @param[in] const core::DescriptorHeapType type
* 
*  @param[in] const std::shared_ptr<core::RHIResourceLayout>& resourceLayout
* 
*  @return 　　DescriptorID (std::uint32_t)
*****************************************************************************/
RHIDescriptorHeap::DescriptorID RHIDescriptorHeap::Allocate(const core::DescriptorHeapType heapType, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	/*-------------------------------------------------------------------
	-			     Check heap type
	---------------------------------------------------------------------*/
	if (!_heapInfo.contains(heapType)) { throw std::runtime_error("Not include heap type"); }
	
	/*-------------------------------------------------------------------
	-			     Set up descriptor set layout
	---------------------------------------------------------------------*/
	if(!resourceLayout){ throw std::runtime_error("Resource layout is nullptr"); }
	VkDevice   vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	const auto vkLayout = std::static_pointer_cast<vulkan::RHIResourceLayout>(resourceLayout);

	const VkDescriptorSetAllocateInfo allocateInfo = 
	{
		.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext              = nullptr,
		.descriptorPool     = _descriptorPool,
		.descriptorSetCount = static_cast<std::uint32_t>(vkLayout->GetDescriptorSetLayouts().size()),
		.pSetLayouts        = vkLayout->GetDescriptorSetLayouts().data()
	};
	
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

	return static_cast<core::RHIDescriptorHeap::DescriptorID>(_resourceAllocator.IssueID(descriptorSet));
}

/****************************************************************************
*                     Free
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex)
*
*  @brief     Free Resource allocator and heap
*
*  @param[in] const core::DescriptorHeapType heapType
*  @param[in] const DescriptorID offsetID
*
*  @return 　　void
*****************************************************************************/
void RHIDescriptorHeap::Free(const core::DescriptorHeapType heapType, const DescriptorID offsetIndex)
{
	/*-------------------------------------------------------------------
	-			     Check heap type
	---------------------------------------------------------------------*/
	if (!_heapInfo.contains(heapType)) { return; }

	/*-------------------------------------------------------------------
	-			     Free ID
	---------------------------------------------------------------------*/
	VkDevice vkDevice  = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	auto descriptorSet = _resourceAllocator.GetDescriptorSet(offsetIndex);
	
	if (vkFreeDescriptorSets(vkDevice, _descriptorPool, 1, &descriptorSet) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to free descriptor set");
	}
	
	_resourceAllocator.FreeID(offsetIndex); // resource view index in each heap.s
}

/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHIDescriptorHeap::Resize(const core::DescriptorHeapType type, const size_t viewCount)
* 
*  @brief     Resize max view count size heap
* 
*  @param[in] const core::DescriptorHeapType type
* 
*  @param[in] const size_t viewCount
* 
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
* 
*  @brief     Resize max view count size heap
* 
*  @param[in] const std::map<core::DescriptorHeapType, MaxDescriptorSize>& heapInfo
* 
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
		const VkDescriptorPoolSize poolSize = 
		{
			.type            = EnumConverter::Convert(heapInfo.first),
			.descriptorCount = static_cast<std::uint32_t>(heapInfo.second)
		};
		poolSizes.emplace_back(poolSize);
		totalHeapCount += poolSize.descriptorCount;
	}
	_heapInfo = heapInfos;

	/*-------------------------------------------------------------------
	-               Max descriptor size check
	---------------------------------------------------------------------*/
	if (_totalHeapCount > totalHeapCount) 
	{ 
		poolSizes.clear(); poolSizes.shrink_to_fit();
		return; 
	}

	/*-------------------------------------------------------------------
	-               Get descriptor heap create info
	---------------------------------------------------------------------*/
	const VkDescriptorPoolCreateInfo createInfo = 
	{
		.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.pNext         = nullptr,
		.flags         = 0,
		.maxSets       = static_cast<std::uint32_t>(totalHeapCount),
		.poolSizeCount = static_cast<std::uint32_t>(poolSizes.size()),
		.pPoolSizes    = poolSizes.data()
	};

	if (vkCreateDescriptorPool(vkDevice, &createInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool");
	}

	_totalHeapCount = totalHeapCount;
	_resourceAllocator.SetResourceAllocator(totalHeapCount, _descriptorPool);
}

void RHIDescriptorHeap::Reset(const ResetFlag flag)
{
	// ポインタのみ破棄
	_resourceAllocator.ResetID();

	/*-------------------------------------------------------------------
	-              以下は中身を全て消す場合
	---------------------------------------------------------------------*/
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