//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.cpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Swapchain.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
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
#pragma region Constructor and Destructor
RHISwapchain::RHISwapchain(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandQueue>& queue,
	const rhi::core::WindowInfo& windowInfo, const rhi::core::PixelFormat& pixelFormat, size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR) 
	: rhi::core::RHISwapchain(device, queue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR)

{
	const auto rhiDevice = static_cast<rhi::directX12::RHIDevice*>(_device.get());
	const auto dxDevice  = static_cast<rhi::directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxQueue   = static_cast<rhi::directX12::RHICommandQueue*>(_commandQueue.get())->GetCommandQueue();

	/*-------------------------------------------------------------------
	-        SwapChain Flag
	---------------------------------------------------------------------*/
	int flag = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (rhiDevice->IsSupportedTearingSupport()) { flag |= (int)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; }
	_swapchainFlag = static_cast<DXGI_SWAP_CHAIN_FLAG>(flag);

	/*-------------------------------------------------------------------
	-        BackBuffer format
	---------------------------------------------------------------------*/
	_backBufferFormat = EnumConverter::Convert(_pixelFormat);

	/*-------------------------------------------------------------------
	-                   Create Swapchain Descriptor
	---------------------------------------------------------------------*/
	DXGI_SWAP_CHAIN_DESC1 sd={};
	sd.BufferCount = static_cast<UINT>(frameBufferCount);  // Current: Triple Buffer
	sd.Width       = static_cast<UINT>(windowInfo.Width);  // Window Size Width
	sd.Height      = static_cast<UINT>(windowInfo.Height); // Window Size Height 
	sd.Format      = _backBufferFormat;                    // Back Buffer Format 
	sd.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED;          // Alpha Mode => transparency behavior is not specified
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // Use the surface or resource as an output render target
	sd.Flags       = _swapchainFlag;                        // Allow Resize Window
	sd.Scaling     = DXGI_SCALING_STRETCH;                 // scaling: stretch
	sd.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD;        // bit-block transfer model
	sd.Stereo      = false;
	sd.SampleDesc.Count   = 1;                             // MSAA: Anti-Alias
	sd.SampleDesc.Quality = 0;                             // MSAA: Anti-Alias
	
	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc = {};
	fullScreenDesc.RefreshRate.Denominator = 60;
	fullScreenDesc.RefreshRate.Numerator   = 1;
	fullScreenDesc.Scaling                 = DXGI_MODE_SCALING_STRETCHED;
	fullScreenDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	fullScreenDesc.Windowed                = FALSE;

	/*-------------------------------------------------------------------
	-                   Create Swapchain for hwnd
	---------------------------------------------------------------------*/
	ThrowIfFailed(static_cast<directX12::RHIInstance*>(device->GetDisplayAdapter()->GetInstance())->GetFactory()->
	CreateSwapChainForHwnd
	(
		dxQueue.Get(),
		(HWND)windowInfo.Handle,
		&sd,
		nullptr, // full screen desc
		nullptr, // main monitor display
		(IDXGISwapChain1**)(_swapchain.GetAddressOf())
	));

	if (rhiDevice->IsSupportedHDR() && _isValidHDR)
	{
		EnsureSwapChainColorSpace();
		SetHDRMetaData();
	}

	/*-------------------------------------------------------------------
	-                   Set Back Buffer
	---------------------------------------------------------------------*/
	_backBuffers.resize(frameBufferCount);
	for (size_t i = 0; i < frameBufferCount; ++i)
	{
		ResourceComPtr backBuffer = nullptr;
		ThrowIfFailed(_swapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(backBuffer.GetAddressOf())));

		auto info = core::GPUTextureMetaData::Texture2D(
			static_cast<size_t>(_windowInfo.Width), static_cast<size_t>(_windowInfo.Height), _pixelFormat, 1, 
			core::ResourceUsage::RenderTarget);
		info.State = core::ResourceState::Common;

		_backBuffers[i] = std::make_shared<directX12::GPUTexture>(_device, backBuffer, info);
	}
}


RHISwapchain::~RHISwapchain()
{
	if (_swapchain) { _swapchain.Reset(); }
}

