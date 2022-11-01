//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUSampler.hpp
///             @brief  Sampler State 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_BUFFER_HPP
#define Vulkan_GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Begin Map Function
		void CopyStart() override;
		// @brief : GPU copy to one element 
		void CopyData(int elementIndex, const void* data) override ;
		// @brief : GPU copy the specified range
		void CopyTotalData(const void* data, int dataLength) override;
		// @brief : Unmap Function
		void CopyEnd() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkBuffer GetBuffer() const noexcept { return _buffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		~GPUBuffer();
		explicit GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData);
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkDeviceMemory _memory = nullptr;
		VkBuffer       _buffer = nullptr;
	private:
		void Prepare();
	};


}
#endif