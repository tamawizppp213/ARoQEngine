//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBuffer.hpp
///  @brief  テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します. 
///  @author Toide Yutaro
///  @date   2024_04_09
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
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHICommandList;
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します.
	*****************************************************************************/
	class GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer (default buffer uses commandList)
		//           https://zenn.dev/lriki/scraps/5bb7f5a23bba16 (今後確認したい)
		virtual void Pack(const void* data, const gu::SharedPointer<RHICommandList>& commandList = nullptr) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd. 
		/*----------------------------------------------------------------------*/
		void         Update(const void* data, const gu::uint64 dataLength);
		
		/*----------------------------------------------------------------------
		*  @brief :  Call map function
		/*----------------------------------------------------------------------*/
		virtual void CopyStart() = 0;
		 
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy to one element
		/*----------------------------------------------------------------------*/
		virtual void CopyData(const void* data, const gu::uint64 elementIndex) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy the specified range
		/*----------------------------------------------------------------------*/
		virtual void CopyTotalData(const void* data, const gu::uint64 dataLength, const gu::uint64 indexOffset = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call unmap function
		/*----------------------------------------------------------------------*/
		virtual void CopyEnd() = 0;

		
		void TransitionBarrierState(const core::BarrierState after) override
		{
			if (_metaData.State != after) { _metaData.State = after; }
		}

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     バッファとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return true; }

		/*!**********************************************************************
		*  @brief     テクスチャとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return false; }

		/*!**********************************************************************
		*  @brief     バッファに格納される要素の最大数を返します. 
		*  @param[in] void
		*  @return    gu::uint64 バッファに格納される要素の最大個数
		*************************************************************************/
		__forceinline gu::uint64 GetElementCount() const { return _metaData.Count; }

		/*!**********************************************************************
		*  @brief     1要素に必要な型情報のバイト数
		*  @param[in] void
		*  @return    gu::uint64 型のバイト数
		*************************************************************************/
		__forceinline gu::uint64 GetElementByteSize() const { return _metaData.Stride; }

		/*!**********************************************************************
		*  @brief     GPUBufferで使用している全体のバイト数を計算します. elementCount * elementByteSizeで返します. 
		*  @param[in] void
		*  @return    gu::uint64 全体のバイト数
		*************************************************************************/
		__forceinline gu::uint64 GetTotalByteSize() const { return _metaData.ByteSize; }

		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource Type. (Basically Buffer or RaytracingAccelerationStructure) 
		/*----------------------------------------------------------------------*/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource State which tells how to read memory 
		/*----------------------------------------------------------------------*/
		BarrierState GetBarrierState() const noexcept override { return _metaData.State; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Usage Flag. (Vertex, Index, or Constant Buffer)
		/*----------------------------------------------------------------------*/
		__forceinline ResourceUsage GetUsage() const { return _metaData.ResourceUsage; }
		
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
		
		#pragma endregion 
		
	protected:
		#pragma region Protected Constructor and Destructor
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer();

		~GPUBuffer() = default;

		explicit GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name);
		
		#pragma endregion
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		GPUBufferMetaData _metaData = {};

		gu::uint8* _mappedData = nullptr;
	};
}


#endif