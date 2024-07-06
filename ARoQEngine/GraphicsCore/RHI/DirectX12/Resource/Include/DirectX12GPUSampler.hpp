//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SAMPLER_HPP
#define DIRECTX12_GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUSampler.hpp"
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
	*  @brief     Buffer
	*****************************************************************************/
	class GPUSampler : public core::GPUSampler
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc(){ return _samplerDesc; }

		const D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc() const noexcept { return _samplerDesc; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		GPUSampler() = default;

		~GPUSampler() = default;

		explicit GPUSampler(const gu::SharedPointer<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo);

		#pragma endregion
	
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		D3D12_STATIC_SAMPLER_DESC _samplerDesc = {};
		#pragma endregion	
	};
}
#endif