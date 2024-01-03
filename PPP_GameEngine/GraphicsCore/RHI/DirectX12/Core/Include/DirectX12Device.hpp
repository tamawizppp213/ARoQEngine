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
	*  @brief     Logical Device : shared_ptrで管理すること
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice, public gu::EnableSharedFromThis<RHIDevice>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Destroy() override;

#pragma region Create Function
		void                                              SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) override;

		gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		gu::SharedPointer<core::RHIFence>                   CreateFence(const std::uint64_t fenceValue = 0, const std::wstring& name = L"") override;
		
		gu::SharedPointer<core::RHICommandList>             CreateCommandList(const gu::SharedPointer<core::RHICommandAllocator>& commandAllocator, const std::wstring& name) override;
		
		gu::SharedPointer<core::RHICommandQueue>            CreateCommandQueue    (const core::CommandListType type, const std::wstring& name) override;
		
		gu::SharedPointer<core::RHICommandAllocator>        CreateCommandAllocator(const core::CommandListType type, const std::wstring& name) override;
		
		gu::SharedPointer<core::RHISwapchain>               CreateSwapchain       (const gu::SharedPointer<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const std::uint32_t vsync = 0, const bool isValidHDR = true) override;
		
		gu::SharedPointer<core::RHISwapchain>               CreateSwapchain(const core::SwapchainDesc& desc) override;

		gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap  (const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		
		gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo) override;
		
		gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth) override;
		
		gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth) override;
		
		gu::SharedPointer<core::GPUGraphicsPipelineState>   CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override ;  // after action: setting pipeline
		
		gu::SharedPointer<core::GPUComputePipelineState>    CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		
		gu::SharedPointer<core::RHIResourceLayout>          CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt, const std::wstring& name=L"ResourceLayout") override;
		
		gu::SharedPointer<core::GPUPipelineFactory>         CreatePipelineFactory() override;
		
		gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo); // both
		
		gu::SharedPointer<core::GPUBuffer>                  CreateBuffer(const core::GPUBufferMetaData& metaData, const std::wstring& name = L"") override;
		
		gu::SharedPointer<core::GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name = L"") override;
		
		gu::SharedPointer<core::GPUTexture>                 CreateTextureEmpty() override;

		//gu::SharedPointer<core::GPURayTracingPipelineState> CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		gu::SharedPointer<core::RayTracingGeometry>         CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer = nullptr) override;
		
		gu::SharedPointer<core::ASInstance>                 CreateASInstance(
			const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF, const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) override;
		
		gu::SharedPointer<core::BLASBuffer>                 CreateRayTracingBLASBuffer(const std::vector<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags) override;
		
		gu::SharedPointer<core::TLASBuffer>                 CreateRayTracingTLASBuffer(const std::vector<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) override;

