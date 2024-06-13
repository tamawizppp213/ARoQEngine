//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDevice.cpp
///             @brief  Device 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanAdapter.hpp"
#include "../Include/VulkanInstance.hpp"
#include "../Include/VulkanFence.hpp"
#include "../Include/VulkanCommandQueue.hpp"
#include "../Include/VulkanCommandList.hpp"
#include "../Include/VulkanCommandAllocator.hpp"
#include "../Include/VulkanDescriptorHeap.hpp"
#include "../Include/VulkanRenderPass.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanFrameBuffer.hpp"
#include "../Include/VulkanResourceLayout.hpp"
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUPipelineState.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUResourceView.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUTexture.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUSampler.hpp"
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingASInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingTLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingGeometry.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <string>
#include <stdexcept>
#include <Windows.h>
#include <set>
#include <cassert>
#include <map>
#include <unordered_map>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26812 4100 4389)
using namespace rhi;
using namespace rhi::vulkan;
#pragma comment(lib, "vulkan-1.lib")

namespace
{
	std::unordered_map<core::CommandListType, gu::DynamicArray<float>> queuePriorities;
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDevice::~RHIDevice()
{
	Destroy();
}

RHIDevice::RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter) : 
	core::RHIDevice(adapter)
{
	CheckSupports();
	SetUpCommandQueueInfo();
	CreateLogicalDevice();
	
	const auto& gpuName    = adapter->GetName();
	const auto  deviceName = L"Device::" + unicode::ToWString(gpuName.CString());
	SetName(gu::tstring(deviceName.c_str()));
}

void RHIDevice::Destroy()
{
	_defaultHeap.Reset();

	_commandQueueInfo.Clear();

	for (auto& queuePriority : queuePriorities)
	{
		queuePriority.second.Clear();
	}
	queuePriorities.clear();

	if (_logicalDevice) 
	{ 
		vkDeviceWaitIdle(_logicalDevice);
		vkDestroyDevice(_logicalDevice, nullptr); 
		_logicalDevice = nullptr;
	} // destroy logical device

	if (_adapter) { _adapter.Reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Create Resource Function
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	gu::SortedMap<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV] = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV] = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV] = heapCount.UAVDescCount;
	_defaultHeap = gu::MakeShared<vulkan::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap->Resize(heapInfoList);
}

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared<vulkan::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTargets, depthStencil));
}

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared <vulkan::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTarget, depthStencil));
}

gu::SharedPointer<core::RHISwapchain>  RHIDevice::CreateSwapchain(const gu::SharedPointer<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR)
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<vulkan::RHISwapchain>(SharedFromThis(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}

gu::SharedPointer<core::RHISwapchain>  RHIDevice::CreateSwapchain(const core::SwapchainDesc& desc)
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<vulkan::RHISwapchain>(SharedFromThis(), desc));
}

gu::SharedPointer<core::RHICommandList> RHIDevice::CreateCommandList(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHICommandList>(gu::MakeShared<vulkan::RHICommandList>(SharedFromThis(), commandAllocator, name));
}

gu::SharedPointer<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type, const gu::tstring& name)
{
	const auto queueFamilyIndex = _commandQueueInfo[type].QueueFamilyIndex;
	const auto queueIndex = _newCreateCommandQueueIndex[type];
	if (queueIndex >= _commandQueueInfo[type].QueueCount) { OutputDebugStringA("Exceed to maxQueueCount"); return nullptr; }
	_newCreateCommandQueueIndex[type]++;
	return gu::StaticPointerCast<core::RHICommandQueue>(gu::MakeShared<vulkan::RHICommandQueue>(SharedFromThis(),type, queueFamilyIndex, queueIndex, name));
}

gu::SharedPointer<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHICommandAllocator>(gu::MakeShared<vulkan::RHICommandAllocator>(SharedFromThis(), type, _commandQueueInfo[type].QueueFamilyIndex, name));
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<vulkan::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapType, maxDescriptorCount);
	return heapPtr;
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const gu::SortedMap<core::DescriptorHeapType, size_t>& heapInfo)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<vulkan::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapInfo);
	return heapPtr;
}
gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<vulkan::RHIRenderPass>(SharedFromThis(), colors, depth));
}

gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const core::Attachment& color, const gu::Optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<vulkan::RHIRenderPass>(SharedFromThis(), color, depth));
}

gu::SharedPointer<core::GPUGraphicsPipelineState> RHIDevice::CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUGraphicsPipelineState>(gu::MakeShared<vulkan::GPUGraphicsPipelineState>(SharedFromThis(), renderPass, resourceLayout));
}

