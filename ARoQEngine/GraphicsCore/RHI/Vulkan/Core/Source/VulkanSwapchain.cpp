//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanSwapchain.cpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanSwapchain.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFence.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <algorithm>
#include <stdexcept>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

#if defined(_WIN32)
#include <Windows.h>
#else
#include <X11/Xlib.h>  
#endif

#undef max
#undef min
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Swapchain Support 
namespace
{
	/*-------------------------------------------------------------------
	-       HDR and SDR pixel format and color space
	---------------------------------------------------------------------*/
	const gu::DynamicArray<VkFormat> g_HDRFormat =
	{
		 VK_FORMAT_R16G16B16A16_SFLOAT,
		 VK_FORMAT_A2B10G10R10_UNORM_PACK32,
		 VK_FORMAT_R32G32B32A32_SFLOAT
	};

	const gu::DynamicArray<VkFormat> g_SDRFormat =
	{
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_B8G8R8A8_SRGB
	};

	const gu::DynamicArray<VkColorSpaceKHR> g_HDRColorSpace =
	{
		VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT,
		VK_COLOR_SPACE_HDR10_ST2084_EXT,
		VK_COLOR_SPACE_HDR10_HLG_EXT,
		VK_COLOR_SPACE_DOLBYVISION_EXT
	};

	const gu::DynamicArray<VkColorSpaceKHR> g_SDRColorSpace =
	{
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR
	};

	bool CheckColorFormat(const gu::DynamicArray<VkFormat>& formats, VkFormat desiredFormat)
	{
		for (const auto format : formats)
		{
			if (format == desiredFormat) { return true; }
		}
		return false;
	}

	bool CheckColorSpace(const gu::DynamicArray<VkColorSpaceKHR>& colorSpaces, VkColorSpaceKHR desiredColorSpace)
	{
		for (const auto colorSpace : colorSpaces)
		{
			if (colorSpace == desiredColorSpace)
			{
				return true;
			}
		}
		return false;
	}

	/****************************************************************************
	*				  			SwapchainSupportDetails
	****************************************************************************/
	/* @struct    SwapchainSupportDetails
	*  @brief     Swapchain support (use query function)
	*****************************************************************************/
	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR         Capabilities = {}; // surface image presentation capability (image count, extent, composit alpha..)
		gu::DynamicArray<VkSurfaceFormatKHR>  Formats      = {}; // available surface color format list
		gu::DynamicArray<VkPresentModeKHR>    PresentModes = {}; // available surface present mode list 

		static SwapchainSupportDetails Query(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			/*-------------------------------------------------------------------
			-               Acquire Surface Capabilities
			---------------------------------------------------------------------*/
			SwapchainSupportDetails details = {};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);
			/*-------------------------------------------------------------------
			-               Acquire Surface Formats
			---------------------------------------------------------------------*/
			UINT32 formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			if (formatCount != 0)
			{
				details.Formats.Resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.Data());
			}
			/*-------------------------------------------------------------------
			-               Acquire Surface Presnent Mode
			---------------------------------------------------------------------*/
			UINT32 presentModeCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			if (presentModeCount != 0)
			{
				details.PresentModes.Resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.Data());
			}
			return details;
		}
	};
}
#pragma endregion Swapchain Support

#pragma region Constructor and Destructor
RHISwapchain::RHISwapchain(
	const gu::SharedPointer<rhi::core::RHIDevice>& device, 
	const gu::SharedPointer<rhi::core::RHICommandQueue>& commandQueue, 
	const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat,
	const size_t frameBufferCount, std::uint32_t vsync, bool isValidHDR) : rhi::core::RHISwapchain(device, commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR)
{
	SetUp();
}

RHISwapchain::RHISwapchain(const gu::SharedPointer<core::RHIDevice>& device, const core::SwapchainDesc& desc)
	: rhi::core::RHISwapchain(device, desc.CommandQueue, desc.WindowInfo, desc.PixelFormat, desc.FrameBufferCount, desc.VSync, desc.IsValidHDR)
{
	SetUp();
}

