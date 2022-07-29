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
	class RHIDevice;
	class RHICommandList;
	class RHICommandAllocator;
	class RHICommandQueue;
	class RHISwapchain;
	class RHIFence;
	class RHIDescriptorHeap;
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
		static const std::uint32_t FRAME_BUFFER_COUNT = 3;
		static const std::uint32_t VSYNC = 0; // 0: don't wait, 1:wait(60fps)
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
		std::shared_ptr<RHICommandList> GetCommandList() const noexcept { return _commandList; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LowLevelGraphicsEngine() = default;
		~LowLevelGraphicsEngine() = default;
	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
#pragma region SetUp
		void SelectDevice(APIVersion apiVersion);
		void CreateCommandObject();
		void CreateSwapchain();
		void CreateSyncObject();
		void CreateDescriptorHeaps();
#pragma endregion SetUp
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		/* @brief : Graphics API version. (DirectX12 or Vulkan)*/
		APIVersion _apiVersion = APIVersion::Unknown;
		/* @brief : Logical Device*/
		std::shared_ptr<RHIDevice>  _device = nullptr;
		/* @brief : Command List*/
		std::shared_ptr<RHICommandQueue>     _commandQueue = nullptr;
		std::shared_ptr<RHICommandAllocator> _commandAllocator[FRAME_BUFFER_COUNT] = { nullptr };
		std::shared_ptr<RHICommandList>      _commandList = nullptr;
		/* @brief : Swap chain */
		std::shared_ptr<RHISwapchain> _swapchain = nullptr;
		/* @brief : Syncronization between CPU and GPU*/
		std::shared_ptr<RHIFence> _fences[FRAME_BUFFER_COUNT];
		/* @brief : DescriptorHeap*/
		std::shared_ptr<RHIDescriptorHeap> _rtvHeap       = nullptr; // for directX12
		std::shared_ptr<RHIDescriptorHeap> _dsvHeap       = nullptr; // for directX12
		std::shared_ptr<RHIDescriptorHeap> _cbvSrvUavHeap = nullptr; // common
		std::shared_ptr<RHIDescriptorHeap> _samplerHeap   = nullptr;
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
	};
}
#endif