gu::SharedPointer<core::GPUComputePipelineState> RHIDevice::CreateComputePipelineState( const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUComputePipelineState>(gu::MakeShared<vulkan::GPUComputePipelineState>(SharedFromThis(), resourceLayout));
}

gu::SharedPointer<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return gu::StaticPointerCast<core::GPUPipelineFactory>(gu::MakeShared<vulkan::GPUPipelineFactory>(SharedFromThis()));
}

gu::SharedPointer<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const gu::Optional<core::Constant32Bits>& constant32Bits, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHIResourceLayout>(gu::MakeShared<vulkan::RHIResourceLayout>(SharedFromThis(), elements, samplers, constant32Bits, name));
}
gu::SharedPointer<core::GPUSampler> RHIDevice::CreateSampler(const core::SamplerInfo& samplerInfo)
{
	return gu::StaticPointerCast<core::GPUSampler>(gu::MakeShared<vulkan::GPUSampler>(SharedFromThis(), samplerInfo));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice, const gu::uint32 planeSlice, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<vulkan::GPUResourceView>(SharedFromThis(), viewType, texture, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice, const gu::uint32 planeSlice, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<vulkan::GPUResourceView>(SharedFromThis(), viewType, buffer, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::GPUBuffer>(gu::MakeShared<vulkan::GPUBuffer>(SharedFromThis(), metaData));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<vulkan::GPUTexture>(SharedFromThis(), metaData));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTextureEmpty()
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<vulkan::GPUTexture>(SharedFromThis()));
}

gu::SharedPointer<core::RHIFence> RHIDevice::CreateFence(const std::uint64_t fenceValue, const gu::tstring& name)
{
	return gu::StaticPointerCast<core::RHIFence>(gu::MakeShared<vulkan::RHIFence>(SharedFromThis(), fenceValue, name));
}

gu::SharedPointer<core::RayTracingGeometry> RHIDevice::CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer)
{
	return gu::StaticPointerCast<core::RayTracingGeometry>(gu::MakeShared<vulkan::RayTracingGeometry>(SharedFromThis(), flags, vertexBuffer, indexBuffer));
}

gu::SharedPointer<core::ASInstance> RHIDevice::CreateASInstance(
	const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
	const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
	const std::uint32_t instanceMask, const core::RayTracingInstanceFlags flags)
{
	return gu::StaticPointerCast<core::ASInstance>(gu::MakeShared<vulkan::ASInstance>(SharedFromThis(), blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags));
}

gu::SharedPointer<core::BLASBuffer>  RHIDevice::CreateRayTracingBLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::BLASBuffer>(gu::MakeShared<vulkan::BLASBuffer>(SharedFromThis(), geometryDesc, flags));
}

gu::SharedPointer<core::TLASBuffer>  RHIDevice::CreateRayTracingTLASBuffer(const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::TLASBuffer>(gu::MakeShared<vulkan::TLASBuffer>(SharedFromThis(), asInstances, flags));
}

#pragma endregion Create Resource Function
#pragma region Set Up Function
/****************************************************************************
*                     CheckSupports
****************************************************************************/
/* @fn        void RHIDevice::CheckSupports()
* 
*  @brief     Check all supports of the device and assign them to the member variable "bool". 
*             Add extension name when needed.
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckSupports()
{
	const auto vkAdapter = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(_adapter);
	
	/*-------------------------------------------------------------------
	-               Support check
	---------------------------------------------------------------------*/
	auto extensions = vkAdapter->GetExtensionProperties();
	for (const auto& extension : extensions)
	{
		/*-------------------------------------------------------------------
		-               Variable Rate Shading
		---------------------------------------------------------------------*/
		if (std::string(extension.extensionName) == VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME) { _isSupportedVariableRateShading = true; continue;}
		/*-------------------------------------------------------------------
		-               RayTracing
		---------------------------------------------------------------------*/
		if (std::string(extension.extensionName) == VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME) { _isSupportedRayTracing = true; continue;}
		if (std::string(extension.extensionName) == VK_KHR_RAY_QUERY_EXTENSION_NAME)            { _isSupportedRayQuery   = true; continue; }
		/*-------------------------------------------------------------------
		-               Draw Indirect
		---------------------------------------------------------------------*/
		if (std::string(extension.extensionName) == VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME) { _isSupportedDrawIndirected = true; continue; }
		/*-------------------------------------------------------------------
		-               MeshShading
		---------------------------------------------------------------------*/
		if (std::string(extension.extensionName) == VK_NV_MESH_SHADER_EXTENSION_NAME) { _isSupportedMeshShading = true; continue; }
	}

	VkPhysicalDeviceFeatures supports = vkAdapter->GetSupports();
	_isSupportedGeometryShader = supports.geometryShader;
	_isSupportedTessellation   = supports.tessellationShader;
}

