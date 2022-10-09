//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUTexture.hpp
///             @brief  GPU Texture
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <DirectXTex/DirectXTex.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace DirectX;
#ifdef _DEBUG
#pragma comment(lib, "Pluguins/DirectXTex.lib")
#else
#pragma comment(lib, "Pluguins/DirectXTexd.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData)
	: core::GPUTexture(device, metaData)
{
	
	const auto dxDevice = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension          = EnumConverter::Convert(metaData.Dimension);
	resourceDesc.Alignment          = 0;
	resourceDesc.Width              = metaData.Width;
	resourceDesc.Height             = static_cast<UINT>(metaData.Height);
	resourceDesc.DepthOrArraySize   = static_cast<UINT16>(metaData.DepthOrArraySize);
	resourceDesc.MipLevels          = static_cast<UINT16>(metaData.MipLevels);
	resourceDesc.Format             = EnumConverter::Convert(metaData.PixelFormat);
	resourceDesc.SampleDesc.Count   = static_cast<UINT>(metaData.Sample);
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags              = EnumConverter::Convert(metaData.ResourceUsage);

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProperty = {};
	bool isDiscreteGPU = metaData.HeapType == core::MemoryHeap::Custom;
	if (isDiscreteGPU)
	{
		heapProperty.Type                 = D3D12_HEAP_TYPE_CUSTOM;
		heapProperty.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // write back
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;               // system memory (–{“–‚É‚¢‚¢‚Ì‚©‚Í‹^–â)
		heapProperty.CreationNodeMask     = 1;
		heapProperty.VisibleNodeMask      = 1;
	}
	else
	{
		heapProperty.Type                 = EnumConverter::Convert(metaData.HeapType);
		heapProperty.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperty.CreationNodeMask     = 1;
		heapProperty.VisibleNodeMask      = 1;
	}

	/*-------------------------------------------------------------------
	-             Render target or depth stencil clear value
	---------------------------------------------------------------------*/
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = resourceDesc.Format;
	if (core::EnumHas(metaData.ResourceUsage, core::ResourceUsage::RenderTarget))
	{
		clearValue.Color[0] = metaData.ClearColor.Red;
		clearValue.Color[1] = metaData.ClearColor.Green;
		clearValue.Color[2] = metaData.ClearColor.Blue;
		clearValue.Color[3] = metaData.ClearColor.Alpha;
	}
	else
	{
		clearValue.DepthStencil.Depth   = metaData.ClearColor.Depth;
		clearValue.DepthStencil.Stencil = metaData.ClearColor.Stencil;
	}

	ThrowIfFailed(dxDevice->CreateCommittedResource(
		&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, EnumConverter::Convert(metaData.Layout),
		core::EnumHas(metaData.ResourceUsage, core::ResourceUsage::RenderTarget) || 
		core::EnumHas(metaData.ResourceUsage, core::ResourceUsage::DepthStencil) ? &clearValue : nullptr,
		IID_PPV_ARGS(_resource.GetAddressOf())));

	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &resourceDesc);
	_physicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	_alignment    = static_cast<size_t>(allocateInfo.Alignment);
	/*-------------------------------------------------------------------
	-             create shader resource view
	---------------------------------------------------------------------*/
	//PrepareSRV(metaData);
}

GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const ResourceComPtr& texture, const core::GPUTextureMetaData& metaData)
	: core::GPUTexture(device, metaData), _resource(texture)
{
	const auto dxDevice     = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxDesc       = texture->GetDesc();
	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &dxDesc);

	// because the size of texture is not always same in different adapters
	// so we need record the real size and the alignment
	_physicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	_alignment    = static_cast<size_t>(allocateInfo.Alignment);
}

void GPUTexture::PrepareSRV(const core::GPUTextureMetaData& metaData)
{
	/*-------------------------------------------------------------------
	-             Setting SRV
	---------------------------------------------------------------------*/
	_resourceViewDesc.Format                  = EnumConverter::Convert(metaData.PixelFormat);
	_resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	switch (metaData.ResourceType)
	{
		case core::ResourceType::Texture1D:
		{
			_resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
			_resourceViewDesc.Texture1D.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.Texture1D.MostDetailedMip     = 0;
			_resourceViewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
			break;
		}
		case core::ResourceType::Texture2D:
		{
			_resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
			_resourceViewDesc.Texture2D.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.Texture2D.MostDetailedMip     = 0;
			_resourceViewDesc.Texture2D.PlaneSlice          = 0;
			_resourceViewDesc.Texture2D.ResourceMinLODClamp = 0;
			break;
		}
		case core::ResourceType::Texture3D:
		{
			_resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
			_resourceViewDesc.Texture3D.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.Texture3D.MostDetailedMip     = 0;
			_resourceViewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
			break;
		}
		case core::ResourceType::TextureCube:
		{
			_resourceViewDesc.ViewDimension                   = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
			_resourceViewDesc.TextureCube.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.TextureCube.MostDetailedMip     = 0;
			_resourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
			break;
		}
		case core::ResourceType::Texture1DArray:
		{
			_resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
			_resourceViewDesc.Texture1DArray.FirstArraySlice     = 0;
			_resourceViewDesc.Texture1DArray.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.Texture1DArray.MostDetailedMip     = 0;
			_resourceViewDesc.Texture1DArray.ArraySize           = static_cast<UINT>(metaData.DepthOrArraySize);
			_resourceViewDesc.Texture1DArray.ResourceMinLODClamp = 0;
			break;
		}
		case core::ResourceType::Texture2DArray:
		{
			_resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
			_resourceViewDesc.Texture2DArray.FirstArraySlice     = 0;
			_resourceViewDesc.Texture2DArray.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.Texture2DArray.MostDetailedMip     = 0;
			_resourceViewDesc.Texture2DArray.ArraySize           = static_cast<UINT>(metaData.DepthOrArraySize);
			_resourceViewDesc.Texture2DArray.PlaneSlice          = 0;
			_resourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0;
			break;
		}
		case core::ResourceType::Texture2DArrayMultiSample:
		{
			_resourceViewDesc.ViewDimension                    = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
			_resourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
			_resourceViewDesc.Texture2DMSArray.ArraySize       = static_cast<UINT>(metaData.DepthOrArraySize);
			break;
		}
		case core::ResourceType::Texture2DMultiSample:
		{
			_resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMS;
			break;
		}

		case core::ResourceType::TextureCubeArray:
		{
			_resourceViewDesc.ViewDimension                        = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
			_resourceViewDesc.TextureCubeArray.First2DArrayFace    = 0;
			_resourceViewDesc.TextureCubeArray.MipLevels           = static_cast<UINT>(metaData.MipLevels);
			_resourceViewDesc.TextureCubeArray.MostDetailedMip     = 0;
			_resourceViewDesc.TextureCubeArray.ResourceMinLODClamp = 0;
			_resourceViewDesc.TextureCubeArray.NumCubes            = static_cast<UINT>(metaData.DepthOrArraySize / 6);
			break;
		}
		default:
		{
			throw std::runtime_error("not supported resource type");
			break;
		}
	}
}