#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*							Resize
*************************************************************************//**
*  @fn        void RHISwapchain::Resize(const size_t width, const size_t height)
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
	if (_windowInfo.Width == width && _windowInfo.Height == height) { return; }

	/*-------------------------------------------------------------------
	-         window size check
	---------------------------------------------------------------------*/
	if (_windowInfo.Width == 0 || _windowInfo.Height == 0) { throw std::runtime_error("Width or height is zero."); }

	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	for (auto& buffer : _backBuffers) { buffer.reset(); }
	ThrowIfFailed(_swapchain->ResizeBuffers(
		static_cast<UINT>(_frameBufferCount),
		static_cast<UINT>(_windowInfo.Width),
		static_cast<UINT>(_windowInfo.Height),
		_backBufferFormat,
		_swapchainFlag));

	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	for (size_t index = 0; index < _backBuffers.size(); ++index)
	{
		ResourceComPtr backBuffer = nullptr;
		ThrowIfFailed(_swapchain->GetBuffer(static_cast<UINT>(index), IID_PPV_ARGS(backBuffer.GetAddressOf())));

		auto info = core::GPUTextureMetaData::Texture2D(
			static_cast<size_t>(_windowInfo.Width),
			static_cast<size_t>(_windowInfo.Height), 
			_pixelFormat, 1, core::ResourceUsage::RenderTarget);

		info.State = core::ResourceState::Present;

		_backBuffers[index] = std::make_shared<directX12::GPUTexture>(_device, backBuffer, info);
	}
}
/****************************************************************************
*							PrepareNextImage
*************************************************************************//**
*  @fn        std::uint32_t RHISwapchain::PrepareNextImage(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t signalValue)
*  @brief     NextImageの準備が完了したときSignalを発行し, 次のframe Indexを返す. 
*  @param[in] const std::shared_ptr<core::RHIFence>
*  @param[in] std::uint64_t signalValue (Normally : ++fenceValueを代入)
*  @return 　　std::uint32_t Backbuffer index
*****************************************************************************/
std::uint32_t RHISwapchain::PrepareNextImage(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t signalValue)
{
	std::uint32_t frameIndex = _swapchain->GetCurrentBackBufferIndex();
	_commandQueue->Signal(fence, signalValue);
	return frameIndex;
}
/****************************************************************************
*							Present
*************************************************************************//**
*  @fn        void RHISwapchain::Present()
*  @brief     Display front buffer
*  @param[in] const std::shared_ptr<core::RHIFence>& fence
*  @param[in] std::uint64_t waitValue
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Present(const std::shared_ptr<core::RHIFence>& fence, std::uint64_t waitValue)
{
	/*synchronization between command queue */
	_commandQueue->Wait(fence, waitValue);
	/* present front buffer*/
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
	return static_cast<size_t>(_swapchain->GetCurrentBackBufferIndex());
}
#pragma endregion Main Function

# pragma region Color Space
/****************************************************************************
*                     EnsureSwapChainColorSpace
*************************************************************************//**
*  @fn        void RHISwapchain::EnsureSwapChainColorSpace()
* 
*  @brief     Check SwapChain Color Space
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHISwapchain::EnsureSwapChainColorSpace()
{

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	DXGI_COLOR_SPACE_TYPE colorSpace;
	switch (desc.Format)
	{
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;    // scrgb
			break;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			break;
		default:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
			break;
	}

	_swapchain->SetColorSpace1(colorSpace);
	
}

/****************************************************************************
*                     SetHDRMetaData
*************************************************************************//**
*  @fn        void RHISwapchain::SetHDRMetaData()
* 
*  @brief     Set HDR Meta Data
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHISwapchain::SetHDRMetaData()
{

	/*-------------------------------------------------------------------
	-          In case False (isHDRSupport)
	---------------------------------------------------------------------*/
	if (!_isValidHDR)
	{
		// not supported
		ThrowIfFailed(_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_NONE, 0, nullptr));
		return;
	}

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	/*-------------------------------------------------------------------
	-          Define Display Chromacity
	---------------------------------------------------------------------*/
	struct DisplayChromacities
	{
		float RedX  ; float RedY;
		float GreenX; float GreenY;
		float BlueX ; float BlueY;
		float WhiteX; float WhiteY;
	};

	static const DisplayChromacities DisplayChromacityList[] =
	{
		 { 0.64000f, 0.33000f, 0.30000f, 0.60000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut Rec709 
		 { 0.70800f, 0.29200f, 0.17000f, 0.79700f, 0.13100f, 0.04600f, 0.31270f, 0.32900f }  // Display Gamut Rec2020
	};

	/*-------------------------------------------------------------------
	-          Select Chroma Format
	---------------------------------------------------------------------*/
	int selectedChroma = 0;
	if (desc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT)
	{
		selectedChroma = 1;
	}

	/*-------------------------------------------------------------------
	-          Set HDR10Meta Data
	---------------------------------------------------------------------*/
	const auto& chroma = DisplayChromacityList[selectedChroma];
	DXGI_HDR_METADATA_HDR10 HDR10MetaData = {};
	HDR10MetaData.RedPrimary[0]             = UINT16(chroma.RedX * 50000.0f);
	HDR10MetaData.RedPrimary[1]             = UINT16(chroma.RedY * 50000.0f);
	HDR10MetaData.GreenPrimary[0]           = UINT16(chroma.GreenX * 50000.0f);
	HDR10MetaData.GreenPrimary[1]           = UINT16(chroma.GreenY * 50000.0f);
	HDR10MetaData.BluePrimary[0]            = UINT16(chroma.BlueX * 50000.0f);
	HDR10MetaData.BluePrimary[1]            = UINT16(chroma.BlueY * 50000.0f);
	HDR10MetaData.WhitePoint[0]             = UINT16(chroma.WhiteX * 50000.0f);
	HDR10MetaData.WhitePoint[1]             = UINT16(chroma.WhiteY * 50000.0f);
	HDR10MetaData.MaxMasteringLuminance     = UINT(1000.0f * 10000.0f);
	HDR10MetaData.MinMasteringLuminance     = UINT(0.001f  * 10000.0f);
	HDR10MetaData.MaxContentLightLevel      = UINT16(2000.0f);
	HDR10MetaData.MaxFrameAverageLightLevel = UINT16(500.0f);

	/*-------------------------------------------------------------------
	-          Set HDRMetaData
	---------------------------------------------------------------------*/
	_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10, sizeof(HDR10MetaData), &HDR10MetaData);
}
#pragma endregion   Color Space