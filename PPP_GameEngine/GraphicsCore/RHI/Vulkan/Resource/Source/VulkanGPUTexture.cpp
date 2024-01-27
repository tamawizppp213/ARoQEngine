//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUSampler.cpp
///             @brief  Sampler State
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUTexture.hpp"
#include "../Include/VulkanGPUBuffer.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include "../../Core/Include/VulkanDevice.hpp"
#include "../../Core/Include/VulkanAdapter.hpp"
#include "../../Core/Include/VulkanInstance.hpp"
#include "../../Core/Include/VulkanCommandList.hpp"
#include "../../Core/Include/VulkanCommandAllocator.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <DirectXTex/DirectXTex.h> 
/* 本来であればDDSLoaderとstbimageを使う予定であるが, 各テクスチャ種類の区別が難しいため, 
   現時点では泣く泣くDirectXTexを使用する*/
#include <stdexcept>

#pragma warning(disable: 4189)
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
using namespace DirectX;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	rhi::core::PixelFormat ConvertDXGIIntoRHICoreFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			case DXGI_FORMAT_R8G8B8A8_UNORM     : return core::PixelFormat::R8G8B8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM     : return core::PixelFormat::B8G8R8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return core::PixelFormat::B8G8R8A8_UNORM_SRGB;
			case DXGI_FORMAT_R16G16B16A16_FLOAT : return core::PixelFormat::R16G16B16A16_FLOAT;
			case DXGI_FORMAT_R32G32B32A32_FLOAT : return core::PixelFormat::R32G32B32A32_FLOAT;
			case DXGI_FORMAT_R32G32B32_FLOAT    : return core::PixelFormat::R32G32B32_FLOAT;
			case DXGI_FORMAT_D24_UNORM_S8_UINT  : return core::PixelFormat::D24_UNORM_S8_UINT;
			case DXGI_FORMAT_R10G10B10A2_UNORM  : return core::PixelFormat::R10G10B10A2_UNORM;
			case DXGI_FORMAT_D32_FLOAT          : return core::PixelFormat::D32_FLOAT;
			case DXGI_FORMAT_BC1_UNORM          : return core::PixelFormat::BC1_UNORM;
			default:
				throw std::runtime_error("not supported Format type");
		}
	}
}