RHISwapchain::~RHISwapchain()
{
	const auto vkDevice  = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	const auto vkAdapter = gu::StaticPointerCast<rhi::vulkan::RHIDisplayAdapter>(vkDevice->GetDisplayAdapter());
	_vkImages.Clear(); _vkImages.ShrinkToFit();
	if (_renderingFinishedSemaphore) 
	{ 
		vkDestroySemaphore(vkDevice->GetDevice(), _renderingFinishedSemaphore, nullptr); 
		_renderingFinishedSemaphore = nullptr; 
	}
	if (_imageAvailableSemaphore) 
	{ 
		vkDestroySemaphore(vkDevice->GetDevice(), _imageAvailableSemaphore, nullptr); 
		_imageAvailableSemaphore = nullptr; 
	}
	if (_swapchain) 
	{ 
		vkDestroySwapchainKHR(vkDevice->GetDevice(), _swapchain, nullptr);
		_swapchain = nullptr; 
	}
	if (_surface) 
	{ 
		vkDestroySurfaceKHR(static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance())->GetVkInstance(), _surface, nullptr);
		_surface = nullptr; 
	}
}
#pragma endregion Constructor and Destructor
#pragma region Render Function
/****************************************************************************
*							SetUp
****************************************************************************/
/* @fn        void RHISwapchain::SetUp()
*  @brief     Prepare the swapchain setting
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::SetUp()
{
	const auto vkDevice   = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	const auto vkAdapter  = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(vkDevice->GetDisplayAdapter());
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance());
	
	/*-------------------------------------------------------------------
	-          Acquire surface (現状ではWindowsのみ対応) // 必要になり次第macroで管理する.
	---------------------------------------------------------------------*/
