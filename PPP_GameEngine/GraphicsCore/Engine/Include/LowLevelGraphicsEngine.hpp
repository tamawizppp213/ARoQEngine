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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct HWND__;
struct HINSTANCE__;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIInstance;
	class RHIDevice;
	class RHIDisplayAdapter;
	class RHICommandList;
	class RHICommandAllocator;
	class RHICommandQueue;
	class RHISwapchain;
	class RHIFence;
	class RHIDescriptorHeap;
	class RHIRenderPass;
	/****************************************************************************
	*				  			LowLevelGraphicsEngine
	*************************************************************************//**
	*  @class     LowLevelGraphicsEngine
	*  @brief     LowLevelGraphicsEngine
	*****************************************************************************/
	class LowLevelGraphicsEngine final : public NonCopyable
	{
		using HWND = HWND__*;
		using HINSTANCE = HINSTANCE__*;
	public:
		/****************************************************************************
		**                Static Configuration
		*****************************************************************************/
		static constexpr std::uint32_t FRAME_BUFFER_COUNT = 3;
		static constexpr std::uint32_t VSYNC = 0; // 0: don't wait, 1:wait(60fps)
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance);
		void BeginDrawFrame(); // call at begin draw frame
		void EndDrawFrame();   // call at end draw frame
		void OnResize();
		void ShutDown();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>      GetDevice     () const noexcept { return _device; }

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
		APIVersion _apiVersion = APIVersion::Unknown;
		/* @brief : graphics API instance (select graphics api)*/
		std::shared_ptr<RHIInstance> _instance = nullptr;
		/* @brief : gpu display adapter (basically discrete gpu adapter)*/
		std::shared_ptr<RHIDisplayAdapter> _adapter = nullptr;
		/* @brief : Logical Device*/
		std::shared_ptr<RHIDevice>  _device = nullptr;
		/* @ brief : Command queue (graphics, compute, transfer)*/
		std::shared_ptr<RHICommandQueue> _graphicsCommandQueue = nullptr;
		std::shared_ptr<RHICommandQueue> _computeCommandQueue  = nullptr;
		std::shared_ptr<RHICommandAllocator> _graphicsCommandAllocator = nullptr;
		std::shared_ptr<RHICommandAllocator> _computeCommandAllocator = nullptr;
		/* @ brief : CPU-GPU synchronization*/
		std::shared_ptr<RHIFence> _fence = nullptr;
		/* @brief : Rendering swapchain*/
		std::shared_ptr<RHISwapchain> _swapchain = nullptr;
		/* @brief : Default rendering pass*/
		std::shared_ptr<RHIRenderPass> _renderPass = nullptr; // color only
		/* @brief : Heap*/
		std::shared_ptr<RHIDescriptorHeap> _csvSrvUavHeap = nullptr;
		std::shared_ptr<RHIDescriptorHeap> _rtvHeap = nullptr;
		std::shared_ptr<RHIDescriptorHeap> _dsvHeap = nullptr;
		/* @brief : current frame index*/
		int _currentFrameIndex = 0;
		/* @brief : Windows API*/
		HWND      _hwnd      = nullptr;
		HINSTANCE _hInstance = nullptr;

		/* @brief : Rendering Configuration*/ // å„Ç≈configÉtÉ@ÉCÉãÇçÏê¨Ç∑ÇÈ.
		bool _useHDR             = false;
		bool _useRayTracing      = true;
		PixelFormat _pixelFormat = PixelFormat::R16G16B16A16_FLOAT;
		
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
		void SetUpRenderPass();
		void SetUpHeap();
	};
}
#endif