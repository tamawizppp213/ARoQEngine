//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Device.cpp
///             @brief  Logical Device : to use for create GPU resources 
///             @author Toide Yutaro
///             @date   2022_06_21
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
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <Windows.h>

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

RHIDevice::RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter) :
	core::RHIDevice(adapter)
{
	/*-------------------------------------------------------------------
	-                   Create Logical Device
	---------------------------------------------------------------------*/
	ThrowIfFailed(D3D12CreateDevice(
		static_pointer_cast<RHIDisplayAdapter>(_adapter)->GetAdapter().Get(),      // default adapter
		D3D_FEATURE_LEVEL_12_0, // minimum feature level
		IID_PPV_ARGS(&_device)));

	const auto& gpuName    = adapter->GetName();
	const auto  deviceName = L"Device::" + unicode::ToWString(gpuName);
	_device->SetName(deviceName.c_str());
	
	/*-------------------------------------------------------------------
	-                   Find Highest support model
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
	CheckDXRSupport();
	CheckVRSSupport();
	CheckMeshShadingSupport();
	CheckDepthBoundsTestSupport();
	CheckResourceTiers();
	SetupDisplayHDRMetaData();
}

#pragma endregion Constructor and Destructor

#pragma region Set up and Destroy

/****************************************************************************
*                     SetUpDefaultHeap
*************************************************************************//**
*  @fn        void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
*
*  @brief     Set up default descriptor heap
*
*  @param[in] const core::DefaultHeapCount& heapCount
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	/*-------------------------------------------------------------------
	-                   Create descriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV] = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());
	_defaultHeap[DefaultHeapType::RTV] = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());
	_defaultHeap[DefaultHeapType::DSV] = std::make_shared<directX12::RHIDescriptorHeap>(shared_from_this());

	/*-------------------------------------------------------------------
	-                   Set up descriptor count
	---------------------------------------------------------------------*/
	std::map<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV] = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV] = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV] = heapCount.UAVDescCount;

	/*-------------------------------------------------------------------
	-                   Allocate decsriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV]->Resize(heapInfoList);
	_defaultHeap[DefaultHeapType::RTV]->Resize(core::DescriptorHeapType::RTV, heapCount.RTVDescCount);
	_defaultHeap[DefaultHeapType::DSV]->Resize(core::DescriptorHeapType::DSV, heapCount.DSVDescCount);

}

/****************************************************************************
*                     Destoy
*************************************************************************//**
*  @fn        void RHIDevice::Destroy()
*
*  @brief     Release command resource and device
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::Destroy()
{
	/*-------------------------------------------------------------------
	-              Clear default descriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap.clear();


	/*-------------------------------------------------------------------
	-              Clear device
	---------------------------------------------------------------------*/
	if (_device) { _device.Reset(); }
}

#pragma endregion       Set up and Destroy

#pragma region CreateResource

std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared<directX12::RHIFrameBuffer>(shared_from_this(), renderPass, renderTargets, depthStencil));
}

std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared<directX12::RHIFrameBuffer>(shared_from_this(), renderPass, renderTarget, depthStencil));
}

std::shared_ptr<core::RHIFence> RHIDevice::CreateFence(const std::uint64_t fenceValue, const std::wstring& name)
{
	// https://suzulang.com/stdshared_ptr%E3%81%A7this%E3%82%92%E4%BD%BF%E3%81%84%E3%81%9F%E3%81%84%E6%99%82%E3%81%AB%E6%B3%A8%E6%84%8F%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8/
	return std::static_pointer_cast<core::RHIFence>(std::make_shared<directX12::RHIFence>(shared_from_this(), fenceValue, name));
}

std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<directX12::RHICommandList>(shared_from_this(),commandAllocator, name));
}

std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<directX12::RHICommandQueue>(shared_from_this(), type, name));
}

std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<directX12::RHICommandAllocator>(shared_from_this(), type, name));
}

std::shared_ptr<core::RHISwapchain> RHIDevice::CreateSwapchain(const std::shared_ptr<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR )
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<directX12::RHISwapchain>(shared_from_this(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}

std::shared_ptr<core::RHISwapchain>  RHIDevice::CreateSwapchain(const core::SwapchainDesc& desc)
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<directX12::RHISwapchain>(shared_from_this(), desc));
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

std::shared_ptr<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHIResourceLayout>(std::make_shared<directX12::RHIResourceLayout>(shared_from_this(), elements, samplers, constant32Bits, name));
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

std::shared_ptr<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData, const std::wstring& name)
{
	return std::static_pointer_cast<core::GPUBuffer>(std::make_shared<directX12::GPUBuffer>(shared_from_this(), metaData, name));
}

