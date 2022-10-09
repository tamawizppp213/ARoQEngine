//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type,  const std::shared_ptr<core::GPUBuffer>& buffer)
	: core::GPUResourceView(device, type)
{
	_buffer  = buffer;
	_texture = nullptr;
	
	if (_buffer) { CreateView(); }

}
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture)
	: core::GPUResourceView(device, type)
{
	_texture = texture;
	_buffer  = nullptr;
	
	if (_texture) { CreateView(); }
}
#pragma endregion Constructor and Destructor
#pragma region Setup view
void GPUResourceView::CreateView()
{
	switch (_resourceViewType)
	{
		case core::ResourceViewType::ConstantBuffer:        { CreateCBV(); }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer:      { CreateSRV(); }
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { CreateUAV(); }
		case core::ResourceViewType::RenderTarget:          { CreateRTV(); }
		case core::ResourceViewType::DepthStencil:          { CreateDSV(); }
		case core::ResourceViewType::AccelerationStructure: { CreateRAS(); }
		default:
		{
			throw std::runtime_error("not support descriptor view. (directX12 api)");
		}
	}
}

void GPUResourceView::CreateSRV()
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};

	/*-------------------------------------------------------------------
	-             Setting SRV
	---------------------------------------------------------------------*/
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	/*-------------------------------------------------------------------
	-             For texture resource
	---------------------------------------------------------------------*/
	if (_texture)
	{
		resourceViewDesc.Format = EnumConverter::Convert(_texture->GetPixelFormat());

		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
				resourceViewDesc.Texture1D.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.Texture1D.MostDetailedMip     = 0;
				resourceViewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::Texture2D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.Texture2D.MostDetailedMip     = 0;
				resourceViewDesc.Texture2D.PlaneSlice          = 0;
				resourceViewDesc.Texture2D.ResourceMinLODClamp = 0;
				break;
			}
			case core::ResourceType::Texture3D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
				resourceViewDesc.Texture3D.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.Texture3D.MostDetailedMip     = 0;
				resourceViewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::TextureCube:
			{
				resourceViewDesc.ViewDimension                   = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
				resourceViewDesc.TextureCube.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.TextureCube.MostDetailedMip     = 0;
				resourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::Texture1DArray:
			{
				resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				resourceViewDesc.Texture1DArray.FirstArraySlice     = 0;
				resourceViewDesc.Texture1DArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.Texture1DArray.MostDetailedMip     = 0;
				resourceViewDesc.Texture1DArray.ArraySize           = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture1DArray.ResourceMinLODClamp = 0;
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice     = 0;
				resourceViewDesc.Texture2DArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.Texture2DArray.MostDetailedMip     = 0;
				resourceViewDesc.Texture2DArray.ArraySize           = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture2DArray.PlaneSlice          = 0;
				resourceViewDesc.Texture2DArray.ResourceMinLODClamp = 0;
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				resourceViewDesc.ViewDimension                    = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
				resourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DMSArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DMultiSample:
			{
				resourceViewDesc.ViewDimension = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DMS;
				break;
			}

			case core::ResourceType::TextureCubeArray:
			{
				resourceViewDesc.ViewDimension                        = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
				resourceViewDesc.TextureCubeArray.First2DArrayFace    = 0;
				resourceViewDesc.TextureCubeArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapLevels());
				resourceViewDesc.TextureCubeArray.MostDetailedMip     = 0;
				resourceViewDesc.TextureCubeArray.ResourceMinLODClamp = 0;
				resourceViewDesc.TextureCubeArray.NumCubes            = static_cast<UINT>(_texture->GetArrayLength() / 6);
				break;
			}
			default:
			{
				throw std::runtime_error("not supported texture resource type");
				break;
			}
		}
		const auto dxTexture = std::static_pointer_cast<directX12::GPUTexture>(_texture);
		resource = dxTexture->GetResource().Get();
	}
	/*-------------------------------------------------------------------
	-             For buffer resource
	---------------------------------------------------------------------*/
	else if (_buffer)
	{
		resourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
		switch (_buffer->GetResourceType())
		{
			case core::ResourceType::Buffer:
			{
				resourceViewDesc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;
				resourceViewDesc.Buffer.FirstElement        = 0;
				resourceViewDesc.Buffer.Flags               = D3D12_BUFFER_SRV_FLAG_NONE;
				resourceViewDesc.Buffer.NumElements         = static_cast<UINT>(_buffer->GetElementCount());
				resourceViewDesc.Buffer.StructureByteStride = static_cast<UINT>(_buffer->GetElementByteSize());
				break;
			}
			default:
			{
				throw std::runtime_error("not supported buffer resource type");
			}
		}
		const auto dxBuffer = std::static_pointer_cast<directX12::GPUBuffer>(_buffer);
		resource = dxBuffer->GetResource().Get();
	}
	else
	{
		throw std::runtime_error("unknown resource type. Select resource type (texture or buffer)");
	}

	if (resource == nullptr) { throw std::runtime_error("GPU resource is nullptr"); }

	dxDevice->CreateShaderResourceView(
		resource, &resourceViewDesc, D3D12_CPU_DESCRIPTOR_HANDLE());
}

