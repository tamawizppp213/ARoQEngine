//////////////////////////////////////////////////////////////////////////////////
///             @file   LowLevelGraphicsEngine.hpp
///             @brief  Low level API grapchis engine. 
///             How To: á@ Call StartUp function at first
///                     áA Create Draw frame, call beginDrawFrame and EndDrawFrame() ...loop....
///                     áB Finally, you should call shutdown when you wanna finish to render
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LOW_LEVEL_GRAPHICS_ENGINE_HPP
#define LOW_LEVEL_GRAPHICS_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIInstance;
	//class RHIDevice;
	class RHIDisplayAdapter;
	//class RHICommandList;
	class RHICommandQueue;
	class RHISwapchain;
	class RHIFence;
	class RHIDescriptorHeap;
	class RHIRenderPass;
	class RHIFrameBuffer;
}
/****************************************************************************
*				  			LowLevelGraphicsEngine
*************************************************************************//**
*  @class     LowLevelGraphicsEngine
*  @brief     LowLevelGraphicsEngine
*****************************************************************************/
class LowLevelGraphicsEngine final : public NonCopyable
{
protected:
	using InstancePtr    = gu::SharedPointer<rhi::core::RHIInstance>;
	using AdapterPtr     = gu::SharedPointer<rhi::core::RHIDisplayAdapter>;
	using DevicePtr      = gu::SharedPointer<rhi::core::RHIDevice>;
	using CommandListPtr = gu::SharedPointer<rhi::core::RHICommandList>;
	using CommandQueuePtr = gu::SharedPointer<rhi::core::RHICommandQueue>;

public:
	/****************************************************************************
	**                Static Configuration
	*****************************************************************************/
	static constexpr std::uint32_t FRAME_BUFFER_COUNT = 3;

	static constexpr std::uint32_t VSYNC = 0; // 0: don't wait, 1:wait(60fps)

	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	/* @brief : Rendering engine start function.*/
	void StartUp(rhi::core::APIVersion apiVersion, void* hwnd, void* hInstance);

	/* @brief : The first call to the Draw function generates the back buffer image and executes the Default render pass. */
	void BeginDrawFrame();

	/* @brief : Draw back buffer render pass*/
	void BeginSwapchainRenderPass();
	
	/* @brief : Call at the end of the Draw function to execute the command list and Flip the Swapchain. */
	void EndDrawFrame();   // call at end draw frame
	
	/* @brief : Resize swapchain*/
	void OnResize(const size_t newWidth, const size_t newHeight);
	
	/* @brief : Release all render resources*/
	void ShutDown();


	/* @brief : Execute command queue. Return fence signal value */
	std::uint64_t FlushGPUCommands(const rhi::core::CommandListType type, const bool stillMidFrame = false);

	/* @brief Wait command queue (in GPU), but if the stopCPU is set true, gpu and cpu wait.*/
	void WaitExecutionGPUCommands(const rhi::core::CommandListType type, const std::uint64_t waitValue, const bool stopCPU);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	/* @brief : Device (Create GPU Resource Function List)*/
	DevicePtr GetDevice() const noexcept { return _device; }

	/* @brief : CommandList (Regist GPU Commands) */
	CommandListPtr GetCommandList(const rhi::core::CommandListType type) const noexcept { return _commandLists.at(type); }
	
	CommandQueuePtr GetCommandQueue(const rhi::core::CommandListType type) const noexcept { return _commandQueues.at(type); }

	/* @brief : Default RenderPass*/
	gu::SharedPointer<rhi::core::RHIRenderPass> GetRenderPass() const noexcept { return _renderPass; }

	/* @brief : Non clear render pass*/
	gu::SharedPointer<rhi::core::RHIRenderPass> GetDrawContinueRenderPass() const noexcept { return _drawContinueRenderPass; }

	/* @brief : Frame buffer*/
	gu::SharedPointer<rhi::core::RHIFrameBuffer> GetFrameBuffer(const std::uint32_t frameIndex) const noexcept { return _frameBuffers[frameIndex]; }

	/* @brief : Return Current Frame Index*/
	std::uint32_t   GetCurrentFrameIndex() const { return _currentFrameIndex; }

	rhi::core::PixelFormat GetBackBufferFormat() const { return _pixelFormat; }

	gu::SharedPointer<rhi::core::RHISwapchain> GetSwapchain() const noexcept { return _swapchain; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	LowLevelGraphicsEngine() = default;

	~LowLevelGraphicsEngine();

protected:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	void SetFrameBuffers(const int width, const int height, 
		const rhi::core::ClearValue& clearColor = rhi::core::ClearValue(0.0f, 0.3f, 0.3f, 1.0f),
		const rhi::core::ClearValue& clearDepthColor = rhi::core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f));

#pragma region SetUp

#pragma endregion SetUp
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/* @brief : Graphics API version. (DirectX12 or Vulkan)*/
	rhi::core::APIVersion _apiVersion = rhi::core::APIVersion::Unknown;

	/* @brief : graphics API instance (select graphics api)*/
	InstancePtr _instance = nullptr;

	/* @brief : gpu display adapter (basically discrete gpu adapter)*/
	AdapterPtr _adapter = nullptr;

	/* @brief : Logical Device*/
	DevicePtr  _device = nullptr;

	/* @ brief : Command queue (graphics, compute, transfer)*/
	std::map<rhi::core::CommandListType, gu::SharedPointer<rhi::core::RHICommandQueue>> _commandQueues;

	/* @brief : Command List*/
	std::map<rhi::core::CommandListType, CommandListPtr> _commandLists;

	/* @ brief : CPU-GPU synchronization*/
	gu::SharedPointer<rhi::core::RHIFence> _fence = nullptr;
	std::uint64_t _fenceValue = 0;  // current frame fence value
	
	/* @brief : Rendering swapchain*/
	gu::SharedPointer<rhi::core::RHISwapchain> _swapchain = nullptr;
	
	/* @brief : Default rendering pass*/
	gu::SharedPointer<rhi::core::RHIRenderPass> _renderPass = { nullptr }; 
	gu::SharedPointer<rhi::core::RHIRenderPass> _drawContinueRenderPass = nullptr;
	std::vector<gu::SharedPointer<rhi::core::RHIFrameBuffer>> _frameBuffers = { nullptr };
	
	/* @brief : current frame index*/
	std::uint32_t _currentFrameIndex = 0;
	
	/* @brief : Windows API*/
	std::int32_t _width = 0;
	std::int32_t _height = 0;

	/* @brief : Rendering Configuration*/ // å„Ç≈configÉtÉ@ÉCÉãÇçÏê¨Ç∑ÇÈ.
	bool _useHDR             = false;
	bool _useRayTracing      = true;
	rhi::core::PixelFormat _pixelFormat        = rhi::core::PixelFormat::R8G8B8A8_UNORM;
	rhi::core::PixelFormat _depthStencilFormat = rhi::core::PixelFormat::D32_FLOAT;
		
	// shutdown check
	bool _hasCalledShutDown = false;

	bool _hasInitialized = false;

	/****************************************************************************
	**                Heap Config
	*****************************************************************************/
	static constexpr int RTV_DESC_COUNT = 1000;
	static constexpr int DSV_DESC_COUNT = 100;
	static constexpr int CBV_DESC_COUNT = 1024 * 10;
	static constexpr int UAV_DESC_COUNT = 1024 * 10;
	static constexpr int SRV_DESC_COUNT = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;

private:
	void SetUpRenderResource();
	void SetUpHeap();
	void SetUpFence();

};

#endif