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
GPUResourceView::GPUResourceView(
	const gu::SharedPointer<core::RHIDevice>& device, 
	const core::ResourceViewType type, 
	const gu::SharedPointer<core::GPUBuffer>& buffer,
	const gu::uint32 mipSlice,
	const gu::uint32 planeSlice,
	const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, mipSlice, planeSlice, customHeap)
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

GPUResourceView::GPUResourceView(
	const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUTexture>& texture, 
	const gu::uint32 mipSlice, const gu::uint32 planeSlice,
	const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
	: core::GPUResourceView(device, type, mipSlice, planeSlice, customHeap)
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

GPUResourceView::~GPUResourceView()
{
	if (_hasCreated)
	{
		const auto heapType = _heapOffset.first;
		const auto id       = _heapOffset.second;
		const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_heap);
		dxHeap->Free(heapType, id);
	}
}
#pragma endregion Constructor and Destructor

#pragma region Bind Function
/****************************************************************************
*                     Bind
*************************************************************************//**
*  @fn        void GPUResourceView::Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const std::uint32_t index)
*
*  @brief     Bind resource layout array index to the command list.
              index : resource layout array index

*  @param[in] const gu::SharedPointer<core::RHICommandList>& commandList pointer
*  @param[in] const gu::uint32_t resource layout array index.
*
*  @return 　　void
*****************************************************************************/
void GPUResourceView::Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const gu::uint32 index, [[maybe_unused]]const gu::SharedPointer<core::RHIResourceLayout>& layout)
{
	/*-------------------------------------------------------------------
	-             Set Descirptor Table
	---------------------------------------------------------------------*/
	const auto dxCommandList = gu::StaticPointerCast<directX12::RHICommandList>(commandList)->GetCommandList();
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
		// Copy command list cannot bind. 
		throw std::runtime_error("failed to bind root descriptor table");
	}
}

#pragma endregion Bind Function
#pragma region Setup view
void GPUResourceView::CreateView(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
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
	_hasCreated = true;
}

/****************************************************************************
*                     CreateSRV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateSRV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create shader resource view
* 
*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateSRV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();
	
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
		resourceViewDesc.Format = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_texture->GetPixelFormat()).PlatformFormat;
		
		// For depth stencil texture
		// EnumConverterではD32が渡されてしまうため, 特別に以下の対応を行いました. 
		if (_texture->GetPixelFormat() == core::PixelFormat::D32_FLOAT)
		{
			resourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}

		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1D;
				resourceViewDesc.Texture1D.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.Texture1D.MostDetailedMip     = 0;
				resourceViewDesc.Texture1D.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::Texture2D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.Texture2D.MostDetailedMip     = 0; 
				resourceViewDesc.Texture2D.PlaneSlice          = static_cast<UINT>(_planeSlice);
				resourceViewDesc.Texture2D.ResourceMinLODClamp = 0; // Accessible all the mipmap level resources. 

				break;
			}
			case core::ResourceType::Texture3D:
			{
				resourceViewDesc.ViewDimension                 = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE3D;
				resourceViewDesc.Texture3D.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.Texture3D.MostDetailedMip     = 0;
				resourceViewDesc.Texture3D.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::TextureCube:
			{
				resourceViewDesc.ViewDimension                   = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURECUBE;
				resourceViewDesc.TextureCube.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.TextureCube.MostDetailedMip     = 0;
				resourceViewDesc.TextureCube.ResourceMinLODClamp = 0.0f;
				break;
			}
			case core::ResourceType::Texture1DArray:
			{
				resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
				resourceViewDesc.Texture1DArray.FirstArraySlice     = 0;
				resourceViewDesc.Texture1DArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.Texture1DArray.MostDetailedMip     = 0;
				resourceViewDesc.Texture1DArray.ArraySize           = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture1DArray.ResourceMinLODClamp = 0; // Accessible all the mipmap level resources. 
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				resourceViewDesc.ViewDimension                      = D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice     = 0;
				resourceViewDesc.Texture2DArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
				resourceViewDesc.Texture2DArray.MostDetailedMip     = 0;
				resourceViewDesc.Texture2DArray.ArraySize           = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture2DArray.PlaneSlice          = _planeSlice;
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
				resourceViewDesc.TextureCubeArray.MipLevels           = static_cast<UINT>(_texture->GetMipMapCount());
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
		const auto dxTexture = gu::StaticPointerCast<directX12::GPUTexture>(_texture);
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
		const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);
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
*  @fn        void GPUResourceView::CreateRAS(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create raytracing acceleration structure

*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateRAS(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{
	if (!_buffer) { return; }

	const auto rhiDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto dxDevice  = rhiDevice->GetDevice();
	const auto dxBuffer  = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);

	// enable to use the ray tracing
	if (!rhiDevice->IsSupportedDxr()) { return; }

	/*-------------------------------------------------------------------
	-             Set up resource view descriptor
	---------------------------------------------------------------------*/
	D3D12_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
	resourceViewDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	resourceViewDesc.ViewDimension           = D3D12_SRV_DIMENSION_RAYTRACING_ACCELERATION_STRUCTURE;

	// offsetは今回は考えない. 
	resourceViewDesc.RaytracingAccelerationStructure.Location = dxBuffer->GetResource()->GetGPUVirtualAddress(); // tras用のbuffer

	// shader resource viewとして作成する.
	gu::uint32 descriptorID = heap->Allocate(core::DescriptorHeapType::SRV);
	dxDevice->CreateShaderResourceView( dxBuffer->GetResourcePtr(), &resourceViewDesc, 
		heap->GetCPUDescHandler(core::DescriptorHeapType::SRV, descriptorID));

}

