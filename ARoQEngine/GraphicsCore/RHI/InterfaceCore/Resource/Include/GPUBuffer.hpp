//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBuffer.hpp
///  @brief  テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します. @n
///          バッファの確保を行う場合は事前にGPUBufferMetaDataを使ってメモリ領域を確保しておき@n
///          CPUからGPUにデータをアップロードします. Upload関数は2種類あり, @n
///          UploadIndex : 配列のインデックスを指定することで確保する方式 @n
/// 　　　　　 UploadByte : バイト数を直接指定することで確保する方式があります. 
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
	/* @class     GPUBuffer
	*  @brief     テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します.
	*****************************************************************************/
	class GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  　　GPUにメモリを配置します. 融通が効くようにbyte単位で指定します. 
		*  @param[in] const void* : GPUにアップロードしたいCPU側のメモリ配列
		*  @param[in] const gu::uint64 メモリの確保するバイトサイズ
		*  @param[in] const gu::uint64 メモリを確保する初期オフセット [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> GraphicsかCopyのコマンドリスト
		*  @param[in] const bool 手動でマップを行うか
		*  @return    void
		*************************************************************************/
		virtual void UploadByte(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr, const bool useMapManually = false) = 0;
		 
		/*!**********************************************************************
		*  @brief  　　配列の要素を指定するインデックスを使ってCPUからGPUにメモリを配置します. 
		*  @note      暗黙的に同じバイト数の並びが存在することが求められます
		*  @param[in] const void* : GPUにアップロードしたいCPU側のメモリ配列
		*  @param[in] const gu::uint64 : 配列の要素数
		*  @param[in] const gu::uint64 : メモリを確保する初期インデックス
		*  @param[in] const gu::SharedPointer<RHICommandList> GraphicsかCopyのコマンドリスト
		*  @param[in] const bool 手動でマップを行うか
		*  @return    void
		*************************************************************************/
		virtual void UploadIndex(const void* data, const gu::uint64 elementCount, const gu::uint64 offsetIndex = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr, const bool useMapManually = false) = 0;

		/*!**********************************************************************
		*  @brief     手動でCPUからGPUにデータをアップロードする準備として使用します.
		*  @attention subresourceのインデックスはバッファとしての利用しか考えていないため, 0が代入されます
		*************************************************************************/
		virtual void Map() = 0;

		/*!**********************************************************************
		*  @brief     CPUからGPUにデータをアップロードするのを止める場合に使用します. 
		*  @attention 1フレームで同一リソースに何回もmap, unmapを呼ばないようにしてください. (処理負荷の観点で)
		*************************************************************************/
		virtual void Unmap() = 0;
		#pragma endregion 
		#pragma region Public Property
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

		/*!**********************************************************************
		*  @brief     GPUResourceの種類を指定します.(Bufferの場合はBufferかRaytracingAccelerationStructureを指定することになります) 
		*  @param[in] void
		*  @return    ResourceType GPUリソースの種類
		*************************************************************************/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*!**********************************************************************
		*  @brief     GPUリソースの状態を定義します. リソース状態を変更する際は, TransitionResourceState(コマンドリスト)を使用してください.  
		*  @return    ResourceState : GPUリソースの状態
		*************************************************************************/
		ResourceState GetResourceState() const noexcept override { return _metaData.State; }
		
		/*!**********************************************************************
		*  @brief     Bufferの使用方法 (Vertex, Index, or Constant Buffer)を定義するEnumを返します 
		*************************************************************************/
		__forceinline BufferCreateFlags GetUsage() const { return _metaData.Usage; }
		
		/*!**********************************************************************
		*  @brief   GPUのメモリを格納するヒープ領域の種類を決定するEnumを返します (Default, Upload, Readback, Custom)
		*************************************************************************/
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

		/*!**********************************************************************
		*  @brief     バッファの情報を直接格納しているメタデータを取得します
		*  @return    GPUBufferMetaData メタデータ
		*************************************************************************/
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

		#pragma region Protected Function
		
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief バッファの情報を格納している構造体*/
		GPUBufferMetaData _metaData = {};

		#pragma endregion 

		
	};
}


#endif