std::shared_ptr<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name)
{
	return std::static_pointer_cast<core::GPUTexture>(std::make_shared<directX12::GPUTexture>(shared_from_this(), metaData, name));
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
* 
*  @brief     ReportLiveObjects
* 
*  @param[in] void
* 
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
* 
*  @brief     Variable Rate Shading support
* 
*  @param[in] void
* 
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
* 
*  @brief     Multi Sample Quality Levels for 4xMsaa (Anti-Alias)
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMultiSampleQualityLevels(const core::PixelFormat format)
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
*  @fn        void DirectX12::CheckHDRDisplaySupport()
* 
*  @brief     CheckHDRDisplaySupport()
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::SetupDisplayHDRMetaData()
{
	/*-------------------------------------------------------------------
	-               Get display adapter
	---------------------------------------------------------------------*/
	const auto adapter = std::static_pointer_cast<directX12::RHIDisplayAdapter>(_adapter);
	auto dxAdapter     = adapter->GetAdapter();

	ComPtr<IDXGIOutput> output = nullptr;

	/*-------------------------------------------------------------------
	-              Search EnableHDR output
	---------------------------------------------------------------------*/
	for(std::uint32_t i = 0; dxAdapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
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
*************************************************************************//**
*  @fn        void DirectX12::CheckMeshShadingSupport()
* 
*  @brief     Mesh Shading support check
* 
*  @param[in] void
* 
*  @return 　　void
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
*                     DepthBoundsTestSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckDepthBoundsTestSupport()
*
*  @brief      深度値が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させ, 範囲外ならば該当ピクセルを早期棄却する方法
		　　　　 Deferred Renderingにおけるライトのaccumulation, Deferred RenderingにおけるCascaded Shadow Map, 被写界深度エフェクト, 遠景描画等に使用可能 
*              https://microsoft.github.io/DirectX-Specs/d3d/DepthBoundsTest.html
*              https://shikihuiku.wordpress.com/tag/depthboundstest/
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckDepthBoundsTestSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS2 options{};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &options, sizeof(options))))
	{
		_isSupportedDepthBoundsTest = options.DepthBoundsTestSupported;
	}
	else
	{
		_isSupportedDepthBoundsTest = false;
	}
}

/****************************************************************************
*                     CheckResourceTiers
*************************************************************************//**
*  @fn        void RHIDevice::CheckResourceTiers()
*
*  @brief     パイプラインで使用可能なリソースの上限値を確認するために使用する
* 　　　　　　　　
*  @param[in] void
*
*  @return 　　void
*  
*  @details    大きく異なるのは以下の点です
*             1. CBV : Tier 1, 2は14まで. Tier3 はDescripterHeapの最大数
*             2. SRV : Tier 1は128まで. Tier2, 3はDescripterHeapの最大数
*             3. UAV : Tier1は機能レベル 11.1+以上で64, それ以外で8, Tier2は64, Tier3はDescripterHeapの最大数
*             4. Sampler : Tier1は16, それ以外で2048
*             5. ヒープ内のDescripterの最大数 Tier1, 2は1,000,000、Tier3は無制限
* 　　　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support
* 
* 　　　　　　　　HeapTierの方では, バッファー, RenderTargetとDepthStencil, TargetStencil, 深度ステンシルテクスチャのレンダリングを同一ヒープで使用できるかを調べます
* 　　　　　　　　Tier1が排他, Tier2が混在可能です.
* 　　　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
*****************************************************************************/
void RHIDevice::CheckResourceTiers()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};
	
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_resourceBindingTier = options.ResourceBindingTier;
	_resourceHeapTier    = options.ResourceHeapTier;
}

/****************************************************************************
*                     FindHighestFeatureLevel
*************************************************************************//**
*  @fn        void RHIDevice::FindHighestFeatureLevel()
*
*  @brief     Set up max feature level.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::FindHighestFeatureLevel()
{
	const D3D_FEATURE_LEVEL featureLevels[] =
	{
#if D3D12_CORE_ENABLED
		D3D_FEATURE_LEVEL_12_2,
#endif
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
*                     FindHighestShaderModel
*************************************************************************//**
*  @fn        void RHIDevice::FindHighestShaderModel()
*
*  @brief     Set up max shader model.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::FindHighestShaderModel()
{
	const D3D_SHADER_MODEL shaderModels[] =
	{
#if D3D12_CORE_ENABLED
		D3D_HIGHEST_SHADER_MODEL,
		D3D_SHADER_MODEL_6_7,
		D3D_SHADER_MODEL_6_6,
#endif
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
}
#pragma endregion  Device Support Function

#pragma region Property
/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void RHIDevice::SetName(const std::wstring& name)
*
*  @brief     Set Logical device name
*
*  @param[in] const std::wstring& name
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetName(const std::wstring& name)
{
	_device->SetName(name.c_str());
}

/****************************************************************************
*                     GetDefaultHeap
*************************************************************************//**
*  @fn        std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
*
*  @brief     Return descriptor heap (CBV, SRV, UAV, RTV, DSV)
*
*  @param[in] const core::DefaultHeapType
*
*  @return    std::shared_ptr<core::RHIDescriptorHeap>
*****************************************************************************/
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
