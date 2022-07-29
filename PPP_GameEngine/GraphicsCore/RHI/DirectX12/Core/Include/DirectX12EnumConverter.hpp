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
#pragma region Sampler State
		static D3D12_FILTER               Convert(const rhi::core::FilterOption filter);
		static D3D12_TEXTURE_ADDRESS_MODE Convert(const rhi::core::SamplerAddressMode addressingMode);
		static D3D12_STATIC_BORDER_COLOR  Convert(const rhi::core::BorderColor borderColor);
#pragma endregion Sampler State
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
#pragma region GPUResource
		static D3D12_DESCRIPTOR_HEAP_TYPE Convert(const rhi::core::DescriptorHeapType heapType);
		static D3D12_RESOURCE_FLAGS       Convert(const rhi::core::ResourceUsage usage);
#pragma region GPUBuffer 
		static D3D12_HEAP_TYPE            Convert(const rhi::core::MemoryHeap memoryHeap);
		static D3D12_RESOURCE_STATES      Convert(const rhi::core::ResourceLayout resourceLayout);
#pragma endregion GPUBuffer
#pragma region GPUTexture
		static D3D12_RESOURCE_DIMENSION   Convert(const rhi::core::ResourceDimension dimension);
		static D3D12_SRV_DIMENSION        Convert(const rhi::core::ResourceType type);
#pragma endregion GPUTexture
#pragma endregion GPUResource
	};
}
#endif