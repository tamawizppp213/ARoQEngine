//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_DEVICE_HPP
#define RHI_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIMultiGPUMask.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
#include "GameUtility/Container/Include/GUSortedMap.hpp" // vulkan���߂�������unordered_map���ǉ�.

#if PLATFORM_OS_WINDOWS
#include <Windows.h> // ����Platform�ˑ���E�p�\��
#endif

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct Float3x4;
}
namespace rhi::core
{
	class RHIDisplayAdapter;
	class RHIFrameBuffer;
	class RHIFence;
	class RHICommandList;
	class RHICommandQueue;
	class RHICommandAllocator;
	class RHISwapchain;
	class RHIDescriptorHeap;
	class RHIResourceLayout;
	class RHIRenderPass;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPURayTracingPipelineState;
	class GPUResourceView;
	class GPUSampler;
	class GPUBuffer;
	class GPUTexture;
	class RHIQuery;
	class GPUPipelineFactory;
	class RayTracingGeometry;
	class BLASBuffer;
	class TLASBuffer;
	class ASInstance;
	struct SwapchainDesc;

	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice 
	*  @brief     Logical Device interface. Device : GPU resources generator.
	*****************************************************************************/
	class RHIDevice : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �_���f�o�C�X��j������.
		*  @note      ���̊֐����Ă΂Ȃ���SharedPointer�Ńf�X�g���N�^���Ă΂�Ȃ��\��������������.
		*************************************************************************/
		virtual void Destroy() = 0; 
		
		#pragma region Create Resource
		/*!**********************************************************************
		*  @brief     �e�f�B�X�N���v�^�q�[�v��DefaultHeapCount�Ɋ�Â��č쐬���܂�
		*  @param[in] const core::DefaultHeapCount �f�B�X�N���v�^�q�[�v�̃T�C�Y�����肷��\����
		*************************************************************************/
		virtual void SetUpDefaultHeap(const core::DefaultHeapCount& heapCount) = 0;
		
