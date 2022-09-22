//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Device.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_DEVICE_HPP
#define DIRECTX12_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include "DirectX12Core.hpp"
#include <dxgiformat.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                          Device class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			Device class
	*************************************************************************//**
	*  @class     Device
	*  @brief     Logical Device : shared_ptrÇ≈ä«óùÇ∑ÇÈÇ±Ç∆
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void SetUp() override;
		void Destroy() override;
#pragma region Create Function
		std::shared_ptr<core::RHIFrameBuffer>           CreateFrameBuffer(const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFrameBuffer>           CreateFrameBuffer(const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		std::shared_ptr<core::RHIFence>                 CreateFence(const std::uint64_t fenceValue = 0) override;
		std::shared_ptr<core::RHICommandList>           CreateCommandList(const std::shared_ptr<core::RHICommandAllocator>& commandAllocator) override;
		std::shared_ptr<core::RHICommandQueue>          CreateCommandQueue(const core::CommandListType type) override;
		std::shared_ptr<core::RHICommandAllocator>      CreateCommandAllocator() override;
		std::shared_ptr<core::RHISwapchain>             CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0, const bool isValidHDR = true) override;
		std::shared_ptr<core::RHIDescriptorHeap>        CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		std::shared_ptr<core::RHIDescriptorHeap>        CreateDescriptorHeap(const std::vector<core::DescriptorHeapType>& heapTypes, const std::vector<size_t>& maxDescriptorCounts) override;
		std::shared_ptr<core::RHIRenderPass>            CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::RHIRenderPass>            CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		std::shared_ptr<core::GPUGraphicsPipelineState> CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override ;  // after action: setting pipeline
		std::shared_ptr<core::GPUComputePipelineState>  CreateComputePipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		std::shared_ptr<core::RHIResourceLayout>        CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt) override;
		std::shared_ptr<core::GPUPipelineFactory>       CreatePipelineFactory() override;
		std::shared_ptr<core::GPUSampler>               CreateSampler(const core::SamplerInfo& samplerInfo); // both
		std::shared_ptr<core::GPUBuffer>                CreateBuffer(const core::GPUBufferMetaData& metaData) override;
		std::shared_ptr<core::GPUTexture>               CreateTexture(const core::GPUTextureMetaData& metaData) override;
#pragma endregion Create Function
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		DeviceComPtr  GetDevice () const noexcept { return _device; }
		DXGI_FORMAT   GetBackBufferFormat() const noexcept { return _backBufferFormat; }
		std::uint32_t GetShadingRateImageTileSize() const { return _variableRateShadingImageTileSize; }
		std::shared_ptr<core::RHICommandQueue> GetCommandQueue(const core::CommandListType commandListType) override ;
		std::shared_ptr<core::RHICommandAllocator> GetCommandAllocator(const core::CommandListType commandListType, const std::uint32_t frameCount = 0) override;
		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		bool IsSupportedTearingSupport() const noexcept { return _isSupportedTearing; }
		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; }
		bool IsSupportedHDR                () const override { return _isSupportedHDR; };
		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShadingTier1 || _isSupportedVariableRateShadingTier2; }
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; }
		bool IsSupportedDrawIndirected     () const override { return true; }
		bool IsSupportedGeometryShader     () const override { return true; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice();
		~RHIDevice();
		RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter, const std::uint32_t frameCount);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		DeviceComPtr  _device        = nullptr;
		DXGI_FORMAT   _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
		bool          _isSupportedRayTracing = true;
		bool          _isSupportedTearing    = true;
		bool          _isSupportedHDR        = true;
		bool          _isSupportedVariableRateShadingTier1 = true;
		bool          _isSupportedVariableRateShadingTier2 = true;
		bool          _isSupportedMeshShading              = true;
		bool          _isSupportedRenderPass               = true;
		bool          _isSupportedRayQuery                 = true;
		std::uint32_t _variableRateShadingImageTileSize = 0;
		std::uint32_t _4xMsaaQuality = 0;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void ReportLiveObjects();

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		void CheckDXRSupport();
		void CheckVRSSupport();
		void CheckHDRDisplaySupport();
		void CheckMultiSampleQualityLevels();
		void CheckMeshShadingSupport();
	};
}
#endif