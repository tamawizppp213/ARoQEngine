//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12EnumConverter.cpp
///             @brief  Enum Converter
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
D3D12_TEXTURE_ADDRESS_MODE  EnumConverter::Convert(const rhi::core::TextureAddressingMode addressingMode)
{
	return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressingMode); // all the same
}
DXGI_FORMAT  EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		case core::PixelFormat::R8G8B8A8_UNORM    : return DXGI_FORMAT_R8G8B8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM    : return DXGI_FORMAT_B8G8R8A8_UNORM;
		case core::PixelFormat::R16G16B16A16_FLOAT: return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case core::PixelFormat::R32G32B32A32_FLOAT: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case core::PixelFormat::R32G32B32_FLOAT   : return DXGI_FORMAT_R32G32B32_FLOAT;
		case core::PixelFormat::D24_UNORM_S8_UINT : return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case core::PixelFormat::R10G10B10A2_UNORM : return DXGI_FORMAT_R10G10B10A2_UNORM;
		case core::PixelFormat::D32_FLOAT         : return DXGI_FORMAT_D32_FLOAT;
		case core::PixelFormat::Unknown: return DXGI_FORMAT_UNKNOWN;
		default:
			throw std::runtime_error("not supported Pixel Format type");
	}
}
D3D12_BLEND_OP EnumConverter::Convert(const rhi::core::BlendOperator blendOperator)
{
	return static_cast<D3D12_BLEND_OP>(blendOperator); // all the same
}
D3D12_BLEND   EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	return static_cast<D3D12_BLEND>(blendFactor); // all the same
}
D3D12_COLOR_WRITE_ENABLE EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<D3D12_COLOR_WRITE_ENABLE>(colorMask); // all the same
}