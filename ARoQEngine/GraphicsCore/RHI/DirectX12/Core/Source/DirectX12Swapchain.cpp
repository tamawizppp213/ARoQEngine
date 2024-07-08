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
	const core::WindowInfo& windowInfo, const rhi::core::PixelFormat& pixelFormat, gu::uint8 frameBufferCount, const gu::uint8 vsync, const bool isValidHDR, const bool isFullScreen) 
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
	if (_swapchain) 
	{
		SwitchFullScreenMode(false);
		_swapchain.Reset();
	}
}

#pragma endregion Constructor and Destructor
#pragma region Main Function
/*!**********************************************************************
*  @brief     画面サイズを変更します. 一度すべてを再構築し、再度アップデートします.
*  @param[in] const gu::uint32 width : 画面の幅
*  @param[in] const gu::uint32 height : 画面の高さ
*  @return    void
*************************************************************************/
void RHISwapchain::Resize(const uint32 width, const uint32 height)
{
	/*-------------------------------------------------------------------
	-          If the size is not change, we do nothing
	---------------------------------------------------------------------*/
	if (_desc.WindowInfo.Width == width && _desc.WindowInfo.Height == height) { return; }

	/*-------------------------------------------------------------------
	-         window size check
	---------------------------------------------------------------------*/
	if (width == 0 || height == 0) { throw "Width or height is zero."; }

	_desc.WindowInfo.Width  = width;
	_desc.WindowInfo.Height = height;

	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	for (auto& buffer : _backBuffers)
	{ 
		buffer.Reset();
	}

	ThrowIfFailed(_swapchain->ResizeBuffers(
		static_cast<uint32>(_desc.FrameBufferCount),
		static_cast<uint32>(_desc.WindowInfo.Width),
		static_cast<uint32>(_desc.WindowInfo.Height),
		_backBufferFormat,
		_swapchainFlag));

	/*-------------------------------------------------------------------
	-         Reset Command List
	---------------------------------------------------------------------*/
	for (uint32 index = 0; index < _backBuffers.Size(); ++index)
	{
		ResourceComPtr backBuffer = nullptr;
		ThrowIfFailed(_swapchain->GetBuffer(index, IID_PPV_ARGS(backBuffer.GetAddressOf())));

		auto info = core::GPUTextureMetaData::Texture2D(
			_desc.WindowInfo.Width,
			_desc.WindowInfo.Height, 
			_desc.PixelFormat, 1, core::TextureCreateFlags::RenderTargetable);

		info.State = core::ResourceState::Present;

		gu::tstring name = gu::tstring(L"BackBuffer") + gu::tstring::FromNumber(index);
		_backBuffers[index] = gu::MakeShared<directX12::GPUTexture>(_device, backBuffer, info, name);
	}
	
}
/*!**********************************************************************
*  @brief     NextImageが準備出来たとき, Signalを発行して次のフレームインデックスを返します
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
*  @param[in] const gu::uint64 signalValue : 準備完了時にフェンスに書き込まれるシグナル値
*  @return    gu::uint32 次のフレームインデックス
*************************************************************************/
gu::uint32 RHISwapchain::PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue)
{
	const auto frameIndex = GetCurrentBufferIndex();
	_desc.CommandQueue->Signal(fence, signalValue);
	return frameIndex;
}
/*!**********************************************************************
*  @brief     バックバッファを次フレームのフロントバッファにします
*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
*  @param[in] const gu::uint64 waitValue : 待機する値
*  @return    void
*************************************************************************/
void RHISwapchain::Present(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 waitValue)
{
	/*synchronization between command queue */
	_desc.CommandQueue->Wait(fence, waitValue);

	/* present front buffer*/
	UINT presentFlags = 0;
	if (!_desc.VSync && !_desc.IsFullScreen && _useAllowTearing)
	{
		presentFlags |= DXGI_PRESENT_ALLOW_TEARING;
	}

	ThrowIfFailed(_swapchain->Present(_desc.VSync, presentFlags));

	/* update current back buffer index */
#if DXGI_MAX_SWAPCHAIN_INTERFACE < 3
	_currentBackBufferIndex = (_currentBackBufferIndex + 1) % _desc.FrameBufferCount;
#endif
}

/*!**********************************************************************
*  @brief     現在描画コマンドを詰め込みたいバックバッファのインデックスを返します
*  @param[in] void
*  @return    gu::uint32 スワップチェインの描画する幅
*************************************************************************/
gu::uint8 RHISwapchain::GetCurrentBufferIndex() const
{
#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 3
	return static_cast<gu::uint8>(_swapchain->GetCurrentBackBufferIndex());
#else
	return _currentBackBufferIndex;
#endif
}

