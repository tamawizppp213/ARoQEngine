//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGraphicsDevice.hpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GRAPHICS_DEVICE_HPP
#define VULKAN_GRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#define VK_USE_PLATFORM_WIN32_KHR
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIGraphicsDevice.hpp"
#include <vector>
#include <optional>
#include <vulkan/vulkan.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHIDevice;
	class RHISwapchain;
	class RHICommandQueue;
	class RHICommandAllocator;
	class RHIFence;
	/****************************************************************************
	*				  			GraphicsDeviceVulkan
	*************************************************************************//**
	*  @class     GraphicsDeviceVulkan final : public core::RHIGraphicsDevice
	*  @brief     Graphics Device
	*****************************************************************************/
	class GraphicsDeviceVulkan final : public core::RHIGraphicsDevice
	{
	public:
		struct GraphicsQueueFamilyIndices
		{
			std::optional<uint32_t> GraphicsFamily;
			std::optional<uint32_t> PresentFamily;
			bool IsComplete() { return GraphicsFamily.has_value() && PresentFamily.has_value(); }
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/** @brief : Start up graphics device. This class is called when start.*/
		void StartUp(HWND hwnd, HINSTANCE hInstance) override;
		/* @brief : Begin draw frame. This function is called when draw frame start.*/
		void BeginDrawFrame() override;
		/* @brief : End draw frame. This function is called when draw frame end.*/
		void EndDrawFrame  () override;
		/** @brief : Shut down graphics device. This class is called when end.*/
		void ShutDown() override;
		/** @brief : Resize on buffer*/
		void OnResize() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GraphicsDeviceVulkan() = default;
		~GraphicsDeviceVulkan();
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
#pragma region SetUp
		/** @brief Create vkInstance. */
		bool CreateInstance();
		/** @brief Create Debug Messenger infomation. */
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		/** @brief Set up debug messenger*/
		void SetUpDebugMessenger();
		/** @brief Acquire physical device*/
		bool SelectPhysicalDevice();
		/** @brief Physical device selector*/
		bool IsDeviceSuitable(VkPhysicalDevice device);
		/** @brief PhysicalDevice extension support check*/
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		/** @brief Logical device representation (application's view of the device) */
		bool CreateLogicalDevice();
		/** @brief Create display surface*/
		void CreateSurface();
		/** @brief Create Display swap chain*/
		void CreateSwapchain();
		/** @brief Create Depth Buffer for forward rendering pass*/
		void CreateDepthBuffer();
		/** @brief Create Swapchain Image views*/
		void CreateImageViews();
		/** @brief Create Render Pass*/
		void CreateRenderPass();
		/** @brief Create Swapchain frame buffer*/
		void CreateFrameBuffer();
		void CreateCommandObjects();
		/** @brief Create fence*/
		void CreateSyncObjects();
#pragma endregion SetUp
#pragma region Clear
		void CleanUpSwapchain();
#pragma endregion Clear
#pragma region Debug
		bool CheckValidationLayerSupport();
		void LogQueueList(VkPhysicalDevice device);
#pragma endregion Debug
		/** @brief Acquire device queue index. */
		GraphicsQueueFamilyIndices FindGraphicsQueueFamilies(VkPhysicalDevice device);
		/** @brief Acquire memory type index*/
		UINT32 GetMemoryTypeIndex(UINT32 requestBits, VkMemoryPropertyFlags propertyFlags)  const;
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::shared_ptr<rhi::vulkan::RHIDevice>             _rhiDevice           = nullptr;
		std::shared_ptr<rhi::vulkan::RHISwapchain>          _rhiSwapchain        = nullptr;
		std::shared_ptr<rhi::vulkan::RHICommandQueue>       _rhiCommandQueue     = nullptr;
		std::vector<std::shared_ptr<rhi::vulkan::RHICommandAllocator>> _rhiCommandAllocators;
		std::vector<std::shared_ptr<rhi::vulkan::RHIFence>> _rhiFences;
		/** @brief Vk Instance: all vulkan functions utility  */
		VkInstance                       _instance = nullptr;
		/** @brief  */
		VkDebugUtilsMessengerEXT         _debugMessenger = nullptr;
		/** @brief GPU detect infomation */
		VkPhysicalDevice                 _physicalDevice = nullptr;
		/** @brief logical device */
		VkDevice                         _logicalDevice  = nullptr;
		/** @brief GPU command buffer queue*/
		VkQueue                          _graphicsQueue  = nullptr;
		VkQueue                          _presentQueue   = nullptr;

		/** @brief swapchain*/
		VkSwapchainKHR                   _swapchain      = nullptr;
		std::vector<VkImage>             _swapchainImages;
		std::vector<VkImageView>         _swapchainImageViews;
		VkFormat                         _swapchainImageFormat;
		VkExtent2D                       _swapchainExtent;      /** @brief swapchain screen infomation*/
		std::vector<VkFramebuffer>       _swapchainFrameBuffers;
		UINT32 _imageIndex;
		VkViewport _viewport;
		VkRect2D   _rect2D;

		/** @brief command list*/
		VkCommandPool                    _commandPool = nullptr;
		std::vector<VkCommandBuffer>     _commandBuffers;
		std::vector<VkFence>             _fences;
		VkSemaphore        _renderCompletedSemaphores;
		VkSemaphore         _presentCompletedSemaphores;

		/** @brief render pass */
		VkRenderPass                     _renderPass        = nullptr;

		/** @brief depth stencil*/
		VkFormat                        _depthStencilFormat = VK_FORMAT_D32_SFLOAT;
		VkImage                         _depthBuffer        = nullptr;
		VkImageView                     _depthBufferView    = nullptr;
		VkDeviceMemory                  _depthBufferMemory  = nullptr;

		

		/** @brief memory property */
		VkPhysicalDeviceMemoryProperties _physicalMemoryProperties;
		/** @brief window surface use Windows */
		VkSurfaceKHR                     _surface;
		UINT32     _apiVersion = VK_API_VERSION_1_3;
	};
}
#endif