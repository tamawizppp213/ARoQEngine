//////////////////////////////////////////////////////////////////////////////////
///             @file   LowLevelGraphicsEngine.hpp
///             @brief  Low level API grapchis engine. 
///             How To: ① Call StartUp function at first
///                     ② Create Draw frame, call beginDrawFrame and EndDrawFrame() ...loop....
///                     ③ Finally, you should call shutdown when you wanna finish to render
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LOW_LEVEL_GRAPHICS_ENGINE_HPP
#define LOW_LEVEL_GRAPHICS_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			LowLevelGraphicsEngine
****************************************************************************/
/* @class     LowLevelGraphicsEngine
*  @brief     LowLevelGraphicsEngine
*****************************************************************************/
class LowLevelGraphicsEngine final : public gu::NonCopyAndMove
{
public:
	#pragma region Static Const
	static constexpr gu::uint32 FRAME_BUFFER_COUNT = 3;

	static constexpr gu::uint32 VSYNC = 1; // 0: don't wait, 1:wait(60fps)
	#pragma endregion

	#pragma region Public Function
	/*!**********************************************************************
	*  @brief     Graphics Engineを起動し, 描画フレームをスタートします. 
	*  @param[in] const rhi::core::GraphicsAPI 
	*  @param[in] void* window handle
	*  @param[in] void* instance Handle
	*  @return    void
	*************************************************************************/
	void StartUp(const rhi::core::GraphicsAPI apiVersion, void* hwnd, void* hInstance);

