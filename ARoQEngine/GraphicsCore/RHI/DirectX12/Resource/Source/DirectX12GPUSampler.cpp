//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

GPUSampler::GPUSampler(const gu::SharedPointer<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo)
	: core::GPUSampler(device, samplerInfo)
{
	_samplerDesc.Filter         = EnumConverter::Convert(samplerInfo.Filter);        // Sampling mode at magnification or shirinking of texture image.  
	_samplerDesc.AddressU       = EnumConverter::Convert(samplerInfo.AddressModeU);  // Texture addressing mode in the U direction
	_samplerDesc.AddressV       = EnumConverter::Convert(samplerInfo.AddressModeV);  // Texture addressing mode in the V direction
	_samplerDesc.AddressW       = EnumConverter::Convert(samplerInfo.AddressModeW);  // Texture addressing mode in the W direction
	_samplerDesc.BorderColor    = EnumConverter::Convert(samplerInfo.Border);        // Texture border color
	_samplerDesc.MipLODBias     = samplerInfo.MipLODBias;                            // LOD bias (本来出したいLODよりもシャープに, ボケて見せれる)   
	_samplerDesc.MaxAnisotropy  = static_cast<UINT>(samplerInfo.MaxAnisotropy);      //  Use anisotropy filtering
	_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	_samplerDesc.MinLOD         = samplerInfo.MinLOD;
	_samplerDesc.MaxLOD         = samplerInfo.MaxLOD;
}