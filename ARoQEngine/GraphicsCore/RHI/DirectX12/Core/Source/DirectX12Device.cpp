//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12Device.cpp
///  @brief  Logical Device : to use for create GPU resources 
///  @author Toide Yutaro
///  @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Adapter.hpp"
#include "../Include/DirectX12CommandQueue.hpp"
#include "../Include/DirectX12CommandAllocator.hpp"
#include "../Include/DirectX12CommandList.hpp"
#include "../Include/DirectX12Swapchain.hpp"
#include "../Include/DirectX12Fence.hpp"
#include "../Include/DirectX12DescriptorHeap.hpp"
#include "../Include/DirectX12RenderPass.hpp"
#include "../Include/DirectX12ResourceLayout.hpp"
#include "../Include/DirectX12FrameBuffer.hpp"
#include "../Include/DirectX12Instance.hpp"
#include "../Include/DirectX12Query.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIPixelFormat.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingASInstance.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingTLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingGeometry.hpp"
#include "Platform/Core/Include/CoreOS.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

#if PLATFORM_OS_WINDOWS
#include <Windows.h>
#endif

#if USE_INTEL_EXTENSION
#include <INTC/igdext.h>
#pragma comment(lib, "Plugins/INTC/igdext64.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor
RHIDevice::RHIDevice()
{
	
}

RHIDevice::~RHIDevice()
{
	if (_device) 
	{
		Destroy(); 
	}
}

RHIDevice::RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter, const core::RHIMultiGPUMask& mask) :
	core::RHIDevice(adapter, mask)
{

	/*-------------------------------------------------------------------
	-                   Create Logical Device
	---------------------------------------------------------------------*/
	ThrowIfFailed(D3D12CreateDevice(
		gu::StaticPointerCast<RHIDisplayAdapter>(_adapter)->GetAdapter().Get(),      // default adapter
		_minSupportedFeatureLevel,
		IID_PPV_ARGS(&_device)));

	const auto& gpuName    = adapter->GetName();
	const gu::tstring deviceName = gu::tstring(SP("Device::")) + gu::StringConverter::ConvertStringToTString(gpuName.CString());
	_device->SetName(deviceName.CString());

	/*-------------------------------------------------------------------
	-                 Set gpu debug break
	---------------------------------------------------------------------*/
	SetGPUDebugBreak();

	/*-------------------------------------------------------------------
	-          Find Highest support feature and shader model
	---------------------------------------------------------------------*/
	FindHighestFeatureLevel();
	FindHighestShaderModel();

	/*-------------------------------------------------------------------
	-                   Device node count
	---------------------------------------------------------------------*/
	_deviceNodeCount = _device->GetNodeCount();
	
	/*-------------------------------------------------------------------
	-                   Device Support Check
	---------------------------------------------------------------------*/
	SetupPlatformPixelFormats();
	CheckDXRSupport();
	CheckVRSSupport();
	CheckRenderPassSupport();
	CheckMeshShadingSupport();
	CheckMultiSampleQualityLevels();
	CheckDepthBoundsTestSupport();
	CheckResourceTiers();
	CheckAdditionalUAVType();
	CheckMaxHeapSize();
	CheckBindlessSupport();
	CheckStencilReferenceFromPixelShaderSupport();
	CheckSamplerFeedbackSupport();
	CheckAllowTearingSupport();
	CheckHighestRootSignatureVersion();
	CheckWaveLaneSupport();
	SetupDisplayHDRMetaData();
	SetupDefaultCommandSignatures();
	CheckAtomicOperation();
	
#if USE_PIX
	_pixDLLHandle = platform::core::OS::GetDLLHandle(L"WinPixEventRuntime.dll");
#endif
}

#pragma endregion Constructor and Destructor

#pragma region Set up and Destroy

/*!**********************************************************************
*  @brief     �e�f�B�X�N���v�^�q�[�v��DefaultHeapCount�Ɋ�Â��č쐬���܂�
*  @param[in] const core::DefaultHeapCount �f�B�X�N���v�^�q�[�v�̃T�C�Y�����肷��\����
*  @return    void
*************************************************************************/
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	/*-------------------------------------------------------------------
	-                   Create descriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV] = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::RTV]         = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::DSV]         = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::Sampler]     = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());

	/*-------------------------------------------------------------------
	-                   Set up descriptor count
	---------------------------------------------------------------------*/
	gu::SortedMap<core::DescriptorHeapType, uint64> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV]     = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV]     = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV]     = heapCount.UAVDescCount;

	/*-------------------------------------------------------------------
	-                   Allocate decsriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV]->Resize(heapInfoList);
	_defaultHeap[DefaultHeapType::RTV]->Resize(core::DescriptorHeapType::RTV, heapCount.RTVDescCount);
	_defaultHeap[DefaultHeapType::DSV]->Resize(core::DescriptorHeapType::DSV, heapCount.DSVDescCount);
	_defaultHeap[DefaultHeapType::Sampler]->Resize(core::DescriptorHeapType::SAMPLER, heapCount.SamplerDescCount);

}

/*!**********************************************************************
*  @brief     �_���f�o�C�X��j������.
*  @note      ���̊֐����Ă΂Ȃ���SharedPointer�Ńf�X�g���N�^���Ă΂�Ȃ��\��������������.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDevice::Destroy()
{
#if USE_INTEL_EXTENSION
	DestroyIntelExtensionContext();
#endif

	/*-------------------------------------------------------------------
	-              Clear default descriptor heap
	---------------------------------------------------------------------*/
	for (auto& heap : _defaultHeap)
	{
		if (heap.Value) 
		{ 
			heap.Value.Reset(); 
		}
	}
	_defaultHeap.Clear();

	if (_drawIndexedIndirectCommandSignature) { _drawIndexedIndirectCommandSignature.Reset(); }

	/*-------------------------------------------------------------------
	-             PIX�𖢎g�p��Ԃɂ��Ă���
	---------------------------------------------------------------------*/
#if USE_PIX
	if (_pixDLLHandle)
	{
		platform::core::OS::FreeDLLHandle(_pixDLLHandle);
		_pixDLLHandle = nullptr;
	}
#endif

	/*-------------------------------------------------------------------
	-              Clear device
	---------------------------------------------------------------------*/
	if (_device) 
	{ 
#ifdef _DEBUG
		ReportLiveObjects();
#endif
		_device.Reset(); 
	}
}

#pragma endregion       Set up and Destroy

#pragma region CreateResource

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared<directX12::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTargets, depthStencil));
}

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared<directX12::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTarget, depthStencil));
}

gu::SharedPointer<core::RHIFence> RHIDevice::CreateFence(const gu::uint64 fenceValue, const gu::tstring& name)
{
	// https://suzulang.com/stdshared_ptr%E3%81%A7this%E3%82%92%E4%BD%BF%E3%81%84%E3%81%9F%E3%81%84%E6%99%82%E3%81%AB%E6%B3%A8%E6%84%8F%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8/
	return gu::StaticPointerCast<core::RHIFence>(gu::MakeShared<directX12::RHIFence>(SharedFromThis(), fenceValue, name));
}

gu::SharedPointer<core::RHICommandList> RHIDevice::CreateCommandList(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const gu::tstring& name)
{
	const auto dxPointer = gu::MakeShared<directX12::RHICommandList>(SharedFromThis(), commandAllocator, name);
	const auto pointer   = gu::StaticPointerCast<core::RHICommandList>(dxPointer);
	return pointer;
}

gu::SharedPointer<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHICommandQueue>(gu::MakeShared<directX12::RHICommandQueue>(SharedFromThis(), type, name));
}

gu::SharedPointer<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name)
{
	const auto pointer  = gu::MakeShared<directX12::RHICommandAllocator>(SharedFromThis(), type, name);
	const auto dxPointer = gu::StaticPointerCast<core::RHICommandAllocator>(pointer);
	return dxPointer;
}

gu::SharedPointer<core::RHISwapchain> RHIDevice::CreateSwapchain(const gu::SharedPointer<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const gu::uint8 frameBufferCount, const gu::uint8 vsync, const bool isValidHDR )
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<directX12::RHISwapchain>(SharedFromThis(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}

gu::SharedPointer<core::RHISwapchain>  RHIDevice::CreateSwapchain(const core::SwapchainDesc& desc)
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<directX12::RHISwapchain>(SharedFromThis(), desc));
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapType, maxDescriptorCount);
	return heapPtr;
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const gu::SortedMap<core::DescriptorHeapType, size_t>& heapInfo)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapInfo);
	return heapPtr;
}

gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<directX12::RHIRenderPass>(SharedFromThis(), colors, depth));
}

gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const core::Attachment& color, const gu::Optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<directX12::RHIRenderPass>(SharedFromThis(), color, depth));
}

gu::SharedPointer<core::GPUGraphicsPipelineState> RHIDevice::CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUGraphicsPipelineState>(gu::MakeShared<directX12::GPUGraphicsPipelineState>(SharedFromThis(), renderPass, resourceLayout));
}

gu::SharedPointer<core::GPUComputePipelineState> RHIDevice::CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUComputePipelineState>(gu::MakeShared<directX12::GPUComputePipelineState>(SharedFromThis(), resourceLayout));
}

gu::SharedPointer<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const gu::Optional<core::Constant32Bits>& constant32Bits, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHIResourceLayout>(gu::MakeShared<directX12::RHIResourceLayout>(SharedFromThis(), elements, samplers, constant32Bits, name));
}

gu::SharedPointer<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return gu::StaticPointerCast<core::GPUPipelineFactory>(gu::MakeShared<directX12::GPUPipelineFactory>(SharedFromThis()));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice, const gu::uint32 planeSlice, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<directX12::GPUResourceView>(SharedFromThis(), viewType, texture, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice, const gu::uint32 planeSlice , const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<directX12::GPUResourceView>(SharedFromThis(), viewType, buffer, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUSampler> RHIDevice::CreateSampler(const core::SamplerInfo& samplerInfo)
{
	return gu::StaticPointerCast<core::GPUSampler>(gu::MakeShared<directX12::GPUSampler>(SharedFromThis(), samplerInfo));
}

gu::SharedPointer<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::GPUBuffer>(gu::MakeShared<directX12::GPUBuffer>(SharedFromThis(), metaData, name));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<directX12::GPUTexture>(SharedFromThis(), metaData, name));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTextureEmpty()
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<directX12::GPUTexture>(SharedFromThis()));
}

//gu::SharedPointer<core::GPURayTracingPipelineState> RHIDevice::CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
//{
//	return nullptr;
//}
gu::SharedPointer<core::RayTracingGeometry> RHIDevice::CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer)
{
	return gu::StaticPointerCast<core::RayTracingGeometry>(gu::MakeShared<directX12::RayTracingGeometry>(SharedFromThis(), flags, vertexBuffer, indexBuffer));
}

gu::SharedPointer<core::ASInstance> RHIDevice::CreateASInstance(
	const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
	const gu::uint32 instanceID, const gu::uint32 instanceContributionToHitGroupIndex,
	const gu::uint32 instanceMask, const core::RayTracingInstanceFlags flags)
{
	return gu::StaticPointerCast<core::ASInstance>(gu::MakeShared<directX12::ASInstance>(SharedFromThis(), blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags));
}

gu::SharedPointer<core::BLASBuffer>  RHIDevice::CreateRayTracingBLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::BLASBuffer>(gu::MakeShared<directX12::BLASBuffer>(SharedFromThis(), geometryDesc, flags));
}

gu::SharedPointer<core::TLASBuffer>  RHIDevice::CreateRayTracingTLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::TLASBuffer>(gu::MakeShared<directX12::TLASBuffer>(SharedFromThis(), asInstances, flags));
}

gu::SharedPointer<core::RHIQuery> RHIDevice::CreateQuery(const core::QueryHeapType heapType)
{
	return gu::StaticPointerCast<core::RHIQuery>(gu::MakeShared<directX12::RHIQuery>(SharedFromThis(), heapType));
}