/****************************************************************************
*                     CreateUAV
*************************************************************************//**
*  @fn        void GPUResourceView::CreateUAV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create unordered access view
* 
*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateUAV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_UNORDERED_ACCESS_VIEW_DESC resourceViewDesc = {};
	
	/*-------------------------------------------------------------------
	-             Setting Texture shader resource view
	---------------------------------------------------------------------*/
	if (_texture)
	{
		resourceViewDesc.Format = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_texture->GetPixelFormat()).PlatformFormat;
		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1D;
				resourceViewDesc.Texture1D.MipSlice = static_cast<UINT>(_mipSlice);
				break;
			}
			case core::ResourceType::Texture2D:
			{
				resourceViewDesc.ViewDimension        = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MipSlice   = static_cast<UINT>(_mipSlice);
				resourceViewDesc.Texture2D.PlaneSlice = static_cast<UINT>(_planeSlice);
				break;
			}
			case core::ResourceType::Texture3D:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE3D;
				resourceViewDesc.Texture3D.FirstWSlice = 0;
				resourceViewDesc.Texture3D.MipSlice    = static_cast<UINT>(_mipSlice);
				resourceViewDesc.Texture3D.WSize       = 0;
				break;
			}
			case core::ResourceType::Texture1DArray:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
				resourceViewDesc.Texture1DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture1DArray.MipSlice        = static_cast<UINT>(_mipSlice);
				resourceViewDesc.Texture1DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DArray.MipSlice   = static_cast<UINT>(_mipSlice);
				resourceViewDesc.Texture2DArray.ArraySize  = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture2DArray.PlaneSlice = static_cast<UINT>(_planeSlice);
				break;
			}
			case core::ResourceType::TextureCube:
			case core::ResourceType::TextureCubeArray:
			{
				resourceViewDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DArray.ArraySize       = static_cast<UINT>(_texture->GetArrayLength());
				resourceViewDesc.Texture2DArray.MipSlice        = static_cast<UINT>(_mipSlice);
				resourceViewDesc.Texture2DArray.PlaneSlice      = static_cast<UINT>(_planeSlice);
				break;
			}
			default:
			{
				throw std::runtime_error("not supported texture resource type");
				break;
			}
		}
		
		const auto dxTexture = gu::StaticPointerCast<directX12::GPUTexture>(_texture);
		resource = dxTexture->GetResource().Get();
	}
	/*-------------------------------------------------------------------
	-             Setting Buffer shader resource view
	---------------------------------------------------------------------*/
	else if (_buffer)
	{
		if (_buffer->GetResourceType() == core::ResourceType::Buffer)
		{
			resourceViewDesc.ViewDimension               = D3D12_UAV_DIMENSION_BUFFER;
			resourceViewDesc.Format                      = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_buffer->GetMetaData().Format).PlatformFormat;
			resourceViewDesc.Buffer.CounterOffsetInBytes = 0;
			resourceViewDesc.Buffer.FirstElement         = 0;
			resourceViewDesc.Buffer.Flags                = D3D12_BUFFER_UAV_FLAG_NONE;
			resourceViewDesc.Buffer.NumElements          = static_cast<UINT>(_buffer->GetElementCount());
			resourceViewDesc.Buffer.StructureByteStride  = static_cast<UINT>(_buffer->GetElementByteSize());
		}
		else
		{
			throw std::runtime_error("unknown resource type. Select resource type (texture or buffer)");
		}
		
		const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);
		resource = dxBuffer->GetResource().Get();
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
*  @fn        void GPUResourceView::CreateRTV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create render target view
* 
*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateRTV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();

	Resource* resource = nullptr;
	D3D12_RENDER_TARGET_VIEW_DESC desc = {};
	/*-------------------------------------------------------------------
	-             Setting Texture Desc
	---------------------------------------------------------------------*/
	if (_texture)
	{
		desc.Format = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_texture->GetPixelFormat()).PlatformFormat;

		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture1D:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION::D3D12_RTV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = static_cast<UINT>(_mipSlice);
				break;
			}
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice   = static_cast<UINT>(_mipSlice);
				desc.Texture2D.PlaneSlice = static_cast<UINT>(_planeSlice);
				break;
			}
			case core::ResourceType::Texture3D:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice    = static_cast<UINT>(_mipSlice);
				desc.Texture3D.WSize       = 0;
				desc.Texture3D.FirstWSlice = 0;
				break;
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
				desc.Texture2DArray.MipSlice        = static_cast<UINT>(_mipSlice);
				desc.Texture2DArray.PlaneSlice      = static_cast<UINT>(_planeSlice);
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

		const auto dxTexture = gu::StaticPointerCast<directX12::GPUTexture>(_texture);
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

		const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);
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
*  @fn        void GPUResourceView::CreateDSV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create depth stencil view
* 
*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateDSV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{
	DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();

	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	if (_texture)
	{
		desc.Format = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_texture->GetPixelFormat()).PlatformFormat;
		desc.Flags  = D3D12_DSV_FLAG_NONE;
		switch (_texture->GetResourceType())
		{
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = static_cast<UINT>(_mipSlice);
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
				desc.Texture2DArray.MipSlice        = static_cast<UINT>(_mipSlice);
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
			gu::StaticPointerCast<directX12::GPUTexture>(_texture)->GetResource().Get(),
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
*  @fn        void GPUResourceView::CreateCBV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
* 
*  @brief     Create constant buffer view 
* 
*  @param[in] const gu::SharedPointer<directX12::RHIDescriptorHeap>
* 
*  @return 　　void
*****************************************************************************/
void GPUResourceView::CreateCBV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap)
{

	if (_buffer)
	{
		DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();
		const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);

		// Set up constant buffer view descriptor
		D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
		desc.BufferLocation = dxBuffer->GetResource()->GetGPUVirtualAddress();
		desc.SizeInBytes    = static_cast<UINT>(_buffer->GetTotalByteSize());

#if _DEBUG
		// 256 alignment check
		assert(desc.SizeInBytes % 256 == 0);
#endif
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
*  @fn        const gu::SharedPointer<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
* 
*  @brief     Select DirectX12 Descriptor Heap. return custom heap or default heap 
* 
*  @param[in] const core::DescriptorHeapType type
* 
*  @return 　　const gu::SharedPointer<directX12::RHIDescriptorHeap> 
*****************************************************************************/
const gu::SharedPointer<directX12::RHIDescriptorHeap> GPUResourceView::SelectDescriptorHeap(const core::ResourceViewType type)
{
	// Set custom Heap
	if (_heap) { return gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_heap); }

	// Select default heap based on core::ResourceViewType
	gu::SharedPointer<directX12::RHIDescriptorHeap> defaultHeap = nullptr;
	switch (type)
	{
		case core::ResourceViewType::ConstantBuffer: { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::CBV)); break; }
		case core::ResourceViewType::Texture:
		case core::ResourceViewType::Buffer:
		case core::ResourceViewType::StructuredBuffer:      { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break;}
		case core::ResourceViewType::RWBuffer:
		case core::ResourceViewType::RWTexture:
		case core::ResourceViewType::RWStructuredBuffer:    { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::UAV)); break;}
		case core::ResourceViewType::RenderTarget:          { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::RTV)); break;}
		case core::ResourceViewType::DepthStencil:          { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::DSV)); break;}
		case core::ResourceViewType::AccelerationStructure: { defaultHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_device->GetDefaultHeap(core::DescriptorHeapType::SRV)); break;}
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
D3D12_CPU_DESCRIPTOR_HANDLE GPUResourceView::GetCPUHandler() const
{
	const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_heap);
	return dxHeap->GetCPUDescHandler(_heapOffset.first, _heapOffset.second);
}
D3D12_GPU_DESCRIPTOR_HANDLE GPUResourceView::GetGPUHandler() const
{
	const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(_heap);
	return dxHeap->GetGPUDescHandler(_heapOffset.first, _heapOffset.second);
}
#pragma endregion Property