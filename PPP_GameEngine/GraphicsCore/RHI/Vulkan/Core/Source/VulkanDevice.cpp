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
	std::unordered_map<core::CommandListType, std::vector<float>> queuePriorities;
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDevice::~RHIDevice()
{
	Destroy();
}

RHIDevice::RHIDevice(const std::shared_ptr<core::RHIDisplayAdapter>& adapter) : 
	core::RHIDevice(adapter)
{
	CheckSupports();
	SetUpCommandQueueInfo();
	CreateLogicalDevice();
	
	const auto& gpuName    = adapter->GetName();
	const auto  deviceName = L"Device::" + unicode::ToWString(gpuName);
	SetName(deviceName);
}

void RHIDevice::Destroy()
{
	_defaultHeap.reset();

	_commandQueueInfo.clear();

	for (auto& queuePriority : queuePriorities)
	{
		queuePriority.second.clear();
	}
	queuePriorities.clear();

	if (_logicalDevice) 
	{ 
		vkDeviceWaitIdle(_logicalDevice);
		vkDestroyDevice(_logicalDevice, nullptr); 
		_logicalDevice = nullptr;
	} // destroy logical device

	if (_adapter) { _adapter.reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Create Resource Function
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	std::map<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV] = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV] = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV] = heapCount.UAVDescCount;
	_defaultHeap = std::make_shared<vulkan::RHIDescriptorHeap>(shared_from_this());
	_defaultHeap->Resize(heapInfoList);
}

std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared<vulkan::RHIFrameBuffer>(shared_from_this(), renderPass, renderTargets, depthStencil));
}

std::shared_ptr<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
{
	return std::static_pointer_cast<core::RHIFrameBuffer>(std::make_shared <vulkan::RHIFrameBuffer>(shared_from_this(), renderPass, renderTarget, depthStencil));
}

std::shared_ptr<core::RHISwapchain>  RHIDevice::CreateSwapchain(const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const std::uint32_t vsync, const bool isValidHDR)
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<vulkan::RHISwapchain>(shared_from_this(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}

std::shared_ptr<core::RHISwapchain>  RHIDevice::CreateSwapchain(const core::SwapchainDesc& desc)
{
	return std::static_pointer_cast<core::RHISwapchain>(std::make_shared<vulkan::RHISwapchain>(shared_from_this(), desc));
}

std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<vulkan::RHICommandList>(shared_from_this(), commandAllocator, name));
}

std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type, const std::wstring& name)
{
	const auto queueFamilyIndex = _commandQueueInfo[type].QueueFamilyIndex;
	const auto queueIndex = _newCreateCommandQueueIndex[type];
	if (queueIndex >= _commandQueueInfo[type].QueueCount) { OutputDebugStringA("Exceed to maxQueueCount"); return nullptr; }
	_newCreateCommandQueueIndex[type]++;
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<vulkan::RHICommandQueue>(shared_from_this(),type, queueFamilyIndex, queueIndex, name));
}

std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<vulkan::RHICommandAllocator>(shared_from_this(), type, _commandQueueInfo[type].QueueFamilyIndex, name));
}

std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	auto heapPtr = std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<vulkan::RHIDescriptorHeap>(shared_from_this()));
	heapPtr->Resize(heapType, maxDescriptorCount);
	return heapPtr;
}

std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo)
{
	auto heapPtr = std::static_pointer_cast<core::RHIDescriptorHeap>(std::make_shared<vulkan::RHIDescriptorHeap>(shared_from_this()));
	heapPtr->Resize(heapInfo);
	return heapPtr;
}
std::shared_ptr<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth)
{
	return std::static_pointer_cast<core::RHIRenderPass>(std::make_shared<vulkan::RHIRenderPass>(shared_from_this(), colors, depth));
}

std::shared_ptr<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth)
{
	return std::static_pointer_cast<core::RHIRenderPass>(std::make_shared<vulkan::RHIRenderPass>(shared_from_this(), color, depth));
}

