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
	*  @brief     Logical Device : shared_ptrで管理すること
	*****************************************************************************/
	class RHIDevice : public core::RHIDevice, public gu::EnableSharedFromThis<RHIDevice>
	{
	public:
		#pragma region Public Function
		void Destroy() override;

		#pragma region Create Function
		void                                              SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) override;

		gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		gu::SharedPointer<core::RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil = nullptr) override;
		
		gu::SharedPointer<core::RHIFence>                   CreateFence(const gu::uint64 fenceValue = 0, const gu::tstring& name = SP("")) override;
		
		gu::SharedPointer<core::RHICommandList>             CreateCommandList(const gu::SharedPointer<core::RHICommandAllocator>& commandAllocator, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHICommandQueue>            CreateCommandQueue    (const core::CommandListType type, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHICommandAllocator>        CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name) override;
		
		gu::SharedPointer<core::RHISwapchain>               CreateSwapchain       (const gu::SharedPointer<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const gu::uint32 vsync = 0, const bool isValidHDR = true) override;
		
		gu::SharedPointer<core::RHISwapchain>               CreateSwapchain(const core::SwapchainDesc& desc) override;

		gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap  (const core::DescriptorHeapType heapType, const size_t maxDescriptorCount) override;
		
		gu::SharedPointer<core::RHIDescriptorHeap>          CreateDescriptorHeap(const gu::SortedMap<core::DescriptorHeapType, size_t>& heapInfo) override;
		
		gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth) override;
		
		gu::SharedPointer<core::RHIRenderPass>              CreateRenderPass(const core::Attachment& color, const gu::Optional<core::Attachment>& depth) override;
		
		gu::SharedPointer<core::GPUGraphicsPipelineState>   CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override ;  // after action: setting pipeline
		
		gu::SharedPointer<core::GPUComputePipelineState>    CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override; // after action: setting pipeline
		
		gu::SharedPointer<core::RHIResourceLayout>          CreateResourceLayout(const gu::DynamicArray<core::ResourceLayoutElement>& elements = {}, const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {}, const gu::Optional<core::Constant32Bits>& constant32Bits = {}, const gu::tstring& name=SP("ResourceLayout")) override;
		
		gu::SharedPointer<core::GPUPipelineFactory>         CreatePipelineFactory() override;
		
		gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUResourceView>            CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) override;
		
		gu::SharedPointer<core::GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo); // both
		
		gu::SharedPointer<core::GPUBuffer>                  CreateBuffer(const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("")) override;
		
		gu::SharedPointer<core::GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP("")) override;
		
		gu::SharedPointer<core::GPUTexture>                 CreateTextureEmpty() override;

		//gu::SharedPointer<core::GPURayTracingPipelineState> CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		gu::SharedPointer<core::RayTracingGeometry>         CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer = nullptr) override;
		
		gu::SharedPointer<core::ASInstance>                 CreateASInstance(
			const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const gu::uint32 instanceID, const gu::uint32 instanceContributionToHitGroupIndex,
			const gu::uint32 instanceMask = 0xFF, const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) override;
		
		gu::SharedPointer<core::BLASBuffer>                 CreateRayTracingBLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags) override;
		
		gu::SharedPointer<core::TLASBuffer>                 CreateRayTracingTLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) override;

		gu::SharedPointer<core::RHIQuery> CreateQuery(const core::QueryHeapType heapType) override;
		/*----------------------------------------------------------------------
		*  @brief : Heap領域の確保と実際にGPUにデータをメモリに確保するのを両方行う関数
		/*----------------------------------------------------------------------*/
		HRESULT CreateCommittedResource
		(
			ResourceComPtr&      resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // ほとんどはconst
			const D3D12_HEAP_PROPERTIES& heapProp,
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);

		/*----------------------------------------------------------------------
		*  @brief : Heap内にまだマップまでは行わない予約済みのリソースを作成
		/*----------------------------------------------------------------------*/
		HRESULT CreateReservedResource
		(
			ResourceComPtr& resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // ほとんどはconst
			const D3D12_HEAP_PROPERTIES& heapProp,
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);

		/*----------------------------------------------------------------------
		*  @brief : 既に作成済みのヒープに配置されるリソースを作成する. 
		*           Committed, Reserved, Placedの中では最も高速に動作する
		/*----------------------------------------------------------------------*/
		HRESULT CreatePlacedResource
		(
			ResourceComPtr& resource,
			const D3D12_RESOURCE_DESC& resourceDesc, // ほとんどはconst
			const HeapComPtr& heap,
			const gu::uint64 heapOffset,
			const D3D12_RESOURCE_STATES initialState,
			const D3D12_CLEAR_VALUE* clearValue = nullptr
		);
		#pragma endregion Create Function

		#pragma endregion Public Function

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief  DirectX12で使用する論理デバイスのComポインタ
		*************************************************************************/
		DeviceComPtr GetDevice () const noexcept { return _device; }

		gu::uint32 GetShadingRateImageTileSize() const { return _variableRateShadingImageTileSize; }
		
		gu::SharedPointer<core::RHIDescriptorHeap>   GetDefaultHeap(const core::DescriptorHeapType heapType) override;

		gu::uint32 GetNodeCount() const { return _deviceNodeCount; }

		const rhi::core::HDRDisplayInfo& GetHDRDisplayInfo() const { return _displayInfo; }

		void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief  DirectXで使用可能な最大の機能レベルを取得します.
		*************************************************************************/
		D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel() const { return _maxSupportedFeatureLevel; }

		/*!**********************************************************************
		*  @brief  DirectXで使用可能な最大のシェーダーモデルを取得します
		*  @note   現在は6_9が指定可能なサポートレベルですが, 環境に応じてレベルは下がる場合があります @n
		*          https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
		*************************************************************************/
		D3D_SHADER_MODEL GetMaxSupportedShaderModel() const { return _maxSupportedShaderModel; }

		/*!**********************************************************************
		*  @brief  パイプラインで使用可能なリソースの上限値を確認するために使用する
		*  @note    1. CBV : Tier 1, 2は14まで. Tier3 はDescripterHeapの最大数 @n
        *           2. SRV : Tier 1は128まで. Tier2, 3はDescripterHeapの最大数 @n
        *           3. UAV : Tier1は機能レベル 11.1+以上で64, それ以外で8, Tier2は64, Tier3はDescripterHeapの最大数@n
        *           4. Sampler : Tier1は16, それ以外で2048 @n
        *           5. ヒープ内のDescripterの最大数 Tier1, 2は1,000,000、Tier3は無制限 pn
        * 　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support
		*************************************************************************/
		D3D12_RESOURCE_BINDING_TIER GetResourceBindingTier() const { return _resourceBindingTier; }

		/*!**********************************************************************
		*  @brief  異種間のResourceHeapをどこまで混在させるのが行えるかを調べるレベルを取得します
		*  @note   HeapTierの方では, バッファー, RenderTargetとDepthStencil, TargetStencil, 深度ステンシルテクスチャのレンダリングを同一ヒープで使用できるかを調べます
        * 　　　　　 Tier1が排他, Tier2が混在可能です.
        * 　　　　　 https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
		*************************************************************************/
		D3D12_RESOURCE_HEAP_TIER GetResourceHeapTier() const { return _resourceHeapTier; }

		D3D12_RAYTRACING_TIER GetRayTracingTier() const { return _rayTracingTier; }

		D3D12_VARIABLE_SHADING_RATE_TIER GetVariableShadingRateTier() const { return _variableRateShadingTier; }

		CommandSignaturePtr GetDefaultDrawIndexedIndirectCommandSignature() const { return _drawIndexedIndirectCommandSignature; }

		/*!**********************************************************************
		*  @brief  現在の指定可能な最大のRootSignatureのバージョンです. @n
		*          1_0 or 1 : 通常のRootSignature @n
		*          1_1      : Descriptorに対して最適化を行うためのフラグを設置可能 @n
		*          1_2      : Static samplerの設定幅が広がる @n
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
		*  @brief  ピクセル描画の最小, 最大範囲を適用する機能が使えるか (DepthBoundsTest)
		*************************************************************************/
		bool IsSupportedDepthBoundsTest () const override { return _isSupportedDepthBoundsTest; }

		bool IsSupportedSamplerFeedback    () const override { return _isSupportedSamplerFeedback; }

		bool IsSupportedStencilReferenceFromPixelShader() const override { return _isSupportedStencilReferenceFromPixelShader; }
		
		bool IsSupportedWaveLane() const override { return _isSupportedWaveLane; }

		bool IsSupportedNative16bitOperation() const override { return _isSupported16bitOperation; }

		bool IsSupportedAtomicOperation() const override { return _isSupportedAtomicOperation; }

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
		/*! @brief : DirectX12の論理デバイス*/
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

		/*! @brief ピクセル描画の最小, 最大範囲を適用する機能が使えるか*/
		bool _isSupportedDepthBoundsTest          = true;


		bool _isSupportedSamplerFeedback          = true;
		bool _isSupportedBindless                 = true;
		bool _isSupportedStencilReferenceFromPixelShader   = true;
		bool _isSupported16bitOperation           = false;
		bool _isSupportedHeapNotZero = false; // Heap確保のオーバーヘッドを減らすため, ゼロ初期化しないようにする

		/*! @brief サポートされる D3D12 機能レベルの最大値。サポートされていない場合は0*/
		D3D_FEATURE_LEVEL _maxSupportedFeatureLevel = (D3D_FEATURE_LEVEL)0;
		
		/*! @brief サポートされるD3D12機能レベルの最小値 (これ以上の最小値はエラーとなります)*/
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
		gu::uint32 _minWaveLaneCount = 0;
		gu::uint32 _maxWaveLaneCount = 0;

		/*-------------------------------------------------------------------
		-              RootSignature
		---------------------------------------------------------------------*/
		/* @brief     現在サポートされている最大のRootSignatureのバージョンです.
		   @attention CheckHighestRootSignatureVersion関数の仕様上, 常に最新バージョンの値に更新しておいてください. */
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
		/****************************************************************************
		**                Private Enum Class
		*****************************************************************************/
		// Descriotor heap
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
		#pragma region Private Function

		void ReportLiveObjects();

		/*!**********************************************************************
		*  @brief  DirectXで使用可能な最大の機能レベルを自動で設定します
		*  @note   現在は12_2が最大サポートレベルです
		*************************************************************************/
		void FindHighestFeatureLevel();

		/*!**********************************************************************
		*  @brief  DirectXで使用可能な最大のシェーダーモデルを設定します
		*  @note   現在は6_9が指定可能なサポートレベルですが, 環境に応じてレベルは下がる場合があります@n
		*          https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
		*************************************************************************/
		void FindHighestShaderModel();

		void CheckDXRSupport();
		void CheckRenderPassSupport();
		void CheckVRSSupport();

		/*!**********************************************************************
		*  @brief  深度値が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させ, 範囲外ならば該当ピクセルを早期棄却する方法
		*  @note   Deferred Renderingにおけるライトのaccumulation, Deferred RenderingにおけるCascaded Shadow Map, 被写界深度エフェクト, 遠景描画等に使用可能 
		*          https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
		*************************************************************************/
		void CheckDepthBoundsTestSupport();

		void CheckMultiSampleQualityLevels();
		void CheckMeshShadingSupport();

		/*!**********************************************************************
		*  @brief  パイプラインで使用可能なリソースの上限値を確認するために使用する
		*  @note   詳細はGetResourceBindingTier, GetResourceHeapTierを確認してください
		* 　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support @n
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

		/*!**********************************************************************
		*  @brief  現在の指定可能な最大のRootSignatureのバージョンを調べます. 
		*  @note   GetHighestRootSignatureVersionを参照してください
		*************************************************************************/
		void CheckHighestRootSignatureVersion();
		void SetupDisplayHDRMetaData();
		void SetupDefaultCommandSignatures();

		/*!**********************************************************************
		*  @brief  RHIInstanceで定義した深刻度の大きさにしたがってGPUのDebugBreakを行う
		*************************************************************************/
		void SetGPUDebugBreak() const;

	#if USE_INTEL_EXTENSION
		/*!**********************************************************************
		*  @brief  :Atomic 64 bitがサポートされているかを返します.
		*************************************************************************/
		bool IsSupportedIntelEmulatedAtomic64();
		
		/*!**********************************************************************
		*  @brief  Intel extension contextを生成します.
		*************************************************************************/
		void CreateIntelExtensionContext();

		/*!**********************************************************************
		*  @brief   Intel extension contextを破棄します. 
		*************************************************************************/
		void DestroyIntelExtensionContext();
	#endif

		#pragma endregion Private Function

		#pragma region Private Member Variables

		/*----------------------------------------------------------------------
		*  @brief : Defaultのディスクリプタヒープ
		*----------------------------------------------------------------------*/
		gu::SortedMap<DefaultHeapType, gu::SharedPointer<core::RHIDescriptorHeap>> _defaultHeap;

		#pragma endregion Private Member Variables
	};
}
#endif