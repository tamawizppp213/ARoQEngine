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
#include <d3d12.h>

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
		void Destroy() override;

#pragma region Create Function
		void                                              SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) override;

		std::shared_ptr<core::RHIFrameBuffer>             CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		
		std::shared_ptr<core::RHIFrameBuffer>             CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil = nullptr) override;
		
		std::shared_ptr<core::RHIFence>                   CreateFence(const std::uint64_t fenceValue = 0, const std::wstring& name = L"") override;
		
		std::shared_ptr<core::RHICommandList>             CreateCommandList(const std::shared_ptr<core::RHICommandAllocator>& commandAllocator, const std::wstring& name) override;
		
		std::shared_ptr<core::RHICommandQueue>            CreateCommandQueue    (const core::CommandListType type, const std::wstring& name) override;
		
		std::shared_ptr<core::RHICommandAllocator>        CreateCommandAllocator(const core::CommandListType type, const std::wstring& name) override;
		
		std::shared_ptr<core::RHISwapchain>               CreateSwapchain       (const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0, const bool isValidHDR = true) override;
		
		std::shared_ptr<core::RHISwapchain>               CreateSwapchain(const core::SwapchainDesc& desc) override;

		std::shared_ptr<core::RHIDescriptorHeap>          CreateDescriptorHeap  (const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		
		std::shared_ptr<core::RHIDescriptorHeap>          CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo) override;
		
		std::shared_ptr<core::RHIRenderPass>              CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		
		std::shared_ptr<core::RHIRenderPass>              CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		
		std::shared_ptr<core::GPUGraphicsPipelineState>   CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override ;  // after action: setting pipeline
		
		std::shared_ptr<core::GPUComputePipelineState>    CreateComputePipelineState(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		
		std::shared_ptr<core::RHIResourceLayout>          CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt, const std::wstring& name=L"ResourceLayout") override;
		
		std::shared_ptr<core::GPUPipelineFactory>         CreatePipelineFactory() override;
		
		std::shared_ptr<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		std::shared_ptr<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		std::shared_ptr<core::GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo); // both
		
		std::shared_ptr<core::GPUBuffer>                  CreateBuffer(const core::GPUBufferMetaData& metaData, const std::wstring& name = L"") override;
		
		std::shared_ptr<core::GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name = L"") override;
		
		std::shared_ptr<core::GPUTexture>                 CreateTextureEmpty() override;

		//std::shared_ptr<core::GPURayTracingPipelineState> CreateRayTracingPipelineState(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override;
		
		std::shared_ptr<core::RayTracingGeometry>         CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const std::shared_ptr<core::GPUBuffer>& vertexBuffer, const std::shared_ptr<core::GPUBuffer>& indexBuffer = nullptr) override;
		
		std::shared_ptr<core::ASInstance>                 CreateASInstance(
			const std::shared_ptr<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF, const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) override;
		
		std::shared_ptr<core::BLASBuffer>                 CreateRayTracingBLASBuffer(const std::vector<std::shared_ptr<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags) override;
		
		std::shared_ptr<core::TLASBuffer>                 CreateRayTracingTLASBuffer(const std::vector<std::shared_ptr<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) override;

#pragma endregion Create Function

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		DeviceComPtr  GetDevice () const noexcept { return _device; }

		std::uint32_t GetShadingRateImageTileSize() const { return _variableRateShadingImageTileSize; }
		
		std::shared_ptr<core::RHIDescriptorHeap>   GetDefaultHeap(const core::DescriptorHeapType heapType) override;

		std::uint32_t GetNodeCount() const { return _deviceNodeCount; }

		const rhi::core::HDRDisplayInfo& GetHDRDisplayInfo() const { return _displayInfo; }

		void SetName(const std::wstring& name) override;

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel() const { return _maxSupportedFeatureLevel; }

		D3D_SHADER_MODEL GetMaxSupportedShaderModel() const { return _maxSupportedShaderModel; }

		bool IsSupportedTearingSupport     () const noexcept { return _isSupportedTearing; }

		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; }

		bool IsSupportedHDR                () const override { return _isSupportedHDR; };

		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShadingTier1 || _isSupportedVariableRateShadingTier2; }
		
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; }

		bool IsSupportedDrawIndirected     () const override { return true; }
		
		bool IsSupportedGeometryShader     () const override { return true; }
		
		bool IsSupportedRenderPass         () const override { return _isSupportedRenderPass; }

		bool IsSupportedDepthBoundsTest    () const override { return _isSupportedDepthBoundsTest; }


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice();

		~RHIDevice();

		RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		DeviceComPtr  _device        = nullptr;

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		bool          _isSupportedRayTracing = true;
		bool          _isSupportedTearing    = true;
		bool          _isSupportedHDR        = true;
		bool          _isSupportedVariableRateShadingTier1 = true;
		bool          _isSupportedVariableRateShadingTier2 = true;
		bool          _isSupportedMeshShading              = true;
		bool          _isSupportedRenderPass               = true;
		bool          _isSupportedRayQuery                 = true;
		bool          _isSupportedDepthBoundsTest          = true;

		/* @brief : The maximum D3D12 feature level supported. 0 if not supported*/
		D3D_FEATURE_LEVEL _maxSupportedFeatureLevel = (D3D_FEATURE_LEVEL)0;

		/*` @brief : Thre maximum Shader Model supported. 0 if not supported*/
		D3D_SHADER_MODEL _maxSupportedShaderModel = (D3D_SHADER_MODEL)0;

		/* @brief Tier1 (few available pipeline resources)-> Tier3 (A lot of available pipeline resources*/
		D3D12_RESOURCE_BINDING_TIER _resourceBindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		/* @brief For the HeapTier, it checks if the buffer, RenderTarget and DepthStencil, TargetStencil and depth stencil texture rendering can be used in the same heap*/
		D3D12_RESOURCE_HEAP_TIER    _resourceHeapTier    = D3D12_RESOURCE_HEAP_TIER_1;

		std::uint32_t _deviceNodeCount = 0;

		std::uint32_t _variableRateShadingImageTileSize = 0;
		std::uint32_t _4xMsaaQuality = 0;

		rhi::core::HDRDisplayInfo _displayInfo;
	private:
		/****************************************************************************
		**                Private Enum Class
		*****************************************************************************/
		enum DefaultHeapType
		{
			CBV_SRV_UAV,
			RTV,
			DSV,
		};

		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void ReportLiveObjects();

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		void FindHighestFeatureLevel();
		void FindHighestShaderModel();
		void CheckDXRSupport();
		void CheckVRSSupport();
		void CheckDepthBoundsTestSupport();
		void CheckMultiSampleQualityLevels(const core::PixelFormat format);
		void CheckMeshShadingSupport();
		void CheckResourceTiers();
		void SetupDisplayHDRMetaData();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::map<DefaultHeapType, std::shared_ptr<core::RHIDescriptorHeap>> _defaultHeap;
	};
}
#endif