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
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
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

		void TransitionState(D3D12_RESOURCE_STATES after)
		{
			_usageState = _usageState == after ? _usageState : after;
		}
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr            GetResource         () { return _resource; }
		
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
		
		void SetName(const gu::tstring& name) override;
		
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

		ResourceComPtr        _stagingBuffer = nullptr; // �K�v�Ȃ��Ȃ����^�C�~���O�Ŏ̂Ă���
		
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