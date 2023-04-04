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
		void Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList) override;
		
		void Save(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList, const std::shared_ptr<core::RHICommandQueue>& commandQueue)override;

		void Write(const std::shared_ptr<core::RHICommandList>& commandList, const std::shared_ptr<gm::RGBA>& pixel) override;

		void TransitionState(D3D12_RESOURCE_STATES after)
		{
			_usageState = _usageState == after ? _usageState : after;
		}
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr            GetResource         () { return _resource; }
		
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
		
		void SetName(const std::wstring& name) override;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;

		~GPUTexture() = default;
		
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const std::wstring& name = L"");
		
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const std::wstring& name = L"");
		
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const ResourceComPtr& texture, const core::GPUTextureMetaData& metaData, const std::wstring& name = L"");
	
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void Pack(const std::shared_ptr<core::RHICommandList>& commandList) override;

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