//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUSampler.cpp
///             @brief  Sampler State
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"

#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData)
	: core::GPUTexture(device, metaData)
{
	Prepare();
}
GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device)
	: core::GPUTexture(device)
{
	_memory = nullptr;
}
GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const VkImage image)
	: core::GPUTexture(device, metaData), _image(image)
{
	_memory = nullptr;
}
GPUTexture::~GPUTexture()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	if (_memory)
	{
		vkFreeMemory(vkDevice, _memory, nullptr);
		vkDestroyImage(vkDevice, _image, nullptr);
	}
}
#pragma endregion Constructor and Destructor
#pragma region Prepare 
void GPUTexture::Prepare()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-               Set Texture View Desc
	---------------------------------------------------------------------*/
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType                 = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.flags                 = EnumConverter::Convert(GetArrayLength());
	imageInfo.imageType             = EnumConverter::Convert(_metaData.Dimension);
	imageInfo.extent                = VkExtent3D(static_cast<std::uint32_t>(_metaData.Width), static_cast<std::uint32_t>(_metaData.Height), 
		                              (_metaData.Dimension == core::ResourceDimension::Dimension3D ? static_cast<std::uint32_t>(_metaData.DepthOrArraySize) : 1));
	imageInfo.mipLevels             = static_cast<std::uint32_t>(_metaData.MipLevels);
	imageInfo.arrayLayers           = static_cast<uint32_t>(_metaData.Dimension == core::ResourceDimension::Dimension3D ? 1 : _metaData.DepthOrArraySize);
	imageInfo.samples               = EnumConverter::Convert(_metaData.Sample);
	imageInfo.initialLayout         = EnumConverter::Convert(_metaData.State);
	imageInfo.usage                 = EnumConverter::Convert(_metaData.ResourceUsage).second;
	imageInfo.queueFamilyIndexCount = 0;
	imageInfo.pQueueFamilyIndices   = nullptr;
	imageInfo.sharingMode           = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
	imageInfo.pNext                 = nullptr;
	imageInfo.tiling                = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
	imageInfo.format                = EnumConverter::Convert(_metaData.PixelFormat);
	if (vkCreateImage(vkDevice, &imageInfo, nullptr, &_image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image (vulkan api)");
	}

	/*-------------------------------------------------------------------
	-               Set Memory Allocate
	---------------------------------------------------------------------*/
	VkMemoryAllocateInfo memoryInfo = {};
	VkMemoryRequirements requirements = {};
	vkGetImageMemoryRequirements(vkDevice, _image, &requirements);
	_physicalSize = requirements.size;
	_alignment    = requirements.alignment;
	
	memoryInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryInfo.pNext = nullptr;
	memoryInfo.allocationSize = _physicalSize;
	memoryInfo.memoryTypeIndex = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetMemoryTypeIndex(requirements.memoryTypeBits, EnumConverter::Convert(_metaData.HeapType));
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
*  @fn        void GPUBuffer::SetName(const std::wstring& name)
*  @brief     Set Buffer Name
*  @param[in] const std::wstring& name
*  @return Å@Å@void
*****************************************************************************/
void GPUTexture::SetName(const std::wstring& name)
{
	std::string utf8Name = unicode::ToUtf8String(name);

	VkDebugUtilsObjectNameInfoEXT info = {};
	info.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;  // structure type
	info.pNext        = nullptr;                                             // no extension
	info.objectType   = VK_OBJECT_TYPE_IMAGE;                                // object type : image
	info.objectHandle = reinterpret_cast<std::uint64_t>(_image);             // vkBuffer to std::uint64_t
	info.pObjectName  = utf8Name.c_str();                                     
	
	/*-------------------------------------------------------------------
	-          GetInstance
	---------------------------------------------------------------------*/
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(vkDevice->GetDisplayAdapter());
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance())->GetVkInstance();

	/*-------------------------------------------------------------------
	-          SetName
	---------------------------------------------------------------------*/
	auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(vkInstance, "vkSetDebugUtilsObjectNameEXT");
	if (func(vkDevice->GetDevice(), &info) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set buffer name");
	}
	_name = name;
}
#pragma endregion Debug