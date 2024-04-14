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
	class  GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  　　GPUにメモリを配置します. 融通が効くようにbyte単位で指定します. 
		*  @param[in] const void* : GPUにアップロードしたCPU側のメモリ配列
		*  @param[in] const gu::uint64 メモリの確保するバイトサイズ
		*  @param[in] const gu::uint64 メモリを確保する初期オフセット [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> GraphicsかCopyのコマンドリスト
		*  @return    void
		*************************************************************************/
		virtual void Upload(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr) = 0;

		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer (default buffer uses commandList)
		//           https://zenn.dev/lriki/scraps/5bb7f5a23bba16 (今後確認したい)
		virtual void Pack(const void* data, const gu::SharedPointer<RHICommandList>& commandList = nullptr) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd. 
		/*----------------------------------------------------------------------*/
		virtual void Update(const void* data, const gu::uint64 dataLength) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call map function
		/*----------------------------------------------------------------------*/
		virtual void CopyStart() = 0;
		 
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy the specified range
		/*----------------------------------------------------------------------*/
		virtual void CopyTotalData(const void* data, const gu::uint64 dataLength, const gu::uint64 indexOffset = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call unmap function
		/*----------------------------------------------------------------------*/
		virtual void CopyEnd() = 0;

		#pragma endregion 
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
		__forceinline gu::uint64 GetTotalByteSize() const { return _metaData.GetTotalByte(); }

		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource Type. (Basically Buffer or RaytracingAccelerationStructure) 
		/*----------------------------------------------------------------------*/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*!**********************************************************************
		*  @brief     GPUリソースの状態を定義します. リソース状態を変更する際は, TransitionResourceState(コマンドリスト)を使用してください.  
		*  @return    ResourceState : GPUリソースの状態
		*************************************************************************/
		ResourceState GetResourceState() const noexcept override { return _metaData.State; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Usage Flag. (Vertex, Index, or Constant Buffer)
		/*----------------------------------------------------------------------*/
		__forceinline ResourceUsage GetUsage() const { return _metaData.ResourceUsage; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Heap region type (Default, Upload, Readback, Custom)
		/*----------------------------------------------------------------------*/
		__forceinline MemoryHeap GetMemoryHeapType() const { return _metaData.HeapType; }

		/*!**********************************************************************
		*  @brief     Mapに登録されるCPUメモリの最初のポインタを返します
		*  @return    gu::uint8* マップした先頭ポインタ
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() = 0;

		/*!**********************************************************************
		*  @brief     GPUのアドレスをgu::uint64という形で仮想的に表現したものを返します. 
		*  @return    gu::uint64 GPUのアドレス値
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress() = 0;

		__forceinline GPUBufferMetaData& GetMetaData()                      { return _metaData; }
		__forceinline const GPUBufferMetaData& GetMetaData() const noexcept { return _metaData; }
		
		/*!**********************************************************************
		*  @brief     現時点のGPUResourceの扱い方 (IndexBufferとして使用するなど...)を設定します
		*  @attention 手動での切り替えは基本的に行わないでください. (この関数はバリアの使用を目的として使用します.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state;}
		#pragma endregion 
		
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUBuffer();

		/*! @brief デストラクタ*/
		~GPUBuffer() = default;

		/*! @brief バッファの作成情報を使うコンストラクタ*/
		explicit GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name);
		
		#pragma endregion
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		GPUBufferMetaData _metaData = {};
	};
}


#endif