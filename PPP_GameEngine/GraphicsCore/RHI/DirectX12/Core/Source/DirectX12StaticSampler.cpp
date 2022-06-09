//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12StaticSampler.cpp
///             @brief  Static Sampler
///             @author Toide Yutaro
///             @date   2022_04_18
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12StaticSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
D3D12_STATIC_SAMPLER_DESC GetStaticSampler(SamplerType type, UINT shaderRegister)
{
	STATIC_SAMPLER_DESC samplerDesc(shaderRegister);
	switch (type)
	{
		case SamplerType::SamplerPointWrap:
		{
			samplerDesc.Filter   = D3D12_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			break;
		}
		case SamplerType::SamplerPointClamp:
		{
			samplerDesc.Filter   = D3D12_FILTER_MIN_MAG_MIP_POINT;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			break;
		}
		case SamplerType::SamplerLinearWrap:
		{
			samplerDesc.Filter   = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			break;
		}
		case SamplerType::SamplerLinearClamp:
		{
			samplerDesc.Filter   = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			break;
		}
		case SamplerType::SamplerAnisotropicWrap:
		{
			samplerDesc.Filter   = D3D12_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
			break;
		}
		case SamplerType::SamplerAnisotropicClamp:
		{
			samplerDesc.Filter   = D3D12_FILTER_ANISOTROPIC;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			break;
		}
	}

	return samplerDesc;
}