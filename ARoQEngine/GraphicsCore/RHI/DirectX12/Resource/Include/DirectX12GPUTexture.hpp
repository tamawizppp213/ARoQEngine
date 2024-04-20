//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_TEXTURE_HPP
#define DIRECTX12_GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
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
	*				  			GPUTexture
	*************************************************************************//**
	/*  @brief     テクスチャとして使用する場合のGPUリソースです.
	*****************************************************************************/
	class GPUTexture: public core::GPUTexture
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const gu::tstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList) override;
		
		void Save(const gu::tstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList, const gu::SharedPointer<core::RHICommandQueue>& commandQueue)override;

		void Write(const gu::SharedPointer<core::RHICommandList>& commandList, const gm::RGBA* pixel) override;

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     バッファとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return false; }

		/*!**********************************************************************
		*  @brief     テクスチャとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return true; }

		ResourceComPtr GetResource () { return _resource; }
		
		Resource* GetResourcePtr() { return _resource.Get(); }

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
		
		void SetName(const gu::tstring& name) override;
		
		#pragma endregion

		/*!**********************************************************************
		*  @brief     現時点のGPUResourceの扱い方 (IndexBufferとして使用するなど...)を設定します
		*  @attention 手動での切り替えは基本的に行わないでください. (この関数はバリアの使用を目的として使用します.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;

		~GPUTexture();
		
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const gu::tstring& name = SP(""));
		
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP(""));
		
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const ResourceComPtr& texture, const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP(""));
	
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void Pack([[maybe_unused]]const gu::SharedPointer<core::RHICommandList>& commandList) override;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ResourceComPtr        _resource = nullptr;

		ResourceComPtr        _stagingBuffer = nullptr; // 必要なくなったタイミングで捨てたい
		
		D3D12_RESOURCE_STATES _usageState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		
		bool _hasAllocated = false;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void AllocateGPUTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, bool isDiscreteGPU );

		void ConvertDxMetaData       (D3D12_RESOURCE_DESC& resourceDesc);
		
	};
}
#endif