/*!**********************************************************************
*  @brief     Heap�̈�̊m�ۂƎ��ۂ�GPU�Ƀf�[�^���������Ɋm�ۂ���̂𗼕��s���֐�
*  @note      �{�֐���DirectX12��p�̊֐��ł�. Heap�̍Œ�Alignment��64kB�ł�. 
*  @param[out] const ResourceComPtr&        :���ꂩ�烁�������m�ۂ�����GPU���\�[�X
*  @param[in]  const D3D12_RESOURCE_DESC&   : ���������m�ۂ���ۂ�GPU���\�[�X���
*  @param[in]  const D3D12_HEAP_PROPERTIES& : �ǂ̏ꏊ�Ƀ��������m�ۂ��邩���������m�ۂ̎d����ݒ肷��
*  @param[in]  const D3D12_RESOURCE_STATES  : �������m�ی�, �ŏ��ɐݒ肳���GPU���\�[�X�̏��
*  @param[in]  const D3D12_CLEAR_VALUE*     : �N���A�J���[
*  @return     HRESULT : ����������������
*************************************************************************/
HRESULT RHIDevice::CreateCommittedResource(ResourceComPtr& resource,
	const D3D12_RESOURCE_DESC& resourceDesc,
	const D3D12_HEAP_PROPERTIES& heapProps,
	const D3D12_RESOURCE_STATES initialState, 
	const D3D12_CLEAR_VALUE* clearValue)
{
	// GPU�̃������m�ۂ�����ɂ��̏����i�[����悪������Ȃ��ꍇ�͂��̂܂܏I��
	if (!resource.GetAddressOf()) 
	{
		return E_POINTER; 
	}

	// clearValue���g���ď��������K�v���ǂ���
	// �����_�[�^�[�Q�b�g��DepthStencil�͏��������K�v�ł�. 
	const bool requireInitialize = (resourceDesc.Flags & 
		(D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)) != 0;
	
	/*-------------------------------------------------------------------
	-        �q�[�v�̍쐬�t���O��GPUResource���쐬����Ƃ��̃t���O���쐬
	---------------------------------------------------------------------*/
	D3D12_HEAP_FLAGS heapFlags = (_isSupportedHeapNotZero && !requireInitialize)
		? D3D12_HEAP_FLAG_CREATE_NOT_ZEROED : D3D12_HEAP_FLAG_NONE;

	// resourceDesc�̃R�s�[�����.
	// ����ɂ��, �ݒ�Ƀ~�X�����������̎����␳���ȉ��ōs����悤�ɂ���
	D3D12_RESOURCE_DESC localDesc = resourceDesc;

	// ���L���\�[�X�ł���ꍇ��Flag_Shared��ǉ�����.
	if (resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS)
	{
		heapFlags |= D3D12_HEAP_FLAG_SHARED;

		// �o�b�t�@�쐬���ɂ͓����A�N�Z�X�t���O�͎g�p�s��
		if (resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		{
			localDesc.Flags &= ~D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS;
		}
	}

	// RayTracing��Acceleration Structure���g�p����ꍇ, UNORDERED_ACCESS�͎g�p�\�ɂ��Ă���
	if (initialState == D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
	{
		localDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	/*-------------------------------------------------------------------
	-            �q�[�v�̈�̊m�ۂ�GPU�Ƀ��������m�ۂ���
	---------------------------------------------------------------------*/
	HRESULT result = S_OK;

	// Intel�g�����g�p����ꍇ�͓Ǝ��̊֐����g�p����
#if USE_INTEL_EXTENSION
	if (IsSupportedIntelEmulatedAtomic64())
	{
		INTC_D3D12_RESOURCE_DESC_0001 intelLocalDesc{};
		intelLocalDesc.pD3D12Desc = &localDesc;
		intelLocalDesc.EmulatedTyped64bitAtomics = true;

		result = INTC_D3D12_CreateCommittedResource(_intelExtensionContext, &heapProps, heapFlags, &intelLocalDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
	}
	else
#endif

	// �񈳏k��UAV���g�p
	if (IsSupportedAdditionalUAVType() && resourceDesc.Format != DXGI_FORMAT::DXGI_FORMAT_UNKNOWN && (localDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS))
	{
		const D3D12_RESOURCE_DESC1 localDesc1 =
		{
			.Dimension                = localDesc.Dimension,
			.Alignment                = localDesc.Alignment,
			.Width                    = localDesc.Width,
			.Height                   = localDesc.Height,
			.DepthOrArraySize         = localDesc.DepthOrArraySize,
			.MipLevels                = localDesc.MipLevels,
			.Format                   = localDesc.Format,
			.SampleDesc               = localDesc.SampleDesc,
			.Layout                   = localDesc.Layout,
			.Flags                    = localDesc.Flags,
			.SamplerFeedbackMipRegion = {}
		};

		// Buffer�ƂȂ�ꍇ�͕K��Undefined����n�߂�K�v�����邪, ����ȊO�̓e�N�X�`���`�����g�p�\�Ȃ���COMMON���g�p����.
		const D3D12_BARRIER_LAYOUT barrierInitialLayout = localDesc1.Dimension == D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER 
			? D3D12_BARRIER_LAYOUT_UNDEFINED : D3D12_BARRIER_LAYOUT_COMMON;

		ID3D12ProtectedResourceSession* protectedSession = nullptr;

		// SRV��URV�Ŏg�p�����DXGI_FORMAT�ɂ�����, �݂��ɃL���X�g�\�Ȃ��̂̃��X�g��񋟂���.
		gu::DynamicArray<DXGI_FORMAT> castableFormats = GetCastableFormats(localDesc1.Format);

		result = _device->CreateCommittedResource3(&heapProps, heapFlags, &localDesc1, barrierInitialLayout, clearValue,
			protectedSession, static_cast<uint32>(castableFormats.Size()), castableFormats.Data(), IID_PPV_ARGS(resource.GetAddressOf()));
	}
	else
	{
		result = _device->CreateCommittedResource(&heapProps, heapFlags, &localDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
	}

	return result;
}

/*!**********************************************************************
*  @brief      Heap���ɂ܂��}�b�v�܂ł͍s��Ȃ��\��ς݂̃��\�[�X���쐬. �v�͏��������s�킸�Ƀ������̈悾���m�ۂ��Ă����Ԃ̂���.
*  @note       �{�֐���DirectX12��p�̊֐��ł�.
*  @param[out] const ResourceComPtr&        :���ꂩ�烁������������GPU���\�[�X
*  @param[in]  const D3D12_RESOURCE_DESC&   : ���������m�ۂ���ۂ�GPU���\�[�X���
*  @param[in]  const D3D12_RESOURCE_STATES  : �������m�ی�, �ŏ��ɐݒ肳���GPU���\�[�X�̏��
*  @param[in]  const D3D12_CLEAR_VALUE*     : �N���A�J���[
*  @return     HRESULT : ����������������
*************************************************************************/
HRESULT RHIDevice::CreateReservedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES initialState,const D3D12_CLEAR_VALUE* clearValue)
{
	// GPU�̃������m�ۂ�����ɂ��̏����i�[����悪������Ȃ��ꍇ�͂��̂܂܏I��
	if (!resource.GetAddressOf())
	{
		return E_POINTER;
	}

	/*-------------------------------------------------------------------
	-            Flag�̐ݒ�
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC localDesc   = resourceDesc;
	localDesc.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;

	// RayTracing��Acceleration Structure���g�p����ꍇ, UNORDERED_ACCESS�͎g�p�\�ɂ��Ă���
	if (initialState == D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
	{
		localDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	/*-------------------------------------------------------------------
	-            Heap���ɂ܂��}�b�v�܂ł͍s��Ȃ��\��ς݂̃��\�[�X���쐬
	---------------------------------------------------------------------*/
	HRESULT result = S_OK;
	if (IsSupportedAdditionalUAVType() && resourceDesc.Format != DXGI_FORMAT::DXGI_FORMAT_UNKNOWN && (localDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS))
	{
		const D3D12_RESOURCE_DESC1 localDesc1 =
		{
			.Dimension                = localDesc.Dimension,
			.Alignment                = localDesc.Alignment,
			.Width                    = localDesc.Width,
			.Height                   = localDesc.Height,
			.DepthOrArraySize         = localDesc.DepthOrArraySize,
			.MipLevels                = localDesc.MipLevels,
			.Format                   = localDesc.Format,
			.SampleDesc               = localDesc.SampleDesc,
			.Layout                   = localDesc.Layout,
			.Flags                    = localDesc.Flags,
			.SamplerFeedbackMipRegion = {}
		};

		// Buffer�ƂȂ�ꍇ�͕K��Undefined����n�߂�K�v�����邪, ����ȊO�̓e�N�X�`���`�����g�p�\�Ȃ���COMMON���g�p����.
		const D3D12_BARRIER_LAYOUT barrierInitialLayout = localDesc1.Dimension == D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER 
			? D3D12_BARRIER_LAYOUT_UNDEFINED : D3D12_BARRIER_LAYOUT_COMMON;

		ID3D12ProtectedResourceSession* protectedSession = nullptr;

		// SRV��URV�Ŏg�p�����DXGI_FORMAT�ɂ�����, �݂��ɃL���X�g�\�Ȃ��̂̃��X�g��񋟂���.
		gu::DynamicArray<DXGI_FORMAT> castableFormats = GetCastableFormats(localDesc1.Format);

		result = _device->CreateReservedResource2(&localDesc, barrierInitialLayout, clearValue,
			protectedSession, static_cast<uint32>(castableFormats.Size()), castableFormats.Data(), IID_PPV_ARGS(resource.GetAddressOf()));
	}
	else
	{
		result = _device->CreateReservedResource(&localDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
	}

	return result;
}

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
HRESULT RHIDevice::CreatePlacedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const HeapComPtr& heap, const gu::uint64 heapOffset, const D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue )
{
	// GPU�̃������m�ۂ�����ɂ��̏����i�[����悪������Ȃ��ꍇ�͂��̂܂܏I��
	if (!resource.GetAddressOf())
	{
		return E_POINTER;
	}

	// Heap�̑��݂��m�F����
	if (heap == nullptr || heap.GetAddressOf() || heap.Get())
	{
		return E_POINTER;
	}

	/*-------------------------------------------------------------------
	-            �q�[�v�̈�̊m�ۂ�GPU�Ƀ��������m�ۂ���
	---------------------------------------------------------------------*/
	HRESULT result = S_OK;

	// Intel�g�����g�p����ꍇ�͓Ǝ��̊֐����g�p����
#if USE_INTEL_EXTENSION
	if (IsSupportedIntelEmulatedAtomic64())
	{
		D3D12_RESOURCE_DESC localDesc = resourceDesc;
		INTC_D3D12_RESOURCE_DESC_0001 intelLocalDesc{};
		intelLocalDesc.pD3D12Desc = &localDesc;
		intelLocalDesc.EmulatedTyped64bitAtomics = true;

		result = INTC_D3D12_CreatePlacedResource(_intelExtensionContext, heap.Get(), heapOffset, &intelLocalDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
	}
	else
#endif
	// �񈳏k��UAV���g�p
	if (IsSupportedAdditionalUAVType() && resourceDesc.Format != DXGI_FORMAT::DXGI_FORMAT_UNKNOWN && (resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS))
	{
		const D3D12_RESOURCE_DESC1 localDesc1 =
		{
			.Dimension                = resourceDesc.Dimension,
			.Alignment                = resourceDesc.Alignment,
			.Width                    = resourceDesc.Width,
			.Height                   = resourceDesc.Height,
			.DepthOrArraySize         = resourceDesc.DepthOrArraySize,
			.MipLevels                = resourceDesc.MipLevels,
			.Format                   = resourceDesc.Format,
			.SampleDesc               = resourceDesc.SampleDesc,
			.Layout                   = resourceDesc.Layout,
			.Flags                    = resourceDesc.Flags,
			.SamplerFeedbackMipRegion = {}
		};

		// Buffer�ƂȂ�ꍇ�͕K��Undefined����n�߂�K�v�����邪, ����ȊO�̓e�N�X�`���`�����g�p�\�Ȃ���COMMON���g�p����.
		const D3D12_BARRIER_LAYOUT barrierInitialLayout = localDesc1.Dimension == D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER
			? D3D12_BARRIER_LAYOUT_UNDEFINED : D3D12_BARRIER_LAYOUT_COMMON;

		// SRV��URV�Ŏg�p�����DXGI_FORMAT�ɂ�����, �݂��ɃL���X�g�\�Ȃ��̂̃��X�g��񋟂���.
		gu::DynamicArray<DXGI_FORMAT> castableFormats = GetCastableFormats(localDesc1.Format);

		result = _device->CreatePlacedResource2(heap.Get(), heapOffset, &localDesc1, barrierInitialLayout, clearValue,
			static_cast<uint32>(castableFormats.Size()), castableFormats.Data(), IID_PPV_ARGS(resource.GetAddressOf()));
	}
	else
	{
		result = _device->CreatePlacedResource(heap.Get(), heapOffset, &resourceDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
	}

	return result;
}

#pragma endregion           Create Resource Function

#pragma region Debug Function

/*!**********************************************************************
*  @brief     �o�͂̏ꏊ��DirectX12�̃G���[���������ꍇ�ɕ񍐂���@�\���f�o�b�O���[�h�̂ݒǉ�����
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDevice::ReportLiveObjects()
{
#ifdef _DEBUG
	ID3D12DebugDevice2* debugInterface = nullptr;
	if (SUCCEEDED(_device.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL); // Only Objects with external reference counts
		debugInterface->Release();
	}
#endif
}

#pragma endregion           Debug Function

#pragma region Device Support Function
/****************************************************************************
*						CheckDXRSupport
****************************************************************************/
/* @fn        void DirectX12::CheckDXRSupport()
* 
/* @brief     Check DXRSupport
* 
*  @param[in] void
* 
*  @return �@�@void
* 
*  @details   Tier1_1�̏ꍇ,�@�ȉ��̓��e���\�ƂȂ�܂� 
�@�@�@�@�@�@�@�@�@�EExecuteIndirect�����DispatchRays�Ăяo���̃T�|�[�g, RayQuery�̎g�p
* �@�@�@�@�@�@�@�@�EAddToStateObject����������̏�ԃI�u�W�F�N�g�ւ̑����ǉ�
�@�@�@�@�@�@�@�@�@�ESkipTriangles, skip procedual primitives�̃t���O�̎g�p
		 �@�@�@�@https://github.com/microsoft/DirectX-Specs/blob/master/d3d/Raytracing.md#d3d12_raytracing_tier�@
			   https://devblogs.microsoft.com/directx/dxr-1-1/�@
*****************************************************************************/
void RHIDevice::CheckDXRSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
		
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options))))) 
	{
		_isSupportedRayTracing = false;
		return; 
	}

	_rayTracingTier        = options.RaytracingTier;
	_isSupportedRayTracing = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_0;
	_isSupportedRayQuery   = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_1;
}

