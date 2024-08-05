//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUSampler.cpp
///  @brief  テクスチャサンプリングの方法の設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_09
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
	_samplerDesc.AddressU       = EnumConverter::Convert(samplerInfo.AddressModeU);   // Texture addressing mode in the U direction
	_samplerDesc.AddressV       = EnumConverter::Convert(samplerInfo.AddressModeV);   // Texture addressing mode in the V direction
	_samplerDesc.AddressW       = EnumConverter::Convert(samplerInfo.AddressModeW);   // Texture addressing mode in the W direction
	_samplerDesc.BorderColor    = EnumConverter::Convert(samplerInfo.Border);         // Texture border color
	_samplerDesc.MipLODBias     = samplerInfo.MipLODBias;                             // LOD bias (本来出したいLODよりもシャープに, ボケて見せれる)   
	_samplerDesc.MaxAnisotropy  = static_cast<gu::uint32>(samplerInfo.MaxAnisotropy); //  Use anisotropy filtering
	_samplerDesc.ComparisonFunc = EnumConverter::Convert(samplerInfo.Compare);	      // Comparison function
	_samplerDesc.MinLOD         = samplerInfo.MinLOD;
	_samplerDesc.MaxLOD         = samplerInfo.MaxLOD;

	const bool isComparison = samplerInfo.Compare != core::CompareOperator::Never;

	switch (samplerInfo.Filter)
	{
		case core::FilterOption::Nearest:
		{
			_samplerDesc.Filter = isComparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_POINT;
			break;
		}
		case core::FilterOption::BiLinear:
		{
			_samplerDesc.Filter = isComparison ? D3D12_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT : D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
			break;
		}
		case core::FilterOption::TriLinear:
		{
			_samplerDesc.Filter = isComparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		}
		case core::FilterOption::AnisotropicNearest:
		case core::FilterOption::AnisotropicLinear:
		{
			if (samplerInfo.MaxAnisotropy == 1)
			{
				_samplerDesc.Filter = isComparison ? D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR : D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			}
			else
			{
				_samplerDesc.Filter = isComparison ? D3D12_FILTER_COMPARISON_ANISOTROPIC : D3D12_FILTER_ANISOTROPIC;
			}
		}
	}
}