#if defined(_WIN32)
	{
		const VkWin32SurfaceCreateInfoKHR surfaceInfo = 
		{
			.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR, // Structure type
			.pNext     = 0,
			.flags     = 0,
			.hinstance = (HINSTANCE)_desc.WindowInfo.HInstance,           // hInstance
			.hwnd      = (HWND)_desc.WindowInfo.Handle                    // hwnd 
		}; // Window32 Surface. (! not cross platform)

		if (vkCreateWin32SurfaceKHR(vkInstance->GetVkInstance(), &surfaceInfo, nullptr, &_surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to prepare surface.");
		}
	}
#else
	{
		const VkXlibSurfaceCreateInfoKHR surfaceInfo =
		{
			.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR,
			.pNext = nullptr,
			.flags = 0,
			.dpy   = XOpenDisplay(nullptr),
		};
		 
		if (vkCreateXlibSurfaceKHR(vkInstance->GetVkInstance(), &surfaceInfo, nullptr, &_surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to prepare surface");
		}
	}
#endif
	/*-------------------------------------------------------------------
	-         null check
	---------------------------------------------------------------------*/
	if (_surface == nullptr) { throw std::runtime_error("failed to prepare surface"); }

	/*-------------------------------------------------------------------
	-          Set Swapchain
	---------------------------------------------------------------------*/
	InitializeSwapchain();
}

/****************************************************************************
*							PrepareNextImage
****************************************************************************/
/* @fn        std::uint32_t RHISwapchain::PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t signalValue)
*  @brief     When NextImage is ready, Signal is issued and the next frame Index is returned.
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
*  @param[in] std::uint64_t signalValue (Normally : ++fenceValueを代入)
*  @return 　　std::uint32_t back buffer index
*****************************************************************************/
std::uint32_t RHISwapchain::PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t signalValue)
{
	const auto vkFence  = gu::StaticPointerCast<vulkan::RHIFence>(fence);
	const auto vkQueue  = gu::StaticPointerCast<vulkan::RHICommandQueue>(_desc.CommandQueue);
	UpdateCurrentFrameIndex();

	/*-------------------------------------------------------------------
	-          Set up timeline semaphore submit info
	---------------------------------------------------------------------*/
	constexpr std::uint64_t temp = std::numeric_limits<std::uint64_t>::max();
	VkTimelineSemaphoreSubmitInfo timelineInfo = {};
	timelineInfo.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
	timelineInfo.waitSemaphoreValueCount   = 1;
	timelineInfo.pWaitSemaphoreValues      = &temp; // not use
	timelineInfo.signalSemaphoreValueCount = 1;
	timelineInfo.pSignalSemaphoreValues    = &signalValue; // for issue signal

	/*-------------------------------------------------------------------
	-          Set up submit info
	---------------------------------------------------------------------*/
	VkSubmitInfo signalSubmitInfo = {};
	signalSubmitInfo.sType                 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	signalSubmitInfo.pNext                 = &timelineInfo;
	signalSubmitInfo.waitSemaphoreCount    = 1;
	signalSubmitInfo.pWaitSemaphores       = &_imageAvailableSemaphore; 
	VkPipelineStageFlags waitDestStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
	signalSubmitInfo.pWaitDstStageMask     = &waitDestStageMask;
	signalSubmitInfo.signalSemaphoreCount  = 1;
	signalSubmitInfo.pSignalSemaphores     = &vkFence->GetFence();

	/*-------------------------------------------------------------------
	-          Submit queue
	---------------------------------------------------------------------*/
	auto result = vkQueueSubmit(vkQueue->GetQueue(), 1, &signalSubmitInfo, {});
	if (result != (VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
	{
		throw std::runtime_error("failed to present front buffer.");
	}

	return _currentBufferIndex;
}
/****************************************************************************
*							Present
****************************************************************************/
/* @fn        void RHISwapchain::Present()
*  @brief     Display front buffer
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
*  @param[in] std::uint64_t waitValue
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Present(const gu::SharedPointer<core::RHIFence>& fence, const std::uint64_t waitValue)
{
	const auto vkFence = gu::StaticPointerCast<vulkan::RHIFence>(fence);
	const auto vkQueue = gu::StaticPointerCast<vulkan::RHICommandQueue>(_desc.CommandQueue);

	/*-------------------------------------------------------------------
	-          Set up timeline semaphore submit info
	---------------------------------------------------------------------*/
	constexpr std::uint64_t temp = std::numeric_limits<std::uint64_t>::max();
	VkTimelineSemaphoreSubmitInfo timelineInfo = {};
	timelineInfo.sType                     = VK_STRUCTURE_TYPE_TIMELINE_SEMAPHORE_SUBMIT_INFO;
	timelineInfo.waitSemaphoreValueCount   = 1;
	timelineInfo.pWaitSemaphoreValues      = &waitValue;
	timelineInfo.signalSemaphoreValueCount = 1;
	timelineInfo.pSignalSemaphoreValues    = &temp;

	/*-------------------------------------------------------------------
	-          Set up submit info
	---------------------------------------------------------------------*/
	VkSubmitInfo signalSubmitInfo = {};
	signalSubmitInfo.sType                 = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	signalSubmitInfo.pNext                 = &timelineInfo;
	signalSubmitInfo.waitSemaphoreCount    = 1;
	signalSubmitInfo.pWaitSemaphores       = &vkFence->GetFence();
	VkPipelineStageFlags waitDestStageMask = VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT;
	signalSubmitInfo.pWaitDstStageMask     = &waitDestStageMask;
	signalSubmitInfo.signalSemaphoreCount  = 1;
	signalSubmitInfo.pSignalSemaphores     = &_renderingFinishedSemaphore;

	/*-------------------------------------------------------------------
	-          Submit queue
	---------------------------------------------------------------------*/
	{
		auto result = vkQueueSubmit(vkQueue->GetQueue(), 1, &signalSubmitInfo, {});
		if (result != (VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
		{
			throw std::runtime_error("failed to present front buffer.");
		}
	}

	VkPresentInfoKHR presentInfo   = {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount     = 1;
	presentInfo.pSwapchains        = &_swapchain;
	presentInfo.pImageIndices      = &_currentBufferIndex;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = &_renderingFinishedSemaphore;
	/*-------------------------------------------------------------------
	-               Present front buffer
	---------------------------------------------------------------------*/
	{
		auto result = vkQueuePresentKHR(vkQueue->GetQueue(), &presentInfo);
		if ((result != VK_SUCCESS) && (result != VK_SUBOPTIMAL_KHR))
		{
			throw std::runtime_error("failed to present front buffer.");
		}
	}
	

}
/****************************************************************************
*                     Resize
****************************************************************************/
/* @fn        void RHISwapchain::Resize(const size_t width, const size_t height)
*  @brief     Resize screen size. (set resized swapchain buffers )
*  @param[in] const size_t width
*  @param[in] const size_t height
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Resize(const size_t width, const size_t height)
{
	/*-------------------------------------------------------------------
	-          If the size is not change, we do nothing
	---------------------------------------------------------------------*/
	if (_desc.WindowInfo.Width == width && _desc.WindowInfo.Height) { return; }
	/*-------------------------------------------------------------------
	-         Window size check
	---------------------------------------------------------------------*/
	if (_desc.WindowInfo.Width == 0 || _desc.WindowInfo.Height == 0) { throw std::runtime_error("Width or height is zero."); }
	/*-------------------------------------------------------------------
	-         Set window size 
	---------------------------------------------------------------------*/
	_desc.WindowInfo.Width  = width;
	_desc.WindowInfo.Height = height;
	/*-------------------------------------------------------------------
	-         Destroy swapchain and present semaphore
	---------------------------------------------------------------------*/
	const auto vkDevice = gu::StaticPointerCast<rhi::vulkan::RHIDevice>(_device).Get()->GetDevice();
	for (auto& buffer : _backBuffers) { buffer.Reset(); }
	vkDestroySwapchainKHR(vkDevice, _swapchain, nullptr);
	vkDestroySemaphore(vkDevice, _imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(vkDevice, _renderingFinishedSemaphore, nullptr);
	/*-------------------------------------------------------------------
	-         Reset swapchain
	---------------------------------------------------------------------*/
	InitializeSwapchain();
}
/****************************************************************************
*                     GetCurrentBufferIndex
****************************************************************************/
/* @fn        size_t RHISwapchain::GetCurrentBufferIndex() const
*  @brief     Return current frame buffer
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t RHISwapchain::GetCurrentBufferIndex() const
{
	return static_cast<std::uint32_t>(_currentBufferIndex);
}
#pragma endregion   Render Function
#pragma region Set Up Function
/****************************************************************************
*                     InitializeSwapchain
****************************************************************************/
/* @fn        void RHISwapchain::InitializeSwapchain()
*  @brief     Initialize swapchain
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::InitializeSwapchain()
{
	const auto vkDevice = gu::StaticPointerCast<rhi::vulkan::RHIDevice>(_device);
	const auto vkAdapter = gu::StaticPointerCast<rhi::vulkan::RHIDisplayAdapter>(vkDevice->GetDisplayAdapter());

	/*-------------------------------------------------------------------
	-               Acquire Surface infomation
	---------------------------------------------------------------------*/
	const auto details          = SwapchainSupportDetails::Query(vkAdapter->GetPhysicalDevice(), _surface);
	      auto imageCount       = SelectImageCount(details.Capabilities);
	const auto surfaceFormat    = SelectSwapchainFormat(details.Formats);
	const auto presentMode      = SelectSwapchainPresentMode(details.PresentModes);
	const auto extent           = SelectSwapExtent(details.Capabilities);
	const auto imageArrayLayers = SelectImageArrayLayers(1, details.Capabilities.maxImageArrayLayers);
	const auto compositeAlpha   = SelectCompositeAlpha(details.Capabilities);
	const std::uint32_t queueFamilyIndex = static_cast<std::uint32_t>(vkDevice->GetQueueFamilyIndex(rhi::core::CommandListType::Graphics));
	
	/*-------------------------------------------------------------------
	-               Acquire Swapchain create infomation
	---------------------------------------------------------------------*/
	VkSwapchainCreateInfoKHR createInfo = 
	{
		.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,   // structure type
		.pNext                 = nullptr,
		.flags                 = 0,
		.surface               = _surface,                                      // display surface
		.minImageCount         = imageCount,                                    // frame buffer image count (default : 3)
		.imageFormat           = surfaceFormat.format,                          // pixel format : SDR B8G8R8A8 HDR : Float
		.imageColorSpace       = surfaceFormat.colorSpace,                      // color space 
		.imageExtent           = extent,                                        // screen pixel size
		.imageArrayLayers      = imageArrayLayers,
		.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE,                     // access to any range or image subresource of the object will be exclusive to a single queue family at a time.
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices   = &queueFamilyIndex,
		.preTransform          = details.Capabilities.currentTransform,         // current monitor rotation (default identity)
		.compositeAlpha        = compositeAlpha,                                // ウィンドウシステムによるアルファ合成の扱い
		.presentMode           = presentMode,
		.clipped               = VK_TRUE,                                       // 画面が隠れている部分の描画処理をなくす.
		.oldSwapchain          = _swapchain                                     // basically nullptr
	};


	/*-------------------------------------------------------------------
	-               Create Swapchain KHR
	---------------------------------------------------------------------*/
	if (vkCreateSwapchainKHR(vkDevice->GetDevice(), &createInfo, nullptr, &_swapchain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swapchain (vulkan api)");
	}
	
	/*-------------------------------------------------------------------
	-               Set HDR metadata
	---------------------------------------------------------------------*/
	if (vkDevice->IsSupportedHDR() && _desc.IsValidHDR)
	{
		
		const VkHdrMetadataEXT hdrMetadata = 
		{
			.sType = VK_STRUCTURE_TYPE_HDR_METADATA_EXT,
			.pNext = nullptr,
			.displayPrimaryRed         = { 0.70800f, 0.29200f },
			.displayPrimaryGreen       = { 0.17000f, 0.79700f },
			.displayPrimaryBlue        = { 0.13100f, 0.04600f },
			.whitePoint                = { 0.31270f, 0.32900f },
			.maxLuminance              = 1000.0f,
			.minLuminance              = 0.001f,
			.maxContentLightLevel      = 2000.0f,
			.maxFrameAverageLightLevel = 500.0f
		};

		PFN_vkSetHdrMetadataEXT pfnVkSetMetadataEXT 
			= reinterpret_cast<PFN_vkSetHdrMetadataEXT>(vkGetDeviceProcAddr(vkDevice->GetDevice(),"vkSetHdrMetadataEXT"));
		pfnVkSetMetadataEXT(vkDevice->GetDevice(), 1, &_swapchain, &hdrMetadata);
	}

	/*-------------------------------------------------------------------
	-               Get swapchain images
	---------------------------------------------------------------------*/
	_vkImages     .Resize(imageCount);
	_backBuffers.Resize(imageCount);
	if (vkGetSwapchainImagesKHR(vkDevice->GetDevice(), _swapchain, &imageCount, _vkImages.Data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to get swapchain images");
	}

	for (size_t index = 0; index < _vkImages.Size(); ++index)
	{
		auto info = core::GPUTextureMetaData::Texture2D(
			static_cast<size_t>(_desc.WindowInfo.Width), static_cast<size_t>(_desc.WindowInfo.Height),
			_desc.PixelFormat, 1, core::TextureCreateFlags::RenderTargetable);

		info.State = core::ResourceState::Common;

		_backBuffers[index] = gu::MakeShared<vulkan::GPUTexture>(_device, info, _vkImages[index]);
	}

	// もしかしたらここにCommandListのClose処理が入るかも.

	/*-------------------------------------------------------------------
	-               Create Semaphore
	---------------------------------------------------------------------*/
	const VkSemaphoreCreateInfo semaphoreCreateInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0
	};
	                          
	if (vkCreateSemaphore(vkDevice->GetDevice(), &semaphoreCreateInfo, nullptr, &_imageAvailableSemaphore   ) != VK_SUCCESS) { throw std::runtime_error("failed to create image available semaphore"); }
	if (vkCreateSemaphore(vkDevice->GetDevice(), &semaphoreCreateInfo, nullptr, &_renderingFinishedSemaphore) != VK_SUCCESS) { throw std::runtime_error("failed to create rendering finished semaphore"); }
}
/****************************************************************************
*							GetCurrentBufferIndex
****************************************************************************/
/* @fn        size_t RHISwapchain::GetCurrentBufferIndex() const
*  @brief     Get current buffer
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
void RHISwapchain::UpdateCurrentFrameIndex()
{
	const auto vkDevice       = gu::StaticPointerCast<rhi::vulkan::RHIDevice>(_device);
	
	/*-------------------------------------------------------------------
	-               Get next frame buffer index
	---------------------------------------------------------------------*/
	auto result = vkAcquireNextImageKHR(vkDevice.Get()->GetDevice(), _swapchain, UINT64_MAX, _imageAvailableSemaphore, VK_NULL_HANDLE, &_currentBufferIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to go to next frame");
	}
}
#pragma endregion   Set Up Function
#pragma region Swap Chain Config
/****************************************************************************
*                     SelectSwapchainFormat
****************************************************************************/
/* @fn        VkSurfaceFormatKHR RHISwapchain::SelectSwapchainFormat(const gu::DynamicArray<VkSurfaceFormatKHR>& format)
* 
*  @brief     Select swapchain format
* 
*  @param[in] const gu::DynamicArray<VkSurfaceFormatKHR>& format
* 
*  @return 　　VkSurfaceFormatKHR
*****************************************************************************/
VkSurfaceFormatKHR RHISwapchain::SelectSwapchainFormat(const gu::DynamicArray<VkSurfaceFormatKHR>& formats)
{
	const auto vkDevice = gu::StaticPointerCast<rhi::vulkan::RHIDevice>(_device);

	for (const auto& format : formats)
	{
		/*-------------------------------------------------------------------
		-               Find HDR Format
		---------------------------------------------------------------------*/
		if (vkDevice->IsSupportedHDR() && _desc.IsValidHDR)
		{
			if (!CheckColorFormat(g_HDRFormat    , format.format))     { continue; }
			if (!CheckColorSpace (g_HDRColorSpace, format.colorSpace)) { continue; }

			return format;
		}
		/*-------------------------------------------------------------------
		-               Find SDR Format
		---------------------------------------------------------------------*/
		else
		{
			if (!CheckColorFormat(g_SDRFormat, format.format)) { continue; }
			if (!CheckColorSpace(g_SDRColorSpace, format.colorSpace)) { continue; }

			ChangeSDRFormat(format.format);
			return format;
		}
	}

	return formats[0];
}

void RHISwapchain::ChangeSDRFormat(const VkFormat format)
{
	if (format == VK_FORMAT_B8G8R8A8_UNORM) { _desc.PixelFormat = core::PixelFormat::B8G8R8A8_UNORM; }
	if (format == VK_FORMAT_B8G8R8A8_SRGB ) { _desc.PixelFormat = core::PixelFormat::B8G8R8A8_UNORM_SRGB; }
}
/****************************************************************************
*                     SelectSwapchainPresentMode
****************************************************************************/
/* @fn        VkPresentModeKHR GraphicsDeviceVulkan::SelectSwapchainPresentMode(const gu::DynamicArray<VkPresentModeKHR>& presentMode)
*  @brief     Select vsync mode
*  @param[in] const gu::DynamicArray<VkPresentModeKHR>& presentMode
*  @return 　　VkPresentModeKHR
*****************************************************************************/
VkPresentModeKHR RHISwapchain::SelectSwapchainPresentMode(const gu::DynamicArray<VkPresentModeKHR>& presentModes)
{
	for (const auto& presentMode : presentModes)
	{
		if (_desc.VSync == 0 && presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) { return presentMode; } // immediate flip screen
		if (_desc.VSync > 0  && presentMode == VK_PRESENT_MODE_MAILBOX_KHR)   { return presentMode; } // wait vsync time
		if (_desc.VSync > 0  && presentMode == VK_PRESENT_MODE_FIFO_KHR)      { return presentMode; }
	}
	return VK_PRESENT_MODE_FIFO_KHR; // wait vsync time
}
/****************************************************************************
*                     SelectSwapExtent
****************************************************************************/
/* @fn        VkExtent2D  GraphicsDeviceVulkan::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
*  @brief     Select swapchain screen size
*  @param[in] const gu::DynamicArray<VkPresentModeKHR>& presentMode
*  @return 　　VkExtent2D
*****************************************************************************/
VkExtent2D RHISwapchain::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// https://wasuregasa.3rin.net/%E3%82%A2%E3%83%9E%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0/c--%20%E3%81%AE%20std--numeric_limits
	if (capabilities.currentExtent.width != (std::numeric_limits<unsigned int>::max)())
	{
		return capabilities.currentExtent;
	}
	else
	{
		const int width               = static_cast<int>(Screen::GetScreenWidth());
		const int height              = static_cast<int>(Screen::GetScreenHeight());
		
		if (width <= 0) { throw std::runtime_error("width needs to over 0"); }
		if (height <= 0) { throw std::runtime_error("height needs to over 0"); }

		VkExtent2D actualExtent = { static_cast<UINT32>(width), static_cast<UINT32>(height) };
		actualExtent.width            = std::clamp(actualExtent.width , capabilities.minImageExtent.width , capabilities.maxImageExtent.width);
		actualExtent.height           = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}

std::uint32_t RHISwapchain::SelectImageArrayLayers(std::uint32_t request, const std::uint32_t maxLayers)
{
	request = std::min(request, maxLayers);
	request = std::max(request, 1u);
	return request;
}

std::uint32_t RHISwapchain::SelectImageCount(const VkSurfaceCapabilitiesKHR& capabilities)
{
	UINT32 imageCount = static_cast<UINT32>(_desc.FrameBufferCount);
	if (capabilities.minImageCount > 0 && imageCount < capabilities.minImageCount)
	{
		imageCount = capabilities.minImageCount;
	}
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
	{
		imageCount = capabilities.maxImageCount;
	}

	return imageCount;
}

VkCompositeAlphaFlagBitsKHR RHISwapchain::SelectCompositeAlpha(const VkSurfaceCapabilitiesKHR& capabilities)
{
	const VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[] =
	{
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
	};

	for (size_t index = 0; index < _countof(compositeAlphaFlags); index++)
	{
		if (capabilities.supportedCompositeAlpha & compositeAlphaFlags[index])
		{
			return compositeAlphaFlags[index];
		}
	}

	return VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
}
#pragma endregion Swap Chain Config