/****************************************************************************
*						CheckRenderPassSupport
****************************************************************************/
/* @fn        void RHIDevice::CheckRenderPassSupport()
*
/* @brief     Check render pass support
*
*  @param[in] void
*
*  @return �@�@void
*
*  @details   Tier0 : �����_�[�p�X�͖�����, �ꉞ�\�t�g�E�F�A�G�~�����[�V��������Ă̂ݒ񋟂Ƃ͌����Ă���̂�, �T�|�[�g�ΏۂƂ͂������܂�. 
*             Tier1 : Render Target, Depth Buffer�̏������݂̍�����, ������, UAV�̏������݂�RenderPass���ł͌����I�ɂ͂Ȃ�Ȃ��Ƃ̂���
*             Tier2 : Render Target, Depth Buffer�̏������݂̍�����, UAV�̏������݂�OK
*****************************************************************************/
void RHIDevice::CheckRenderPassSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options)))))
	{
		_isSupportedRenderPass = false;
		_renderPassTier        = D3D12_RENDER_PASS_TIER_0; 
		return;
	}

	_isSupportedRenderPass = options.RenderPassesTier > D3D12_RENDER_PASS_TIER_0;
	_renderPassTier        = options.RenderPassesTier;
}

/****************************************************************************
*						CheckSamplerFeedbackSupport
****************************************************************************/
/* @fn        void RHIDevice::CheckSamplerFeedbackSupport()
*
/* @brief     Check sampelr feedback support
*
*  @param[in] void
*
*  @return �@�@void
*
*  @details   Sampler feedback��, �e�N�X�`���T���v�����O���ƈʒu���L���v�`��, �L�^���邽�߂̋@�\. 
*             https://microsoft.github.io/DirectX-Specs/d3d/SamplerFeedback.html
*             Tier 0.9 : D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMP�݂̂ɑΉ�����, mipmap��0�̂ݑΉ�
* �@�@�@�@�@�@�@ Tier 1.0 : �S�Ă�Texture addressing mode�Ŏg�p�\
*****************************************************************************/
void RHIDevice::CheckSamplerFeedbackSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options{};

	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options, UINT(sizeof(options)))))
	{
		_isSupportedSamplerFeedback = options.SamplerFeedbackTier >= D3D12_SAMPLER_FEEDBACK_TIER_0_9;
		_samplerFeedbackTier        = options.SamplerFeedbackTier;
	}
	else
	{
		_isSupportedSamplerFeedback = false;
		_samplerFeedbackTier        = D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;
	}

}

/****************************************************************************
*						CheckBindlessSupport
****************************************************************************/
/* @fn        void RHIDevice::CheckBindlessSupport()
*
/* @brief     Check bindless support
*
*  @param[in] void
*
*  @return �@�@void
*
*  @details   �ʏ��RootParameter���g���Ċe���\�[�X�r���[�ƃO���t�B�b�N�p�C�v���C���̕R�Â����s����, 
*             DescriptorHeap�̃C���f�b�N�X�������g���ăo�C���h��, �����̃C���f�N�X��S��RootConstant�ɔz�u�\.
*             https://rtarun9.github.io/blogs/bindless_rendering/
*****************************************************************************/
void RHIDevice::CheckBindlessSupport()
{
	if (GetMaxSupportedShaderModel() >= D3D_SHADER_MODEL_6_6 && _resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_3)
	{
		_isSupportedBindless = true;
		_bindlessResourceType = GetMaxSupportedFeatureLevel() <= D3D_FEATURE_LEVEL_11_1 ? core::BindlessResourceType::OnlyRayTracing : core::BindlessResourceType::AllShaderTypes;
	}
}
/****************************************************************************
*                     CheckVRSSupport
****************************************************************************/
/* @fn        void GraphicsDeviceDirectX12::CheckVRSSupport()
* 
/* @brief     Variable Rate Shading support
* 
*  @param[in] void
* 
*  @return �@�@void
* 
*  @details   �σ��[�g�V�F�[�f�B���O��, �����s�N�Z���̉��1�s�N�Z���Ƃ��Čv�Z�����@�ł�. 
*             Tier1��DrawCall���ƂɎw��ł�, �`�悳���ΏۂɈꗥ�ɓK�p���܂�.
*             Tier2��DrawCall�����łȂ�, �v���~�e�B�u����, �^�C�����Ƃɂ��ݒ�ł���悤�ɂȂ�܂�. 
*             ���ʂ���, 1x1, 1x2, 2x1, 2x2�͑S�Ẵ��x���ŃT�|�[�g����Ă�����̂�, 2x4, 4x2, 4x4��AdditionalShadingRatesSupported���m�F����K�v������܂�.
*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/vrs
*             https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC168%E5%9B%9E
*****************************************************************************/
void RHIDevice::CheckVRSSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS6 options = {};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options, sizeof(options))))
	{
		_variableRateShadingTier = options.VariableShadingRateTier;

		if (_variableRateShadingTier != D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED)
		{
			_isSupportedLargerVariableRateShadingSize = options.AdditionalShadingRatesSupported;
		}

		if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_1)
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading Tier1 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = false;
			_variableRateShadingImageTileSize    = 1;    // Tier2�݂̂̋@�\�Ȃ̂ŕK�v�Ȃ�
		}
		else if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_2)
		{
			OutputDebugStringA("Gpu api: Valiable Rate Shading Tier2 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = true;
			_variableRateShadingImageTileSize = options.ShadingRateImageTileSize; // 8, 16, 32���Ԃ����
		}
		else
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading not supported");
			_isSupportedVariableRateShadingTier1 = false;
			_isSupportedVariableRateShadingTier2 = false;
			_variableRateShadingImageTileSize = 1;
		}
	}
	else
	{
		OutputDebugStringA("GpuApi : Variable Rate Shading note supported on current gpu hardware.\n");
		_isSupportedVariableRateShadingTier1 = false;
		_isSupportedVariableRateShadingTier2 = false;
		_variableRateShadingImageTileSize = 1;
	}
}

