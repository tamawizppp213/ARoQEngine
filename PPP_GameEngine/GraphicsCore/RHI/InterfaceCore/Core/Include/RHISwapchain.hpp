//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.hpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_SWAP_CHAIN_HPP
#define RHI_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandQueue;
	class RHIFence;
	class GPUTexture;

	/****************************************************************************
	*				  			SwapchainDesc
	*************************************************************************//**
	*  @struct    SwapchainDesc
	*  @brief     Swapchain descriptor
	*****************************************************************************/
	struct SwapchainDesc
	{
	protected:
		using CommandQueuePtr = std::shared_ptr<RHICommandQueue>;

	public:
		CommandQueuePtr CommandQueue     = nullptr;
		WindowInfo      WindowInfo       = {};
		PixelFormat     PixelFormat      = PixelFormat::Unknown; 
		size_t          FrameBufferCount = 3;
		std::uint32_t   VSync            = 0;
		bool            IsValidHDR       = true;
		bool            IsValidStereo    = false;
		bool            IsFullScreen     = false;

		~SwapchainDesc()
		{
			if (CommandQueue) { CommandQueue.reset(); }
		}
	};

	/****************************************************************************
	*				  			RHISwapchain
	*************************************************************************//**
	*  @class     RHISwapchain
	*  @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : When NextImage is ready, Signal is issued and the next frame Index is returned. */
		virtual std::uint32_t PrepareNextImage(const std::shared_ptr<RHIFence>& fence, std::uint64_t signalValue) = 0;
		
		/* @brief : Display front buffer*/
		virtual void Present(const std::shared_ptr<RHIFence>& fence, std::uint64_t waitValue) = 0;
		
		/* @brief : Resize screen size. Rebuild everything once and update again.*/
		virtual void Resize(const size_t width, const size_t height) = 0;
		
		/* @brief : Return current frame buffer*/
		virtual size_t GetCurrentBufferIndex() const = 0;

		virtual void SwitchFullScreenMode(const bool isOn) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief: Return swapchain's render pixel width*/
		size_t      GetWidth      () const noexcept { return _desc.WindowInfo.Width; }

		/* @brief: Return swapchain's render pixel height*/
		size_t      GetHeight     () const noexcept { return _desc.WindowInfo.Height; }

		/* @brief : Return pixel format */
		PixelFormat GetPixelFormat() const noexcept { return _desc.PixelFormat; }

		/* @brief : Return render window information (width, height, window handle pointer)*/
		WindowInfo  GetWindowInfo () const noexcept { return _desc.WindowInfo; }

		/* @brief : Return back buffer of the specified frame*/
		std::shared_ptr<GPUTexture> GetBuffer(const size_t index) { return _backBuffers[index]; }
		
		/* @biref : Return back buffer's total frame count*/
		size_t      GetBufferCount() const noexcept { return _backBuffers.size(); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHISwapchain() = default;

		virtual ~RHISwapchain()
		{
			_backBuffers.clear(); _backBuffers.shrink_to_fit();
			if (_device)       { _device.reset(); }
		};

		explicit RHISwapchain(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, PixelFormat pixelFormat, size_t frameBufferCount = 3, std::uint32_t vsync = 0, bool isValidHDR = true)
		{
			_device = device; _desc.CommandQueue = commandQueue; _desc.WindowInfo = windowInfo; _desc.PixelFormat = pixelFormat; _desc.VSync = vsync; _desc.FrameBufferCount = frameBufferCount; _desc.IsValidHDR = isValidHDR;
		}

		explicit RHISwapchain(const std::shared_ptr<RHIDevice>& device, const SwapchainDesc& desc):
			_device(device), _desc(desc)
		{
			
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>       _device = nullptr;
		
		std::vector<std::shared_ptr<GPUTexture>> _backBuffers; //[0] : render target 
		
		SwapchainDesc _desc = {};

		bool _isFullScreen = false;
	};
}
#endif