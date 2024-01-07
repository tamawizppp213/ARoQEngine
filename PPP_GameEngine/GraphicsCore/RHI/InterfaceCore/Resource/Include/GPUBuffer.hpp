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
	class RHICommandList;
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer (default buffer uses commandList)
		//           https://zenn.dev/lriki/scraps/5bb7f5a23bba16 (ç°å„ämîFÇµÇΩÇ¢)
		virtual void Pack(const void* data, const gu::SharedPointer<RHICommandList>& commandList = nullptr) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd. 
		/*----------------------------------------------------------------------*/
		void         Update(const void* data, const size_t dataLength);
		
		/*----------------------------------------------------------------------
		*  @brief :  Call map function
		/*----------------------------------------------------------------------*/
		virtual void CopyStart() = 0;
		 
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy to one element
		/*----------------------------------------------------------------------*/
		virtual void CopyData(const void* data, const size_t elementIndex) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy the specified range
		/*----------------------------------------------------------------------*/
		virtual void CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call unmap function
		/*----------------------------------------------------------------------*/
		virtual void CopyEnd() = 0;

		
		void TransitionResourceState(const core::ResourceState after) override
		{
			if (_metaData.State != after) { _metaData.State = after; }
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Array Length
		/*----------------------------------------------------------------------*/
		__forceinline size_t GetElementCount   () const { return _metaData.Count; }

		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Element Byte Size
		/*----------------------------------------------------------------------*/
		__forceinline size_t GetElementByteSize() const { return _metaData.Stride; }

		/*----------------------------------------------------------------------
		*  @brief :  Return Count * Stride
		/*----------------------------------------------------------------------*/
		__forceinline size_t GetTotalByteSize  () const { return _metaData.ByteSize; }

		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource Type. (Basically Buffer or RaytracingAccelerationStructure) 
		/*----------------------------------------------------------------------*/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource State which tells how to read memory 
		/*----------------------------------------------------------------------*/
		ResourceState GetResourceState() const noexcept override { return _metaData.State; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Usage Flag. (Vertex, Index, or Constant Buffer)
		/*----------------------------------------------------------------------*/
		__forceinline ResourceUsage GetUsage() const { return _metaData.ResourceUsage; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Type
		/*----------------------------------------------------------------------*/
		__forceinline BufferType GetBufferType() const { return _metaData.BufferType; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Heap region type (Default, Upload, Readback, Custom)
		/*----------------------------------------------------------------------*/
		__forceinline MemoryHeap GetMemoryHeapType() const { return _metaData.HeapType; }

		/*----------------------------------------------------------------------
		*  @brief :  First pointer of CPU memory to be registered in Map
		/*----------------------------------------------------------------------*/
		__forceinline gu::uint8* GetCPUMemory() { return _mappedData; }

		__forceinline GPUBufferMetaData& GetMetaData()                      { return _metaData; }
		__forceinline const GPUBufferMetaData& GetMetaData() const noexcept { return _metaData; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() { _isTexture = false; };

		~GPUBuffer() = default;

		explicit GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, const std::wstring& name);
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::uint8* _mappedData = nullptr;

		GPUBufferMetaData _metaData = {};
	};
}


#endif