void GPUResourceView::CreateRAS()
{
	if (_buffer)
	{
		
	}
}
void GPUResourceView::CreateUAV()
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_UNORDERED_ACCESS_VIEW_DESC resourceViewDesc = {};
	
	/*-------------------------------------------------------------------
	-             Setting SRV
	---------------------------------------------------------------------*/
	if (_texture)
	{
		resourceViewDesc.Format = EnumConverter::Convert(_texture->GetPixelFormat());
		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1D;
				resourceViewDesc.Texture1D.MipSlice = 0;
				break;
			}
			case core::ResourceType::Texture2D:
			{
				resourceViewDesc.ViewDimension        = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MipSlice   = 0;
				resourceViewDesc.Texture2D.PlaneSlice = 0;
				break;
			}
			case core::ResourceType::Texture3D:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE3D;
				resourceViewDesc.Texture3D.FirstWSlice = 0;
				resourceViewDesc.Texture3D.MipSlice    = 0;
				resourceViewDesc.Texture3D.WSize       = 0.0f;
				break;
			}
			case core::ResourceType::Texture1DArray:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
				resourceViewDesc.Texture1DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture1DArray.MipSlice        = 0;
				resourceViewDesc.Texture1DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DArray.MipSlice   = 0;
				resourceViewDesc.Texture2DArray.ArraySize  = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture2DArray.PlaneSlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("not supported texture resource type");
				break;
			}
		}
		
		const auto dxTexture = std::static_pointer_cast<directX12::GPUTexture>(_texture);
		resource = dxTexture->GetResource().Get();
	}
	else if (_buffer)
	{
		switch (_buffer->GetResourceType())
		{
			case core::ResourceType::Buffer:
			{
				resourceViewDesc.Format = DXGI_FORMAT_UNKNOWN;
				resourceViewDesc.Buffer.CounterOffsetInBytes = 0;
				resourceViewDesc.Buffer.FirstElement         = 0;
				resourceViewDesc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAG_NONE;
				resourceViewDesc.Buffer.NumElements          = static_cast<UINT>(_buffer->GetElementCount());
				resourceViewDesc.Buffer.StructureByteStride  = static_cast<UINT>(_buffer->GetElementByteSize());
				break;
			}
			default:
			{
				throw std::runtime_error("not supported buffer resource type");
			}
		}

		const auto dxBuffer = std::static_pointer_cast<directX12::GPUBuffer>(_buffer);
		resource = dxBuffer->GetResource().Get();
	}
	else
	{
		throw std::runtime_error("unknown resource type. Select resource type (texture or buffer)");
	}

	if (resource == nullptr) { throw std::runtime_error("GPU resource is nullptr"); }

	dxDevice->CreateUnorderedAccessView(
		resource, nullptr, &resourceViewDesc, D3D12_CPU_DESCRIPTOR_HANDLE());
}
void GPUResourceView::CreateRTV()
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	if (_texture)
	{
		desc.Format = EnumConverter::Convert(_texture->GetPixelFormat());

		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = static_cast<UINT>(0);
			}
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice   = 0;
				desc.Texture2D.PlaneSlice = 0;
				break;
			}
			case core::ResourceType::Texture3D:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice    = 0;
				desc.Texture3D.WSize       = 0;
				desc.Texture3D.FirstWSlice = 0;
			}
			case core::ResourceType::Texture2DMultiSample:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice        = 0;
				desc.Texture2DArray.PlaneSlice      = 0;
				desc.Texture2DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				desc.Texture2DMSArray.FirstArraySlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("wrong render target resource type (directX12 api)");
			}
		}

		const auto dxTexture = std::static_pointer_cast<directX12::GPUTexture>(_texture);
		resource = dxTexture->GetResource().Get();
	}
	else if (_buffer)
	{
		desc.Format = DXGI_FORMAT_UNKNOWN;
		
		switch (_buffer->GetResourceType())
		{
			case core::ResourceType::Buffer:
			{
				desc.Buffer.FirstElement = 0;
				desc.Buffer.NumElements  = static_cast<UINT>(_buffer->GetElementCount());
				break;
			}
			default:
			{
				throw std::runtime_error("not support buffer resource type");
			}
		}

		const auto dxBuffer = std::static_pointer_cast<directX12::GPUBuffer>(_buffer);
		resource = dxBuffer->GetResource().Get();
	}
	else
	{
		throw std::runtime_error("unknown resource type. Select resource type (texture or buffer)");

	}


	if (resource == nullptr) { throw std::runtime_error("GPU resource is nullptr"); }

	dxDevice->CreateRenderTargetView(resource, &desc,D3D12_CPU_DESCRIPTOR_HANDLE());
}

void GPUResourceView::CreateDSV()
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	if (_texture)
	{
		desc.Format = EnumConverter::Convert(_texture->GetPixelFormat());
		desc.Flags  = D3D12_DSV_FLAG_NONE;
		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = 0;
				break;
			}
			case core::ResourceType::Texture2DMultiSample:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice        = 0;
				desc.Texture2DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = static_cast<UINT>(_texture->GetArrayLength());
				desc.Texture2DMSArray.FirstArraySlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("wrong resource type (directX12 api)");
			}
		}
		dxDevice->CreateDepthStencilView(
			std::static_pointer_cast<directX12::GPUTexture>(_texture)->GetResource().Get(),
			&desc,
			D3D12_CPU_DESCRIPTOR_HANDLE()
		);
	}
	else
	{
		throw std::runtime_error("not support resource type (texture only)");
	}
}
void GPUResourceView::CreateCBV()
{

	if (_buffer)
	{
		DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = 0;
		desc.SizeInBytes    = static_cast<UINT>(_buffer->GetTotalByteSize());
		assert(desc.SizeInBytes % 256 == 0);
		dxDevice->CreateConstantBufferView(&desc, D3D12_CPU_DESCRIPTOR_HANDLE());
	}
	else
	{
		throw std::runtime_error("not support resource type (buffer only)");
	}
}
#pragma endregion Setup view