//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Device.cpp
///             @brief  Logical Device : to use for create GPU resources 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Swapchain.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Fence.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RenderPass.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceLayout.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
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
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor
RHIDevice::RHIDevice()
{
	
}
RHIDevice::~RHIDevice()
{
#ifdef _DEBUG
	if (_device) { ReportLiveObjects(); };
#endif
	Destroy();
}
RHIDevice::RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter, const std::uint32_t frameCount) :
	core::RHIDevice(adapter, frameCount)
{
	/*-------------------------------------------------------------------
	-                   Create Logical Device
	---------------------------------------------------------------------*/
	ThrowIfFailed(D3D12CreateDevice(
		static_pointer_cast<RHIDisplayAdapter>(_adapter)->GetAdapter().Get(),      // default adapter
		D3D_FEATURE_LEVEL_12_0, // minimum feature level
		IID_PPV_ARGS(&_device)));

	_device->SetName(L"DirectX12::Device");

	/*-------------------------------------------------------------------
	-                   Device Support
	---------------------------------------------------------------------*/
	CheckDXRSupport();
	CheckVRSSupport();
	CheckMeshShadingSupport();
	CheckHDRDisplaySupport(); // まだどのクラスに配置するか決めてないので未実装
}

void RHIDevice::SetUp()
{
	/*-------------------------------------------------------------------
	-                   Create command queue
	---------------------------------------------------------------------*/
	_commandQueues[core::CommandListType::Graphics] = CreateCommandQueue(core::CommandListType::Graphics);
	_commandQueues[core::CommandListType::Compute]  = CreateCommandQueue(core::CommandListType::Compute);
	_commandQueues[core::CommandListType::Copy]     = CreateCommandQueue(core::CommandListType::Copy);
	/*-------------------------------------------------------------------
	-                   Create command allocators
	---------------------------------------------------------------------*/
	_commandAllocators.resize(_frameCount);
	for (std::uint32_t i = 0; i < _frameCount; ++i)
	{
		_commandAllocators[i][core::CommandListType::Graphics] = CreateCommandAllocator(core::CommandListType::Graphics);
		_commandAllocators[i][core::CommandListType::Compute]  = CreateCommandAllocator(core::CommandListType::Compute);
		_commandAllocators[i][core::CommandListType::Copy]     = CreateCommandAllocator(core::CommandListType::Copy);
	}

}