/****************************************************************************
*                     MultiSampleQualityLevels
****************************************************************************/
/* @fn        void DirectX12::CheckMultiSampleQualityLevels(void)
* 
/* @brief     Multi Sample Quality Levels for Msaa (Anti-Alias)
* 
*  @param[in] void
* 
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckMultiSampleQualityLevels()
{
	for (gu::uint32 sampleCount = DESIRED_MAX_MSAA_SAMPLE_COUNT; sampleCount > 0; sampleCount--)
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
		msQualityLevels.SampleCount = sampleCount;

		if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels))))
		{
			_msaaQuality     = msQualityLevels.NumQualityLevels;
			_maxMSAASampleCount = sampleCount;
			break;
		}
	}
}

/****************************************************************************
*                     CheckHDRDisplaySupport
****************************************************************************/
/* @fn        void DirectX12::CheckHDRDisplaySupport()
* 
/* @brief     CheckHDRDisplaySupport()�@https://qiita.com/dgtanaka/items/672d2e7b3152f4e5ed49
* 
*  @param[in] void
* 
*  @return �@�@void
*****************************************************************************/
void RHIDevice::SetupDisplayHDRMetaData()
{
	/*-------------------------------------------------------------------
	-               Get display adapter
	---------------------------------------------------------------------*/
	const auto adapter = gu::StaticPointerCast<directX12::RHIDisplayAdapter>(_adapter);
	auto dxAdapter     = adapter->GetAdapter();

	ComPtr<IDXGIOutput> output = nullptr;

	/*-------------------------------------------------------------------
	-               Adapter��AMD or Nvidia�ł���Ώ��������s����
	---------------------------------------------------------------------*/
	if (!(adapter->IsAdapterAMD() || adapter->IsAdapterNVIDIA()))
	{
		_isSupportedHDR = false; 
		return;
	}

	/*-------------------------------------------------------------------
	-              Search EnableHDR output
	---------------------------------------------------------------------*/
	for(uint32 i = 0; dxAdapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
#if DXGI_MAX_OUTPUT_INTERFACE >= 6
		
		OutputComPtr output6 = nullptr;

		if (SUCCEEDED(output->QueryInterface(IID_PPV_ARGS(output6.GetAddressOf()))))
		{
			DXGI_OUTPUT_DESC1 desc = {};
			output6->GetDesc1(&desc);

			/*-------------------------------------------------------------------
			-              IS HDR Enable Color Space
			---------------------------------------------------------------------*/
			if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
			{
				_isSupportedHDR = true;

				/*-------------------------------------------------------------------
				-              Set up hdr display information
				---------------------------------------------------------------------*/
				_displayInfo = 
				{
					core::DisplayColorGamut::Rec2020_D65,
					desc.MaxLuminance <= 1000.0f ? core::DisplayOutputFormat::HDR_ACES_1000nit_ST2084 : core::DisplayOutputFormat::HDR_ACES_2000nit_ST2084,
					{desc.RedPrimary  [0], desc.RedPrimary  [1]},
					{desc.GreenPrimary[0], desc.GreenPrimary[1]},
					{desc.BluePrimary [0], desc.BluePrimary [1]},
					{desc.WhitePoint  [0], desc.WhitePoint  [1]},
					{desc.MinLuminance},
					{desc.MaxLuminance},
					{desc.MaxFullFrameLuminance},
					{desc.DesktopCoordinates.left, desc.DesktopCoordinates.top, desc.DesktopCoordinates.right, desc.DesktopCoordinates.bottom},
				};


				output.Reset();
				break;
			}
		}
		
#endif
		output.Reset();
	}

	_isSupportedHDR = false;
}

/****************************************************************************
*                     CheckMeshShadingSupport
****************************************************************************/
/* @fn        void DirectX12::CheckMeshShadingSupport()
* 
/* @brief     Mesh Shading support check
* 
*  @param[in] void
* 
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckMeshShadingSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options{};

	if (FAILED(_device->CheckFeatureSupport(
		D3D12_FEATURE_D3D12_OPTIONS7, &options, UINT(sizeof(options)))))
	{
		_isSupportedMeshShading = false; return;
	}

	_isSupportedMeshShading = options.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1;
}

/****************************************************************************
*                     CheckAllowTearingSupport
****************************************************************************/
/* @fn        void RHIDevice::CheckAllowTearingSupport()
*
/* @brief     Allow tearing support
*            
*  @param[in] void
*
*  @return �@�@void
* 
*  @details   �σ��[�g�̃��t���b�V�����[�g���g�p����Ƃ��Ɏg�p���܂�. 
*             ��ʂ̃e�B�A�����O (�����ł��ĂȂ��Ƃ��Ƀt���b�v�����������Ă��܂���ʂ̂����)�������Ȃ��悤��, ���̃t���[���̏�����������܂Œx�������Ă����@�\�ł�.
*             ������, VSync��0�ɂ��Ă�������. 
*****************************************************************************/
void RHIDevice::CheckAllowTearingSupport()
{
#if DXGI_MAX_FACTORY_INTERFACE >= 5
	const auto dxInstance = static_cast<directX12::RHIInstance*>(_adapter->GetInstance());
	const auto factory    = dxInstance->GetFactory();
	
	BOOL allowTearing = FALSE; // Bool��int�^�Ȃ̂�, bool���Ɓ~
	if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
	{
		_isSupportedAllowTearing = allowTearing;
	}
	else
	{
		_isSupportedAllowTearing = false;
	}
#endif
}

/****************************************************************************
*                     DepthBoundsTestSupport
****************************************************************************/
/* @brief      �[�x�l���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳��, �͈͊O�Ȃ�ΊY���s�N�Z���𑁊����p������@
		�@�@�@�@ Deferred Rendering�ɂ����郉�C�g��accumulation, Deferred Rendering�ɂ�����Cascaded Shadow Map, ��ʊE�[�x�G�t�F�N�g, ���i�`�擙�Ɏg�p�\ 
*              https://microsoft.github.io/DirectX-Specs/d3d/DepthBoundsTest.html
*              https://shikihuiku.wordpress.com/tag/depthboundstest/
* 
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckDepthBoundsTestSupport()
{
	#if PLATFORM_OS_WINDOWS
	D3D12_FEATURE_DATA_D3D12_OPTIONS2 options{};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &options, sizeof(options))))
	{
		_isSupportedDepthBoundsTest = options.DepthBoundsTestSupported;
	}
	else
	{
		_isSupportedDepthBoundsTest = false;
	}
	#endif
}

/****************************************************************************
*                     CheckResourceTiers
****************************************************************************/
/* @brief     �p�C�v���C���Ŏg�p�\�ȃ��\�[�X�̏���l���m�F���邽�߂Ɏg�p����
* 
*  @details   �傫���قȂ�͈̂ȉ��̓_�ł�
*             1. CBV : Tier 1, 2��14�܂�. Tier3 ��DescripterHeap�̍ő吔
*             2. SRV : Tier 1��128�܂�. Tier2, 3��DescripterHeap�̍ő吔
*             3. UAV : Tier1�͋@�\���x�� 11.1+�ȏ��64, ����ȊO��8, Tier2��64, Tier3��DescripterHeap�̍ő吔
*             4. Sampler : Tier1��16, ����ȊO��2048
*             5. �q�[�v����Descripter�̍ő吔 Tier1, 2��1,000,000�ATier3�͖�����
* �@�@�@�@�@�@�@�@https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support
* 
* �@�@�@�@�@�@�@�@HeapTier�̕��ł�, �o�b�t�@�[, RenderTarget��DepthStencil, TargetStencil, �[�x�X�e���V���e�N�X�`���̃����_�����O�𓯈�q�[�v�Ŏg�p�ł��邩�𒲂ׂ܂�
* �@�@�@�@�@�@�@�@Tier1���r��, Tier2�����݉\�ł�.
* �@�@�@�@�@�@�@�@https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
*****************************************************************************/
void RHIDevice::CheckResourceTiers()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};
	
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_resourceBindingTier = options.ResourceBindingTier;
	_resourceHeapTier    = options.ResourceHeapTier;
}

