//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.hpp
///             @brief  Swapchain
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
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandQueue;
	class GPUTexture;
	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
	*****************************************************************************/
	class RHISwapchain : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Display front buffer*/
		virtual void Present() = 0;
		/* @brief : Resize screen size. (set resized swapchain buffers )*/
		virtual void Resize(const size_t width, const size_t height) = 0;
		/* @brief : Return current frame buffer*/
		virtual size_t GetCurrentBufferIndex() const = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t      GetWidth      () const noexcept { return _windowInfo.Width; }
		size_t      GetHeight     () const noexcept { return _windowInfo.Height; }
		PixelFormat GetPixelFormat() const noexcept { return _pixelFormat; }
		WindowInfo  GetWindowInfo () const noexcept { return _windowInfo; }
		std::shared_ptr<GPUTexture> GetBuffer(const size_t index) const { return _buffers[index]; }
		size_t      GetBufferCount() const noexcept { return _buffers.size(); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHISwapchain() = default;
		~RHISwapchain() = default;
		explicit RHISwapchain(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, PixelFormat pixelFormat, size_t frameBufferCount = 2, std::uint32_t vsync = 0)
		{
			_device = device; _commandQueue = commandQueue; _windowInfo = windowInfo; _pixelFormat = pixelFormat; _vsync = vsync;_frameBufferCount = frameBufferCount;
		}
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>       _device = nullptr;
		std::shared_ptr<RHICommandQueue> _commandQueue = nullptr;
		
		std::vector<std::shared_ptr<GPUTexture>> _buffers; //[0] : render target 
		/* pixel color format*/
		PixelFormat _pixelFormat;
		/* screen size and hwnd, hInstance (Windows API) */
		WindowInfo  _windowInfo;
		/* vertical syncronization: 0 : not wait, 1: 60 fps fixed frame rate*/
		std::uint32_t _vsync = 0;
		/* frame buffer count (default : 2)*/
		size_t        _frameBufferCount = 0;
	};
}
#endif