std::shared_ptr<core::GPUGraphicsPipelineState> RHIDevice::CreateGraphicPipelineState(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	return std::static_pointer_cast<core::GPUGraphicsPipelineState>(std::make_shared<vulkan::GPUGraphicsPipelineState>(shared_from_this(), renderPass, resourceLayout));
}

std::shared_ptr<core::GPUComputePipelineState> RHIDevice::CreateComputePipelineState( const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	return std::static_pointer_cast<core::GPUComputePipelineState>(std::make_shared<vulkan::GPUComputePipelineState>(shared_from_this(), resourceLayout));
}

std::shared_ptr<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return std::static_pointer_cast<core::GPUPipelineFactory>(std::make_shared<vulkan::GPUPipelineFactory>(shared_from_this()));
}

std::shared_ptr<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHIResourceLayout>(std::make_shared<vulkan::RHIResourceLayout>(shared_from_this(), elements, samplers, constant32Bits, name));
}
std::shared_ptr<core::GPUSampler> RHIDevice::CreateSampler(const core::SamplerInfo& samplerInfo)
{
	return std::static_pointer_cast<core::GPUSampler>(std::make_shared<vulkan::GPUSampler>(shared_from_this(), samplerInfo));
}

std::shared_ptr<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
{
	return std::static_pointer_cast<core::GPUResourceView>(std::make_shared<vulkan::GPUResourceView>(shared_from_this(), viewType, texture, customHeap));
}

std::shared_ptr<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
{
	return std::static_pointer_cast<core::GPUResourceView>(std::make_shared<vulkan::GPUResourceView>(shared_from_this(), viewType, buffer, customHeap));
}

std::shared_ptr<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData, const std::wstring& name)
{
	return std::static_pointer_cast<core::GPUBuffer>(std::make_shared<vulkan::GPUBuffer>(shared_from_this(), metaData));
}

std::shared_ptr<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name)
{
	return std::static_pointer_cast<core::GPUTexture>(std::make_shared<vulkan::GPUTexture>(shared_from_this(), metaData));
}

std::shared_ptr<core::GPUTexture> RHIDevice::CreateTextureEmpty()
{
	return std::static_pointer_cast<core::GPUTexture>(std::make_shared<vulkan::GPUTexture>(shared_from_this()));
}

std::shared_ptr<core::RHIFence> RHIDevice::CreateFence(const std::uint64_t fenceValue, const std::wstring& name)
{
	return std::static_pointer_cast<core::RHIFence>(std::make_shared<vulkan::RHIFence>(shared_from_this(), fenceValue, name));
}

std::shared_ptr<core::RayTracingGeometry> RHIDevice::CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const std::shared_ptr<core::GPUBuffer>& vertexBuffer, const std::shared_ptr<core::GPUBuffer>& indexBuffer)
{
	return std::static_pointer_cast<core::RayTracingGeometry>(std::make_shared<vulkan::RayTracingGeometry>(shared_from_this(), flags, vertexBuffer, indexBuffer));
}

std::shared_ptr<core::ASInstance> RHIDevice::CreateASInstance(
	const std::shared_ptr<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
	const std::uint32_t instanceID, const std::uint32_t instanceContributionToHitGroupIndex,
	const std::uint32_t instanceMask, const core::RayTracingInstanceFlags flags)
{
	return std::static_pointer_cast<core::ASInstance>(std::make_shared<vulkan::ASInstance>(shared_from_this(), blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags));
}

std::shared_ptr<core::BLASBuffer>  RHIDevice::CreateRayTracingBLASBuffer(const std::vector<std::shared_ptr<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags)
{
	return std::static_pointer_cast<core::BLASBuffer>(std::make_shared<vulkan::BLASBuffer>(shared_from_this(), geometryDesc, flags));
}

