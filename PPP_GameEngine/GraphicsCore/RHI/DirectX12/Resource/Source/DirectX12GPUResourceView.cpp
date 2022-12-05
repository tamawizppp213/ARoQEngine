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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
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
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type,  const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, customHeap)
{
	_buffer  = buffer; 
	_texture = nullptr;
	/*-------------------------------------------------------------------
	-             Select DirectX12 DescriptorHeap
	---------------------------------------------------------------------*/
	const auto heap = SelectDescriptorHeap(type);
	/*-------------------------------------------------------------------
	-             Create Need View
	---------------------------------------------------------------------*/
	if (_buffer) { CreateView(heap); }

}
GPUResourceView::GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, customHeap)
{
	_texture = texture;
	_buffer  = nullptr;
	/*-------------------------------------------------------------------
	-             Select DirectX12 DescriptorHeap
	---------------------------------------------------------------------*/
	const auto heap = SelectDescriptorHeap(type);
	/*-------------------------------------------------------------------
	-             Create Need View
	---------------------------------------------------------------------*/
	if (_texture) { CreateView(heap); }
}
#pragma endregion Constructor and Destructor
#pragma region Bind Function
void GPUResourceView::Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index)
{
	/*-------------------------------------------------------------------
	-             Set Descirptor Table
	---------------------------------------------------------------------*/
	const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(commandList)->GetCommandList();
	if (commandList->GetType() == core::CommandListType::Graphics)
	{
		dxCommandList->SetGraphicsRootDescriptorTable(index, GetGPUHandler());
	}
	else if (commandList->GetType() == core::CommandListType::Compute)
	{
		dxCommandList->SetComputeRootDescriptorTable(index, GetGPUHandler());
	}
	else
	{
		throw std::runtime_error("failed to bind root descriptor table");
	}
}
#pragma endregion Bind Function
#pragma region Setup view
void GPUResourceView::CreateView(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
{
	switch (_resourceViewType)
	{
		case core::ResourceViewType::ConstantBuffer:        { CreateCBV(heap); break; }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer:      { CreateSRV(heap); break; }
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { CreateUAV(heap); break; }
		case core::ResourceViewType::RenderTarget:          { CreateRTV(heap); break; }
		case core::ResourceViewType::DepthStencil:          { CreateDSV(heap); break; }
		case core::ResourceViewType::AccelerationStructure: { CreateRAS(heap); break; }
		default:
		{
			throw std::runtime_error("not support descriptor view. (directX12 api)");
		}
	}
}
/****************************************************************************
*                     CreateSRV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateSRV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create shader resource view
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateSRV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
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

	// Set heap descriptor id.
	_heapOffset.first = core::DescriptorHeapType::SRV;
	_heapOffset.second = heap->Allocate(core::DescriptorHeapType::SRV);

	// create shader resource view
	dxDevice->CreateShaderResourceView(
		resource, &resourceViewDesc, heap->GetCPUDescHandler(core::DescriptorHeapType::SRV, _heapOffset.second));
}
/****************************************************************************
*                     CreateRAS
*************************************************************************//**
*  @fn        void GPUResourceView::CreateRAS(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create raytracing acceleration structure
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateRAS(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
{
	if (!_buffer) { return; }
	const auto dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();
	const auto dxBuffer = std::static_pointer_cast<directX12::GPUBuffer>(_buffer);
	/*-------------------------------------------------------------------
	-             Set up resource view descriptor
	---------------------------------------------------------------------*/
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resourceViewDesc.ViewDimension           = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;
	resourceViewDesc.RaytracingAccelerationStructure.Location = dxBuffer->GetResource()->GetGPUVirtualAddress(); // tras�p��buffer

	std::uint32_t descriptorID = heap->Allocate(core::DescriptorHeapType::SRV);
	dxDevice->CreateShaderResourceView( dxBuffer->GetResourcePtr(), &resourceViewDesc, 
		heap->GetCPUDescHandler(core::DescriptorHeapType::SRV, descriptorID));

}
/****************************************************************************
*                     CreateUAV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateUAV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create unordered access view
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateUAV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_UNORDERED_ACCESS_VIEW_DESC resourceViewDesc = {};
	
	/*-------------------------------------------------------------------
	-             Setting Texture shader resource view
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
				resourceViewDesc.Texture3D.WSize       = 0;
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
	/*-------------------------------------------------------------------
	-             Setting Buffer shader resource view
	---------------------------------------------------------------------*/
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

	// Select heap descriptor id
	_heapOffset.first  = core::DescriptorHeapType::UAV;
	_heapOffset.second = heap->Allocate(core::DescriptorHeapType::UAV);
	// create unordered access view
	dxDevice->CreateUnorderedAccessView(
		resource, nullptr, &resourceViewDesc, heap->GetCPUDescHandler(core::DescriptorHeapType::UAV, _heapOffset.second));
}