/*!**********************************************************************
*  @brief     フルスクリーンモードと指定された解像度のスクリーンモードを切り替える
* 　　　　　　  (isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する)
*  @param[in] const bool isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する
*  @return    void
*************************************************************************/
void RHISwapchain::SwitchFullScreenMode(const bool isOn)
{
	// 現在のモードが同じときは特に何もしない. 
	if (_desc.IsFullScreen == isOn) { return; } 

	if (FAILED(_swapchain->SetFullscreenState(isOn, nullptr)))
	{
#if PLATFORM_OS_WINDOWS
		OutputDebugStringA("failed to switch swapchain mode\n");
#else
		_RPT0(_CRT_WARN, "failed to switch swapchain mode\n");
#endif
		return;
	}
	_desc.IsFullScreen = isOn;
}

/*!**********************************************************************
*  @brief     Swapchainの初期化
*  @param[in] void
*  @return    void
*************************************************************************/
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
	_backBufferFormat = (DXGI_FORMAT)core::PixelFormatInfo::GetConst(_desc.PixelFormat).PlatformFormat;

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
			.Windowed     = !_desc.IsFullScreen,                             // Full screen setting is border less window
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
	EnsureSwapChainColorSpace(rhiDevice->GetHDRDisplayInfo().ColorGamut, rhiDevice->GetHDRDisplayInfo().DisplayFormat);
	LogHDROutput();

	/*-------------------------------------------------------------------
	-                   Set Back Buffer
	---------------------------------------------------------------------*/
	_backBuffers.Resize(_desc.FrameBufferCount);
	for (uint64 i = 0; i < _desc.FrameBufferCount; ++i)
	{
		ResourceComPtr backBuffer = nullptr;
		ThrowIfFailed(_swapchain->GetBuffer(static_cast<uint32>(i), IID_PPV_ARGS(backBuffer.GetAddressOf())));
		backBuffer->SetName(L"BackBuffer");


		auto info = core::GPUTextureMetaData::Texture2D(
			_desc.WindowInfo.Width, _desc.WindowInfo.Height, _desc.PixelFormat, 1, 
			core::TextureCreateFlags::RenderTargetable);
		info.State = core::ResourceState::Common;

		_backBuffers[i] = gu::MakeShared<directX12::GPUTexture>(_device, backBuffer, info, L"BackBuffer");
	}

}

/*!**********************************************************************
*  @brief     HDRモードとSDRモードを切り替える.
*  @param[in] const bool enableHDR : true -> HDR, false -> SDR
*  @return    void
*************************************************************************/
void RHISwapchain::SwitchHDRMode(const bool enableHDR)
{
	const auto rhiDevice = gu::StaticPointerCast<RHIDevice>(_device);
	const auto& hdrInfo  = rhiDevice->GetHDRDisplayInfo();
	if (rhiDevice->IsSupportedHDR() && enableHDR)
	{
		EnsureSwapChainColorSpace(hdrInfo.ColorGamut, hdrInfo.DisplayFormat);
	}
	else
	{
		EnsureSwapChainColorSpace(core::DisplayColorGamut::SRGB_D65, core::DisplayOutputFormat::SDR_SRGB);
	}
}

/*!**********************************************************************
*  @brief     現在のディスプレイ出力がHDR機能をサポートしているかを調べる
*  @param[in] void
*  @return    bool HDRがサポートされているか
*************************************************************************/
bool RHISwapchain::IsSupportedHDRInCurrentDisplayOutput()
{
	if constexpr (DXGI_MAX_SWAPCHAIN_INTERFACE < 4) { return false; }

	/*-------------------------------------------------------------------
	-        Prepare
	---------------------------------------------------------------------*/
	const auto rhiDevice  = gu::StaticPointerCast<RHIDevice>(_device);
	const auto dxInstance = static_cast<directX12::RHIInstance*>(rhiDevice->GetDisplayAdapter()->GetInstance());
	const auto factory    = dxInstance->GetFactory();

	/*-------------------------------------------------------------------
	-        Get the display information that we are presenting to 
	---------------------------------------------------------------------*/
	ComPtr<IDXGIOutput> output = nullptr;
	ThrowIfFailed(_swapchain->GetContainingOutput(output.GetAddressOf()));

	ComPtr<IDXGIOutput6> output6 = nullptr;
	if (FAILED(output->QueryInterface(IID_PPV_ARGS(output6.GetAddressOf())))) { return false; }

	DXGI_OUTPUT_DESC1 outputDesc = {};
	ThrowIfFailed(output6->GetDesc1(&outputDesc));

	/*-------------------------------------------------------------------
	-        Check the color space which is able to use in the HDR mode
	         (rgb color format + P2020)
	---------------------------------------------------------------------*/
	return outputDesc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
}

