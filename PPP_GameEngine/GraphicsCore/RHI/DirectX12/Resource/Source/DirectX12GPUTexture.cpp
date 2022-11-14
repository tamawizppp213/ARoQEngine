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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
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
namespace
{
	rhi::core::PixelFormat ConvertDXGIIntoRHICoreFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			case DXGI_FORMAT_R8G8B8A8_UNORM: return core::PixelFormat::R8G8B8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM: return core::PixelFormat::B8G8R8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return core::PixelFormat::B8G8R8A8_UNORM_SRGB;
			case DXGI_FORMAT_R16G16B16A16_FLOAT: return core::PixelFormat::R16G16B16A16_FLOAT;
			case DXGI_FORMAT_R32G32B32A32_FLOAT: return core::PixelFormat::R32G32B32A32_FLOAT;
			case DXGI_FORMAT_R32G32B32_FLOAT: return core::PixelFormat::R32G32B32_FLOAT;
			case  DXGI_FORMAT_D24_UNORM_S8_UINT: return core::PixelFormat::D24_UNORM_S8_UINT;
			case DXGI_FORMAT_R10G10B10A2_UNORM: return core::PixelFormat::R10G10B10A2_UNORM;
			case DXGI_FORMAT_D32_FLOAT: return core::PixelFormat::D32_FLOAT;

			default:
				throw std::runtime_error("not supported Format type");
		}
	}
}
GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device) : core::GPUTexture(device)
{
	
}

GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData)
	: core::GPUTexture(device, metaData)
{
	
	const auto dxDevice = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = {};
	ConvertDxMetaData(resourceDesc);
	AllocateGPUTextureBuffer(resourceDesc, _device->IsDiscreteGPU());
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
	_hasAllocated = true;
}

#pragma region Public Function
void GPUTexture::SetName(const std::wstring& name)
{
	_name = name;
	ThrowIfFailed(_resource->SetName(name.c_str()));
}