std::shared_ptr<core::TLASBuffer>  RHIDevice::CreateRayTracingTLASBuffer(const std::vector<std::shared_ptr<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags)
{
	return std::static_pointer_cast<core::TLASBuffer>(std::make_shared<vulkan::TLASBuffer>(shared_from_this(), asInstances, flags));
}

#pragma endregion Create Resource Function
#pragma region Set Up Function
/****************************************************************************
*                     CheckSupports
*************************************************************************//**
*  @fn        void RHIDevice::CheckSupports()
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
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);
	
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
*************************************************************************//**
*  @fn        void RHIDevice::SetUpCommandQueueInfo()
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
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);
	auto queueFamilies   = vkAdapter->GetQueueFamilyProperties();

	/*-------------------------------------------------------------------
	-               Bit check function
	---------------------------------------------------------------------*/
	auto HasAllBits = [](auto flags, auto bits) { return (flags & bits) == bits; };
	auto HasAnyBits = [](auto flags, auto bits) { return flags & bits; };

	/*-------------------------------------------------------------------
	-     Get each queueFamilyIndex and queue count (Graphics, Compute, Copy)
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < queueFamilies.size(); ++i)
	{
		const auto& queue = queueFamilies[i];
		// なんでもキューをGraphicsに設定する.
		if (queue.queueCount > 0 && HasAllBits(queue.queueFlags, VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT | VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT | VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT))
		{
			// キューを多く持っているやつを使用する.
			if (_commandQueueInfo.contains(core::CommandListType::Graphics)
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
			if (_commandQueueInfo.contains(core::CommandListType::Compute)
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
			if (_commandQueueInfo.contains(core::CommandListType::Copy)
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
*************************************************************************//**
*  @fn        bool GraphicsDeviceVulkan::CreateLogicalDevice()
* 
*  @brief     Get Logical Device
* 
*  @param[in] bool useRaytracing
* 
*  @return 　　bool
*****************************************************************************/
void RHIDevice::CreateLogicalDevice()
{
	const auto vkAdapter  = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);
	const auto vkInstance = static_cast<vulkan::RHIInstance*>(vkAdapter->GetInstance());

	/*-------------------------------------------------------------------
	-               Get Extension name list
	---------------------------------------------------------------------*/
	const auto& extensionNameList = vkAdapter->GetExtensionNameList();
	std::vector<const char*> reviseExtensionNameList = {}; // convert const char* 
	for (const auto& name : extensionNameList)
	{
		// ignore
		if (name == "VK_EXT_buffer_device_address"){ continue;}
		if (name == "VK_NV_cuda_kernel_launch") { continue; }
		reviseExtensionNameList.push_back(name.c_str());
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
	std::vector<ExtensionHeader*> featureStructs = {};

	VkPhysicalDeviceFeatures2 features2 =
	{
		.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2,
		.pNext    = nullptr,
		.features = physicalDeviceInfo.Features10
	};
	featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&features2));

	if (vkInstance->MeetRequiredVersion(1, 1))
	{
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features11));
	}
	if (vkInstance->MeetRequiredVersion(1, 2))
	{
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features12));
	}
	if (vkInstance->MeetRequiredVersion(1, 3))
	{
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&physicalDeviceInfo.Features13));
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
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&fragmentShadingRateFeatures));
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
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&rayTracingPipelineFeature));

		// This class is used to bulid acceleration structures
		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeature =
		{
			.sType                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR,
			.pNext                 = nullptr,
			.accelerationStructure = VK_TRUE
		};
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&accelerationStructureFeature));

		// Ray query
		if (_isSupportedRayQuery)
		{
			VkPhysicalDeviceRayQueryFeaturesKHR rayQueryPipelineFeature = 
			{
				.sType    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR,
				.pNext    = nullptr,
				.rayQuery = VK_TRUE
			};
			featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&rayQueryPipelineFeature));
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
		featureStructs.push_back(reinterpret_cast<ExtensionHeader*>(&meshShaderFeature));
	}
	
	/*-------------------------------------------------------------------
	-            Set next pointer list
	---------------------------------------------------------------------*/
	if (!featureStructs.empty())
	{
		for (size_t i = 0; i < featureStructs.size(); ++i)
		{
			auto* header = reinterpret_cast<ExtensionHeader*>(featureStructs[i]);
			header->Next = i < featureStructs.size() - 1 ? featureStructs[i + 1] : nullptr;
		}

		vkGetPhysicalDeviceFeatures2(vkAdapter->GetPhysicalDevice(), &features2);
	}
	
	/*-------------------------------------------------------------------
	-               Set device queue create info
	---------------------------------------------------------------------*/
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfo = {};

	for (const auto& queueInfo : _commandQueueInfo)
	{
		queuePriorities[queueInfo.first] = std::vector<float>(queueInfo.second.QueueCount, 1.0f);
		_newCreateCommandQueueIndex[queueInfo.first] = 0;

		VkDeviceQueueCreateInfo& createInfo = deviceQueueCreateInfo.emplace_back();
		createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;  // structure type
		createInfo.queueFamilyIndex = queueInfo.second.QueueFamilyIndex;           // queue index
		createInfo.queueCount       = queueInfo.second.QueueCount;                 // queue count : 1
		createInfo.pQueuePriorities = queuePriorities[queueInfo.first].data();      // queue property : 1.0
	}

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	const VkDeviceCreateInfo deviceCreateInfo =
	{
		.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,   // structure type
		.pNext                   = &features2,
		.flags                   = 0,
		.queueCreateInfoCount    = static_cast<std::uint32_t>(deviceQueueCreateInfo.size()),
		.pQueueCreateInfos       = deviceQueueCreateInfo.data(),
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = nullptr,
		.enabledExtensionCount   = static_cast<UINT32>(reviseExtensionNameList.size()),
		.ppEnabledExtensionNames = reviseExtensionNameList.data(),
		.pEnabledFeatures        = nullptr
	};

	/*-------------------------------------------------------------------
	-               Set Device Create Info
	---------------------------------------------------------------------*/
	VkResult result = vkCreateDevice(vkAdapter->GetPhysicalDevice(), &deviceCreateInfo, nullptr, &_logicalDevice);
	if (result != VK_SUCCESS) { throw std::runtime_error("failed to create logical device"); return; }
}


