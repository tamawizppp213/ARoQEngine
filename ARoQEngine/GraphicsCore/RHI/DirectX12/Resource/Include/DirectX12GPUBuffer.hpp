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
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     テクスチャ以外のバッファ. 頂点, インデックスバッファなどの作成に使用します.
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
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
		virtual void Upload(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte, const gu::SharedPointer<core::RHICommandList>& commandList) override;

		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd.
		/*----------------------------------------------------------------------*/
		virtual void Update(const void* data, const gu::uint64 dataLength) override
		{
			CopyStart();
			CopyTotalData(data, dataLength);
		}

		/*----------------------------------------------------------------------
		*  @brief : 　Map関数を呼び出す
		/*----------------------------------------------------------------------*/
		void CopyStart() override;

		/*----------------------------------------------------------------------
		*  @brief :  指定されたインデックスのoffsetにしたがって, 指定の配列数分だけコピーする
		/*----------------------------------------------------------------------*/
		void CopyTotalData(const void* data, const gu::uint64 dataLength, const gu::uint64 indexOffset = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  Unmapを呼び出すときに使用します
		/*----------------------------------------------------------------------*/
		void CopyEnd() override ;

		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr GetResource   () const noexcept { return _resource; }
		
		Resource*      GetResourcePtr() const noexcept { return _resource.Get(); }
		
		Resource**     GetAddressOf  () { return _resource.GetAddressOf();}

		D3D12_GPU_VIRTUAL_ADDRESS GetDxGPUVirtualAddress() const { return _resource->GetGPUVirtualAddress(); }

		void SetName(const gu::tstring& name) override;
		
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

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		
		~GPUBuffer();
		
		explicit GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer"));
	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Member Variables
		/*! @brief DirectX12で使用するGPUリソース*/
		ResourceComPtr _resource = nullptr;

		/*! @brief GPUにアップロードする際に使用する中間バッファ*/
		ResourceComPtr _intermediateBuffer = nullptr; // for default buffer

		/*! @brief Mapを行う際に使用するCPU側のバッファ領域*/
		gu::uint8* _mappedData = nullptr;

		/*! @brief CPUからGPUにUploadが実行中かどうか(Mapが既に呼ばれているかどうか)*/
		bool _useCPUMapped = false;
		#pragma endregion Protected Member Variables
	};
}
#endif