void GPUTexture::Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList)
{
	const auto dxDevice      = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxCommandList = static_cast<directX12::RHICommandList*>(commandList.get())->GetCommandList();
	
	/*-------------------------------------------------------------------
	-               Copy command list check
	---------------------------------------------------------------------*/
	assert(commandList->GetCommandAllocator()->GetCommandListType() == core::CommandListType::Graphics);
	/*-------------------------------------------------------------------
	-                Choose Extension and Load Texture Data
	---------------------------------------------------------------------*/
	TexMetadata  dxMetaData     = {};
	ScratchImage scratchImage = {};
	bool isDXT                = false;
	std::wstring extension    = file::FileSystem::GetExtension(filePath);
	
	/*-------------------------------------------------------------------
	-    Select the appropriate texture loading function for each extension
	---------------------------------------------------------------------*/
	if (extension == L"tga")
	{
		ThrowIfFailed(LoadFromTGAFile(filePath.c_str(), TGA_FLAGS_NONE, &dxMetaData, scratchImage));
	}
	else if (extension == L"dds")
	{
		ThrowIfFailed(LoadFromDDSFile(filePath.c_str(),DDS_FLAGS_NONE, &dxMetaData, scratchImage));
		isDXT = true;
	}
	else if (extension == L"hdr")
	{
		ThrowIfFailed(LoadFromHDRFile(filePath.c_str(), &dxMetaData, scratchImage));
	}
	else
	{
		ThrowIfFailed(LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, &dxMetaData, scratchImage));
	}
	auto image = scratchImage.GetImage(0, 0, 0);

	/*-------------------------------------------------------------------
	-                 Create core texture metadata
	---------------------------------------------------------------------*/
	_metaData = core::GPUTextureMetaData::Texture2DArray(image->width, image->height, dxMetaData.arraySize,
		::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	/*-------------------------------------------------------------------
	-                 Create directX12 texture buffer
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = {};
	ConvertDxMetaData(resourceDesc);

	if (!_hasAllocated)
	{
		AllocateGPUTextureBuffer(resourceDesc, _device->IsDiscreteGPU());
	}
	/*-------------------------------------------------------------------
	-                 pack
	---------------------------------------------------------------------*/
	if (_device->IsDiscreteGPU())
	{
		/*-------------------------------------------------------------------
		-                 Prepare Upload Buffer Setting
		---------------------------------------------------------------------*/
		std::vector<D3D12_SUBRESOURCE_DATA> subResources;
		ThrowIfFailed(PrepareUpload(dxDevice.Get(), image, scratchImage.GetImageCount(), dxMetaData, subResources));
		/*-------------------------------------------------------------------
		-                 Calculate Upload Buffer Size
		---------------------------------------------------------------------*/
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			_resource.Get(), 0, static_cast<UINT>(subResources.size()));
		/*-------------------------------------------------------------------
		-                 Create Upload Buffer
		---------------------------------------------------------------------*/
		D3D12_HEAP_PROPERTIES heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   uploadDesc = RESOURCE_DESC::Buffer(uploadBufferSize);
		
		dxDevice->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&uploadDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(_intermediateBuffer.GetAddressOf())
		);

		/*-------------------------------------------------------------------
		-                 Copy Texture Data
		---------------------------------------------------------------------*/
		const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(commandList)->GetCommandList();
		
		const auto beforeState = EnumConverter::Convert(_metaData.State);
		const auto before = BARRIER::Transition(_resource.Get(), beforeState, D3D12_RESOURCE_STATE_COPY_DEST);

		dxCommandList->ResourceBarrier(1, &before);
		UpdateSubresources(dxCommandList.Get(),
			_resource.Get(), _intermediateBuffer.Get(),
			0, 0, static_cast<unsigned int>(subResources.size()),
			subResources.data());

		auto after = BARRIER::Transition(_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, beforeState);
		dxCommandList->ResourceBarrier(1, &after);
	}
	else
	{
		ThrowIfFailed(_resource->WriteToSubresource(
			0, nullptr, 
			image->pixels, 
			static_cast<UINT>(image->rowPitch), 
			static_cast<UINT>(image->slicePitch)));
	}
}
#pragma endregion Public Function
void GPUTexture::Pack(const std::shared_ptr<core::RHICommandList>& commandList)
{
	
}
void GPUTexture::AllocateGPUTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, bool isDiscreteGPU)
{
	const auto dxDevice = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProperty = {};
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
		heapProperty.Type                 = EnumConverter::Convert(_metaData.HeapType);
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
	if (core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::RenderTarget))
	{
		clearValue.Color[0] = _metaData.ClearColor.Color[0];
		clearValue.Color[1] = _metaData.ClearColor.Color[1];
		clearValue.Color[2] = _metaData.ClearColor.Color[2];
		clearValue.Color[3] = _metaData.ClearColor.Color[3];
	}
	else
	{
		clearValue.DepthStencil.Depth   = _metaData.ClearColor.Depth;
		clearValue.DepthStencil.Stencil = _metaData.ClearColor.Stencil;
	}

	ThrowIfFailed(dxDevice->CreateCommittedResource(
		&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, EnumConverter::Convert(_metaData.State),
		core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::RenderTarget) || 
		core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::DepthStencil) ? &clearValue : nullptr,
		IID_PPV_ARGS(_resource.GetAddressOf())));

	/*-------------------------------------------------------------------
	-             Get total byte size and alighment size
	---------------------------------------------------------------------*/
	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &resourceDesc);
	_physicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	_alignment    = static_cast<size_t>(allocateInfo.Alignment);
	_hasAllocated = true;
}

void GPUTexture::ConvertDxMetaData(D3D12_RESOURCE_DESC& resourceDesc)
{
	resourceDesc.Dimension          = EnumConverter::Convert(_metaData.Dimension);
	resourceDesc.Alignment          = 0;
	resourceDesc.Width              = _metaData.Width;
	resourceDesc.Height             = static_cast<UINT>(_metaData.Height);
	resourceDesc.DepthOrArraySize   = static_cast<UINT16>(_metaData.DepthOrArraySize);
	resourceDesc.MipLevels          = static_cast<UINT16>(_metaData.MipLevels);
	resourceDesc.Format             = EnumConverter::Convert(_metaData.PixelFormat);
	resourceDesc.SampleDesc.Count   = static_cast<UINT>(_metaData.Sample);
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags              = EnumConverter::Convert(_metaData.ResourceUsage);

}