/****************************************************************************
*                     SetUpCommandQueueInfo
****************************************************************************/
/* @fn        void RHIDevice::SetUpCommandQueueInfo()
*
*  @brief     Set command queue information (QueueFamilyIndex + QueueCount)
*             QueueFamily      : 同じ能力を持っているキューをひとまとめにしたもの.
* 　　　　　　　　　　　　　　　　　　　　　基本的には, 何でもOK + コンピュート + Copyの3種類が登録されている.
*             QueueFamilyIndex : どのキューファミリーか. 
*             QueueCount       : 対象キューファミリーに対するキューの個数 
* 
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::SetUpCommandQueueInfo()
{
	const auto vkAdapter = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(_adapter);
	auto queueFamilies   = vkAdapter->GetQueueFamilyProperties();

	/*-------------------------------------------------------------------
	-               Bit check function
	---------------------------------------------------------------------*/
	auto HasAllBits = [](auto flags, auto bits) { return (flags & bits) == bits; };
	auto HasAnyBits = [](auto flags, auto bits) { return flags & bits; };

	/*-------------------------------------------------------------------
	-     Get each queueFamilyIndex and queue count (Graphics, Compute, Copy)
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < queueFamilies.Size(); ++i)
	{
		const auto& queue = queueFamilies[i];
		// なんでもキューをGraphicsに設定する.
		if (queue.queueCount > 0 && HasAllBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT | VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT | VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT))
		{
			// キューを多く持っているやつを使用する.
			if (_commandQueueInfo.Contains(core::CommandListType::Graphics)
				&& _commandQueueInfo[core::CommandListType::Graphics].QueueCount > queue.queueCount)
			{
				continue;
			}

			_commandQueueInfo[core::CommandListType::Graphics].QueueFamilyIndex = static_cast<std::uint32_t>(i);
			_commandQueueInfo[core::CommandListType::Graphics].QueueCount       = static_cast<std::uint32_t>(queue.queueCount);
		}
		// Compute と Copy まで許容するのをComputeCommandListに
		else if (queue.queueCount > 0 && HasAllBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT | VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) && (!HasAnyBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)))
		{
			if (_commandQueueInfo.Contains(core::CommandListType::Compute)
				&& _commandQueueInfo[core::CommandListType::Compute].QueueCount > queue.queueCount)
			{
				continue;
			}

			_commandQueueInfo[core::CommandListType::Compute].QueueFamilyIndex = static_cast<std::uint32_t>(i);
			_commandQueueInfo[core::CommandListType::Compute].QueueCount = static_cast<std::uint32_t>(queue.queueCount);
		}
		// Copy のみ
		else if (queue.queueCount > 0 && HasAllBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) && (!HasAnyBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT | VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT)))
		{
			if (_commandQueueInfo.Contains(core::CommandListType::Copy)
				&& _commandQueueInfo[core::CommandListType::Copy].QueueCount > queue.queueCount)
			{
				continue;
			}

			_commandQueueInfo[core::CommandListType::Copy].QueueFamilyIndex = static_cast<std::uint32_t>(i);
			_commandQueueInfo[core::CommandListType::Copy].QueueCount       = static_cast<std::uint32_t>(queue.queueCount);
		}
		else
		{
			continue;
		}
	}
}

/****************************************************************************
*                     GetLogicalDevice
****************************************************************************/
/* @fn        bool GraphicsDeviceVulkan::CreateLogicalDevice()
* 
*  @brief     Get Logical Device
* 
*  @param[in] bool useRaytracing
* 
*  @return 　　bool
*****************************************************************************/
void RHIDevice::CreateLogicalDevice()
{
	const auto vkAdapter  = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(_adapter);
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance());

	/*-------------------------------------------------------------------
	-               Get Extension name list
	---------------------------------------------------------------------*/
	const auto& extensionNameList = vkAdapter->GetExtensionNameList();
	gu::DynamicArray<const char*> reviseExtensionNameList = {}; // convert const char* 
	for (const auto& name : extensionNameList)
	{
		// ignore
		if (name == "VK_EXT_buffer_device_address"){ continue;}
		if (name == "VK_NV_cuda_kernel_launch") { continue; }
		reviseExtensionNameList.Push(name.CString());
	}

	/*-------------------------------------------------------------------
	-               Get Extension name list
	---------------------------------------------------------------------*/
	struct ExtensionHeader
	{
		VkStructureType Type;
		void*           Next;
	};
	auto& physicalDeviceInfo = vkAdapter->GetPhysicalDeviceInfo();

	/*-------------------------------------------------------------------
	-               Default extension
	---------------------------------------------------------------------*/
	gu::DynamicArray<ExtensionHeader*> featureStructs = {};

	VkPhysicalDeviceFeatures2 features2 =
	{
		.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext    = nullptr,
		.features = physicalDeviceInfo.Features10
	};
	featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&features2));

	if (vkInstance->MeetRequiredVersion(1, 1))
	{
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features11));
	}
	if (vkInstance->MeetRequiredVersion(1, 2))
	{
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features12));
	}
	if (vkInstance->MeetRequiredVersion(1, 3))
	{
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features13));
	}

	/*-------------------------------------------------------------------
	-             Add Variable Rate Shading Extension
	---------------------------------------------------------------------*/
	if (_isSupportedVariableRateShading)
	{
		/*-------------------------------------------------------------------
		-             Add fragment shading rate features extension
		---------------------------------------------------------------------*/
		VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragmentShadingRateFeatures = 
		{
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR,
			.pNext = nullptr,
			.attachmentFragmentShadingRate = VK_TRUE
		};
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&fragmentShadingRateFeatures));
	}
	
	/*-------------------------------------------------------------------
	-            RayTracing Extenison
	---------------------------------------------------------------------*/
	if (_isSupportedRayTracing)
	{
		// activate the ray tracing extension
		// This class is used to use vkCmdTraceRaysKHR
		VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeature = 
		{
			.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR,
			.pNext              = nullptr,
			.rayTracingPipeline = VK_TRUE,
		};
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&rayTracingPipelineFeature));

		// This class is used to bulid acceleration structures
		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeature =
		{
			.sType                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
			.pNext                 = nullptr,
			.accelerationStructure = VK_TRUE
		};
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&accelerationStructureFeature));

		// Ray query
		if (_isSupportedRayQuery)
		{
			VkPhysicalDeviceRayQueryFeaturesKHR rayQueryPipelineFeature = 
			{
				.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR,
				.pNext    = nullptr,
				.rayQuery = VK_TRUE
			};
			featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&rayQueryPipelineFeature));
			rayTracingPipelineFeature.rayTraversalPrimitiveCulling = VK_TRUE;
		}
	}
	/*-------------------------------------------------------------------
	-            Support mesh shading 
	---------------------------------------------------------------------*/
	if (_isSupportedMeshShading)
	{
		VkPhysicalDeviceMeshShaderFeaturesNV meshShaderFeature = 
		{
			.sType      = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV,
			.pNext      = nullptr,
			.taskShader = true,
			.meshShader = true
		};
		featureStructs.Push(reinterpret_cast<ExtensionHeader*>(&meshShaderFeature));
	}
	
	/*-------------------------------------------------------------------
	-            Set next pointer list
	---------------------------------------------------------------------*/
	if (!featureStructs.IsEmpty())
	{
		for (size_t i = 0; i < featureStructs.Size(); ++i)
		{
			auto* header = reinterpret_cast<ExtensionHeader*>(featureStructs[i]);
			header->Next = i < featureStructs.Size() - 1 ? featureStructs[i + 1] : nullptr;
		}

		vkGetPhysicalDeviceFeatures2(vkAdapter->GetPhysicalDevice(), &features2);
	}
	
	/*-------------------------------------------------------------------
	-               Set device queue create info
	---------------------------------------------------------------------*/
	gu::DynamicArray<VkDeviceQueueCreateInfo> deviceQueueCreateInfo = {};

	for (const auto& queueInfo : _commandQueueInfo)
	{
		queuePriorities[queueInfo.Key] = gu::DynamicArray<float>(queueInfo.Value.QueueCount, 1.0f);
		_newCreateCommandQueueIndex[queueInfo.Key] = 0;

		VkDeviceQueueCreateInfo createInfo = {};
		createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;  // structure type
		createInfo.queueFamilyIndex = queueInfo.Value.QueueFamilyIndex;           // queue index
		createInfo.queueCount       = queueInfo.Value.QueueCount;                 // queue count : 1
		createInfo.pQueuePriorities = queuePriorities[queueInfo.Key].Data();      // queue property : 1.0

		deviceQueueCreateInfo.Push(createInfo);
	}

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	const VkDeviceCreateInfo deviceCreateInfo =
	{
		.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,   // structure type
		.pNext                   = &features2,
		.flags                   = 0,
		.queueCreateInfoCount    = static_cast<std::uint32_t>(deviceQueueCreateInfo.Size()),
		.pQueueCreateInfos       = deviceQueueCreateInfo.Data(),
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = nullptr,
		.enabledExtensionCount   = static_cast<UINT32>(reviseExtensionNameList.Size()),
		.ppEnabledExtensionNames = reviseExtensionNameList.Data(),
		.pEnabledFeatures        = nullptr
	};

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkResult result = vkCreateDevice(vkAdapter->GetPhysicalDevice(), &deviceCreateInfo, nullptr, &_logicalDevice);
	if (result != VK_SUCCESS) { throw std::runtime_error("failed to create logical device"); return; }
}


