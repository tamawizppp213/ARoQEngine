//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.cpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12Swapchain.hpp"
#include "../Include/DirectX12Instance.hpp"
#include "../Include/DirectX12CommandQueue.hpp"
#include "../Include/DirectX12Adapter.hpp"
#include "../Include/DirectX12Fence.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Debug.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "../../Resource/Include/DirectX12GPUTexture.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHISwapchain::RHISwapchain(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandQueue>& queue,
	const core::WindowInfo& windowInfo, const rhi::core::PixelFormat& pixelFormat, size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR, const bool isFullScreen) 
	: core::RHISwapchain(device, queue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR, isFullScreen)
{
	SetUp();
}

RHISwapchain::RHISwapchain(const gu::SharedPointer<core::RHIDevice>& device, const core::SwapchainDesc& desc) : rhi::core::RHISwapchain(device, desc)
{
	SetUp();
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
	if (_desc.WindowInfo.Width == width && _desc.WindowInfo.Height == height) { return; }

	/*-------------------------------------------------------------------
	-         window size check
	---------------------------------------------------------------------*/
	if (_desc.WindowInfo.Width == 0 || _desc.WindowInfo.Height == 0) { throw std::runtime_error("Width or height is zero."); }

	_desc.WindowInfo.Width  = width;
	_desc.WindowInfo.Height = height;
	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	for (auto& buffer : _backBuffers) { buffer.Reset(); }
	ThrowIfFailed(_swapchain->ResizeBuffers(
		static_cast<UINT>(_desc.FrameBufferCount),
		static_cast<UINT>(_desc.WindowInfo.Width),
		static_cast<UINT>(_desc.WindowInfo.Height),
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
			static_cast<size_t>(_desc.WindowInfo.Width),
			static_cast<size_t>(_desc.WindowInfo.Height), 
			_desc.PixelFormat, 1, core::ResourceUsage::RenderTarget);

		info.State = core::ResourceState::Present;

		_backBuffers[index] = gu::MakeShared<directX12::GPUTexture>(_device, backBuffer, info, L"BackBuffer");
	}
	
}
/****************************************************************************
*							PrepareNextImage
*************************************************************************//**
*  @fn        std::uint32_t RHISwapchain::PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t signalValue)
*  @brief     NextImageの準備が完了したときSignalを発行し, 次のframe Indexを返す. 
*  @param[in] const gu::SharedPointer<core::RHIFence>
*  @param[in] uint64 signalValue (Normally : ++fenceValueを代入)
*  @return 　　uint32 Backbuffer index
*****************************************************************************/
gu::uint32 RHISwapchain::PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue)
{
	uint32 frameIndex = _swapchain->GetCurrentBackBufferIndex();
	_desc.CommandQueue->Signal(fence, signalValue);
	return frameIndex;
}
/****************************************************************************
*							Present
*************************************************************************//**
*  @fn        void RHISwapchain::Present()
*  @brief     Display front buffer
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence
*  @param[in] std::uint64_t waitValue
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Present(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 waitValue)
{
	/*synchronization between command queue */
	_desc.CommandQueue->Wait(fence, waitValue);

	/* present front buffer*/
	UINT presentFlags = 0;
	if (!_desc.VSync && !_isFullScreen && _useAllowTearing)
	{
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
	}
	ThrowIfFailed(_swapchain->Present(_desc.VSync, presentFlags));
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

void RHISwapchain::SwitchFullScreenMode(const bool isOn)
{
	// 現在のモードが同じときは特に何もしない. 
	if (_isFullScreen == isOn) { return; } 

	if (FAILED(_swapchain->SetFullscreenState(isOn, nullptr)))
	{
		OutputDebugStringA("failed to switch swapchain mode");
		return;
	}
	_isFullScreen = isOn;
}

