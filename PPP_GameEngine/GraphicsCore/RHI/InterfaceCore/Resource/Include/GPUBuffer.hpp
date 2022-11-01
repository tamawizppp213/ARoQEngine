//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUResource.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{

	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public GPUResource
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Begin Map Function
		virtual void CopyStart() = 0;
		// @brief : GPU copy to one element 
		virtual void CopyData(int elementIndex, const void* data) = 0;
		/* @brief : GPU copy the specified range*/
		virtual void CopyTotalData(const void* data, int dataLength) = 0;
		// @brief : Unmap Function
		virtual void CopyEnd() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief : Return Buffer Array Length
		size_t GetElementCount   () const { return _metaData.Count; }
		// @brief : Return Buffer Element Byte Size
		size_t GetElementByteSize() const { return _metaData.Stride; }
		// @brief : Return Count * Stride
		size_t GetTotalByteSize  () const { return _metaData.ByteSize; }
		// @brief : Return GPU Resource Type. (Basically Buffer or RaytracingAccelerationStructure) 
		ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		// @brief : Return GPU Resource State
		ResourceState GetResourceState() const { return _metaData.State; }
		// @brief : Return Buffer Usage Flag. (Vertex, Index, or Constant Buffer)
		ResourceUsage GetUsage() const { return _metaData.ResourceUsage; }
		// @brief : Return Buffer Type
		BufferType GetBufferType() const { return _metaData.BufferType; }
		      GPUBufferMetaData& GetMetaData()                { return _metaData; }
		const GPUBufferMetaData& GetMetaData() const noexcept { return _metaData; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		~GPUBuffer() = default;
		explicit GPUBuffer(const std::shared_ptr<RHIDevice>& device, const core::GPUBufferMetaData& metaData);;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::uint8_t*   _mappedData = nullptr;
		GPUBufferMetaData _metaData = {};
	};
}


#endif