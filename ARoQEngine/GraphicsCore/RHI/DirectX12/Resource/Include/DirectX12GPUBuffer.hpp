//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUBuffer.hpp
///  @brief  テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します. 
///  @author Toide Yutaro
///  @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BUFFER_HPP
#define DIRECTX12_GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			GPUBuffer
	****************************************************************************/
	/* @class     GPUBuffer
	*  @brief     テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します.
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
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
		virtual void UploadByte(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually) override;

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
		virtual void UploadIndex(const void* data, const gu::uint64 elementCount, const gu::uint64 offsetIndex, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually) override;
		
		/*!**********************************************************************
		*  @brief     手動でCPUからGPUにデータをアップロードする準備として使用します.
		*  @attention subresourceのインデックスはバッファとしての利用しか考えていないため, 0が代入されます
		*************************************************************************/
		virtual void Map() override;

		/*!**********************************************************************
		*  @brief     CPUからGPUにデータをアップロードするのを止める場合に使用します.
		*  @attention 1フレームで同一リソースに何回もmap, unmapを呼ばないようにしてください. (処理負荷の観点で) @n
		*             subresourceのインデックスはバッファとしての利用しか考えていないため, 0が代入されます
		*************************************************************************/
		virtual void Unmap() override;

		#pragma region Public Member Varialbles
		/*!**********************************************************************
		*  @brief     DirectX12で使用するGPUリソース
		*  @return    ResourceComptr　ComPtrのGPUリソース
		*************************************************************************/
		ResourceComPtr GetResource   () const noexcept { return _resource; }
		
		/*!**********************************************************************
		*  @brief     DirectX12で使用するGPUリソースの生ポインタ
		*************************************************************************/
		Resource* GetResourcePtr() const noexcept { return _resource.Get(); }
		
		/*!**********************************************************************
		*  @brief     DirectX12で使用するGPUリソースのアドレス
		*************************************************************************/
		Resource** GetAddressOf  () { return _resource.GetAddressOf();}

		/*!**********************************************************************
		*  @brief     DirectX12で使用するGPUアドレス
		*  @return    D3D12_GPU_VIRTUAL_ADDRESS : uint64型で表現されるGPUアドレス
		*************************************************************************/
		D3D12_GPU_VIRTUAL_ADDRESS GetDxGPUVirtualAddress() const { return _resource->GetGPUVirtualAddress(); }

		/*!**********************************************************************
		*  @brief     デバッグ表示名を設定します.
		*  @param[in] const gu::tstring デバッグ表示名
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		
		/*!**********************************************************************
		*  @brief     Mapに登録されるCPUメモリの最初のポインタを返します
		*  @return    gu::uint8* マップした先頭ポインタ
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() override { return _mappedData; };

		/*!**********************************************************************
		*  @brief     GPUのアドレスをgu::uint64という形で仮想的に表現したものを返します.
		*  @return    gu::uint64 GPUのアドレス値
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress() override { return _resource->GetGPUVirtualAddress(); }

		#pragma endregion

		#pragma region Public Constructor and Destrcutor
		/*! @brief デフォルトコンストラクタ*/
		GPUBuffer() = default;
		
		/*! @brief デストラクタ*/
		~GPUBuffer();
		
		/*! @brief デバイスとバッファの作成情報を使って生成するコンストラクタ*/
		explicit GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer"));
		#pragma endregion 
	protected:
		#pragma region Protected Function
		
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief DirectX12で使用するGPUリソース*/
		ResourceComPtr _resource = nullptr;

		/*! @brief GPUにアップロードする際に使用する中間バッファ*/
		ResourceComPtr _intermediateBuffer = nullptr; // for default buffer

		/*! @brief Mapを行う際に使用するCPU側のバッファ領域*/
		gu::uint8* _mappedData = nullptr;

		/*! @brief CPUからGPUにUploadが実行中かどうか(Mapが既に呼ばれているかどうか)*/
		bool _useCPUMapped = false;
		#pragma endregion Protected Property
	};
}
#endif