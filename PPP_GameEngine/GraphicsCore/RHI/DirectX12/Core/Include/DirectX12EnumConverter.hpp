//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12EnumConverter.hpp
///             @brief  EnumConverter
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_ENUM_CONVERTER_HPP
#define DIRECTX12_ENUM_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			EnumConverter
	*************************************************************************//**
	*  @class     EnumConverter
	*  @brief     RHI::Core -> DirectX12
	*****************************************************************************/
	class EnumConverter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static D3D12_TEXTURE_ADDRESS_MODE Convert(const rhi::core::TextureAddressingMode addressingMode);
		static DXGI_FORMAT                Convert(const rhi::core::PixelFormat pixelFormat);
#pragma region BlendState
		static D3D12_BLEND_OP             Convert(const rhi::core::BlendOperator blendOperator);
		static D3D12_BLEND                Convert(const rhi::core::BlendFactor blendFactor);
		static D3D12_COLOR_WRITE_ENABLE   Convert(const rhi::core::ColorMask colorMask);
#pragma endregion BlendState
#pragma region RasterizerState
		static D3D12_FILL_MODE            Convert(const rhi::core::FillMode    fillMode);
		static D3D12_CULL_MODE            Convert(const rhi::core::CullingMode cullingMode);
		static bool                       Convert(const rhi::core::FrontFace   frontFace);
#pragma endregion RasterizerState
#pragma region DepthStencilState
		static D3D12_COMPARISON_FUNC      Convert(const rhi::core::CompareOperator compareOperator);
		static D3D12_STENCIL_OP           Convert(const rhi::core::StencilOperator stencilOperator);
#pragma endregion DepthStencilState
#pragma region Input Layout
		static D3D_PRIMITIVE_TOPOLOGY     Convert(const rhi::core::PrimitiveTopology primitiveTopology);

#pragma endregion Input Layout 
	};
}
#endif