std::uint64_t RHIDevice::GetDeviceAddress(VkBuffer buffer) const
{
	VkBufferDeviceAddressInfo addressInfo = {};
	addressInfo.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	addressInfo.pNext  = nullptr;
	addressInfo.buffer = buffer;
	return vkGetBufferDeviceAddress(_logicalDevice, &addressInfo);
}

/****************************************************************************
*                     GetMemoryTypeIndex
****************************************************************************/
/* @fn        std::uint32_t RHIDevice::GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags)
*
*  @brief     使いたいメモリの種類を持っているmemoryIndexを返す.
*
*  @param[in] const std::uint32_t requiredTypeBits,
*  @param[in] const VkMemoryPropertyFlags& flags
* 
*
*  @return 　　bool
*****************************************************************************/
std::uint32_t RHIDevice::GetMemoryTypeIndex(std::uint32_t requiredTypeBits, const VkMemoryPropertyFlags& flags)
{
	const auto vkAdapter        = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(_adapter);
	const auto memoryProperties = vkAdapter->GetMemoryProperties();

	for (size_t index = 0; index < memoryProperties.memoryTypeCount; ++index)
	{
		if ((requiredTypeBits & 1) == 1)
		{
			if ((memoryProperties.memoryTypes[index].propertyFlags & flags) == flags)
			{
				return static_cast<uint32_t>(index);
			}
		}

		requiredTypeBits >>= 1;
	}

	throw std::runtime_error("failed to get memory type index");
}
#pragma endregion          Set Up Function
#pragma region Property
/****************************************************************************
*                     SetName
****************************************************************************/
/* @fn        void RHIDevice::SetName(const gu::tstring& name)
*
*  @brief     Set Logical device name
*
*  @param[in] const gu::tstring& name
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetName(const gu::tstring& name)
{
	SetVkResourceName(name, VK_OBJECT_TYPE_DEVICE, reinterpret_cast<std::uint64_t>(_logicalDevice));
}

/****************************************************************************
*                     SetVkResourceName
****************************************************************************/
/* @fn        void RHIDevice::SetVkResourceName(const gu::tstring& name, const VkObjectType type, const std::uint64_t objectHandle)
*
*  @brief     Set Vulkan resource name. 
*
*  @param[in] const gu::tstring& name for debug
*  @param[in] const VkObjectType type 
*  @param[in] const std::uint64_t objectPointer  
* 
*  @return    void
*****************************************************************************/
void RHIDevice::SetVkResourceName(const gu::tstring& name, const VkObjectType type, const std::uint64_t objectHandle)
{
	std::wstring stdName = std::wstring(name.CString());
	std::string utf8Name = unicode::ToUtf8String(stdName);

	/*-------------------------------------------------------------------
	-          Set Object Name Info
	---------------------------------------------------------------------*/
	const VkDebugUtilsObjectNameInfoEXT info = 
	{
		.sType        = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,  // structure type
		.pNext        = nullptr,                                             // no extension
		.objectType   = type,                                                // object type : buffer
		.objectHandle = objectHandle,                                        // vkBuffer to std::uint64_t
		.pObjectName  = utf8Name.c_str()
	};

	/*-------------------------------------------------------------------
	-          GetInstance
	---------------------------------------------------------------------*/
	const auto vkAdapter = gu::StaticPointerCast<vulkan::RHIDisplayAdapter>(GetDisplayAdapter());
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance())->GetVkInstance();

	/*-------------------------------------------------------------------
	-          SetName
	---------------------------------------------------------------------*/
	auto func = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr( vkInstance, "vkSetDebugUtilsObjectNameEXT");
	if (func(_logicalDevice, &info) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to set resource name");
	}
}

/****************************************************************************
*                     SetDefaultHeap
****************************************************************************/
/* @fn        gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
*
*  @brief     Set Default descriptor heap
*
*  @param[in] const core::DescriptorHeapType
*
*  @return    void
*****************************************************************************/
gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV:
		case core::DescriptorHeapType::SRV:
		case core::DescriptorHeapType::UAV: { return _defaultHeap;}
		default: { return nullptr;}
	}
}
#pragma endregion Property
