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
GPUDynamicSampler::GPUDynamicSampler(const std::shared_ptr<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo)
	: core::GPUSampler(device, samplerInfo)
{

}

GPUStaticSampler::GPUStaticSampler(const std::shared_ptr<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo)
	: core::GPUSampler(device, samplerInfo)
{
	_samplerDesc.Filter         = EnumConverter::Convert(samplerInfo.Filter);
	_samplerDesc.AddressU       = EnumConverter::Convert(samplerInfo.AddressModeU);
	_samplerDesc.AddressV       = EnumConverter::Convert(samplerInfo.AddressModeV);
	_samplerDesc.AddressW       = EnumConverter::Convert(samplerInfo.AddressModeW);
	_samplerDesc.BorderColor    = EnumConverter::Convert(samplerInfo.Border);
	_samplerDesc.MipLODBias     = samplerInfo.MipLODBias;
	_samplerDesc.MaxAnisotropy  = static_cast<UINT>(samplerInfo.MaxAnisotropy);
	_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	_samplerDesc.MinLOD         = samplerInfo.MinLOD;
	_samplerDesc.MaxLOD         = samplerInfo.MaxLOD;
}