	/*!**********************************************************************
	*  @brief     CommandListを開き, 描画フレームをスタートします.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void BeginDrawFrame();

	/*!**********************************************************************
	*  @brief     バックバッファに書き込むためのRenderPassを開始します.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void BeginSwapchainRenderPass();
	
	/*!**********************************************************************
	*  @brief     CommandListを閉じ, 描画フレームを終了し, バックバッファを切り替えます.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void EndDrawFrame();
	
	/*!**********************************************************************
	*  @brief     グラフィクスエンジンを終了します. 
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void ShutDown();

	/*!**********************************************************************
	*  @brief     Swapchainをリサイズします.
	*  @param[in] const gu::uint32 新しい幅
	*  @param[in] const gu::uint32 新しい高さ
	*  @return    void
	*************************************************************************/
	void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight);

	/*!**********************************************************************
	*  @brief     対象のコマンドリストを実行し, フェンスのシグナル値を返します.
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @param[in] const bool まだフレーム中かどうか
	*  @return    gu::uint64 フェンスのシグナル値
	*************************************************************************/
	gu::uint64 FlushGPUCommands(const rhi::core::CommandListType type, const bool stillMidFrame = false);

	/*!**********************************************************************
	*  @brief     コマンドキューを呼び出して前までの処理が完了するまでGPUを待機します. 必要に応じてCPUも待機します.
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @param[in] const gu::uint64 待機するフェンスの値
	*  @param[in] const bool CPUも待機するかどうか
	*  @return    void
	*************************************************************************/
	void WaitExecutionGPUCommands(const rhi::core::CommandListType type, const gu::uint64 waitValue, const bool stopCPU);
	
	#pragma endregion

	#pragma region Public Property
	/*!**********************************************************************
	*  @brief     論理デバイスを取得します. 
	*  @param[in] void
	*  @return    DevicePtr : 論理デバイスのSharedPointer
	*************************************************************************/
	__forceinline RHIDevicePtr GetDevice() const noexcept { return _device; }

	/*!**********************************************************************
	*  @brief     ComandListを取得します.
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @return    CommandListPtr : CommandListのSharedPointer
	*************************************************************************/
	__forceinline RHICommandListPtr GetCommandList(const rhi::core::CommandListType type) const noexcept { return _commandLists.At(type); }
	
	/*!**********************************************************************
	*  @brief     ComandQueueを取得します.
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @return    CommandQueuePtr : CommandQueueのSharedPointer
	*************************************************************************/
	__forceinline RHICommandQueuePtr GetCommandQueue(const rhi::core::CommandListType type) const noexcept { return _commandQueues.At(type); }

	/*!**********************************************************************
	*  @brief     デフォルトのRenderPassの取得. レンダーパス開始時に, 画面のクリアを行います.  
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @return    gu::SharedPointer<rhi::core::RHIRenderPass>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIRenderPass> GetDrawClearRenderPass() const noexcept { return _renderPass; }

	/*!**********************************************************************
	*  @brief     RenderPassの取得. レンダーパス開始時に, 画面のクリアは行いません.
	*  @param[in] const rhi::core::CommandListType コマンドリストの種類
	*  @return    gu::SharedPointer<rhi::core::RHIRenderPass>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIRenderPass> GetDrawContinueRenderPass() const noexcept { return _drawContinueRenderPass; }

	/*!**********************************************************************
	*  @brief     レンダーターゲットとデプスステンシルのテクスチャやResourceViewを使用するクラスです.
	*  @param[in] const gu::uint32 Frameのインデックス
	*  @return    gu::SharedPointer<rhi::core::RHIFrameBuffer>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIFrameBuffer> GetFrameBuffer(const gu::uint32 frameIndex) const noexcept { return _frameBuffers[frameIndex]; }

	/*!**********************************************************************
	*  @brief     現在書き込み中のフレーム番号を取得します. 
	*  @param[in] void
	*  @return    gu::uint32 フレーム番号
	*************************************************************************/
	__forceinline gu::uint32 GetCurrentFrameIndex() const { return _currentFrameIndex; }

	/*!**********************************************************************
	*  @brief     バックバッファのピクセルフォーマットを取得します
	*  @param[in] void
	*  @return    rhi::core::PixelFormat
	*************************************************************************/
	__forceinline rhi::core::PixelFormat GetBackBufferFormat() const { return _pixelFormat; }

	/*!**********************************************************************
	*  @brief     描画バッファを毎フレーム交換するためのSwapchainのポインタを返します
	*  @param[in] void
	*  @return    RHISwapchainPtr
	*************************************************************************/
	__forceinline RHISwapchainPtr GetSwapchain() const noexcept 
	{
		return _swapchain; 
	}
	
	/*!**********************************************************************
	*  @brief     GPU計測のQueryHeapを返します
	*  @param[in] const rhi::core::QueryHeapType QueryHeapの種類
	*  @return    RHIQueryPtr
	*************************************************************************/
	__forceinline RHIQueryPtr GetQuery(const rhi::core::QueryHeapType queryType) 
	{
		return _queryHeaps.At(queryType); 
	}
	#pragma endregion

	#pragma region Public Constructor and Destructor
	/*! @brief デフォルトコンストラクタ*/
	LowLevelGraphicsEngine() = default;

	/*! @brief デストラクタ*/
	~LowLevelGraphicsEngine();

	#pragma endregion

protected:
	#pragma region Protected Function
	void SetFrameBuffers(const int width, const int height, 
		const rhi::core::ClearValue& clearColor = rhi::core::ClearValue(0.0f, 0.3f, 0.3f, 1.0f),
		const rhi::core::ClearValue& clearDepthColor = rhi::core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f));
	#pragma endregion 

	#pragma region Protected Property
	/*! @brief Graphics API バージョン (現在はDirectX12 or Vulkan)*/
	rhi::core::GraphicsAPI _apiVersion = rhi::core::GraphicsAPI::Unknown;

	/* @brief : graphics API instance (select graphics api)*/
	RHIInstancePtr _instance = nullptr;

	/* @brief : gpu display adapter (basically discrete gpu adapter)*/
	RHIDisplayAdapterPtr _adapter = nullptr;

	/* @brief : Logical Device*/
	RHIDevicePtr  _device = nullptr;

	/* @ brief : Command queue (graphics, compute, transfer)*/
	gu::SortedMap<rhi::core::CommandListType, RHICommandQueuePtr> _commandQueues;

	/* @brief : Command List*/
	gu::SortedMap<rhi::core::CommandListType, RHICommandListPtr> _commandLists;
	
	/* @brief : Default rendering pass*/
	gu::SharedPointer<rhi::core::RHIRenderPass> _renderPass = { nullptr }; 
	gu::SharedPointer<rhi::core::RHIRenderPass> _drawContinueRenderPass = nullptr;
	gu::DynamicArray<gu::SharedPointer<rhi::core::RHIFrameBuffer>> _frameBuffers = { nullptr };
	
	/* @brief : current frame index*/
	gu::uint32 _currentFrameIndex = 0;

	/*! @brief TimeStamp*/
	rhi::core::GPUTimingCalibrationTimestamp _beginDrawFrameTimeStamp = {};
	rhi::core::GPUTimingCalibrationTimestamp _endDrawFrameTimeStamp = {};
	gu::double64 _gpuTimer = 0.0;
	gu::double64 _cpuTimer = 0.0;