/****************************************************************************
*                     CheckStencilReferenceFromPixelShaderSupport
****************************************************************************/
/* @fn        void RHIDevice::CheckStencilReferenceFromPixelShaderSupport()
*
/* @brief     �X�e���V���o�b�t�@�̎Q�ƒl���s�N�Z���V�F�[�_�[�ŏo�͏o����悤�ɂ��܂�. 
*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/shader-specified-stencil-reference-value
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckStencilReferenceFromPixelShaderSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_isSupportedStencilReferenceFromPixelShader = options.PSSpecifiedStencilRefSupported != 0;
}

void RHIDevice::CheckAdditionalUAVType()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_isSupportedAdditionalUAVType = options.TypedUAVLoadAdditionalFormats != 0;
}

/****************************************************************************
*                     CheckMaxHeapSize()
****************************************************************************/
/* @fn        void RHIDevice::CheckMaxHeapSize()
*
/* @brief     �ő��Heap Size���Z�b�g���܂�.
* 
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckMaxHeapSize()
{
	if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_1)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1;
	}
	else if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_2)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}
	else if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_3)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}
	else
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}

	_maxSamplerHeapCount = D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE;
}

/*!**********************************************************************
*  @brief     DirectX�Ŏg�p�\�ȍő�̋@�\���x���������Őݒ肵�܂�
*  @note      ���݂�12_2���ő�T�|�[�g���x���ł�
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDevice::FindHighestFeatureLevel()
{
	const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelCaps = {};
	featureLevelCaps.pFeatureLevelsRequested = featureLevels;
	featureLevelCaps.NumFeatureLevels        = _countof(featureLevels);

	/*-------------------------------------------------------------------
	-               Feature Support Check
	---------------------------------------------------------------------*/
	ThrowIfFailed(_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelCaps, sizeof(featureLevelCaps)));
	_maxSupportedFeatureLevel = featureLevelCaps.MaxSupportedFeatureLevel;
}

/****************************************************************************
*                    SetupPlatformPixelFormats
****************************************************************************/
/* @brief     DirectX12�Ŏg�p�\�ȃs�N�Z���t�H�[�}�b�g��ݒ肷��
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::SetupPlatformPixelFormats()
{
	using namespace rhi::core;
	using enum PixelFormat;
	
	PixelFormatInfo::Get(Unknown             ).PlatformFormat = DXGI_FORMAT_UNKNOWN;
	PixelFormatInfo::Get(R32G32B32A32_FLOAT  ).PlatformFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	PixelFormatInfo::Get(R32G32B32A32_UINT   ).PlatformFormat = DXGI_FORMAT_R32G32B32A32_UINT;
	PixelFormatInfo::Get(R32G32B32A32_SINT   ).PlatformFormat = DXGI_FORMAT_R32G32B32A32_SINT;
	PixelFormatInfo::Get(R16G16B16A16_FLOAT  ).PlatformFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	PixelFormatInfo::Get(R16G16B16A16_UINT   ).PlatformFormat = DXGI_FORMAT_R16G16B16A16_UINT;
	PixelFormatInfo::Get(R16G16B16A16_SINT   ).PlatformFormat = DXGI_FORMAT_R16G16B16A16_SINT;
	PixelFormatInfo::Get(R16G16B16A16_UNORM  ).PlatformFormat = DXGI_FORMAT_R16G16B16A16_UNORM;
	PixelFormatInfo::Get(R16G16B16A16_SNORM  ).PlatformFormat = DXGI_FORMAT_R16G16B16A16_SNORM;
	PixelFormatInfo::Get(R8G8B8A8_UINT       ).PlatformFormat = DXGI_FORMAT_R8G8B8A8_UINT;
	PixelFormatInfo::Get(R8G8B8A8_SINT       ).PlatformFormat = DXGI_FORMAT_R8G8B8A8_SINT;
	PixelFormatInfo::Get(R8G8B8A8_UNORM      ).PlatformFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	PixelFormatInfo::Get(R8G8B8A8_SNORM      ).PlatformFormat = DXGI_FORMAT_R8G8B8A8_SNORM;
	PixelFormatInfo::Get(R8G8B8A8_UNORM_SRGB ).PlatformFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	PixelFormatInfo::Get(B8G8R8A8_UNORM      ).PlatformFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	PixelFormatInfo::Get(B8G8R8A8_UNORM_SRGB ).PlatformFormat = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	PixelFormatInfo::Get(R32G32B32_FLOAT     ).PlatformFormat = DXGI_FORMAT_R32G32B32_FLOAT;
	PixelFormatInfo::Get(R32G32B32_UINT      ).PlatformFormat = DXGI_FORMAT_R32G32B32_UINT;
	PixelFormatInfo::Get(R32G32B32_SINT      ).PlatformFormat = DXGI_FORMAT_R32G32B32_SINT;
	PixelFormatInfo::Get(R32G32_FLOAT        ).PlatformFormat = DXGI_FORMAT_R32G32_FLOAT;
	PixelFormatInfo::Get(R32G32_UINT         ).PlatformFormat = DXGI_FORMAT_R32G32_UINT;
	PixelFormatInfo::Get(R32G32_SINT         ).PlatformFormat = DXGI_FORMAT_R32G32_SINT;
	PixelFormatInfo::Get(R16G16_FLOAT        ).PlatformFormat = DXGI_FORMAT_R16G16_FLOAT;
	PixelFormatInfo::Get(R16G16_UINT         ).PlatformFormat = DXGI_FORMAT_R16G16_UINT;
	PixelFormatInfo::Get(R16G16_SINT         ).PlatformFormat = DXGI_FORMAT_R16G16_SINT;
	PixelFormatInfo::Get(R8G8_UINT           ).PlatformFormat = DXGI_FORMAT_R8G8_UINT;
	PixelFormatInfo::Get(R8G8_SINT           ).PlatformFormat = DXGI_FORMAT_R8G8_SINT;
	PixelFormatInfo::Get(R8G8_UNORM          ).PlatformFormat = DXGI_FORMAT_R8G8_UNORM;
	PixelFormatInfo::Get(R8G8_SNORM          ).PlatformFormat = DXGI_FORMAT_R8G8_SNORM;
	PixelFormatInfo::Get(D32_FLOAT           ).PlatformFormat = DXGI_FORMAT_D32_FLOAT;
	PixelFormatInfo::Get(R32_FLOAT           ).PlatformFormat = DXGI_FORMAT_R32_FLOAT;
	PixelFormatInfo::Get(R32_UINT            ).PlatformFormat = DXGI_FORMAT_R32_UINT;
	PixelFormatInfo::Get(R32_SINT            ).PlatformFormat = DXGI_FORMAT_R32_SINT;
	PixelFormatInfo::Get(D16_UNORM           ).PlatformFormat = DXGI_FORMAT_D16_UNORM;
	PixelFormatInfo::Get(R16_FLOAT           ).PlatformFormat = DXGI_FORMAT_R16_FLOAT;
	PixelFormatInfo::Get(R16_UINT            ).PlatformFormat = DXGI_FORMAT_R16_UINT;
	PixelFormatInfo::Get(R16_SINT            ).PlatformFormat = DXGI_FORMAT_R16_SINT;
	PixelFormatInfo::Get(R16_UNORM           ).PlatformFormat = DXGI_FORMAT_R16_UNORM;
	PixelFormatInfo::Get(R16_SNORM           ).PlatformFormat = DXGI_FORMAT_R16_SNORM;
	PixelFormatInfo::Get(R8_UINT             ).PlatformFormat = DXGI_FORMAT_R8_UINT;
	PixelFormatInfo::Get(R8_SINT             ).PlatformFormat = DXGI_FORMAT_R8_SINT;
	PixelFormatInfo::Get(R8_UNORM            ).PlatformFormat = DXGI_FORMAT_R8_UNORM;
	PixelFormatInfo::Get(R8_SNORM            ).PlatformFormat = DXGI_FORMAT_R8_SNORM;
	PixelFormatInfo::Get(A8_UNORM            ).PlatformFormat = DXGI_FORMAT_A8_UNORM;
	PixelFormatInfo::Get(R1_UNORM            ).PlatformFormat = DXGI_FORMAT_R1_UNORM;
	PixelFormatInfo::Get(D32_FLOAT_S8X24_UINT).PlatformFormat = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
	PixelFormatInfo::Get(D24_UNORM_S8_UINT   ).PlatformFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	PixelFormatInfo::Get(R10G10B10A2_UNORM   ).PlatformFormat = DXGI_FORMAT_R10G10B10A2_UNORM;
	PixelFormatInfo::Get(R10G10B10A2_UINT    ).PlatformFormat = DXGI_FORMAT_R10G10B10A2_UINT;
	PixelFormatInfo::Get(BC1_UNORM           ).PlatformFormat = DXGI_FORMAT_BC1_UNORM;
	PixelFormatInfo::Get(BC1_UNORM_SRGB      ).PlatformFormat = DXGI_FORMAT_BC1_UNORM_SRGB;
	PixelFormatInfo::Get(BC2_UNORM           ).PlatformFormat = DXGI_FORMAT_BC2_UNORM;
	PixelFormatInfo::Get(BC2_UNORM_SRGB      ).PlatformFormat = DXGI_FORMAT_BC2_UNORM_SRGB;
	PixelFormatInfo::Get(BC3_UNORM           ).PlatformFormat = DXGI_FORMAT_BC3_UNORM;
	PixelFormatInfo::Get(BC3_UNORM_SRGB      ).PlatformFormat = DXGI_FORMAT_BC3_UNORM_SRGB;
	PixelFormatInfo::Get(BC4_UNORM           ).PlatformFormat = DXGI_FORMAT_BC4_UNORM;
	PixelFormatInfo::Get(BC4_SNORM           ).PlatformFormat = DXGI_FORMAT_BC4_SNORM;
	PixelFormatInfo::Get(BC5_UNORM           ).PlatformFormat = DXGI_FORMAT_BC5_UNORM;
	PixelFormatInfo::Get(BC5_SNORM           ).PlatformFormat = DXGI_FORMAT_BC5_SNORM;
	PixelFormatInfo::Get(BC7_UNORM           ).PlatformFormat = DXGI_FORMAT_BC7_UNORM;
	PixelFormatInfo::Get(BC7_UNORM_SRGB      ).PlatformFormat = DXGI_FORMAT_BC7_UNORM_SRGB;
	PixelFormatInfo::Get(BC6H_UFLOAT16       ).PlatformFormat = DXGI_FORMAT_BC6H_UF16;
	PixelFormatInfo::Get(BC6H_SFLOAT16       ).PlatformFormat = DXGI_FORMAT_BC6H_SF16;
}

/*!**********************************************************************
*  @brief     DirectX�Ŏg�p�\�ȍő�̃V�F�[�_�[���f����ݒ肵�܂�
*  @note      ���݂�6_9���w��\�ȃT�|�[�g���x���ł���, ���ɉ����ă��x���͉�����ꍇ������܂�@n
*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/overviews-direct3d-11-devices-downlevel-intro
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDevice::FindHighestShaderModel()
{
	
	const D3D_SHADER_MODEL shaderModels[] =
	{
		D3D_HIGHEST_SHADER_MODEL,
		D3D_SHADER_MODEL_6_9,
		D3D_SHADER_MODEL_6_8,
		D3D_SHADER_MODEL_6_7,
		D3D_SHADER_MODEL_6_6,
		D3D_SHADER_MODEL_6_5,
		D3D_SHADER_MODEL_6_4,
		D3D_SHADER_MODEL_6_3,
		D3D_SHADER_MODEL_6_2,
		D3D_SHADER_MODEL_6_1,
		D3D_SHADER_MODEL_6_0,
	};

	D3D12_FEATURE_DATA_SHADER_MODEL featureShaderModel = {};
	for (const auto shaderModel : shaderModels)
	{
		featureShaderModel.HighestShaderModel = shaderModel;

		if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &featureShaderModel, sizeof(featureShaderModel))))
		{
			_maxSupportedShaderModel = featureShaderModel.HighestShaderModel;
			return;
		}
	}

	// �S��������Ȃ������ꍇ
	_maxSupportedShaderModel = D3D_SHADER_MODEL_5_1;
}

/*!**********************************************************************
*  @brief     HLSL�Ŗ����I��GPU��ŕ����X���b�h�̎g�p���\. Wave : �v���Z�b�T��̓����Ɏ��s�����X���b�h�̏W��, Lane : �X�̃X���b�h
*  @param[in] void
*  @return    void
*************************************************************************/
void RHIDevice::CheckWaveLaneSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS1 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &options, sizeof(options)))) { return; }
	_isSupportedWaveLane = !!options.WaveOps;
	_minWaveLaneCount    = options.WaveLaneCountMin;
	_maxWaveLaneCount    = options.WaveLaneCountMax;
}

