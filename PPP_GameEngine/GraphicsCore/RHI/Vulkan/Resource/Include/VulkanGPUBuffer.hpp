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
		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer
		void Pack(const void* data, const std::shared_ptr<core::RHICommandList>& copyCommandList) override;
		// @brief : Begin Map Function
		void CopyStart() override;
		// @brief : GPU copy to one element 
		void CopyData(const void* data, const size_t elementIndex) override ;
		// @brief : GPU copy the specified range
		void CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset = 0) override;
		// @brief : Unmap Function
		void CopyEnd() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkBuffer GetBuffer() const noexcept { return _buffer; }
		void SetName(const std::wstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		~GPUBuffer();
		explicit GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const std::wstring& name = L"");
		
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