/****************************************************************************
*                     CreateRTV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateRTV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create render target view
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateRTV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	/*-------------------------------------------------------------------
	-             Setting Texture Desc
	---------------------------------------------------------------------*/
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
	/*-------------------------------------------------------------------
	-             Setting Buffer Desc
	---------------------------------------------------------------------*/
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

	// set descriptor id
	_heapOffset.first  = core::DescriptorHeapType::RTV;
	_heapOffset.second = heap->Allocate(core::DescriptorHeapType::RTV);

	// create render target view 
	dxDevice->CreateRenderTargetView(resource, &desc, heap->GetCPUDescHandler(core::DescriptorHeapType::RTV, _heapOffset.second));
}

/****************************************************************************
*                     CreateDSV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateDSV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create depth stencil view
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateDSV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
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
				desc.ViewDimension                  = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice        = 0;
				desc.Texture2DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				desc.ViewDimension                    = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				desc.Texture2DMSArray.FirstArraySlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("wrong resource type (directX12 api)");
			}
		}

		// set heap descriptor id
		_heapOffset.first  = core::DescriptorHeapType::DSV;
		_heapOffset.second = heap->Allocate(core::DescriptorHeapType::DSV);

		// create depth stencil view
		dxDevice->CreateDepthStencilView(
			std::static_pointer_cast<directX12::GPUTexture>(_texture)->GetResource().Get(),
			&desc,
			heap->GetCPUDescHandler(core::DescriptorHeapType::DSV, _heapOffset.second)
		);
	}
	else
	{
		throw std::runtime_error("not support resource type (texture only)");
	}
}

/****************************************************************************
*                     CreateCBV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateCBV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
*  @brief     Create constant buffer view 
*  @param[in] const std::shared_ptr<directX12::RHIDescriptorHeap>
*  @return �@�@void
*****************************************************************************/
void GPUResourceView::CreateCBV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap)
{

	if (_buffer)
	{
		DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();
		const auto dxBuffer = std::static_pointer_cast<directX12::GPUBuffer>(_buffer);

		// Set up constant buffer view descriptor
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = dxBuffer->GetResource()->GetGPUVirtualAddress();
		desc.SizeInBytes    = static_cast<UINT>(_buffer->GetTotalByteSize());

		// 256 alignment check
		assert(desc.SizeInBytes % 256 == 0);

		// Set heap descriptor id.
		_heapOffset.first = core::DescriptorHeapType::CBV;
		_heapOffset.second = heap->Allocate(core::DescriptorHeapType::CBV);

		// create constant buffer view
		dxDevice->CreateConstantBufferView(&desc, heap->GetCPUDescHandler(core::DescriptorHeapType::CBV, _heapOffset.second));
	}
	else
	{
		throw std::runtime_error("not support resource type (buffer only)");
	}
}

/****************************************************************************
*                     SelectDescriptorHeap
*************************************************************************//**
*  @fn        const std::shared_ptr<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
*  @brief     Select DirectX12 Descriptor Heap. return custom heap or default heap 
*  @param[in] const core::DescriptorHeapType type
*  @return �@�@const std::shared_ptr<directX12::RHIDescriptorHeap> 
*****************************************************************************/
const std::shared_ptr<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
{
	// Set custom Heap
	if (_heap) { return std::static_pointer_cast<directX12::RHIDescriptorHeap>(_heap); }

	// Select default heap based on core::ResourceViewType
	std::shared_ptr<directX12::RHIDescriptorHeap> defaultHeap = nullptr;
	switch (type)
	{
		case core::ResourceViewType::ConstantBuffer: { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::CBV)); break; }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer:      { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break;}
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::UAV)); break;}
		case core::ResourceViewType::RenderTarget:          { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::RTV)); break;}
		case core::ResourceViewType::DepthStencil:          { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::DSV)); break;}
		case core::ResourceViewType::AccelerationStructure: { defaultHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break;}
		default:
		{
			throw std::runtime_error("not support descriptor view. (directX12 api)");
		}
	}
	_heap = defaultHeap;
	
	return defaultHeap;
}
#pragma endregion Setup view
#pragma region Property
D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::GetCPUHandler()
{
	const auto dxHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_heap);
	return dxHeap->GetCPUDescHandler(_heapOffset.first, _heapOffset.second);
}
D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceView::GetGPUHandler()
{
	const auto dxHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(_heap);
	return dxHeap->GetGPUDescHandler(_heapOffset.first, _heapOffset.second);
}
#pragma endregion Property