#pragma region Constructor and Destructor
GPUTexture::GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const gu::wstring& name)
	: core::GPUTexture(device, metaData, name)
{
	Prepare();
}
GPUTexture::GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const gu::wstring& name)
	: core::GPUTexture(device, name)
{
	_memory = nullptr;
}
GPUTexture::GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image, const gu::wstring& name)
	: core::GPUTexture(device, metaData, name), _image(image)
{
	_memory = nullptr;
}
GPUTexture::~GPUTexture()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	if (_memory)
	{
		vkFreeMemory(vkDevice, _memory, nullptr);
		vkDestroyImage(vkDevice, _image, nullptr);
	}
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     Load
*************************************************************************//**
*  @fn        void GPUTexture::Load(const gu::wstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList)
*
*  @brief     Load texture
*
*  @param[in] const gu::wstring& filePath
*  @param[in] const gu::SharedPointer<core::RHICommandList> graphics type commandList
*
*  @return 　　void
*****************************************************************************/
void GPUTexture::Load(const gu::wstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList)
{
#ifdef _DEBUG
	assert(_device);
	assert(commandList);
	assert(commandList->GetCommandAllocator()->GetCommandListType() == core::CommandListType::Graphics);
#endif

	const auto vkDevice      = static_cast<vulkan::RHIDevice*>(_device.Get())->GetDevice();
	const auto vkCommandList = static_cast<vulkan::RHICommandList*>(commandList.Get())->GetCommandList();

	/*-------------------------------------------------------------------
	-                Choose Extension and Load Texture Data
	---------------------------------------------------------------------*/
	const auto stdFilePath  = std::wstring(filePath.CString());
	const auto extension    = file::FileSystem::GetExtension(stdFilePath);
	const auto fileName     = file::FileSystem::GetFileName(stdFilePath, false);
	std::string utf8FilePath        = unicode::ToUtf8String(stdFilePath);

	/*-------------------------------------------------------------------
	-    Select the appropriate texture loading function for each extension
		 Load texture to the cpu memory
	---------------------------------------------------------------------*/
	TexMetadata  dxMetaData = {};
	ScratchImage scratchImage = {};
	if (extension == L"tga")
	{
		LoadFromTGAFile(filePath.CString(), TGA_FLAGS_NONE, &dxMetaData, scratchImage);
	}
	else if (extension == L"dds")
	{
		LoadFromDDSFile(filePath.CString(), DDS_FLAGS_NONE, &dxMetaData, scratchImage);
	}
	else if (extension == L"hdr")
	{
		LoadFromHDRFile(filePath.CString(), &dxMetaData, scratchImage);
	}
	else
	{
		LoadFromWICFile(filePath.CString(), WIC_FLAGS_NONE, &dxMetaData, scratchImage);
	}

	auto image = scratchImage.GetImage(0, 0, 0);
	const auto pixelSize = scratchImage.GetPixelsSize();

	/*-------------------------------------------------------------------
	-                 Create core texture metadata
	---------------------------------------------------------------------*/
	if (dxMetaData.IsCubemap())
	{
		_metaData = core::GPUTextureMetaData::CubeMap(image->width, image->height, ::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}
	else if (dxMetaData.IsVolumemap())
	{
		_metaData = core::GPUTextureMetaData::Texture3D(image->width, image->height, dxMetaData.depth, ::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}
	else
	{
		_metaData = core::GPUTextureMetaData::Texture2DArray(image->width, image->height, dxMetaData.arraySize,
			::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}
	// 必ず最初はUndefinedから始める.
	_metaData.State = rhi::core::ResourceState::Common;

	/*-------------------------------------------------------------------
	-                 Create texture buffers
	---------------------------------------------------------------------*/
	if (!_hasAllocated)
	{
		Prepare();
	}

	/*-------------------------------------------------------------------
	-                 Create staging buffer
	---------------------------------------------------------------------*/
	{
		const auto stagingInfo = core::GPUBufferMetaData::UploadBuffer
		(
			sizeof(std::uint8_t),
			pixelSize
		);

		_stagingBuffer = _device->CreateBuffer(stagingInfo, L"StagingBuffer");
		_stagingBuffer->Update(image->pixels, pixelSize);
	}

	/*-------------------------------------------------------------------
	-         Copy staging buffer to the main texture buffer
	---------------------------------------------------------------------*/
	const VkBufferImageCopy copyRegion = 
	{
		.bufferOffset      = 0,
		.bufferRowLength   = 0, // bufferはただただpixelを蓄えています. 
		.bufferImageHeight = 0,
		.imageSubresource  = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, (std::uint32_t)GetArrayLength()},
		.imageOffset       = {0,0,0},
		.imageExtent       = {(std::uint32_t)_metaData.Width, (std::uint32_t)_metaData.Height, (std::uint32_t)GetDepth()}
	};

	const auto vkBuffer      = gu::StaticPointerCast<vulkan::GPUBuffer>(_stagingBuffer)->GetBuffer();
	commandList->TransitionResourceState(SharedFromThis(), core::ResourceState::CopyDestination);
	vkCmdCopyBufferToImage(vkCommandList, vkBuffer, _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
	commandList->TransitionResourceState(SharedFromThis(), core::ResourceState::GeneralRead);

}
#pragma endregion Main Function
#pragma region Prepare 
/****************************************************************************
*                     Prepare
*************************************************************************//**
*  @fn        void GPUTexture::Prepare()
*
*  @brief     Prepare texture
*
*  @param[in] void

*  @return 　　void
*****************************************************************************/
void GPUTexture::Prepare()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Set Texture View Desc
	---------------------------------------------------------------------*/
	const VkImageCreateInfo imageInfo = 
	{
		.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext                 = nullptr,
		.flags                 = EnumConverter::Convert( _metaData.ResourceType, GetArrayLength()),                      // VkImageCreateFlags : image viewの作成の仕方
		.imageType             = EnumConverter::Convert(_metaData.Dimension),                   // image dimension type
		.format                = EnumConverter::Convert(_metaData.PixelFormat),                 // pixel format
		.extent                = VkExtent3D(static_cast<std::uint32_t>(_metaData.Width),        // texel size
		                                    static_cast<std::uint32_t>(_metaData.Height),       
		                        (_metaData.Dimension == core::ResourceDimension::Dimension3D ?
								static_cast<std::uint32_t>(_metaData.DepthOrArraySize) : 1)),
		.mipLevels             = static_cast<std::uint32_t>(_metaData.MipLevels),               // max mipmap levels
		.arrayLayers           = static_cast<uint32_t>(_metaData.Dimension == core::ResourceDimension::Dimension3D ? 
								  1 : _metaData.DepthOrArraySize),
		.samples               = EnumConverter::Convert(_metaData.Sample),
		.tiling                = VkImageTiling::VK_IMAGE_TILING_OPTIMAL,
		.usage                 = EnumConverter::Convert(_metaData.ResourceUsage).second,        // image resource usage
		.sharingMode           = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 0,
		.pQueueFamilyIndices   = nullptr,
		.initialLayout         = VK_IMAGE_LAYOUT_UNDEFINED // imageはUndefined or preinitialized のみ.
	};

	// EnumConverter::Convert(_metaData.State) initial layout?

	if (vkCreateImage(vkDevice, &imageInfo, nullptr, &_image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image (vulkan api)");
	}

	/*-------------------------------------------------------------------
	-           Compute memory size
	---------------------------------------------------------------------*/
	VkMemoryRequirements memoryRequirement = {};
	vkGetImageMemoryRequirements(vkDevice, _image, &memoryRequirement);

	/*-------------------------------------------------------------------
	-               Set Memory Allocate
	---------------------------------------------------------------------*/
	const VkMemoryAllocateInfo memoryInfo = 
	{
		.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.pNext           = nullptr,
		.allocationSize  = memoryRequirement.size,
		.memoryTypeIndex = gu::StaticPointerCast<vulkan::RHIDevice>(_device)
		->GetMemoryTypeIndex(memoryRequirement.memoryTypeBits, EnumConverter::Convert(_metaData.HeapType))
	};
	
	if (vkAllocateMemory(vkDevice, &memoryInfo, nullptr, &_memory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate memory");
	}

	/*-------------------------------------------------------------------
	-               Bind memory
	---------------------------------------------------------------------*/
	if (vkBindImageMemory(vkDevice, _image, _memory, 0) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to bind image memory");
	}
}
#pragma endregion Prepare
#pragma region Debug
/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void GPUBuffer::SetName(const gu::wstring& name)
*  @brief     Set Buffer Name
*  @param[in] const gu::wstring& name
*  @return 　　void
*****************************************************************************/
void GPUTexture::SetName(const gu::wstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_IMAGE, reinterpret_cast<std::uint64_t>(_image));
}
#pragma endregion Debug