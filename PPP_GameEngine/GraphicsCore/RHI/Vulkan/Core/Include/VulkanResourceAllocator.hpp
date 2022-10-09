//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanResourceAllocator.hpp
///             @brief  GPU resource view index management
///             @author Toide Yutaro
///             @date   2022_10_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RESOURCE_ALLOCATOR_HPP
#define VULKAN_RESOURCE_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vulkan/vulkan.h>
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::vulkan
{

	/****************************************************************************
	*				  			 ResourceAllocator
	*************************************************************************//**
	*  @class     ResourceAllocator
	*  @brief     Resource view index management
	*****************************************************************************/
	class ResourceAllocator : public NonCopyable
	{
		using DescriptorID = std::uint64_t;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief: increment and return currentID*/
		inline DescriptorID IssueID(VkDescriptorSet descriptorSet)
		{
			_descriptorSets.emplace_back(std::move(descriptorSet));
			return _descriptorSets.size() - 1;
		}
		/* @brief: Back offset ID (The contents of the heap are not reset.)*/
		inline void ResetID()
		{
			_descriptorSets.clear(); _descriptorSets.shrink_to_fit();
		}

		inline VkDescriptorSet GetDescriptorSet(DescriptorID index = 0) { return _descriptorSets[index]; }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline void SetDescriptorPool(const VkDescriptorPool pool) { _descriptorPool = pool; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceAllocator() = default;
		ResourceAllocator(VkDescriptorPool descriptorPool) { }
		~ResourceAllocator()
		{
			ResetID();
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkDescriptorPool             _descriptorPool = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> _descriptorSets = {};
	};
}
#endif