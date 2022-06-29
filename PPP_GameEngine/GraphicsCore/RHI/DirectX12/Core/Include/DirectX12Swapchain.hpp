//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Swapchain.hpp
///             @brief  Swapchain
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_SWAP_CHAIN_HPP
#define DIRECTX12_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			RHIFence
	*************************************************************************//**
	*  @class     RHIFence
	*  @brief     CPU-GPU synchronization
	*****************************************************************************/
	class RHISwapchain : public  rhi::core::RHISwapchain
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Display front buffer */
		virtual void Present() override ;
		/* @brief : Resize screen size. (set resized swapchain buffers )*/
		virtual void Resize(const size_t width, const size_t height) override ;
		/* @brief : Return current frame buffer*/
		virtual size_t GetCurrentBufferIndex() const override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~RHISwapchain() = default;
		explicit RHISwapchain(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const std::shared_ptr<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const size_t frameBufferCount = 2, std::uint32_t vsync = 0);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SwapchainComPtr      _swapchain = nullptr;
		DXGI_SWAP_CHAIN_FLAG _swapchainFlag;
		DXGI_FORMAT          _backBufferFormat;
		bool                 _isHDRSupport;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void EnsureSwapChainColorSpace();
		void SetHDRMetaData();
	};
}
#endif