#pragma region Rendering Variables
	/*----------------------------------------------------------------------
	*  @brief : 描画バッファを毎フレーム交換するためのSwapchain
	*----------------------------------------------------------------------*/
	gu::SharedPointer<rhi::core::RHISwapchain> _swapchain = nullptr;

	/*----------------------------------------------------------------------
	*  @brief : CPUとGPU間の同期・待機処理を行うためのフェンス
	*----------------------------------------------------------------------*/
	gu::SharedPointer<rhi::core::RHIFence> _fence = nullptr;
	gu::uint64 _fenceValue = 0;  // current frame fence value

	/*----------------------------------------------------------------------
	*  @brief : HDRを使用する (true : 使用, false : 使用しない)
	*----------------------------------------------------------------------*/
	bool _useHDR = false;

	/*----------------------------------------------------------------------
	*  @brief : レイトレーシングを使用する (true : 使用, false : 使用しない)
	*----------------------------------------------------------------------*/
	bool _useRayTracing = true;

	/*----------------------------------------------------------------------
	*  @brief : Swapchain等で使用するデフォルトのピクセルフォーマット
	*           SDR想定はR8G8B8A8_UNORM
	* 　　　　　　　HDR想定はR16G16B16A16_FLOATを使用します.
	*----------------------------------------------------------------------*/
	rhi::core::PixelFormat _pixelFormat = rhi::core::PixelFormat::R8G8B8A8_UNORM;

	/*----------------------------------------------------------------------
	*  @brief : 通常描画で使用するDepth Stencilフォーマット
	*           デフォルトはStencilを使用せず, D32_FLOATを使用します
	*----------------------------------------------------------------------*/
	rhi::core::PixelFormat _depthStencilFormat = rhi::core::PixelFormat::D32_FLOAT;
		
	/*----------------------------------------------------------------------
	*  @brief : スクリーンの縦横
	*----------------------------------------------------------------------*/
	gu::uint32 _width  = 0;
	gu::uint32 _height = 0;

#pragma endregion Rendering Variables

	// shutdown check
	bool _hasCalledShutDown = false;

	bool _hasInitialized = false;

	/*----------------------------------------------------------------------
	*  @brief : GPU負荷計測のためのQueryHeap
	*----------------------------------------------------------------------*/
	gu::SortedMap<rhi::core::QueryHeapType, gu::SharedPointer<rhi::core::RHIQuery>> _queryHeaps = {};

	/****************************************************************************
	**                Heap Config
	*****************************************************************************/
	static constexpr int RTV_DESC_COUNT = 1000;
	static constexpr int DSV_DESC_COUNT = 100;
	static constexpr int CBV_DESC_COUNT = 1024 * 10;
	static constexpr int UAV_DESC_COUNT = 1024 * 10;
	static constexpr int SRV_DESC_COUNT = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;

	#pragma endregion 

private:
	#pragma region Private Function
	void SetUpRenderResource();
	void SetUpHeap();
	void SetUpFence();
	void SetUpQuery();
	#pragma endregion

};

#endif