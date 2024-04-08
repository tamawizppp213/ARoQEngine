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
#if USE_INTEL_EXTENSION
struct INTCExtensionContext;
struct INTCExtensionInfo;
#endif

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
	class RHIDevice : public core::RHIDevice, public gu::EnableSharedFromThis<RHIDevice>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �_���f�o�C�X��j������.
		*  @note      ���̊֐����Ă΂Ȃ���SharedPointer�Ńf�X�g���N�^���Ă΂�Ȃ��\��������������.
		*************************************************************************/
		void Destroy() override;

		#pragma region Create Function
		/*!**********************************************************************
		*  @brief     �e�f�B�X�N���v�^�q�[�v��DefaultHeapCount�Ɋ�Â��č쐬���܂�
		*  @param[in] const core::DefaultHeapCount �f�B�X�N���v�^�q�[�v�̃T�C�Y�����肷��\����
		*  @return    void
		*************************************************************************/
		virtual void SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) override;

		virtual gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		virtual gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		virtual gu::SharedPointer<core::RHIFence>                   CreateFence(const gu::uint64 fenceValue = 0, const gu::tstring& name = SP("")) override;
		
		virtual gu::SharedPointer<core::RHICommandList>             CreateCommandList(const gu::SharedPointer<core::RHICommandAllocator>& commandAllocator, const gu::tstring& name) override;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M����R�}���h�L���[���쐬���܂�
		*  @param[in] const core::CommandListType : �R�}���h���X�g�̎��
		*  @param[in] const gu::tstring : �f�o�b�O��
		*  @return    gu::SharedPointer<core::RHICommandQueue> �R�}���h�L���[�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHICommandQueue> CreateCommandQueue(const core::CommandListType type, const gu::tstring& name) override;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p�����R�}���h�A���P�[�^���쐬���܂�
		*  @param[in] const core::CommandListType : �R�}���h���X�g�̎��
		*  @param[in] const gu::tstring : �f�o�b�O��
		*  @return    gu::SharedPointer<core::RHICommandAllocator> �R�}���h�A���P�[�^�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHICommandAllocator> CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name) override;
		
		virtual gu::SharedPointer<core::RHISwapchain>               CreateSwapchain       (const gu::SharedPointer<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const gu::uint32 vsync = 0, const bool isValidHDR = true) override;
		
		virtual gu::SharedPointer<core::RHISwapchain>               CreateSwapchain(const core::SwapchainDesc& desc) override;

		virtual gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap  (const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		
		virtual gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap(const gu::SortedMap<core::DescriptorHeapType, size_t>& heapInfo) override;
		
		virtual gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth) override;
		
		virtual gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const core::Attachment& color, const gu::Optional<core::Attachment>& depth) override;
		
		virtual gu::SharedPointer<core::GPUGraphicsPipelineState>   CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override ;  // after action: setting pipeline
		
		virtual gu::SharedPointer<core::GPUComputePipelineState>    CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		
		virtual gu::SharedPointer<core::RHIResourceLayout>          CreateResourceLayout(const gu::DynamicArray<core::ResourceLayoutElement>& elements = {}, const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {}, const gu::Optional<core::Constant32Bits>& constant32Bits = {}, const gu::tstring& name=SP("ResourceLayout")) override;
		
		virtual gu::SharedPointer<core::GPUPipelineFactory>         CreatePipelineFactory() override;
		
		virtual gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		virtual gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		virtual gu::SharedPointer<core::GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo); // both
		
		/*!**********************************************************************
		*  @brief     GPU�Ŋm�ۂ���o�b�t�@���쐬���܂�. (���_�f�[�^��C���f�b�N�X�f�[�^�̊m�ۂȂǂɎg�p���܂�.)
		*  @param[in] const core::GPUBufferMetaData& : �o�b�t�@�����ۂɕK�v�ȏ����܂Ƃ߂��\����
		*  @param[in] const gu::tstring& : �f�o�b�O��
		*  @return    gu::SharedPointer<GPUBuffer> : GPU�o�b�t�@�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::GPUBuffer> CreateBuffer(const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("")) override;
		
		virtual gu::SharedPointer<core::GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP("")) override;
		
		virtual gu::SharedPointer<core::GPUTexture>                 CreateTextureEmpty() override;

		//gu::SharedPointer<core::GPURayTracingPipelineState> CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		virtual gu::SharedPointer<core::RayTracingGeometry>         CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer = nullptr) override;
		
		virtual gu::SharedPointer<core::ASInstance>                 CreateASInstance(
			const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const gu::uint32 instanceID, const gu::uint32 instanceContributionToHitGroupIndex,
			const gu::uint32 instanceMask = 0xFF, const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) override;
		
		virtual gu::SharedPointer<core::BLASBuffer>                 CreateRayTracingBLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags) override;
		
		virtual gu::SharedPointer<core::TLASBuffer>                 CreateRayTracingTLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) override;

		virtual gu::SharedPointer<core::RHIQuery> CreateQuery(const core::QueryHeapType heapType) override;

		/*!**********************************************************************
		*  @brief     Heap�̈�̊m�ۂƎ��ۂ�GPU�Ƀf�[�^���������Ɋm�ۂ���̂𗼕��s���֐�
		*  @note      �{�֐���DirectX12��p�̊֐��ł�. 
		*  @param[out] const ResourceComPtr&        :���ꂩ�烁������������GPU���\�[�X
		*  @param[in]  const D3D12_RESOURCE_DESC&   : ���������m�ۂ���ۂ�GPU���\�[�X���
		*  @param[in]  const D3D12_HEAP_PROPERTIES& : �ǂ̏ꏊ�Ƀ��������m�ۂ��邩���������m�ۂ̎d����ݒ肷��
		*  @param[in]  const D3D12_RESOURCE_STATES  : �������m�ی�, �ŏ��ɐݒ肳���GPU���\�[�X�̏��
		*  @param[in]  const D3D12_CLEAR_VALUE*     : �N���A�J���[
		*  @return     HRESULT : ����������������
		*************************************************************************/
		HRESULT CreateCommittedResource
		(
			ResourceComPtr&            resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // �قƂ�ǂ�const
			const D3D12_HEAP_PROPERTIES& heapProp,
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);

		/*!**********************************************************************
		*  @brief      Heap���ɂ܂��}�b�v�܂ł͍s��Ȃ��\��ς݂̃��\�[�X���쐬. �v�͏��������s�킸�Ƀ������̈悾���m�ۂ��Ă����Ԃ̂���.
		*  @note       �{�֐���DirectX12��p�̊֐��ł�.
		*  @param[out] const ResourceComPtr&        :���ꂩ�烁������������GPU���\�[�X
		*  @param[in]  const D3D12_RESOURCE_DESC&   : ���������m�ۂ���ۂ�GPU���\�[�X���
		*  @param[in]  const D3D12_RESOURCE_STATES  : �������m�ی�, �ŏ��ɐݒ肳���GPU���\�[�X�̏��
		*  @param[in]  const D3D12_CLEAR_VALUE*     : �N���A�J���[
		*  @return     HRESULT : ����������������
		*************************************************************************/
		HRESULT CreateReservedResource
		(
			ResourceComPtr& resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // �قƂ�ǂ�const
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);

		/*!**********************************************************************
		*  @brief      ���ɍ쐬�ς݂̃q�[�v�ɔz�u����郊�\�[�X���쐬����.  Committed, Reserved, Placed�̒��ł͍ł������ɓ��삷��
		*  @note       �{�֐���DirectX12��p�̊֐��ł�.
		*  @param[out] const ResourceComPtr&        :���ꂩ�烁������������GPU���\�[�X
		*  @param[in]  const D3D12_RESOURCE_DESC&   : ���������m�ۂ���ۂ�GPU���\�[�X���
		*  @param[in]  const HeapComPtr&            : ���Ɋm�ۍς݂�GPU�q�[�v�̈�
		*  @param[in]  const gu::uint64             : �m�ۂ���q�[�v�̃I�t�Z�b�g
		*  @param[in]  const D3D12_RESOURCE_STATES  : �������m�ی�, �ŏ��ɐݒ肳���GPU���\�[�X�̏��
		*  @param[in]  const D3D12_CLEAR_VALUE*     : �N���A�J���[
		*  @return     HRESULT : ����������������
		*************************************************************************/
		HRESULT CreatePlacedResource
		(
			ResourceComPtr& resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // �قƂ�ǂ�const
			const HeapComPtr& heap,
			const gu::uint64 heapOffset,
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);
		#pragma endregion Create Function

		#pragma endregion Public Function

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief  DirectX12�Ŏg�p����_���f�o�C�X��Com�|�C���^
		*  @return DeviceComPtr : �_���f�o�C�X��COM�|�C���^. (GPUResource�̍쐬���Ɏg�p����)
		*************************************************************************/
		DeviceComPtr GetDevice() const noexcept { return _device; }

		gu::uint32 GetShadingRateImageTileSize() const { return _variableRateShadingImageTileSize; }
		
		virtual gu::SharedPointer<core::RHIDescriptorHeap> GetDefaultHeap(const core::DescriptorHeapType heapType) override;

		gu::uint32 GetNodeCount() const { return _deviceNodeCount; }

		const rhi::core::HDRDisplayInfo& GetHDRDisplayInfo() const { return _displayInfo; }

		/*!**********************************************************************
		*  @brief  �f�o�b�O����ݒ肷��.
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief  DirectX�Ŏg�p�\�ȍő�̋@�\���x�����擾���܂�.
		*************************************************************************/
		D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel() const { return _maxSupportedFeatureLevel; }

		/*!**********************************************************************
		*  @brief  DirectX�Ŏg�p�\�ȍő�̃V�F�[�_�[���f�����擾���܂�
		*  @note   ���݂�6_9���w��\�ȃT�|�[�g���x���ł���, ���ɉ����ă��x���͉�����ꍇ������܂� @n
		*          https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
		*************************************************************************/
		D3D_SHADER_MODEL GetMaxSupportedShaderModel() const { return _maxSupportedShaderModel; }

		/*!**********************************************************************
		*  @brief  �p�C�v���C���Ŏg�p�\�ȃ��\�[�X�̏���l���m�F���邽�߂Ɏg�p����
		*  @note    1. CBV : Tier 1, 2��14�܂�. Tier3 ��DescripterHeap�̍ő吔 @n
        *           2. SRV : Tier 1��128�܂�. Tier2, 3��DescripterHeap�̍ő吔 @n
        *           3. UAV : Tier1�͋@�\���x�� 11.1+�ȏ��64, ����ȊO��8, Tier2��64, Tier3��DescripterHeap�̍ő吔@n
        *           4. Sampler : Tier1��16, ����ȊO��2048 @n
        *           5. �q�[�v����Descripter�̍ő吔 Tier1, 2��1,000,000�ATier3�͖����� pn
        * �@�@�@�@�@�@https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support
		*************************************************************************/
		D3D12_RESOURCE_BINDING_TIER GetResourceBindingTier() const { return _resourceBindingTier; }

		/*!**********************************************************************
		*  @brief  �َ�Ԃ�ResourceHeap���ǂ��܂ō��݂�����̂��s���邩�𒲂ׂ郌�x�����擾���܂�
		*  @note   HeapTier�̕��ł�, �o�b�t�@�[, RenderTarget��DepthStencil, TargetStencil, �[�x�X�e���V���e�N�X�`���̃����_�����O�𓯈�q�[�v�Ŏg�p�ł��邩�𒲂ׂ܂�
        * �@�@�@�@�@ Tier1���r��, Tier2�����݉\�ł�.
        * �@�@�@�@�@ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
		*************************************************************************/
		D3D12_RESOURCE_HEAP_TIER GetResourceHeapTier() const { return _resourceHeapTier; }

		D3D12_RAYTRACING_TIER GetRayTracingTier() const { return _rayTracingTier; }

		D3D12_VARIABLE_SHADING_RATE_TIER GetVariableShadingRateTier() const { return _variableRateShadingTier; }

		CommandSignaturePtr GetDefaultDrawIndexedIndirectCommandSignature() const { return _drawIndexedIndirectCommandSignature; }

		/*!**********************************************************************
		*  @brief  ���݂̎w��\�ȍő��RootSignature�̃o�[�W�����ł�. @n
		*          1_0 or 1 : �ʏ��RootSignature @n
		*          1_1      : Descriptor�ɑ΂��čœK�����s�����߂̃t���O��ݒu�\ @n
		*          1_2      : Static sampler�̐ݒ蕝���L���� @n
		*************************************************************************/
		D3D_ROOT_SIGNATURE_VERSION GetHighestRootSignatureVersion() const { return _highestRootSignatureVersion; }

		gu::uint32 MaxUsableSamplerHeapCount() const { return _maxSamplerHeapCount; }

		gu::uint32 MaxUsableDescriptorHeapCount() const { return _maxDescriptorHeapCount; }

		bool IsSupportedAllowTearing       () const noexcept { return _isSupportedAllowTearing; }

		bool IsSupportedDxr                () const override { return _isSupportedRayTracing; }

		bool IsSupportedHDR                () const override { return _isSupportedHDR; };

		bool IsSupportedVariableRateShading() const override { return _isSupportedVariableRateShadingTier1 || _isSupportedVariableRateShadingTier2; }
		
		bool IsSupportedMeshShading        () const override { return _isSupportedMeshShading; }

		bool IsSupportedDrawIndirected     () const override { return true; }
		
		bool IsSupportedGeometryShader     () const override { return true; }
		
		bool IsSupportedRenderPass         () const override { return _isSupportedRenderPass; }

		/*!**********************************************************************
		*  @brief  �s�N�Z���`��̍ŏ�, �ő�͈͂�K�p����@�\���g���邩 (DepthBoundsTest)
		*************************************************************************/
		bool IsSupportedDepthBoundsTest () const override { return _isSupportedDepthBoundsTest; }

		bool IsSupportedSamplerFeedback    () const override { return _isSupportedSamplerFeedback; }

		bool IsSupportedStencilReferenceFromPixelShader() const override { return _isSupportedStencilReferenceFromPixelShader; }
		
		bool IsSupportedWaveLane() const override { return _isSupportedWaveLane; }

		bool IsSupportedNative16bitOperation() const override { return _isSupported16bitOperation; }

		bool IsSupportedAtomicOperation() const override { return _isSupportedAtomicOperation; }

		/*!**********************************************************************
		*  @brief  �񈳏k��UAV���g�p�\�����m�F���܂�. 
		*************************************************************************/
		bool IsSupportedAdditionalUAVType() const { return _isSupportedAdditionalUAVType; }

		#pragma endregion Public Member Function

		#pragma region Public Constructor and Destructor
		RHIDevice();

		~RHIDevice();

		RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter, const core::RHIMultiGPUMask& mask = core::RHIMultiGPUMask::SingleGPU());

		#pragma endregion Public Constructor and Destructor

	protected:
		#pragma region Protected Function

		#pragma endregion Protected Function

		#pragma region Protected Member Variables

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/*! @brief : DirectX12�̘_���f�o�C�X*/
		DeviceComPtr  _device = nullptr;

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

		/*! @brief �s�N�Z���`��̍ŏ�, �ő�͈͂�K�p����@�\���g���邩*/
		bool _isSupportedDepthBoundsTest          = true;

		/*! @brief �ǉ���UAV�ǂݍ��݂̎�ނ��g�p�ł��邩*/
		bool _isSupportedAdditionalUAVType        = true;

		bool _isSupportedSamplerFeedback          = true;
		bool _isSupportedBindless                 = true;
		bool _isSupportedStencilReferenceFromPixelShader   = true;
		bool _isSupported16bitOperation           = false;

		/*! @brief Heap�m�ۂ̃I�[�o�[�w�b�h�����炷����, �[�����������Ȃ��悤�ɂ���t���O*/
		bool _isSupportedHeapNotZero = false;

		/*! @brief �T�|�[�g����� D3D12 �@�\���x���̍ő�l�B�T�|�[�g����Ă��Ȃ��ꍇ��0*/
		D3D_FEATURE_LEVEL _maxSupportedFeatureLevel = (D3D_FEATURE_LEVEL)0;
		
		/*! @brief �T�|�[�g�����D3D12�@�\���x���̍ŏ��l (����ȏ�̍ŏ��l�̓G���[�ƂȂ�܂�)*/
		D3D_FEATURE_LEVEL _minSupportedFeatureLevel = (D3D_FEATURE_LEVEL_11_0);

		/*` @brief : Thre maximum Shader Model supported. 0 i
		f not supported*/
		D3D_SHADER_MODEL _maxSupportedShaderModel = (D3D_SHADER_MODEL)0;

		/* @brief For the HeapTier, it checks if the buffer, RenderTarget and DepthStencil, TargetStencil and depth stencil texture rendering can be used in the same heap*/
		D3D12_RESOURCE_HEAP_TIER    _resourceHeapTier    = D3D12_RESOURCE_HEAP_TIER_1;

		gu::uint32 _deviceNodeCount = 0;

		/*-------------------------------------------------------------------
		-                Descriptor Heap Info
		--------------------------------------------------------------------- */
		/* @brief Tier1 (few available pipeline resources)-> Tier3 (A lot of available pipeline resources*/
		D3D12_RESOURCE_BINDING_TIER _resourceBindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		gu::uint32 _maxDescriptorHeapCount = 0;
		gu::uint32 _maxSamplerHeapCount    = 0;

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

		gu::uint32 _variableRateShadingImageTileSize = 0;

		bool _isSupportedLargerVariableRateShadingSize  = false;

		/*-------------------------------------------------------------------
		-                       MSAA
		---------------------------------------------------------------------*/
		gu::uint32 _msaaQuality     = 0;
		gu::uint32 _maxMSAASampleCount = 0;
		static constexpr gu::uint32 DESIRED_MAX_MSAA_SAMPLE_COUNT = 16;

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
		gu::uint32 _minWaveLaneCount = 0;
		gu::uint32 _maxWaveLaneCount = 0;

		/*-------------------------------------------------------------------
		-              RootSignature
		---------------------------------------------------------------------*/
		/* @brief     ���݃T�|�[�g����Ă���ő��RootSignature�̃o�[�W�����ł�.
		   @attention CheckHighestRootSignatureVersion�֐��̎d�l��, ��ɍŐV�o�[�W�����̒l�ɍX�V���Ă����Ă�������. */
		D3D_ROOT_SIGNATURE_VERSION _highestRootSignatureVersion = D3D_ROOT_SIGNATURE_VERSION_1_2;

		/*-------------------------------------------------------------------
		-               Atomic
		---------------------------------------------------------------------*/
		bool _isSupportedAtomicInt64OnGroupSharedSupported      = false;
		bool _isSupportedInt64OnDescriptorHeapResourceSupported = false;
		bool _isSupportedAtomicOperation                        = false;
		bool _isSupportedAtomicInt64OnTypedResource             = false;
		bool _isSupportedAtomicUInt64                           = false;
