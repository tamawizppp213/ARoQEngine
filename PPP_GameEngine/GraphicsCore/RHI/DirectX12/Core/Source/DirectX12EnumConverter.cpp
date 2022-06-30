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
#pragma region BlendState
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
#pragma endregion        BlendState
#pragma region RasterizerState
D3D12_FILL_MODE EnumConverter::Convert(const rhi::core::FillMode    fillMode)
{
	switch (fillMode)
	{
		case core::FillMode::Solid    : return D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		case core::FillMode::WireFrame: return D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
		default:
			throw std::runtime_error("not supported fill mode type");
	}
}
D3D12_CULL_MODE EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
{
	switch (cullingMode)
	{
		case core::CullingMode::None : return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
		case core::CullingMode::Front: return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		case core::CullingMode::Back : return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		default:
			throw std::runtime_error("not supported culling mode type");
	}
}
bool EnumConverter::Convert(const rhi::core::FrontFace   frontFace)
{
	return frontFace == rhi::core::FrontFace::CounterClockwise;
}
#pragma endregion   RasterizerState
#pragma region DepthStencilState
D3D12_COMPARISON_FUNC EnumConverter::Convert(const core::CompareOperator op)
{
	switch (op)
	{
		case core::CompareOperator::Never       : return D3D12_COMPARISON_FUNC_NEVER;
		case core::CompareOperator::Less        : return D3D12_COMPARISON_FUNC_LESS;
		case core::CompareOperator::Equal       : return D3D12_COMPARISON_FUNC_EQUAL;
		case core::CompareOperator::LessEqual   : return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case core::CompareOperator::Greater     : return D3D12_COMPARISON_FUNC_GREATER;
		case core::CompareOperator::NotEqual    : return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case core::CompareOperator::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case core::CompareOperator::Always      : return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			throw std::runtime_error("not supported compare operator type");
	}
}

D3D12_STENCIL_OP EnumConverter::Convert(const core::StencilOperator op)
{
	switch (op)
	{
		case core::StencilOperator::Keep             : return D3D12_STENCIL_OP_KEEP;
		case core::StencilOperator::Zero             : return D3D12_STENCIL_OP_ZERO;
		case core::StencilOperator::Replace          : return D3D12_STENCIL_OP_REPLACE;
		case core::StencilOperator::IncrementAndClamp: return D3D12_STENCIL_OP_INCR_SAT;
		case core::StencilOperator::DecrementAndClamp: return D3D12_STENCIL_OP_DECR_SAT;
		case core::StencilOperator::Invert           : return D3D12_STENCIL_OP_INVERT;
		case core::StencilOperator::IncrementAndWrap : return D3D12_STENCIL_OP_INCR;
		case core::StencilOperator::DecrementAndWrap : return D3D12_STENCIL_OP_DECR;
		default:
			throw std::runtime_error("not supported stencil operator type");
	}
}
#pragma endregion DepthStencilState
#pragma region Input Layout
D3D_PRIMITIVE_TOPOLOGY  EnumConverter::Convert(const rhi::core::PrimitiveTopology primitiveTopology)
{
	return static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitiveTopology);
}
#pragma endregion      Input Layput