void RHIDevice::Destroy()
{
	_defaultHeap.clear();
	_commandQueues.clear();
	if (!_commandAllocators.empty())
	{
		for (std::uint32_t i = 0; i < _frameCount; ++i)
		{
			_commandAllocators[i].clear();
		}
		_commandAllocators.clear(); _commandAllocators.shrink_to_fit();
	}
	if (_device) { _device.Reset(); }
}
#pragma endregion Constructor and Destructor
#pragma region CreateResource
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV] = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());
	_defaultHeap[DefaultHeapType::RTV]         = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());
	_defaultHeap[DefaultHeapType::DSV]         = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());

	std::map<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV] = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV] = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV] = heapCount.UAVDescCount;
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV]->Resize(heapInfoList);
	_defaultHeap[DefaultHeapType::RTV]->Resize(core::DescriptorHeapType::RTV, heapCount.RTVDescCount);
	_defaultHeap[DefaultHeapType::DSV]->Resize(core::DescriptorHeapType::DSV, heapCount.DSVDescCount);
}
std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared<directX12::RHIFrameBuffer>(shared_from_this(), renderPass, renderTargets, depthStencil));
}
std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared<directX12::RHIFrameBuffer>(shared_from_this(), renderPass, renderTarget, depthStencil));
}
std::shared_ptr<core::RHIFence> RHIDevice::CreateFence(const std::uint64_t fenceValue)
{
	// https://suzulang.com/stdshared_ptr%E3%81%A7this%E3%82%92%E4%BD%BF%E3%81%84%E3%81%9F%E3%81%84%E6%99%82%E3%81%AB%E6%B3%A8%E6%84%8F%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8/
	return std::static_pointer_cast<core::RHIFence>(std::make_shared<directX12::RHIFence>(shared_from_this(), fenceValue));
}
std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<directX12::RHICommandList>(shared_from_this(),commandAllocator));
}
std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type)
{
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<directX12::RHICommandQueue>(shared_from_this(), type));
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type)
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<directX12::RHICommandAllocator>(shared_from_this(), type));
}
std::shared_ptr<core::RHISwapchain> RHIDevice::CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR )
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<directX12::RHISwapchain>(shared_from_this(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}
std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	auto heapPtr = std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this()));
	heapPtr->Resize(heapType, maxDescriptorCount);
	return heapPtr;
}
std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo)
{
	auto heapPtr = std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this()));
	heapPtr->Resize(heapInfo);
	return heapPtr;
}
std::shared_ptr<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth)
{
	return std::static_pointer_cast<core::RHIRenderPass>(std::make_shared<directX12::RHIRenderPass>(shared_from_this(), colors, depth));
}
std::shared_ptr<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth)
{
	return std::static_pointer_cast<core::RHIRenderPass>(std::make_shared<directX12::RHIRenderPass>(shared_from_this(), color, depth));
}
std::shared_ptr<core::GPUGraphicsPipelineState> RHIDevice::CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	return std::static_pointer_cast<core::GPUGraphicsPipelineState>(std::make_shared<directX12::GPUGraphicsPipelineState>(shared_from_this(), renderPass, resourceLayout));
}
std::shared_ptr<core::GPUComputePipelineState> RHIDevice::CreateComputePipelineState(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	return std::static_pointer_cast<core::GPUComputePipelineState>(std::make_shared<directX12::GPUComputePipelineState>(shared_from_this(), resourceLayout));
}
std::shared_ptr<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits)
{
	return std::static_pointer_cast<core::RHIResourceLayout>(std::make_shared<directX12::RHIResourceLayout>(shared_from_this(), elements, samplers, constant32Bits));
}
std::shared_ptr<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return std::static_pointer_cast<core::GPUPipelineFactory>(std::make_shared<directX12::GPUPipelineFactory>(shared_from_this()));
}
std::shared_ptr<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
{
	return std::static_pointer_cast<core::GPUResourceView>(std::make_shared<directX12::GPUResourceView>(shared_from_this(), viewType, texture, customHeap));
}
std::shared_ptr<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
{
	return std::static_pointer_cast<core::GPUResourceView>(std::make_shared<directX12::GPUResourceView>(shared_from_this(), viewType, buffer, customHeap));
}
std::shared_ptr<core::GPUSampler> RHIDevice::CreateSampler(const core::SamplerInfo& samplerInfo)
{
	return std::static_pointer_cast<core::GPUSampler>(std::make_shared<directX12::GPUSampler>(shared_from_this(), samplerInfo));
}
std::shared_ptr<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData)
{
	return std::static_pointer_cast<core::GPUBuffer>(std::make_shared<directX12::GPUBuffer>(shared_from_this(), metaData));
}
std::shared_ptr<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData)
{
	return std::static_pointer_cast<core::GPUTexture>(std::make_shared<directX12::GPUTexture>(shared_from_this(), metaData));
}
std::shared_ptr<core::GPUTexture> RHIDevice::CreateTextureEmpty()
{
	return std::static_pointer_cast<core::GPUTexture>(std::make_shared<directX12::GPUTexture>(shared_from_this()));
}
//std::shared_ptr<core::GPURayTracingPipelineState> RHIDevice::CreateRayTracingPipelineState(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
//{
//	return nullptr;
//}
std::shared_ptr<core::RayTracingGeometry> RHIDevice::CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const std::shared_ptr<core::GPUBuffer>& vertexBuffer, const std::shared_ptr<core::GPUBuffer>& indexBuffer)
{
	return std::static_pointer_cast<core::RayTracingGeometry>(std::make_shared<directX12::RayTracingGeometry>(shared_from_this(), flags, vertexBuffer, indexBuffer));
}
std::shared_ptr<core::ASInstance> RHIDevice::CreateASInstance(
	const std::shared_ptr<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
	const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
	const std::uint32_t instanceMask, const core::RayTracingInstanceFlags flags)
{
	return std::static_pointer_cast<core::ASInstance>(std::make_shared<directX12::ASInstance>(shared_from_this(), blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags));
}
std::shared_ptr<core::BLASBuffer>  RHIDevice::CreateRayTracingBLASBuffer(const std::vector<std::shared_ptr<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags)
{
	return std::static_pointer_cast<core::BLASBuffer>(std::make_shared<directX12::BLASBuffer>(shared_from_this(), geometryDesc, flags));
}
std::shared_ptr<core::TLASBuffer>  RHIDevice::CreateRayTracingTLASBuffer(const std::vector<std::shared_ptr<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags)
{
	return std::static_pointer_cast<core::TLASBuffer>(std::make_shared<directX12::TLASBuffer>(shared_from_this(), asInstances, flags));
}
#pragma endregion           Create Resource Function
#pragma region Debug Function

