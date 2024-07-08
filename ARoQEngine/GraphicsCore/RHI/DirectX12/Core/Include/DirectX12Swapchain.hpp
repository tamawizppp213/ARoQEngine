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
	****************************************************************************/
	/* @brief Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public  rhi::core::RHISwapchain
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     NextImageが準備出来たとき, Signalを発行して次のフレームインデックスを返します
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
		*  @param[in] const gu::uint64 signalValue : 準備完了時にフェンスに書き込まれるシグナル値
		*  @return    gu::uint32 次のフレームインデックス
		*************************************************************************/
		virtual gu::uint32 PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue) override;
		
		/*!**********************************************************************
		*  @brief     バックバッファを次フレームのフロントバッファにします 
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
		*  @param[in] const gu::uint64 waitValue : 待機する値
		*  @return    void
		*************************************************************************/
		virtual void Present(const gu::SharedPointer<core::RHIFence>& fence, gu::uint64 waitValue) override ;
		
		/*!**********************************************************************
		*  @brief     画面サイズを変更します. 一度すべてを再構築し、再度アップデートします. @n
		* 　　　　　　　 事前にコマンドの実行が終了していることを確認してください.
		*  @param[in] const gu::uint32 width : 画面の幅
		*  @param[in] const gu::uint32 height : 画面の高さ
		*  @return    void
		*************************************************************************/
		virtual void Resize(const gu::uint32 width, const gu::uint32 height) override;

		/*!**********************************************************************
		*  @brief     フルスクリーンモードと指定された解像度のスクリーンモードを切り替える
		* 　　　　　　  (isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する)
		*  @param[in] const bool isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する
		*  @return    void
		*************************************************************************/
		virtual void SwitchFullScreenMode(const bool isOn) override;
		
		/*!**********************************************************************
		*  @brief     HDRモードとSDRモードを切り替える.
		*  @param[in] const bool enableHDR : true -> HDR, false -> SDR
		*  @return    void
		*************************************************************************/
		virtual void SwitchHDRMode(const bool enableHDR) override;

		/*!**********************************************************************
		*  @brief     現在のディスプレイ出力がHDR機能をサポートしているかを調べる
		*  @param[in] void
		*  @return    bool HDRがサポートされているか
		*************************************************************************/
		bool IsSupportedHDRInCurrentDisplayOutput();

		/*!**********************************************************************
		*  @brief     HDR上でのモニター設定を出力上に表示します. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void LogHDROutput() const;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     現在描画コマンドを詰め込みたいバックバッファのインデックスを返します
		*  @param[in] void
		*  @return    gu::uint32 スワップチェインの描画する幅
		*************************************************************************/
		virtual gu::uint8 GetCurrentBufferIndex() const override;

		/*!**********************************************************************
		*  @brief     DirectX12で使用するSwapchainのComptrを返します.
		*  @param[in] void
		*  @return    SwapchainComPtr SwapchainのComポインタ
		*************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief デストラクタ*/
		~RHISwapchain();
		
		/*! @brief 一個ずつ初期化*/
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const gu::SharedPointer<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const gu::uint8 frameBufferCount = 3, const gu::uint8 vsync = 0,
			const bool isValidHDR = true,
			const bool isFullScreen = false
			);

		/*! @brief Descriptorでまとめて初期化*/
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::SwapchainDesc& desc);

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Swapchainの初期化
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void SetUp();

		#pragma endregion

		#pragma region Protected Property
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
		*----------------------------------------------------------------------*/
		bool _useAllowTearing = false;

		/*! @brief フルスクリーン状態が解除されたか*/
		bool _isFullScreenLost = false;

	#if DXGI_MAX_SWAPCHAIN_INTERFACE < 3
		gu::uint32 _currentBackBufferIndex = 0;
	#endif
		#pragma endregion Protected Property

	private:
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     ピクセルフォーマットに従って色空間を設定します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void EnsureSwapChainColorSpace();
		/*!**********************************************************************
		*  @brief     手動で色空間を設定します.
		*  @param[in] const core::DisplayColorGamut colorGamut : 色空間
		*  @param[in] const core::DisplayOutputFormat displayFormat : 出力フォーマット
		*  @return    void
		*************************************************************************/
		void EnsureSwapChainColorSpace(const core::DisplayColorGamut colorGamut, const core::DisplayOutputFormat displayFormat);

		void SetHDRMetaData();
		#pragma endregion
	};
}
#endif