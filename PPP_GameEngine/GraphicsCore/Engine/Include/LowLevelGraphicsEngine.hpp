//////////////////////////////////////////////////////////////////////////////////
///             @file   LowLevelGraphicsEngine.hpp
///             @brief  Low level API grapchis engine
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
public:
	/****************************************************************************
	**                Static Configuration
	*****************************************************************************/
	static constexpr std::uint32_t FRAME_BUFFER_COUNT = 3;
	static constexpr std::uint32_t VSYNC = 0; // 0: don't wait, 1:wait(60fps)
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp(rhi::core::APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance);
	void BeginDrawFrame(); // call at begin draw frame
	void EndDrawFrame();   // call at end draw frame
	void OnResize(const size_t newWidth, const size_t newHeight);
	void ShutDown();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	/* @brief : Device (Create GPU Resource Function List)*/
	std::shared_ptr<rhi::core::RHIDevice> GetDevice() const noexcept { return _device; }
	/* @brief : CommandList (Regist GPU Commands) */
	std::shared_ptr<rhi::core::RHICommandList> GetCommandList(const rhi::core::CommandListType type, const std::uint32_t frameIndex) const noexcept { return _commandLists.at(frameIndex).at(type); }
	/* @brief : Return Current Frame Index*/
	std::uint32_t   GetCurrentFrameIndex() const noexcept { return _currentFrameIndex; }

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	LowLevelGraphicsEngine() = default;
	~LowLevelGraphicsEngine();
protected:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
#pragma region SetUp

#pragma endregion SetUp
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/* @brief : Graphics API version. (DirectX12 or Vulkan)*/
	rhi::core::APIVersion _apiVersion = rhi::core::APIVersion::Unknown;
	/* @brief : graphics API instance (select graphics api)*/
	std::shared_ptr<rhi::core::RHIInstance> _instance = nullptr;
	/* @brief : gpu display adapter (basically discrete gpu adapter)*/
	std::shared_ptr<rhi::core::RHIDisplayAdapter> _adapter = nullptr;
	/* @brief : Logical Device*/
	std::shared_ptr<rhi::core::RHIDevice>  _device = nullptr;
	/* @ brief : Command queue (graphics, compute, transfer)*/
	std::shared_ptr<rhi::core::RHICommandQueue> _graphicsCommandQueue = nullptr;
	std::shared_ptr<rhi::core::RHICommandQueue> _computeCommandQueue  = nullptr;
	/* @brief : Command List*/
	std::vector<std::map<rhi::core::CommandListType, std::shared_ptr<rhi::core::RHICommandList>>> _commandLists;

	/* @ brief : CPU-GPU synchronization*/
	std::shared_ptr<rhi::core::RHIFence> _fence = nullptr;
	std::uint64_t _fenceValues[FRAME_BUFFER_COUNT] = {0}; // for swapchain fence value
	std::uint64_t _fenceValue = 0;                        // current frame fence value
	/* @brief : Rendering swapchain*/
	std::shared_ptr<rhi::core::RHISwapchain> _swapchain = nullptr;
	/* @brief : Default rendering pass*/
	std::vector<std::shared_ptr<rhi::core::RHIRenderPass>> _renderPasses = { nullptr }; // color only
	std::vector<std::shared_ptr<rhi::core::RHIFrameBuffer>> _frameBuffers = { nullptr };
	/* @brief : current frame index*/
	std::uint32_t _currentFrameIndex = 0;
	/* @brief : Windows API*/
	HWND      _hwnd      = nullptr;
	HINSTANCE _hInstance = nullptr;

	/* @brief : Rendering Configuration*/ // å„Ç≈configÉtÉ@ÉCÉãÇçÏê¨Ç∑ÇÈ.
	bool _useHDR             = false;
	bool _useRayTracing      = true;
	rhi::core::PixelFormat _pixelFormat = rhi::core::PixelFormat::R8G8B8A8_UNORM;
		
	// shutdown check
	bool _hasCalledShutDown = false;
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
};

#endif