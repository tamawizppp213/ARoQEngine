//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanSwapchain.hpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_SWAP_CHAIN_HPP
#define VULKAN_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#else
#define VK_USE_PLATFORM_XLIB_KHR
#endif
#include <vulkan/vulkan.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Fence
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHISwapchain
	****************************************************************************/
	/* @class     RHISwapchain
	*  @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public rhi::core::RHISwapchain
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : When NextImage is ready, Signal is issued and the next frame Index is returned. */
		gu::uint32 PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue) override;
		
		/* @brief : Display front buffer*/
		void Present(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t waitValue) override ;
		
		/* @brief : Resize screen size. Rebuild everything once and update again.*/
		void Resize(const size_t width, const size_t height) override ;
		
		/* @brief : Return current frame buffer*/
		size_t GetCurrentBufferIndex() const override ;

		/*----------------------------------------------------------------------
		*  @brief :  フルスクリーンモードと指定された解像度のスクリーンモードを切り替える
		* 　　　　　　  (isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する)
		*----------------------------------------------------------------------*/
		void SwitchFullScreenMode([[maybe_unused]] const bool isOn) override {};
		
		/*----------------------------------------------------------------------
		*  @brief :  HDRモードとSDRモードを切り替える
		*----------------------------------------------------------------------*/
		void SwitchHDRMode([[maybe_unused]]const bool enableHDR) override {};

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkSwapchainKHR GetSwapchain() { return _swapchain; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHISwapchain() = default;

		explicit RHISwapchain(const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const gu::SharedPointer<rhi::core::RHICommandQueue>& commandQueue,
			const core::WindowInfo& windowInfo,
			const core::PixelFormat& pixelFormat,
			const size_t frameBufferCount = 3,std::uint32_t vsync = 0, bool isValidHDR = true);

		explicit RHISwapchain(const gu::SharedPointer<core::RHIDevice>& device,
			const core::SwapchainDesc& desc);

		~RHISwapchain();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUp();

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkSwapchainKHR _swapchain = VK_NULL_HANDLE;

		VkSurfaceKHR   _surface   = nullptr;

		VkSemaphore    _imageAvailableSemaphore    = nullptr;
		
		VkSemaphore    _renderingFinishedSemaphore = nullptr;
		
		std::uint32_t _currentBufferIndex = 0;
		
		gu::DynamicArray<VkImage> _vkImages;
	private:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void InitializeSwapchain();

		void ChangeSDRFormat(const VkFormat format);
		
		void UpdateCurrentFrameIndex();
		
		/** @brief Select color format and color space format*/
		VkSurfaceFormatKHR SelectSwapchainFormat(const gu::DynamicArray<VkSurfaceFormatKHR>& format);
		
		/** @brief Select vsync mode*/
		VkPresentModeKHR   SelectSwapchainPresentMode(const gu::DynamicArray<VkPresentModeKHR>& presentMode);
		
		/** @brief Select swapchain screen size*/
		VkExtent2D         SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		/* @brief Select image array layer*/
		std::uint32_t      SelectImageArrayLayers(std::uint32_t request, const std::uint32_t maxLayers);

		/* @brief Select frame buffer count*/
		std::uint32_t      SelectImageCount(const VkSurfaceCapabilitiesKHR& capabilities);

		VkCompositeAlphaFlagBitsKHR SelectCompositeAlpha(const VkSurfaceCapabilitiesKHR& capabilities);
	};
}
#endif