std::uint64_t RHIDevice::GetDeviceAddress(VkBuffer buffer)
{
	VkBufferDeviceAddressInfo addressInfo = {};
	addressInfo.sType  = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
	addressInfo.pNext  = nullptr;
	addressInfo.buffer = buffer;
	return vkGetBufferDeviceAddress(_logicalDevice, &addressInfo);
}

/****************************************************************************
*                     GetMemoryTypeIndex
*************************************************************************//**
*  @fn        std::uint32_t RHIDevice::GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags)
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
	const auto vkAdapter        = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);
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
	SetVkResourceName(name, VK_OBJECT_TYPE_DEVICE, reinterpret_cast<std::uint64_t>(_logicalDevice));
}

/****************************************************************************
*                     SetVkResourceName
*************************************************************************//**
*  @fn        void RHIDevice::SetVkResourceName(const std::wstring& name, const VkObjectType type, const std::uint64_t objectHandle)
*
*  @brief     Set Vulkan resource name. 
*
*  @param[in] const std::wstring& name for debug
*  @param[in] const VkObjectType type 
*  @param[in] const std::uint64_t objectPointer  
* 
*  @return    void
*****************************************************************************/
void RHIDevice::SetVkResourceName(const std::wstring& name, const VkObjectType type, const std::uint64_t objectHandle)
{
	std::string utf8Name = unicode::ToUtf8String(name);

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
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(GetDisplayAdapter());
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
*************************************************************************//**
*  @fn        std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
*
*  @brief     Set Default descriptor heap
*
*  @param[in] const core::DescriptorHeapType
*
*  @return    void
*****************************************************************************/
std::shared_ptr<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
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