#pragma endregion Create Function

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		DeviceComPtr  GetDevice () const noexcept { return _device; }

		std::uint32_t GetShadingRateImageTileSize() const { return _variableRateShadingImageTileSize; }
		
		gu::SharedPointer<core::RHIDescriptorHeap>   GetDefaultHeap(const core::DescriptorHeapType heapType) override;

		std::uint32_t GetNodeCount() const { return _deviceNodeCount; }

		const rhi::core::HDRDisplayInfo& GetHDRDisplayInfo() const { return _displayInfo; }

		void SetName(const std::wstring& name) override;

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel() const { return _maxSupportedFeatureLevel; }

		D3D_SHADER_MODEL GetMaxSupportedShaderModel() const { return _maxSupportedShaderModel; }

		D3D12_RESOURCE_BINDING_TIER GetResourceBindingTier() const { return _resourceBindingTier; }

		D3D12_RESOURCE_HEAP_TIER GetResourceHeapTier() const { return _resourceHeapTier; }

		D3D12_RAYTRACING_TIER GetRayTracingTier() const { return _rayTracingTier; }

		D3D12_VARIABLE_SHADING_RATE_TIER GetVariableShadingRateTier() const { return _variableRateShadingTier; }

		CommandSignaturePtr GetDefaultDrawIndexedIndirectCommandSignature() const { return _drawIndexedIndirectCommandSignature; }

		D3D_ROOT_SIGNATURE_VERSION GetMaxRootSignatureVersion() const { return _maxRootSignatureVersion; }

		std::uint32_t MaxUsableSamplerHeapCount() const { return _maxSamplerHeapCount; }

		std::uint32_t MaxUsableDescriptorHeapCount() const { return _maxDescriptorHeapCount; }

		bool IsSupportedAllowTearing       () const noexcept { return _isSupportedAllowTearing; }

		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; }

		bool IsSupportedHDR                () const override { return _isSupportedHDR; };

		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShadingTier1 || _isSupportedVariableRateShadingTier2; }
		
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; }

		bool IsSupportedDrawIndirected     () const override { return true; }
		
		bool IsSupportedGeometryShader     () const override { return true; }
		
		bool IsSupportedRenderPass         () const override { return _isSupportedRenderPass; }

		bool IsSupportedDepthBoundsTest    () const override { return _isSupportedDepthBoundsTest; }

		bool IsSupportedSamplerFeedback    () const override { return _isSupportedSamplerFeedback; }

		bool IsSupportedStencilReferenceFromPixelShader() const override { return _isSupportedStencilReferenceFromPixelShader; }
		
		bool IsSupportedWaveLane() const override { return _isSupportedWaveLane; }

		bool IsSupportedNative16bitOperation() const override { return _isSupported16bitOperation; }

		bool IsSupportedAtomicOperation() const override { return _isSupportedAtomicOperation; }


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDevice();

		~RHIDevice();

		RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter, const core::RHIMultiGPUMask& mask = core::RHIMultiGPUMask::SingleGPU());

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
		bool _isSupportedRayTracing = true;
		bool _isSupportedAllowTearing = true;
		bool _isSupportedHDR        = true;
		bool _isSupportedVariableRateShadingTier1 = true;
		bool _isSupportedVariableRateShadingTier2 = true;
		bool _isSupportedMeshShading              = true;
		bool _isSupportedRenderPass               = true;
		bool _isSupportedRayQuery                 = true;
		bool _isSupportedDepthBoundsTest          = true;
		bool _isSupportedSamplerFeedback          = true;
		bool _isSupportedBindless                 = true;
		bool _isSupportedStencilReferenceFromPixelShader   = true;
		bool _isSupported16bitOperation           = false;

		/* @brief : The maximum D3D12 feature level supported. 0 if not supported*/
		D3D_FEATURE_LEVEL _maxSupportedFeatureLevel = (D3D_FEATURE_LEVEL)0;

		/*` @brief : Thre maximum Shader Model supported. 0 if not supported*/
		D3D_SHADER_MODEL _maxSupportedShaderModel = (D3D_SHADER_MODEL)0;

		/* @brief For the HeapTier, it checks if the buffer, RenderTarget and DepthStencil, TargetStencil and depth stencil texture rendering can be used in the same heap*/
		D3D12_RESOURCE_HEAP_TIER    _resourceHeapTier    = D3D12_RESOURCE_HEAP_TIER_1;

		std::uint32_t _deviceNodeCount = 0;

		/*-------------------------------------------------------------------
		-                Descriptor Heap Info
		--------------------------------------------------------------------- */
		/* @brief Tier1 (few available pipeline resources)-> Tier3 (A lot of available pipeline resources*/
		D3D12_RESOURCE_BINDING_TIER _resourceBindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		std::uint32_t _maxDescriptorHeapCount = 0;
		std::uint32_t _maxSamplerHeapCount    = 0;

		/*-------------------------------------------------------------------
		-                    Bindless resource
		---------------------------------------------------------------------*/
		core::BindlessResourceType _bindlessResourceType = core::BindlessResourceType::Unsupported;

		/*-------------------------------------------------------------------
		-                RenderPass
		--------------------------------------------------------------------- */
		/*----------------------------------------------------------------------    
		*  @brief : Tier0 : レンダーパスは未実装, 一応ソフトウェアエミュレーションを介してのみ提供とは言っているので, サポート対象とはいたします. 
        *           Tier1 : Render Target, Depth Bufferの書き込みの高速化, しかし, UAVの書き込みはRenderPass内では効率的にはならないとのこと
        *           Tier2 : Render Target, Depth Bufferの書き込みの高速化, UAVの書き込みもOK 
		/*----------------------------------------------------------------------*/
		D3D12_RENDER_PASS_TIER _renderPassTier = D3D12_RENDER_PASS_TIER_0;

		/*-------------------------------------------------------------------
		-               DirectX12RayTracing
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief: Tier1_1の場合,　以下の内容が可能となります
		*        ・ExecuteIndirectを介したDispatchRays呼び出しのサポート, RayQueryの使用
	    *　　　　  ・AddToStateObjectを介した既存の状態オブジェクトへの増分追加
		*        ・SkipTriangles, skip procedual primitivesのフラグの使用
		*----------------------------------------------------------------------*/
		D3D12_RAYTRACING_TIER _rayTracingTier = D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

		/*-------------------------------------------------------------------
		-               variable rate shading
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : Tier1はDrawCallごとに指定でき, 描画される対象に一律に適用します.
        *           Tier2はDrawCallだけでなく, プリミティブごと, タイルごとにも設定できるようになります
		*----------------------------------------------------------------------*/
		D3D12_VARIABLE_SHADING_RATE_TIER _variableRateShadingTier = D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED;

		std::uint32_t _variableRateShadingImageTileSize = 0;

		bool _isSupportedLargerVariableRateShadingSize  = false;

		/*-------------------------------------------------------------------
		-                       MSAA
		---------------------------------------------------------------------*/
		std::uint32_t _msaaQuality     = 0;
		std::uint32_t _maxMSAASampleCount = 0;
		static constexpr std::uint32_t DESIRED_MAX_MSAA_SAMPLE_COUNT = 16;

		/*-------------------------------------------------------------------
		-                        HDR
		---------------------------------------------------------------------*/
		rhi::core::HDRDisplayInfo _displayInfo;

		/*-------------------------------------------------------------------
		-                        SamplerFeedback
		---------------------------------------------------------------------*/
		/* @brief テクスチャのサンプリング情報をキャプチャ, 記録するための機能です.: 
		      Tier 0.9 : D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMPのみに対応する, mipmapも0のみ対応
* 　　　　　　　 Tier 1.0 : 全てのTexture addressing modeで使用可能*/
		D3D12_SAMPLER_FEEDBACK_TIER _samplerFeedbackTier = D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;

		/*-------------------------------------------------------------------
		-               Wave 
		---------------------------------------------------------------------*/
		/* @brief : HLSLで明示的にGPU上で複数スレッドの使用が可能となります.
		            Wave : プロセッサ上の同時に実行されるスレッドの集合
					Lane : 個々のスレッド*/
		bool _isSupportedWaveLane = false;
		std::uint32_t _minWaveLaneCount = 0;
		std::uint32_t _maxWaveLaneCount = 0;

		/*-------------------------------------------------------------------
		-              RootSignature
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : RootSignatureの最新バージョン
		* 　　　　　　　1_0 or 1: Default 
		*           1_1     : Descriptorに対して最適化を行うためのフラグを設置可能
		/*----------------------------------------------------------------------*/
		D3D_ROOT_SIGNATURE_VERSION _maxRootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1;

		/*-------------------------------------------------------------------
		-               Atomic
		---------------------------------------------------------------------*/
		bool _isSupportedAtomicInt64OnGroupSharedSupported      = false;
		bool _isSupportedInt64OnDescriptorHeapResourceSupported = false;
		bool _isSupportedAtomicOperation                        = false;
		bool _isSupportedAtomicInt64OnTypedResource             = false;
		bool _isSupportedAtomicUInt64                           = false;

		/*-------------------------------------------------------------------
		-               IndirectDraw
		---------------------------------------------------------------------*/
		CommandSignaturePtr _drawIndexedIndirectCommandSignature = nullptr;

	private:
		/****************************************************************************
		**                Private Enum Class
		*****************************************************************************/
		enum DefaultHeapType
		{
			CBV_SRV_UAV,
			RTV,
			DSV,
			Sampler,
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
		void CheckRenderPassSupport();
		void CheckVRSSupport();
		void CheckDepthBoundsTestSupport();
		void CheckMultiSampleQualityLevels();
		void CheckMeshShadingSupport();
		void CheckResourceTiers();
		void CheckMaxHeapSize();
		void CheckBindlessSupport();
		void CheckSamplerFeedbackSupport();
		void CheckAllowTearingSupport();
		void CheckStencilReferenceFromPixelShaderSupport();
		void CheckWaveLaneSupport();
		void CheckNative16bitOperation();
		void CheckAtomicOperation();
		void CheckMaxRootSignatureVersion();
		void SetupDisplayHDRMetaData();
		void SetupDefaultCommandSignatures();
		void SetGPUDebugBreak();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::map<DefaultHeapType, gu::SharedPointer<core::RHIDescriptorHeap>> _defaultHeap;
	};
}
#endif