/****************************************************************************
*                     CheckNative16bitOperation
****************************************************************************/
/* @fn        void RHIDevice::CheckNative16bitOperation()
*
/* @brief     16 bit�̃V�F�[�_�[���삪�\���ǂ����𒲂ׂ܂�
* 
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckNative16bitOperation()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS4 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &options, sizeof(options)))) { return; }
	_isSupported16bitOperation = !!options.Native16BitShaderOpsSupported;
}

/****************************************************************************
*                     CheckAtomicOperation
****************************************************************************/
/* @fn        void RHIDevice::CheckAtomicOperation()
*
/* @brief     Wave�p��Atomic���삪�\���ǂ����𒲂ׂ܂�.
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckAtomicOperation()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options7{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS9 options9{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS11 options11{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options7, sizeof(options7)))) { return; }
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &options9, sizeof(options9)))) { return; }
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &options11, sizeof(options11)))) { return; }

	_isSupportedAtomicOperation = !!options9.AtomicInt64OnTypedResourceSupported
#if USE_INTEL_EXTENSION
		|| IsSupportedIntelEmulatedAtomic64();
#else
		;
#endif

	_isSupportedAtomicInt64OnTypedResource             = !!options9.AtomicInt64OnTypedResourceSupported;
	_isSupportedAtomicInt64OnGroupSharedSupported      = !!options9.AtomicInt64OnGroupSharedSupported;
	_isSupportedInt64OnDescriptorHeapResourceSupported = !!options11.AtomicInt64OnDescriptorHeapResourceSupported;
	_isSupportedAtomicUInt64 = _isSupportedAtomicInt64OnTypedResource && _isSupportedInt64OnDescriptorHeapResourceSupported;
}

/****************************************************************************
*                     CheckHighestRootSignatureVersion
****************************************************************************/
/* @brief  ���݂̎w��\�ȍő��RootSignature�̃o�[�W�����𒲂ׂ܂�. @n
*           GetHighestRootSignatureVersion���Q�Ƃ��Ă�������
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void RHIDevice::CheckHighestRootSignatureVersion()
{
	D3D12_FEATURE_DATA_ROOT_SIGNATURE options{};

	// �����l�̐ݒ�
	options.HighestVersion = _highestRootSignatureVersion;

	// �o�[�W�����������k��Ȃ���, �T�|�[�g����Ă���ő�l���擾����
	while (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &options, sizeof(options))))
	{
		options.HighestVersion = static_cast<D3D_ROOT_SIGNATURE_VERSION>(options.HighestVersion - 1);
	}

	_highestRootSignatureVersion = options.HighestVersion;
}

#if USE_INTEL_EXTENSION

/*!**********************************************************************
*  @brief  Intel extension context�𐶐����܂�.
*************************************************************************/
void RHIDevice::CreateIntelExtensionContext()
{
	_isSupportedIntelEmulatedAtomic64 = false;

	/*-------------------------------------------------------------------
	-      �h���C�o�[�̃C���X�g�[���f�B���N�g����������, igdext64.dll�������܂�
	---------------------------------------------------------------------*/
	if (FAILED(INTC_LoadExtensionsLibrary(false)))
	{
		_RPT0(_CRT_WARN, "Failed to load Intel Extension Library\n");
	}

	/*-------------------------------------------------------------------
	-         �T�|�[�g����Ă���o�[�W�������̎擾
	---------------------------------------------------------------------*/
	gu::DynamicArray<INTCExtensionVersion> supportedExtensionsVersions = {};
	gu::uint32 supportedExtensionVersionCount = 0;

	if (FAILED(INTC_D3D12_GetSupportedVersions(_device.Get(), nullptr, &supportedExtensionVersionCount)))
	{
		return;
	}

	// �T�|�[�g����Ă���o�[�W�����������C���X�^���X�쐬
	supportedExtensionsVersions.Resize(supportedExtensionVersionCount);

	/*-------------------------------------------------------------------
	-         �T�|�[�g����Ă���o�[�W���������擾����
	---------------------------------------------------------------------*/
	if (FAILED(INTC_D3D12_GetSupportedVersions(_device.Get(), supportedExtensionsVersions.Data(), &supportedExtensionVersionCount)))
	{
		return;
	}

	_RPT0(_CRT_WARN, "/////////////////////////////////////////////////\n");
	_RPT0(_CRT_WARN, " Supported Extension Versions in this driver: \n");
	_RPT0(_CRT_WARN, "/////////////////////////////////////////////////\n");

	// �ڕW�̃o�[�W����
	const INTCExtensionVersion atomicsRequiredVersion =
	{
		.HWFeatureLevel = 4,
		.APIVersion     = 8,
		.Revision       = 0
	};

	INTCExtensionInfo intelExtensionInfo = {};
	for (uint32 i = 0; i < supportedExtensionVersionCount; ++i)
	{
		if((supportedExtensionsVersions[i].HWFeatureLevel >= atomicsRequiredVersion.HWFeatureLevel) && 
		   (supportedExtensionsVersions[i].APIVersion     >= atomicsRequiredVersion.APIVersion    ) &&  
		   (supportedExtensionsVersions[i].Revision       >= atomicsRequiredVersion.Revision     ))
		{
			_RPTN(_CRT_WARN, "Intel Extension loaded requested version: %u.%u.%u\n",
				&supportedExtensionsVersions[i].HWFeatureLevel,
				&supportedExtensionsVersions[i].APIVersion,
				&supportedExtensionsVersions[i].Revision);

			intelExtensionInfo.RequestedExtensionVersion = supportedExtensionsVersions[i];
			break;
		}
	}

	/*-------------------------------------------------------------------
	-         �T�|�[�g���ꂽ�o�[�W������Device���쐬����
	---------------------------------------------------------------------*/
	INTCExtensionContext* intelExtensionContext = nullptr;

	INTCExtensionAppInfo applicationInfo = {};
	applicationInfo.pEngineName   = L"PPP_Engine";
	applicationInfo.EngineVersion = 1;

	const auto result = INTC_D3D12_CreateDeviceExtensionContext(_device.Get(), &intelExtensionContext, &intelExtensionInfo, &applicationInfo);
	
	if (SUCCEEDED(result))
	{
		_RPT0(_CRT_WARN, "Intel Extension Framework enabled.\n");
	}
	else
	{
		if (result == E_NOINTERFACE)
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework not supported by driver. Please check if a driver update is available\n");
		}
		else if (result == E_INVALIDARG)
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework passed invalid creation arguments\n");
		}
		else
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework failed to initialize\n");
		}
	}

	/*-------------------------------------------------------------------
	-         �I������
	---------------------------------------------------------------------*/
	if (!supportedExtensionsVersions.IsEmpty())
	{
		supportedExtensionsVersions.Clear();
		supportedExtensionsVersions.ShrinkToFit();
	}

	_intelExtensionContext = intelExtensionContext;
	_isSupportedIntelEmulatedAtomic64 = true;
}