/****************************************************************************
*                      SetUp
*************************************************************************//**
*  @fn        void RHISwapchain::SetUp()
*
*  @brief     Set up
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHISwapchain::SetUp()
{
	/*-------------------------------------------------------------------
	-        Prepare
	---------------------------------------------------------------------*/
	const auto rhiDevice  = gu::StaticPointerCast<RHIDevice>(_device);
	const auto dxDevice   = rhiDevice->GetDevice();
	const auto dxQueue    = gu::StaticPointerCast<RHICommandQueue>(_desc.CommandQueue)->GetCommandQueue();
	const auto dxInstance = static_cast<directX12::RHIInstance*>(rhiDevice->GetDisplayAdapter()->GetInstance());
	const auto factory    = dxInstance->GetFactory();

	Checkf(_desc.CommandQueue->GetType() == core::CommandListType::Graphics, "swapchain must be graphics type command queue");
	
	/*-------------------------------------------------------------------
	-        SwapChain Flag
	---------------------------------------------------------------------*/
	int flag = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (rhiDevice->IsSupportedAllowTearing()) 
	{ 
		flag |= (int)DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING; 
		
		_useAllowTearing = _desc.VSync == 0;
	}
	else
	{
		_useAllowTearing = false;
	}

	_swapchainFlag = static_cast<DXGI_SWAP_CHAIN_FLAG>(flag);

	/*-------------------------------------------------------------------
	-        BackBuffer format
	---------------------------------------------------------------------*/
	_backBufferFormat = EnumConverter::Convert(_desc.PixelFormat);

	/*-------------------------------------------------------------------
	-        立体視を行うために, 両目で画像をずらす対応を行うかどうか
	---------------------------------------------------------------------*/
	if (_desc.IsValidStereo)
	{
		if (factory->IsWindowedStereoEnabled())
		{
			const DXGI_SWAP_CHAIN_DESC1 desc = 
			{
				.Width       = static_cast<UINT>(_desc.WindowInfo.Width),  // Window Size Pixel Width
				.Height      = static_cast<UINT>(_desc.WindowInfo.Height), // Window Size Pixel Height 
				.Format      = _backBufferFormat,                          // Back Buffer Format 
				.Stereo      = true,                                       // Use stereoscopic rendering
				.SampleDesc  = {1, 0},                                     // Count + quality (Not support multi sampling.)
				.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT,  // Use the surface or resource as an output render target
				.BufferCount = static_cast<UINT>(_desc.FrameBufferCount),  // Current: Triple Buffer
				.Scaling     = DXGI_SCALING_NONE,                          // fixed
				.SwapEffect  = DXGI_SWAP_EFFECT_FLIP_DISCARD,              // bit-block transfer model
				.AlphaMode   = DXGI_ALPHA_MODE_UNSPECIFIED,                // Alpha Mode => transparency behavior is not specified
				.Flags       = (UINT)_swapchainFlag,                       // Allow Resize Window
			};

			ThrowIfFailed(factory->CreateSwapChainForHwnd (
					dxQueue.Get(),
					(HWND)_desc.WindowInfo.Handle,
					&desc,
					nullptr, // full screen desc
					nullptr, // main monitor display
					(IDXGISwapChain1**)(_swapchain.GetAddressOf())));
		}
		else
		{
			_desc.IsValidStereo = false;
#if PLATFORM_OS_WINDOWS
			OutputDebugStringA("Failed to use stereo rendering\n");
#else 
			_RPT0(_CRT_WARN, "Failed to use stereo rendering\n");
#endif
		}
	}

	/*-------------------------------------------------------------------
	-         ステレオが有効化出来なかった場合か, 使わない場合の対応
	---------------------------------------------------------------------*/
	if (_swapchain == nullptr)
	{
		const DXGI_MODE_DESC modeDesc =
		{
			.Width            = static_cast<UINT>(_desc.WindowInfo.Width),  // Window Size Pixel Width
			.Height           = static_cast<UINT>(_desc.WindowInfo.Height), // Window Size Pixel Height 
			.RefreshRate      = {0, 0},                                     // Use 0 to avoid a potential mismatch with hw
			.Format           = _backBufferFormat,                          // Back Buffer Format 
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,       // 順序指定なし
			.Scaling          = DXGI_MODE_SCALING_STRETCHED,                // 拡大スケーリングを適用する
		};

		DXGI_SWAP_CHAIN_DESC desc = 
		{
			.BufferDesc   = modeDesc,
			.SampleDesc   = {1, 0},                                     // Count + quality (Not support multi sampling.)
			.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT, // Use the surface or resource as an output render target
			.BufferCount  = static_cast<UINT>(_desc.FrameBufferCount),  // Current: Triple Buffer
			.OutputWindow = (HWND)_desc.WindowInfo.Handle,              
			.Windowed     = !_isFullScreen,                             // Full screen setting is border less window
			.SwapEffect   = DXGI_SWAP_EFFECT_FLIP_DISCARD,              // bit-block transfer model
			.Flags        = (UINT)_swapchainFlag,                       // Allow Resize Window
		};

		/*-------------------------------------------------------------------
		-                   Create Swapchain for hwnd
		---------------------------------------------------------------------*/
		ThrowIfFailed(factory->CreateSwapChain(dxQueue.Get(), &desc, (IDXGISwapChain**)(_swapchain.GetAddressOf())));
	}
	
	// 背後でウィンドウを変更しないように、DXGIメッセージフックを設定する。
	factory->MakeWindowAssociation((HWND)_desc.WindowInfo.Handle, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

	/*-------------------------------------------------------------------
	-                  HDRの設定
	---------------------------------------------------------------------*/
	if (rhiDevice->IsSupportedHDR() && _desc.IsValidHDR)
	{
		EnsureSwapChainColorSpace();
		SetHDRMetaData();
	}

	/*-------------------------------------------------------------------
	-                   Set Back Buffer
	---------------------------------------------------------------------*/
	_backBuffers.resize(_desc.FrameBufferCount);
	for (size_t i = 0; i < _desc.FrameBufferCount; ++i)
	{
		ResourceComPtr backBuffer = nullptr;
		ThrowIfFailed(_swapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(backBuffer.GetAddressOf())));
		backBuffer->SetName(L"BackBuffer");


		auto info = core::GPUTextureMetaData::Texture2D(
			static_cast<size_t>(_desc.WindowInfo.Width), static_cast<size_t>(_desc.WindowInfo.Height), _desc.PixelFormat, 1, 
			core::ResourceUsage::RenderTarget);
		info.State = core::ResourceState::Common;

		_backBuffers[i] = gu::MakeShared<directX12::GPUTexture>(_device, backBuffer, info, L"BackBuffer");
	}

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
	if (!_desc.IsValidHDR)
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