#if USE_INTEL_EXTENSION
		bool _isSupportedIntelEmulatedAtomic64                  = false;
		INTCExtensionContext* _intelExtensionContext            = nullptr;
#endif
		/*-------------------------------------------------------------------
		-               IndirectDraw
		---------------------------------------------------------------------*/
		CommandSignaturePtr _drawIndexedIndirectCommandSignature = nullptr;

	private:
		#pragma region Private Enum Class
		// Descriotor heap
		enum DefaultHeapType
		{
			CBV_SRV_UAV,
			RTV,
			DSV,
			Sampler,
		};

		#pragma endregion 

		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		#pragma region Private Function

		void ReportLiveObjects();

		/*!**********************************************************************
		*  @brief  DirectX�Ŏg�p�\�ȍő�̋@�\���x���������Őݒ肵�܂�
		*  @note   ���݂�12_2���ő�T�|�[�g���x���ł�
		*************************************************************************/
		void FindHighestFeatureLevel();

		/*!**********************************************************************
		*  @brief  DirectX�Ŏg�p�\�ȍő�̃V�F�[�_�[���f����ݒ肵�܂�
		*  @note   ���݂�6_9���w��\�ȃT�|�[�g���x���ł���, ���ɉ����ă��x���͉�����ꍇ������܂�@n
		*          https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
		*************************************************************************/
		void FindHighestShaderModel();

		void CheckDXRSupport();
		void CheckRenderPassSupport();
		void CheckVRSSupport();

		/*!**********************************************************************
		*  @brief     �[�x�l���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳��, �͈͊O�Ȃ�ΊY���s�N�Z���𑁊����p������@
		*             Deferred Rendering�ɂ����郉�C�g��accumulation, Deferred Rendering�ɂ�����Cascaded Shadow Map, ��ʊE�[�x�G�t�F�N�g, ���i�`�擙�Ɏg�p�\ @n
		*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro @n
		*  @attention PLATFORM_OS_WINDOWS���Ŏg�p�\�ȋ@�\�ł�.
		*************************************************************************/
		void CheckDepthBoundsTestSupport();

		void CheckMultiSampleQualityLevels();
		void CheckMeshShadingSupport();

		/*!**********************************************************************
		*  @brief  �p�C�v���C���Ŏg�p�\�ȃ��\�[�X�̏���l���m�F���邽�߂Ɏg�p����
		*  @note   �ڍׂ�GetResourceBindingTier, GetResourceHeapTier���m�F���Ă�������@n
		* �@�@�@�@�@�@https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support @n
		*          https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier @n
		*************************************************************************/
		void CheckResourceTiers();
		void CheckMaxHeapSize();
		void CheckBindlessSupport();
		void CheckSamplerFeedbackSupport();
		void CheckAllowTearingSupport();
		void CheckStencilReferenceFromPixelShaderSupport();
		void CheckWaveLaneSupport();
		void CheckNative16bitOperation();
		void CheckAtomicOperation();
		void CheckAdditionalUAVType();

		/*!**********************************************************************
		*  @brief  ���݂̎w��\�ȍő��RootSignature�̃o�[�W�����𒲂ׂ܂�. 
		*  @note   GetHighestRootSignatureVersion���Q�Ƃ��Ă�������
		*************************************************************************/
		void CheckHighestRootSignatureVersion();
		void SetupDisplayHDRMetaData();
		void SetupDefaultCommandSignatures();

		/*!**********************************************************************
		*  @brief  RHIInstance�Œ�`�����[���x�̑傫���ɂ���������GPU��DebugBreak���s��
		*************************************************************************/
		void SetGPUDebugBreak() const;

		/*!**********************************************************************
		*  @brief  : �݂���Cast�\��PixelFormart���w�肵�܂�
		*************************************************************************/
		gu::DynamicArray<DXGI_FORMAT> GetCastableFormats([[maybe_unused]]const DXGI_FORMAT target) { return {}; };

	#if USE_INTEL_EXTENSION
		/*!**********************************************************************
		*  @brief  :Atomic 64 bit���T�|�[�g����Ă��邩��Ԃ��܂�.
		*************************************************************************/
		bool IsSupportedIntelEmulatedAtomic64();
		
		/*!**********************************************************************
		*  @brief  Intel extension context�𐶐����܂�.
		*************************************************************************/
		void CreateIntelExtensionContext();

		/*!**********************************************************************
		*  @brief   Intel extension context��j�����܂�. 
		*************************************************************************/
		void DestroyIntelExtensionContext();
	#endif

		#pragma endregion Private Function

		#pragma region Private Member Variables

		/*----------------------------------------------------------------------
		*  @brief : Default�̃f�B�X�N���v�^�q�[�v
		*----------------------------------------------------------------------*/
		gu::SortedMap<DefaultHeapType, gu::SharedPointer<core::RHIDescriptorHeap>> _defaultHeap;

		#pragma endregion Private Member Variables
	};
}
#endif