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
		virtual void Present() = 0;
		virtual void Resize(const size_t width, const size_t height) = 0;
		virtual size_t GetCurrentBufferIndex() const = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t      GetWidth      () const noexcept { return _windowInfo.Width; }
		size_t      GetHeight     () const noexcept { return _windowInfo.Height; }
		PixelFormat GetPixelFormat() const noexcept { return _pixelFormat; }
		WindowInfo  GetWindowInfo () const noexcept { return _windowInfo; }
		// buffer
		// buffer count

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
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
		// texture
		PixelFormat _pixelFormat;
		WindowInfo  _windowInfo;
		std::uint32_t _vsync = 0;
		size_t        _frameBufferCount = 0;
	};
}
#endif