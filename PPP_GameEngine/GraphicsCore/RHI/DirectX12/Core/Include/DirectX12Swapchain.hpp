//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Swapchain.hpp
///             @brief  Update frame buffer image
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
	*				  			RHISwapchain
	*************************************************************************//**
	*  @class     RHISwapchain
	*  @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public  rhi::core::RHISwapchain
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : When NextImage is ready, Signal is issued and the next frame Index is returned. */
		std::uint32_t PrepareNextImage(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t signalValue) override;
		
		/* @brief : Display front buffer */
		void Present(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t waitValue) override ;
		
		/* @brief : Resize screen size. Rebuild everything once and update again.*/
		void Resize(const size_t width, const size_t height) override ;
		
		/* @brief : Return current frame buffer*/
		size_t GetCurrentBufferIndex() const override;

		/* @brief : Switch between fullscreen mode and specified resolution screen mode.
		            isOn = true : proceed fullscreen mode. false : specified resolution screen mode */
		void SwitchFullScreenMode(const bool isOn) override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~RHISwapchain();
		
		explicit RHISwapchain(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const std::shared_ptr<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const size_t frameBufferCount = 3, const std::uint32_t vsync = 0,
			const bool isValidHDR = true);

		explicit RHISwapchain(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const core::SwapchainDesc& desc);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUp();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SwapchainComPtr      _swapchain = nullptr;

		DXGI_SWAP_CHAIN_FLAG _swapchainFlag;

		DXGI_FORMAT          _backBufferFormat; // color format

		/*----------------------------------------------------------------------
		*  @brief : VSyncが0のときかつ, AllowTearingがサポートされているなら,　
		*           スワップの準備が出来るまで待つようにします.
		/*----------------------------------------------------------------------*/
		bool _useAllowTearing = false;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void EnsureSwapChainColorSpace();
		void SetHDRMetaData();
	};
}
#endif