		virtual gu::SharedPointer<RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr) = 0;
		
		virtual gu::SharedPointer<RHIFrameBuffer>             CreateFrameBuffer(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr) = 0;
		
		virtual gu::SharedPointer<RHIFence>                   CreateFence(const gu::uint64 fenceValue = 0, const gu::tstring& name = SP("Fence")) = 0;
		
		virtual gu::SharedPointer<RHICommandList>             CreateCommandList(const gu::SharedPointer<RHICommandAllocator>& commandAllocator, const gu::tstring& name = SP("CommandList")) = 0;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɂ���Ē��߂�ꂽ�`��R�}���h���܂Ƃ߂�GPU���ɑ��M����R�}���h�L���[���쐬���܂�
		*  @param[in] const core::CommandListType : �R�}���h���X�g�̎��
		*  @param[in] const gu::tstring : �f�o�b�O��
		*  @return    gu::SharedPointer<core::RHICommandQueue> �R�}���h�L���[�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<RHICommandQueue> CreateCommandQueue(const core::CommandListType type, const gu::tstring& name = SP("CommandQueue")) = 0;
		
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�̃��������i�[���邽�߂Ɏg�p�����R�}���h�A���P�[�^���쐬���܂�
		*  @param[in] const core::CommandListType : �R�}���h���X�g�̎��
		*  @param[in] const gu::tstring : �f�o�b�O��
		*  @return    gu::SharedPointer<core::RHICommandAllocator> �R�}���h�A���P�[�^�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<RHICommandAllocator> CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name = SP("CommandAllocator")) = 0;
		
		virtual gu::SharedPointer<RHISwapchain>               CreateSwapchain(const gu::SharedPointer<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, const PixelFormat& pixelFormat, const size_t frameBufferCount = 2, const gu::uint32 vsync = 0, const bool isValidHDR = true) = 0;
		
		virtual gu::SharedPointer<RHISwapchain>               CreateSwapchain(const SwapchainDesc& desc) = 0;

		virtual gu::SharedPointer<RHIDescriptorHeap>          CreateDescriptorHeap(const DescriptorHeapType heapType, const size_t maxDescriptorCount) = 0;
		
		virtual gu::SharedPointer<RHIDescriptorHeap>          CreateDescriptorHeap(const gu::SortedMap<DescriptorHeapType, size_t>& heapInfo) = 0;
		
		virtual gu::SharedPointer<RHIResourceLayout>          CreateResourceLayout(const gu::DynamicArray<ResourceLayoutElement>& elements = {}, const gu::DynamicArray<SamplerLayoutElement>& samplers = {}, const gu::Optional<Constant32Bits>& constant32Bits = {}, const gu::tstring& name = SP("ResourceLayout")) = 0;
		
		virtual gu::SharedPointer<GPUPipelineFactory>         CreatePipelineFactory() = 0;
		
		virtual gu::SharedPointer<GPUGraphicsPipelineState>   CreateGraphicPipelineState(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		
		virtual gu::SharedPointer<GPUComputePipelineState>    CreateComputePipelineState(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0; // after action: setting pipeline
		
		virtual gu::SharedPointer<RHIRenderPass>              CreateRenderPass(const gu::DynamicArray<Attachment>& colors, const gu::Optional<Attachment>& depth) = 0;
		
		virtual gu::SharedPointer<RHIRenderPass>              CreateRenderPass(const Attachment& color, const gu::Optional<Attachment>& depth) = 0;
		
		virtual gu::SharedPointer<GPUResourceView>            CreateResourceView(const ResourceViewType viewType, const gu::SharedPointer<GPUTexture>& texture, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) = 0;
		
		virtual gu::SharedPointer<GPUResourceView>            CreateResourceView(const ResourceViewType viewType, const gu::SharedPointer<GPUBuffer>& buffer, const gu::uint32 mipSlice = 0, const gu::uint32 placeSlice = 0, const gu::SharedPointer<RHIDescriptorHeap>& customHeap = nullptr) = 0;
		
		virtual gu::SharedPointer<GPUSampler>                 CreateSampler(const core::SamplerInfo& samplerInfo) = 0; // both
		
		/*!**********************************************************************
		*  @brief     GPU�Ŋm�ۂ���o�b�t�@���쐬���܂�. (���_�f�[�^��C���f�b�N�X�f�[�^�̊m�ۂȂǂɎg�p���܂�.)
		*  @param[in] const core::GPUBufferMetaData& : �o�b�t�@�����ۂɕK�v�ȏ����܂Ƃ߂��\����
		*  @param[in] const gu::tstring& : �f�o�b�O��
		*  @return    gu::SharedPointer<GPUBuffer> : GPU�o�b�t�@�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<GPUBuffer> CreateBuffer (const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("")) = 0;
		
		virtual gu::SharedPointer<GPUTexture>                 CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP("")) = 0;
		
		virtual gu::SharedPointer<GPUTexture>                 CreateTextureEmpty() = 0;
		
		//virtual gu::SharedPointer<GPURayTracingPipelineState> CreateRayTracingPipelineState(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0;
		
		virtual gu::SharedPointer<RayTracingGeometry>         CreateRayTracingGeometry(const RayTracingGeometryFlags flags, const gu::SharedPointer<GPUBuffer>& vertexBuffer, const gu::SharedPointer<GPUBuffer>& indexBuffer = nullptr) = 0;
		
		virtual gu::SharedPointer<ASInstance>                 CreateASInstance(
			const gu::SharedPointer<BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform, 
			const gu::uint32 instanceID, const gu::uint32 instanceContributionToHitGroupIndex, 
			const gu::uint32 instanceMask = 0xFF, 
			const RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) = 0;
		
		virtual gu::SharedPointer<BLASBuffer>                 CreateRayTracingBLASBuffer(const gu::DynamicArray<gu::SharedPointer<RayTracingGeometry>>& geometryDesc, const BuildAccelerationStructureFlags flags) = 0;
		
		virtual gu::SharedPointer<TLASBuffer>                 CreateRayTracingTLASBuffer(const gu::DynamicArray<gu::SharedPointer<ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags) = 0;
		
		virtual gu::SharedPointer<RHIQuery> CreateQuery(const core::QueryHeapType heapType) = 0;
		#pragma endregion Create Resource
		#pragma endregion 
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief      �_���f�o�C�X���Ńf�t�H���g(SetupDefaultHeap)�ŗp�ӂ����f�B�X�N���v�^�q�[�v���擾���܂�.
		*  @param[in]  const core::DescriptorHeapType DescriptorHeap�̎��
		*  @return     gu::SharedPointer<RHIDescriptorHeap> �f�B�X�N���v�^�q�[�v
		*************************************************************************/
		virtual gu::SharedPointer<RHIDescriptorHeap> GetDefaultHeap(const core::DescriptorHeapType heapType) = 0;
		
		virtual gu::uint32 GetShadingRateImageTileSize() const = 0;
		
		/*!**********************************************************************
		*  @brief  �����f�o�C�X�̃|�C���^��n���܂�.
		*  @return gu::SharedPointer<core::RHIDisplayAdapter> �����f�o�C�X
		*************************************************************************/
		gu::SharedPointer<RHIDisplayAdapter> GetDisplayAdapter() const noexcept { return _adapter; }
		
		/*!**********************************************************************
		*  @brief  GPU���w�肷��}�X�N�ł�. 
		*************************************************************************/
		      RHIMultiGPUMask& GetGPUMask()       { return _gpuMask; }
		const RHIMultiGPUMask& GetGPUMask() const { return _gpuMask; }

		/*!**********************************************************************
		*  @brief  �f�o�b�O����ݒ肵�܂�. 
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		/*-------------------------------------------------------------------
		-               Device Support Check
		---------------------------------------------------------------------*/
		virtual bool IsSupportedDxr                () const = 0; // Ray Tracing 

		virtual bool IsSupportedHDR                () const = 0; // HDR check ()

		virtual bool IsSupportedVariableRateShading() const = 0;

		virtual bool IsSupportedMeshShading        () const = 0;

		virtual bool IsSupportedDrawIndirected     () const = 0;

		virtual bool IsSupportedGeometryShader     () const = 0;

		virtual bool IsSupportedRenderPass         () const = 0;

		/*!**********************************************************************
		*  @brief  �s�N�Z���`��̍ŏ�, �ő�͈͂�K�p����@�\���g���邩 (DepthBoundsTest)
		*************************************************************************/
		virtual bool IsSupportedDepthBoundsTest    () const = 0;

		virtual bool IsSupportedSamplerFeedback    () const = 0;

		virtual bool IsSupportedStencilReferenceFromPixelShader() const = 0;

		virtual bool IsSupportedWaveLane() const = 0;

		virtual bool IsSupportedNative16bitOperation() const = 0;

		virtual bool IsSupportedAtomicOperation() const = 0;

		/*!**********************************************************************
		*  @brief  �f�B�X�N���[�gGPU���ǂ���. @n
		*          �O��GPU(xGPU)��true�Ɣ��肳��, ����GPU(iGPU)�̂�false�Ɣ��肳��܂�.
		*************************************************************************/
		bool IsDiscreteGPU() const;

		#pragma endregion
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*! @brief �_���f�o�C�X�̃f�t�H���g�R���X�g���N�^*/
		RHIDevice()  = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHIDevice()
		{ 
			_adapter.Reset(); 
		}

		/*! @brief �����A�_�v�^��GPU���w�肷��C���f�b�N�X���g���č쐬����R���X�g���N�^*/
		RHIDevice(const gu::SharedPointer<RHIDisplayAdapter>& adapter, const RHIMultiGPUMask& mask = RHIMultiGPUMask::SingleGPU()) 
			: _adapter(adapter), _gpuMask(mask) {};

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief : Use Display Apapter (GPU)*/
		gu::SharedPointer<RHIDisplayAdapter> _adapter = nullptr;

		// @brief : GPU�̃C���f�b�N�X
		RHIMultiGPUMask _gpuMask = RHIMultiGPUMask::SingleGPU();
	};
}
#endif