/*!**********************************************************************
*  @brief     HDR上でのモニター設定を出力上に表示します.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHISwapchain::LogHDROutput() const 
{
	/*-------------------------------------------------------------------
	-        Acquire proper swapchain version 
	---------------------------------------------------------------------*/
	if constexpr (DXGI_MAX_SWAPCHAIN_INTERFACE < 4) { return; }

	/*-------------------------------------------------------------------
	-        Prepare the variables
	---------------------------------------------------------------------*/
	const auto rhiDevice  = gu::StaticPointerCast<RHIDevice>(_device);
	const auto dxInstance = static_cast<directX12::RHIInstance*>(rhiDevice->GetDisplayAdapter()->GetInstance());
	const auto factory    = dxInstance->GetFactory();

	/*-------------------------------------------------------------------
	-        Get the display information that we are presenting to 
	---------------------------------------------------------------------*/
	ComPtr<IDXGIOutput> output = nullptr;
	ThrowIfFailed(_swapchain->GetContainingOutput(output.GetAddressOf()));

	ComPtr<IDXGIOutput6> output6 = nullptr;
	if (FAILED(output->QueryInterface(IID_PPV_ARGS(output6.GetAddressOf())))) { return; }

	DXGI_OUTPUT_DESC1 outputDesc = {};
	ThrowIfFailed(output6->GetDesc1(&outputDesc));

	/*-------------------------------------------------------------------
	-        Get the display information that we are presenting to
	---------------------------------------------------------------------*/
	if (outputDesc.ColorSpace != DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020) { return; }

	/*-------------------------------------------------------------------
	-       Log output
	---------------------------------------------------------------------*/
	gu::string hdrSentense 
		         = "////////////////////////////////////////////////\n";
	hdrSentense += "             HDR meta data\n";
	hdrSentense += "////////////////////////////////////////////////\n";
	hdrSentense += gu::string("MinLuminance : ") + gu::string::FromNumber(outputDesc.MinLuminance) + "[nits]\n";
	hdrSentense += gu::string("MaxLuminance : ") + gu::string::FromNumber(outputDesc.MaxLuminance) + "[nits]\n";
	hdrSentense += gu::string("MaxFullFrameLuminance : ") + gu::string::FromNumber(outputDesc.MaxFullFrameLuminance) + "[nits]\n";
	_RPT1(_CRT_WARN, "%s\n",hdrSentense.CString());
}
#pragma endregion Main Function

# pragma region Color Space
/*!**********************************************************************
*  @brief     ピクセルフォーマットに従って色空間を設定します.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHISwapchain::EnsureSwapChainColorSpace()
{
#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 4
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
	_colorSpace = colorSpace;
#endif
}

/*!**********************************************************************
*  @brief     手動で色空間を設定します.
*  @param[in] const core::DisplayColorGamut colorGamut : 色空間
*  @param[in] const core::DisplayOutputFormat displayFormat : 出力フォーマット
*  @return    void
*************************************************************************/
void RHISwapchain::EnsureSwapChainColorSpace(const core::DisplayColorGamut colorGamut, const core::DisplayOutputFormat displayFormat)
{
	Confirm(_swapchain);

	/*-------------------------------------------------------------------
	-                  Displayの出力フォーマットの設定
	---------------------------------------------------------------------*/
	auto colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709; // srgb
	
	switch (displayFormat)
	{
		// gamma 2.2
		case core::DisplayOutputFormat::SDR_SRGB:
		case core::DisplayOutputFormat::SDR_Rec709:
		{
			colorSpace = colorGamut == core::DisplayColorGamut::Rec2020_D65 ? 
				DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P2020 : DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
			break;
		}

		// gamma ST.2084
		case core::DisplayOutputFormat::HDR_ACES_1000nit_ST2084:
		case core::DisplayOutputFormat::HDR_ACES_2000nit_ST2084:
		{
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			break;
		}

		// gamma 1.0 (Linear)
		case core::DisplayOutputFormat::HDR_ACES_1000nit_ScRGB:
		case core::DisplayOutputFormat::HDR_ACES_2000nit_ScRGB:
		{
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
			break;
		}

	}

	/*-------------------------------------------------------------------
	-                  色空間を設定する
	---------------------------------------------------------------------*/
	if (_colorSpace == colorSpace) { return; }

	// color spaceがサポートされているかのチェック
#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 4
	uint32 colorSpaceSupport = 0;

	const auto result = _swapchain->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport);
	if (SUCCEEDED(result) && ((colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) != 0))
#endif

#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 3
	{
		ThrowIfFailed(_swapchain->SetColorSpace1(colorSpace));
		_colorSpace = colorSpace;
	}
#endif
}

/****************************************************************************
*                     SetHDRMetaData
****************************************************************************/
/* @fn        void RHISwapchain::SetHDRMetaData()
* 
*  @brief     Set HDR Meta Data
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHISwapchain::SetHDRMetaData()
{
#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 4
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
#endif
}
#pragma endregion   Color Space