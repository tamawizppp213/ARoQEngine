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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
		using CommandQueuePtr = gu::SharedPointer<RHICommandQueue>;

	public:
		CommandQueuePtr CommandQueue     = nullptr;
		WindowInfo      WindowInfo       = {};
		PixelFormat     PixelFormat      = PixelFormat::Unknown; 
		gu::uint64      FrameBufferCount = 3;
		gu::uint32      VSync            = 0;
		bool            IsValidHDR       = true;
		bool            IsValidStereo    = false;
		bool            IsFullScreen     = false;

		~SwapchainDesc()
		{
			if (CommandQueue) { CommandQueue.Reset(); }
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
		/*----------------------------------------------------------------------
		*  @brief :  When NextImage is ready, 
		　　　　　　　　　Signal is issued and the next frame Index is returned.
		/*----------------------------------------------------------------------*/
		virtual gu::uint32 PrepareNextImage(const gu::SharedPointer<RHIFence>& fence, const gu::uint64 signalValue) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Display front buffer
		/*----------------------------------------------------------------------*/
		virtual void Present(const gu::SharedPointer<RHIFence>& fence, std::uint64_t waitValue) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Resize screen size. Rebuild everything once and update again.
		/*----------------------------------------------------------------------*/
		virtual void Resize(const size_t width, const size_t height) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Return current frame buffer
		/*----------------------------------------------------------------------*/
		virtual size_t GetCurrentBufferIndex() const = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Switch full screen mode
		/*----------------------------------------------------------------------*/
		virtual void SwitchFullScreenMode(const bool isOn) = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Switch hdr mode. 
		*            enableHDR : true -> HDR, false -> SDR
		/*----------------------------------------------------------------------*/
		virtual void SwitchHDRMode(const bool enableHDR) = 0;

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
		gu::SharedPointer<GPUTexture> GetBuffer(const size_t index) { return _backBuffers[index]; }
		
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
			if (_device)       { _device.Reset(); }
		};

		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, PixelFormat pixelFormat, size_t frameBufferCount = 3, std::uint32_t vsync = 0, bool isValidHDR = true, bool isFullScreen = false)
		{
			_device = device; _desc.CommandQueue = commandQueue; _desc.WindowInfo = windowInfo; _desc.PixelFormat = pixelFormat; _desc.VSync = vsync; _desc.FrameBufferCount = frameBufferCount; _desc.IsValidHDR = isValidHDR;
			_isFullScreen = isFullScreen;
		}

		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const SwapchainDesc& desc):
			_device(device), _desc(desc)
		{
			_isFullScreen = _desc.IsFullScreen;
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice>       _device = nullptr;
		
		std::vector<gu::SharedPointer<GPUTexture>> _backBuffers; //[0] : render target 
		
		SwapchainDesc _desc = {};

		bool _isFullScreen = false;
	};
}
#endif