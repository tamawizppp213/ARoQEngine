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
#include <cassert>
#include <queue>
#include <Windows.h>
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
		using DescriptorID = std::uint32_t;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief: increment and return currentID*/
		inline DescriptorID IssueID(VkDescriptorSet descriptorSet)
		{
			assert(("descriptor set is nullptr", descriptorSet));

			if ((std::uint32_t)_currentID + 1 > _maxDescriptorCount) 
			{
				throw std::runtime_error("The number of IDs has exceeded the expected number. "); 
			}

			if (!_availableID.empty())
			{
				const auto id = _availableID.front();
				_availableID.pop();
				_descriptorSets[id] = descriptorSet;
				return id;
			}

			_descriptorSets.emplace_back(std::move(descriptorSet));
			_currentID++;
			return _currentID;
		}

		/* @brief : free descriptor set and push available id*/
		inline void FreeID(const std::uint32_t id)
		{
			if (id >= _maxDescriptorCount) { OutputDebugStringA("Non available id"); return; }

			_descriptorSets[id] = VK_NULL_HANDLE;
			_availableID.push(id);
		}

		/* @brief: Back offset ID (The contents of the heap are not reset.)*/
		inline void ResetID()
		{
			_descriptorSets.clear(); _descriptorSets.shrink_to_fit();
			_currentID = INVALID_ID;
			std::queue<std::uint32_t>().swap(_availableID); // ‹ó‚É‚·‚é
		}

		inline VkDescriptorSet GetDescriptorSet(DescriptorID index = 0) 
		{
			if (_descriptorSets.empty()) { return VK_NULL_HANDLE; }

			return _descriptorSets[index]; 
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline void SetResourceAllocator(const std::uint32_t maxDescriptorCount, const VkDescriptorPool pool)
		{
			_maxDescriptorCount = maxDescriptorCount;
			_descriptorPool     = pool;
			_currentID          = INVALID_ID;
			ResetID();
		}
		inline void SetDescriptorPool(const VkDescriptorPool pool) { _descriptorPool = pool; }
		
		/* @brief: Return current descriptor index */
		inline int GetCurrentID() const { return _currentID; }

		/* @brief : Return max descritor count*/
		inline std::uint32_t GetMaxDescriptorCount() const { return _maxDescriptorCount; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceAllocator() = default;

		explicit ResourceAllocator(const std::uint32_t maxDescriptorCount, const VkDescriptorPool descriptorPool) 
			: _descriptorPool(descriptorPool), _maxDescriptorCount(maxDescriptorCount) {}
		
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
		
		std::uint32_t _maxDescriptorCount = 0;

		int _currentID = INVALID_ID;

		std::queue<std::uint32_t> _availableID = {};

		static constexpr int INVALID_ID = -1;
	};
}
#endif