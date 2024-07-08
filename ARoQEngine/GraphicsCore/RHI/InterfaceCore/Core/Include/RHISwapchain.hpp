//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.hpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_SWAP_CHAIN_HPP
#define RHI_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandQueue;
	class RHIFence;
	class GPUTexture;

	/****************************************************************************
	*				  			SwapchainDesc
	****************************************************************************/
	/* @brief Swapchainを生成する際の初期設定
	*****************************************************************************/
	struct SwapchainDesc
	{
	protected:
		using CommandQueuePtr = gu::SharedPointer<RHICommandQueue>;

	public:
		/*! @brief コマンドを実行するキュー. フロントバッファに表示する際に使用します.*/
		CommandQueuePtr CommandQueue = nullptr;

		/*! @brief 初期フレームにおけるウィンドウ情報*/
		WindowInfo WindowInfo = {};

		/*! @brief 描画で使用するピクセルフォーマット*/
		PixelFormat PixelFormat = PixelFormat::Unknown; 

		/*! @brief フレームバッファの数*/
		gu::uint8 FrameBufferCount = 3;

		/*! @brief VSyncの設定 (0: VSync無効化, 1: VSync有効, 以降fpsが半分になります.)*/
		gu::uint8 VSync = 0;

		/*! @brief HDRの有効化*/
		bool IsValidHDR = true;

		/*! @brief 立体視の使用*/
		bool IsValidStereo = false;

		/*! @brief フルスクリーンの使用*/
		bool IsFullScreen = false;

		~SwapchainDesc()
		{
			if (CommandQueue) { CommandQueue.Reset(); }
		}
	};

	/****************************************************************************
	*				  			RHISwapchain
	****************************************************************************/
	/* @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     NextImageが準備出来たとき, Signalを発行して次のフレームインデックスを返します
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
		*  @param[in] const gu::uint64 signalValue : 準備完了時にフェンスに書き込まれるシグナル値
		*  @return    gu::uint32 次のフレームインデックス
		*************************************************************************/
		virtual gu::uint32 PrepareNextImage(const gu::SharedPointer<RHIFence>& fence, const gu::uint64 signalValue) = 0;
		
		/*!**********************************************************************
		*  @brief     バックバッファを次フレームのフロントバッファにします
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : フェンス
		*  @param[in] const gu::uint64 waitValue : 待機する値
		*  @return    void
		*************************************************************************/
		virtual void Present(const gu::SharedPointer<RHIFence>& fence, gu::uint64 waitValue) = 0;
		
		/*!**********************************************************************
		*  @brief     画面サイズを変更します. 一度すべてを再構築し、再度アップデートします. @n
		* 　　　　　　　 事前にコマンドの実行が終了していることを確認してください.
		*  @param[in] const gu::uint32 width : 画面の幅
		*  @param[in] const gu::uint32 height : 画面の高さ
		*  @return    void
		*************************************************************************/
		virtual void Resize(const gu::uint32 width, const gu::uint32 height) = 0;

		/*!**********************************************************************
		*  @brief     フルスクリーンモードと指定された解像度のスクリーンモードを切り替える
		* 　　　　　　  (isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する)
		*  @param[in] const bool isOn : true->フルスクリーンモードに移行する. false : windowモードに移行する
		*  @return    void
		*************************************************************************/
		virtual void SwitchFullScreenMode(const bool isOn) = 0;

		/*!**********************************************************************
		*  @brief     HDRモードとSDRモードを切り替える.
		*  @param[in] const bool enableHDR : true -> HDR, false -> SDR
		*  @return    void
		*************************************************************************/
		virtual void SwitchHDRMode(const bool enableHDR) = 0;
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     現在描画コマンドを詰め込みたいバックバッファのインデックスを返します
		*  @param[in] void
		*  @return    gu::uint32 スワップチェインの描画する幅
		*************************************************************************/
		virtual gu::uint8 GetCurrentBufferIndex() const = 0;

		/*!**********************************************************************
		*  @brief     スワップチェインの描画する幅を返します
		*  @param[in] void
		*  @return    gu::uint32 スワップチェインの描画する幅
		*************************************************************************/
		__forceinline gu::uint32  GetWidth() const noexcept { return _desc.WindowInfo.Width; }

		/*!**********************************************************************
		*  @brief     スワップチェインの描画する高さを返します
		*  @param[in] void
		*  @return    gu::uint32 スワップチェインの描画する高さ
		*************************************************************************/
		__forceinline gu::uint32  GetHeight() const noexcept { return _desc.WindowInfo.Height; }

		/*!**********************************************************************
		*  @brief     描画で使用するピクセルフォーマット
		*  @param[in] void
		*  @return    PixelFormat 描画で使用するピクセルフォーマット
		*************************************************************************/
		__forceinline PixelFormat GetPixelFormat() const noexcept { return _desc.PixelFormat; }

		/*!**********************************************************************
		*  @brief     Windowの幅や高さ, ハンドルを返します
		*  @param[in] void
		*  @return    const WindowInfo& Windowの情報
		*************************************************************************/
		__forceinline const WindowInfo& GetWindowInfo() const noexcept { return _desc.WindowInfo; }

		/*!**********************************************************************
		*  @brief     バックバッファの参照を返します
		*  @param[in] const gu::uint8 フレームバッファのインデックス
		*  @return    gu::SharedPointer<GPUTexture> バックバッファ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUTexture> GetBuffer(const gu::uint8 index) { return _backBuffers[index]; }
		
		/*!**********************************************************************
		*  @brief     フレームバッファの総数を返します. 
		*  @param[in] void
		*  @return    gu::uint8 フレームバッファの数
		*************************************************************************/
		__forceinline gu::uint8 GetBufferCount() const noexcept { return _desc.FrameBufferCount; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion
		
	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		RHISwapchain() = default;

		/*! @brief デストラクタ*/
		virtual ~RHISwapchain()
		{
			_backBuffers.Clear(); _backBuffers.ShrinkToFit();
			if (_device)       { _device.Reset(); }
		};

		/*! @brief 一つずつ設定*/
		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, PixelFormat pixelFormat, gu::uint8 frameBufferCount = 3, gu::uint8 vsync = 0, bool isValidHDR = true, bool isFullScreen = false)
		{
			_device = device; _desc.CommandQueue = commandQueue; _desc.WindowInfo = windowInfo; _desc.PixelFormat = pixelFormat; _desc.VSync = vsync; _desc.FrameBufferCount = frameBufferCount; _desc.IsValidHDR = isValidHDR; _desc.IsFullScreen = isFullScreen;
		}

		/*! @brief SwapchainのDescriptorを使って初期化*/
		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const SwapchainDesc& desc):
			_device(device), _desc(desc)
		{
			
		}
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		
		/*! @brief バックバッファ*/
		gu::DynamicArray<gu::SharedPointer<GPUTexture>> _backBuffers; 
		
		/*! @brief スワップチェインの現在の設定*/
		SwapchainDesc _desc = {};

		#pragma endregion
	};
}
#endif