/****************************************************************************
*                     ReportLiveObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
*  @brief     ReportLiveObjects
*  @param[in] void
*  @return 　　void
*****************************************************************************/
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
*************************************************************************//**
*  @fn        void DirectX12::CheckDXRSupport
*  @brief     Check DXRSupport
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckDXRSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
		
	if (FAILED(_device->CheckFeatureSupport(
		D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options))))) { return; }

	_isSupportedRayTracing = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_0;
	_isSupportedRenderPass = options.RenderPassesTier >= D3D12_RENDER_PASS_TIER_0;
	_isSupportedRayQuery   = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_1;
}

/****************************************************************************
*                     CheckVRSSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckVRSSupport()
*  @brief     Variable Rate Shading support
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckVRSSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS6 options = {};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options, sizeof(options))))
	{
		if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_1)
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading Tier1 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = false;
			_variableRateShadingImageTileSize    = options.ShadingRateImageTileSize;
		}
		else if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_2)
		{
			OutputDebugStringA("Gpu api: Valiable Rate Shading Tier2 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = true;
			_variableRateShadingImageTileSize = options.ShadingRateImageTileSize;
		}
	}
	else
	{
		OutputDebugStringA("GpuApi : Variable Rate Shading note supported on current gpu hardware.\n");
		_isSupportedVariableRateShadingTier1 = false;
		_isSupportedVariableRateShadingTier2 = false;
	}
}
/****************************************************************************
*                     MultiSampleQualityLevels
*************************************************************************//**
*  @fn        void DirectX12::CheckMultiSampleQualityLevels(void)
*  @brief     Multi Sample Quality Levels for 4xMsaa (Anti-Alias)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMultiSampleQualityLevels()
{
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
	msQualityLevels.Format           = DXGI_FORMAT_R16G16B16A16_FLOAT; // back buffer にした方がいいかと
	msQualityLevels.SampleCount      = 4;
	msQualityLevels.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	ThrowIfFailed(_device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

	_4xMsaaQuality = msQualityLevels.NumQualityLevels;
#ifdef _DEBUG
	assert(_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
#endif
}

/****************************************************************************
*                     CheckHDRDisplaySupport
*************************************************************************//**
*  @fn        bool DirectX12::CheckHDRDisplaySupport()
*  @brief     CheckHDRDisplaySupport()
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckHDRDisplaySupport()
{
	//bool isEnabledHDR =
	//	_backBufferFormat == DXGI_FORMAT_R16G16B16A16_FLOAT ||
	//	_backBufferFormat == DXGI_FORMAT_R10G10B10A2_UNORM;

	//if (isEnabledHDR)
	//{
	//	bool isDisplayHDR10 = false;
	//	UINT index = 0;

	//	ComPtr<IDXGIOutput> currentOutput = nullptr;
	//	ComPtr<IDXGIOutput> bestOutput    = nullptr;
	//	while (_useAdapter->EnumOutputs(index, &currentOutput) != DXGI_ERROR_NOT_FOUND)
	//	{
	//		OutputComPtr output;
	//		currentOutput.As(&output);

	//		DXGI_OUTPUT_DESC1 desc;
	//		output->GetDesc1(&desc);

	//		isDisplayHDR10 = (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
	//		++index;
	//		if (isDisplayHDR10) { break; }
	//	}

	//	if (!isDisplayHDR10)
	//	{
	//		//_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM; (shader側で選択出来るようになったら)
	//		isEnabledHDR = false;
	//	}
	//}

	//_isHDRSupport = isEnabledHDR;

}

/****************************************************************************
*                     CheckMeshShadingSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckMeshShadingSupport()
*  @brief     Mesh Shading support check
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMeshShadingSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options{};

	if (FAILED(_device->CheckFeatureSupport(
		D3D12_FEATURE_D3D12_OPTIONS7, &options, UINT(sizeof(options))))) { return; }
	_isSupportedMeshShading = options.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1;
}
#pragma endregion  Device Support Function
#pragma region Property
std::shared_ptr<core::RHICommandQueue> RHIDevice::GetCommandQueue(const core::CommandListType commandListType)
{
	return _commandQueues.at(commandListType);
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::GetCommandAllocator(const core::CommandListType commandListType, const std::uint32_t frameCount)
{
	assert(frameCount < _frameCount);
	return _commandAllocators[frameCount].at(commandListType);
}

std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
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
#pragma endregion Property
