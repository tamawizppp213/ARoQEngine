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
		virtual void CopyStart() = 0;
		virtual void CopyData(int elementIndex, const void* data) = 0;
		virtual void CopyTotalData(const void* data, int dataLength) = 0;
		virtual void CopyEnd() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline size_t GetElementCount   () { return _metaData.Count; }
		inline size_t GetElementByteSize() { return _metaData.Stride; }
		inline size_t GetTotalByteSize  () { return _metaData.ByteSize; }
		inline const GPUBufferMetaData GetMetaData() { return _metaData; }
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