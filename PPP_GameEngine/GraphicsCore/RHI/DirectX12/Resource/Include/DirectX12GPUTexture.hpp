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

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr            GetResource         () { return _resource; }
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTexture() = default;
		~GPUTexture() = default;
		explicit GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData);
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ResourceComPtr        _resource = nullptr;
		D3D12_RESOURCE_STATES _usageState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		D3D12_SHADER_RESOURCE_VIEW_DESC _resourceViewDesc = {};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void PrepareSRV(const core::GPUTextureMetaData& metaData);
	};
}
#endif