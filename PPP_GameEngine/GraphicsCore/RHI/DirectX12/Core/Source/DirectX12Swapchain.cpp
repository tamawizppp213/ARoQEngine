//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandQueue.cpp
///             @brief  Command queue 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Swapchain.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHISwapchain::RHISwapchain( const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandQueue>& queue,
	const rhi::core::WindowInfo& windowInfo, const rhi::core::PixelFormat& pixelFormat, size_t frameBufferCount, std::uint32_t vsync) : rhi::core::RHISwapchain(device, queue, windowInfo, pixelFormat, frameBufferCount, vsync)
{
	const auto rhiDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get());
	const auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxQueue  = static_cast<rhi::directX12::RHICommandQueue*>(_commandQueue.get())->GetCommandQueue();

	/*-------------------------------------------------------------------
	-        SwapChain Flag
	---------------------------------------------------------------------*/
	int flag = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (rhiDevice->IsTearingSupport()) { flag |= (int)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; }
	_swapchainFlag = static_cast<DXGI_SWAP_CHAIN_FLAG>(flag);

	/*-------------------------------------------------------------------
	-        BackBuffer format
	---------------------------------------------------------------------*/
	_backBufferFormat = rhiDevice->GetBackBufferFormat();
	/*-------------------------------------------------------------------
	-                   Create Swapchain Descriptor
	---------------------------------------------------------------------*/
	DXGI_SWAP_CHAIN_DESC1 sd={};
	sd.BufferCount = static_cast<UINT>(frameBufferCount);  // Current: Triple Buffer
	sd.Width       = static_cast<UINT>(windowInfo.Width);  // Window Size Width
	sd.Height      = static_cast<UINT>(windowInfo.Height); // Window Size Height 
	sd.Format      = rhiDevice->GetBackBufferFormat();     // Back Buffer Format 
	sd.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;          // Alpha Mode => transparency behavior is not specified
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // Use the surface or resource as an output render target
	sd.Flags       = _swapchainFlag;                        // Allow Resize Window
	sd.Scaling     = DXGI_SCALING_STRETCH;                 // scaling: stretch
	sd.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // bit-block transfer model
	sd.Stereo      = false;
	sd.SampleDesc.Count   = 1;                             // MSAA: Anti-Alias
	sd.SampleDesc.Quality = 0;                             // MSAA: Anti-Alias
	
	/*-------------------------------------------------------------------
	-                   Create Swapchain for hwnd
	---------------------------------------------------------------------*/
	ThrowIfFailed(rhiDevice->GetFactory()->CreateSwapChainForHwnd(
		dxQueue.Get(),
		(HWND)windowInfo.Handle,
		&sd,
		nullptr,
		nullptr, // main monitor display
		(IDXGISwapChain1**)(_swapchain.GetAddressOf())
	));
}
/****************************************************************************
*							Resize
*************************************************************************//**
*  @fn        void RHISwapchain::Resize(const size_t width, const size_t height)
*  @brief     Resize
*  @param[in] const size_t width
*  @param[in] const size_t height
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Resize(const size_t width, const size_t height)
{
	/*-------------------------------------------------------------------
	-          If the size is not change, we do nothing
	---------------------------------------------------------------------*/
	if (_windowInfo.Width == width && _windowInfo.Height) { return; }
	/*-------------------------------------------------------------------
	-         window size check
	---------------------------------------------------------------------*/
	if (_windowInfo.Width == 0 || _windowInfo.Height == 0) { throw std::runtime_error("Width or height is zero."); }
	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	ThrowIfFailed(_swapchain->ResizeBuffers(
		_frameBufferCount,
		_windowInfo.Width,
		_windowInfo.Height,
		_backBufferFormat,
		_swapchainFlag));

	// build
}
/****************************************************************************
*							Present
*************************************************************************//**
*  @fn        void RHISwapchain::Present()
*  @brief     Present
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Present()
{
	ThrowIfFailed(_swapchain->Present(_vsync, 0));
}
/****************************************************************************
*							GetCurrentBufferIndex
*************************************************************************//**
*  @fn        size_t RHISwapchain::GetCurrentBufferIndex() const
*  @brief     Get current buffer 
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t RHISwapchain::GetCurrentBufferIndex() const
{
	return static_cast<UINT>(_swapchain->GetCurrentBackBufferIndex());
}