/*!**********************************************************************
*  @brief   Intel extension context��j�����܂�.
*************************************************************************/
void RHIDevice::DestroyIntelExtensionContext()
{
	if (_intelExtensionContext == nullptr) { return; }

	/*-------------------------------------------------------------------
	-              INTCExtensionContext���������ꍇ�͔j�����s��
	---------------------------------------------------------------------*/
	const auto result = INTC_DestroyDeviceExtensionContext(&_intelExtensionContext);

	/*-------------------------------------------------------------------
	-              �\��
	---------------------------------------------------------------------*/
	if (result == S_OK)
	{
		_RPT0(_CRT_WARN, "Intel Extensions Framework unloaded\n");
	}
	else
	{
		_RPT0(_CRT_WARN, "Intel Extensions Framework error when unloading\n");
	}
}

/*!**********************************************************************
*  @brief  :Atomic 64 bit���T�|�[�g����Ă��邩��Ԃ��܂�.
*************************************************************************/
bool RHIDevice::IsSupportedIntelEmulatedAtomic64()
{
	/*-------------------------------------------------------------------
	-          Use intel adapter
	---------------------------------------------------------------------*/
	const auto rhiAdapter = static_cast<directX12::RHIDisplayAdapter*>(_adapter.Get());

	if (!rhiAdapter->IsAdapterIntel()) { return false; }

	/*-------------------------------------------------------------------
	-          Created intel extension context
	---------------------------------------------------------------------*/
	return _intelExtensionContext && _isSupportedIntelEmulatedAtomic64;
}

#endif // USE_INTEL_EXTENSION


#pragma endregion  Device Support Function

#pragma region Property
/****************************************************************************
*                     SetName
****************************************************************************/
/* @fn        void RHIDevice::SetName(const gu::tstring& name)
*
/* @brief     Set Logical device name
*
*  @param[in] const gu::tstring& name
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetName(const gu::tstring& name)
{
	_device->SetName(name.CString());
}

/****************************************************************************
*                     GetDefaultHeap
****************************************************************************/
/* @fn        gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
*
/* @brief     Return descriptor heap (CBV, SRV, UAV, RTV, DSV)
*
*  @param[in] const core::DefaultHeapType
*
*  @return    gu::SharedPointer<core::RHIDescriptorHeap>
*****************************************************************************/
gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV:
		case core::DescriptorHeapType::SRV:
		case core::DescriptorHeapType::UAV: { return _defaultHeap[DefaultHeapType::CBV_SRV_UAV];}
		case core::DescriptorHeapType::RTV: { return _defaultHeap[DefaultHeapType::RTV]; }
		case core::DescriptorHeapType::DSV: { return _defaultHeap[DefaultHeapType::DSV]; }
		default: { return nullptr;}
	}
}

/****************************************************************************
*                     SetGPUDebugBreak
****************************************************************************/
/* @brief     RHIInstance�Œ�`�����[���x�̑傫���ɂ���������GPU��DebugBreak���s��
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetGPUDebugBreak() const
{
	/*-------------------------------------------------------------------
	-              GPU debug break���g�p���邩
	---------------------------------------------------------------------*/
	const auto dxInstance = static_cast<rhi::directX12::RHIInstance*>(_adapter->GetInstance());
	if (!dxInstance->UseGPUDebugBreak()) { return; }

	/*-------------------------------------------------------------------
	-              Info queue�����݂��Ă��邩
	---------------------------------------------------------------------*/
	InfoQueuePtr infoQueue = nullptr;
	_device.As(&infoQueue);

	if (!infoQueue) { return; }
	
	/*-------------------------------------------------------------------
	-              �[���x�̐ݒ� (���݂�error�̂ݑΉ��������܂�.)
	---------------------------------------------------------------------*/
	for (uint32 i = 0; i <= (uint32)dxInstance->GetMessageSeverity(); ++i)
	{
		infoQueue->SetBreakOnSeverity((D3D12_MESSAGE_SEVERITY)i, TRUE);
	}
}

/****************************************************************************
*                     SetupDefaultCommandSignatures
****************************************************************************/
/* @fn        void RHIDevice::SetupDefaultCommandSignatures()
*
/* @brief     Set up command signatures
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetupDefaultCommandSignatures()
{
	/*-------------------------------------------------------------------
	-              ExecuteIndirect draw index command signatures
	---------------------------------------------------------------------*/
	{
		D3D12_INDIRECT_ARGUMENT_DESC indirectArgumentDesc = {};
		indirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

		const D3D12_COMMAND_SIGNATURE_DESC commandSignatureDesc =
		{
			.ByteStride = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS),
			.NumArgumentDescs = 1,
			.pArgumentDescs = &indirectArgumentDesc,
			.NodeMask = 0
		};

		ThrowIfFailed(_device->CreateCommandSignature(&commandSignatureDesc, nullptr, IID_PPV_ARGS(_drawIndexedIndirectCommandSignature.GetAddressOf())));
	}
}
#pragma endregion Property
