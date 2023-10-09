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
	*  @brief     Logical Device : shared_ptr�ŊǗ����邱��
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

		D3D12_RESOURCE_BINDING_TIER GetResourceBindingTier() const { return _resourceBindingTier; }

		D3D12_RESOURCE_HEAP_TIER GetResourceHeapTier() const { return _resourceHeapTier; }

		D3D12_RAYTRACING_TIER GetRayTracingTier() const { return _rayTracingTier; }

		D3D12_VARIABLE_SHADING_RATE_TIER GetVariableShadingRateTier() const { return _variableRateShadingTier; }

		CommandSignaturePtr GetDefaultDrawIndexedIndirectCommandSignature() const { return _drawIndexedIndirectCommandSignature; }

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
		*  @brief : Tier0 : �����_�[�p�X�͖�����, �ꉞ�\�t�g�E�F�A�G�~�����[�V��������Ă̂ݒ񋟂Ƃ͌����Ă���̂�, �T�|�[�g�ΏۂƂ͂������܂�. 
        *           Tier1 : Render Target, Depth Buffer�̏������݂̍�����, ������, UAV�̏������݂�RenderPass���ł͌����I�ɂ͂Ȃ�Ȃ��Ƃ̂���
        *           Tier2 : Render Target, Depth Buffer�̏������݂̍�����, UAV�̏������݂�OK 
		/*----------------------------------------------------------------------*/
		D3D12_RENDER_PASS_TIER _renderPassTier = D3D12_RENDER_PASS_TIER_0;

		/*-------------------------------------------------------------------
		-               DirectX12RayTracing
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief: Tier1_1�̏ꍇ,�@�ȉ��̓��e���\�ƂȂ�܂�
		*        �EExecuteIndirect�����DispatchRays�Ăяo���̃T�|�[�g, RayQuery�̎g�p
	    *�@�@�@�@  �EAddToStateObject����������̏�ԃI�u�W�F�N�g�ւ̑����ǉ�
		*        �ESkipTriangles, skip procedual primitives�̃t���O�̎g�p
		*----------------------------------------------------------------------*/
		D3D12_RAYTRACING_TIER _rayTracingTier = D3D12_RAYTRACING_TIER_NOT_SUPPORTED;

		/*-------------------------------------------------------------------
		-               variable rate shading
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : Tier1��DrawCall���ƂɎw��ł�, �`�悳���ΏۂɈꗥ�ɓK�p���܂�.
        *           Tier2��DrawCall�����łȂ�, �v���~�e�B�u����, �^�C�����Ƃɂ��ݒ�ł���悤�ɂȂ�܂�
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
		/* @brief �e�N�X�`���̃T���v�����O�����L���v�`��, �L�^���邽�߂̋@�\�ł�.: 
		      Tier 0.9 : D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP�݂̂ɑΉ�����, mipmap��0�̂ݑΉ�
* �@�@�@�@�@�@�@ Tier 1.0 : �S�Ă�Texture addressing mode�Ŏg�p�\*/
		D3D12_SAMPLER_FEEDBACK_TIER _samplerFeedbackTier = D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;

		/*-------------------------------------------------------------------
		-               Wave 
		---------------------------------------------------------------------*/
		/* @brief : HLSL�Ŗ����I��GPU��ŕ����X���b�h�̎g�p���\�ƂȂ�܂�.
		            Wave : �v���Z�b�T��̓����Ɏ��s�����X���b�h�̏W��
					Lane : �X�̃X���b�h*/
		bool _isSupportedWaveLane = false;
		std::uint32_t _minWaveLaneCount = 0;
		std::uint32_t _maxWaveLaneCount = 0;

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
		void SetupDisplayHDRMetaData();
		void SetupDefaultCommandSignatures();
		void SetGPUDebugBreak();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::map<DefaultHeapType, std::shared_ptr<core::RHIDescriptorHeap>> _defaultHeap;
	};
}
#endif