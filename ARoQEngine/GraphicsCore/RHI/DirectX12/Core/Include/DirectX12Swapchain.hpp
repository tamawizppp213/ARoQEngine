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
		/*----------------------------------------------------------------------
		*  @brief :  NextImageが準備出来たとき, Signalを発行して次のフレームインデックスを返します
		/*----------------------------------------------------------------------*/
		gu::uint32 PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  フロントバッファを表示します. 
		/*----------------------------------------------------------------------*/
		void Present(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t waitValue) override ;
		
		/*----------------------------------------------------------------------
		*  @brief :  画面サイズを変更する。一度すべてを再構築し、再度アップデートする。
		/*----------------------------------------------------------------------*/
		void Resize(const size_t width, const size_t height) override ;
		
		/* @brief : Return current frame buffer*/
		size_t GetCurrentBufferIndex() const override;

		/*----------------------------------------------------------------------
		*  @brief :  フルスクリーンモードと指定された解像度のスクリーンモードを切り替える
		* 　　　　　　  (isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する)
		/*----------------------------------------------------------------------*/       
		void SwitchFullScreenMode(const bool isOn) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  HDRモードとSDRモードを切り替える
		/*----------------------------------------------------------------------*/
		void SwitchHDRMode(const bool enableHDR) override;

		/*----------------------------------------------------------------------
		*  @brief :  現在のディスプレイ出力がHDR機能をサポートしているかを調べる
		/*----------------------------------------------------------------------*/
		bool IsSupportedHDRInCurrentDisplayOutput();

		/*----------------------------------------------------------------------
		*  @brief :  HDR上でのモニター設定を出力上に表示します.
		/*----------------------------------------------------------------------*/
		void LogHDROutput();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~RHISwapchain();
		
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const gu::SharedPointer<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const size_t frameBufferCount = 3, const std::uint32_t vsync = 0,
			const bool isValidHDR = true,
			const bool isFullScreen = false
			);

		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::SwapchainDesc& desc);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUp();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : swapchain
		SwapchainComPtr      _swapchain = nullptr;

		// @brief : swapchainの設定
		DXGI_SWAP_CHAIN_FLAG _swapchainFlag;

		// @brief : ピクセルの色フォーマット
		DXGI_FORMAT          _backBufferFormat; // color format

		// @brief : 色空間
		DXGI_COLOR_SPACE_TYPE _colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

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
		void EnsureSwapChainColorSpace(const core::DisplayColorGamut colorGamut, const core::DisplayOutputFormat displayFormat);

		void SetHDRMetaData();
	};
}
#endif