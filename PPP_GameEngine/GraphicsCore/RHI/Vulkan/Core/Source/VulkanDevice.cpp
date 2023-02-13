//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDevice.cpp
///             @brief  Device 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFence.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandAllocator.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDescriptorHeap.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanRenderPass.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanSwapchain.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanFrameBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanResourceLayout.hpp"
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
std::shared_ptr<core::RHICommandList> RHIDevice::CreateCommandList(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator)
{
	return std::static_pointer_cast<core::RHICommandList>(std::make_shared<vulkan::RHICommandList>(shared_from_this(), commandAllocator));
}
std::shared_ptr<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type)
{
	const auto index = _newCreateCommandQueueIndex[type];
	if (index >= _commandQueueInfo[type].QueueCount) { OutputDebugStringA("Exceed to maxQueueCount"); return nullptr; }
	_newCreateCommandQueueIndex[type]++;
	return std::static_pointer_cast<core::RHICommandQueue>(std::make_shared<vulkan::RHICommandQueue>(shared_from_this(),type,index));
}
std::shared_ptr<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type)
{
	return std::static_pointer_cast<core::RHICommandAllocator>(std::make_shared<vulkan::RHICommandAllocator>(shared_from_this(), type, _commandQueueInfo[type].QueueFamilyIndex));
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
std::shared_ptr<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits)
{
	return std::static_pointer_cast<core::RHIResourceLayout>(std::make_shared<vulkan::RHIResourceLayout>(shared_from_this(), elements, samplers, constant32Bits));
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
std::shared_ptr<core::RHIFence> RHIDevice::CreateFence(const std::uint64_t fenceValue)
{
	return std::static_pointer_cast<core::RHIFence>(std::make_shared<vulkan::RHIFence>(shared_from_this(), fenceValue));
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
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);

	/*-------------------------------------------------------------------
	-               Proceed next extension function 
	---------------------------------------------------------------------*/
	void* deviceCreateInfoNext = nullptr;
	auto AddExtension = [&](auto& extension)
	{
		extension.pNext      = deviceCreateInfoNext;
		deviceCreateInfoNext = &extension;
	};

	/*-------------------------------------------------------------------
	-               Get Extension name list
	---------------------------------------------------------------------*/
	const auto& extensionNameList = vkAdapter->GetExtensionNameList();
	std::vector<const char*> reviseExtensionNameList = {}; // convert const char* 
	for (const auto& name : extensionNameList)
	{
		if (name == "VK_EXT_buffer_device_address"){ continue;}
		reviseExtensionNameList.push_back(name.c_str());
	}


	/*-------------------------------------------------------------------
	-             Add Variable Rate Shading Extension
	---------------------------------------------------------------------*/
	if (_isSupportedVariableRateShading)
	{
//		std::map<core::ShadingRate, VkExtent2D> shadingRatePalette =
//		{
//			{ core::ShadingRate::K_1x1, { 1, 1 } },
//			{ core::ShadingRate::K_1x2, { 1, 2 } },
//			{ core::ShadingRate::K_2x1, { 2, 1 } },
//			{ core::ShadingRate::K_2x2, { 2, 2 } },
//			{ core::ShadingRate::K_2x4, { 2, 4 } },
//			{ core::ShadingRate::K_4x2, { 4, 2 } },
//			{ core::ShadingRate::K_4x4, { 4, 4 } },
//		};
//		
//#ifndef USE_STATIC_MOLTENVK
//		/*-------------------------------------------------------------------
//		-             Acquire fragment shading rates
//		---------------------------------------------------------------------*/
//		std::uint32_t fragmentShadingRatesCount = 0;
//		
//		vkGetPhysicalDeviceFragmentShadingRatesKHR(vkAdapter->GetPhysicalDevice(), &fragmentShadingRatesCount, nullptr);
//		std::vector<VkPhysicalDeviceFragmentShadingRateKHR> fragmentShadingRates(fragmentShadingRatesCount);
//		vkGetPhysicalDeviceFragmentShadingRatesKHR(vkAdapter->GetPhysicalDevice(), &fragmentShadingRatesCount, fragmentShadingRates.data());
//		
//		for (const auto& fragmentShadingRate : fragmentShadingRates)
//		{
//			VkExtent2D size = fragmentShadingRate.fragmentSize;
//			std::uint8_t shadingRate = static_cast<std::uint8_t>(((size.width >> 1) << 2) | (size.height >> 1));
//			assert((1 << ((shadingRate >> 2) & 3)) == size.width);
//			assert((1 << (shadingRate & 3)) == size.height);
//			assert(shadingRatePalette.at((core::ShadingRate)shadingRate).width  == size.width);
//			assert(shadingRatePalette.at((core::ShadingRate)shadingRate).height == size.height);
//			shadingRatePalette.erase((core::ShadingRate)shadingRate);
//		}
//#endif
//
//		assert(shadingRatePalette.empty());

		///*-------------------------------------------------------------------
		//-             Acquire fragment shading rates properties
		//---------------------------------------------------------------------*/
		//VkPhysicalDeviceFragmentShadingRatePropertiesKHR shadingRateImageProperties = {};
		//shadingRateImageProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_KHR;

		//VkPhysicalDeviceProperties2 deviceProperties = {};
		//deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		//deviceProperties.pNext = &shadingRateImageProperties;
		//vkGetPhysicalDeviceProperties2(vkAdapter->GetPhysicalDevice(), &deviceProperties);
		//assert(shadingRateImageProperties.minFragmentShadingRateAttachmentTexelSize.width == shadingRateImageProperties.maxFragmentShadingRateAttachmentTexelSize.width);
		//assert(shadingRateImageProperties.minFragmentShadingRateAttachmentTexelSize.height == shadingRateImageProperties.maxFragmentShadingRateAttachmentTexelSize.height);
		//assert(shadingRateImageProperties.minFragmentShadingRateAttachmentTexelSize.width == shadingRateImageProperties.minFragmentShadingRateAttachmentTexelSize.height);
		//assert(shadingRateImageProperties.maxFragmentShadingRateAttachmentTexelSize.width == shadingRateImageProperties.maxFragmentShadingRateAttachmentTexelSize.height);
		//_shadingRateImageTileSize = shadingRateImageProperties.maxFragmentShadingRateAttachmentTexelSize.width;
		//
		///*-------------------------------------------------------------------
		//-             Add fragment shading rate features extension
		//---------------------------------------------------------------------*/
		//VkPhysicalDeviceFragmentShadingRateFeaturesKHR fragmentShadingRateFeatures = {};
		//fragmentShadingRateFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FRAGMENT_SHADING_RATE_FEATURES_KHR;
		//fragmentShadingRateFeatures.attachmentFragmentShadingRate = VK_TRUE;
		//AddExtension(fragmentShadingRateFeatures);
	}

	/*-------------------------------------------------------------------
	-            RayTracing Extenison
	---------------------------------------------------------------------*/
	if (_isSupportedRayTracing)
	{
		VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayTracingProperties = {};
		rayTracingProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;

		VkPhysicalDeviceProperties2 deviceProperties = {};
		deviceProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		deviceProperties.pNext = &rayTracingProperties;
		vkGetPhysicalDeviceProperties2(vkAdapter->GetPhysicalDevice(), &deviceProperties);

		VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayTracingPipelineFeature = {};
		rayTracingPipelineFeature.sType              = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
		rayTracingPipelineFeature.rayTracingPipeline = VK_TRUE;

		VkPhysicalDeviceAccelerationStructureFeaturesKHR accelerationStructureFeature = {};
		accelerationStructureFeature.sType                 = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		accelerationStructureFeature.accelerationStructure = VK_TRUE;

		AddExtension(rayTracingPipelineFeature);
		AddExtension(accelerationStructureFeature);

		if (_isSupportedRayQuery)
		{
			VkPhysicalDeviceRayQueryFeaturesKHR rayQueryPipelineFeature = {};
			rayQueryPipelineFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_QUERY_FEATURES_KHR;
			rayQueryPipelineFeature.rayQuery = VK_TRUE;
			rayTracingPipelineFeature.rayTraversalPrimitiveCulling = VK_TRUE;
			AddExtension(rayQueryPipelineFeature);
		}
	}
	/*-------------------------------------------------------------------
	-            Support mesh shading 
	---------------------------------------------------------------------*/
	if (_isSupportedMeshShading)
	{
		VkPhysicalDeviceMeshShaderFeaturesNV meshShaderFeature = {};
		meshShaderFeature.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MESH_SHADER_FEATURES_NV;
		meshShaderFeature.meshShader = true;
		meshShaderFeature.taskShader = true;
		AddExtension(meshShaderFeature);
	}
	
	/*-------------------------------------------------------------------
	-               Physical device features setup
	---------------------------------------------------------------------*/
	VkPhysicalDeviceFeatures defaultFeatures = vkAdapter->GetSupports();
	_isSupportedGeometryShader = defaultFeatures.geometryShader;
	

	VkPhysicalDeviceVulkan12Features vulkan12Features = {};
	vulkan12Features.sType                    = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
	vulkan12Features.drawIndirectCount        = VK_TRUE;
#ifndef USE_STATIC_MOLTENVK
	vulkan12Features.bufferDeviceAddress      = VK_TRUE;
#endif
	vulkan12Features.timelineSemaphore        = VK_TRUE;
	vulkan12Features.shaderOutputLayer        = VK_TRUE;
	vulkan12Features.runtimeDescriptorArray   = VK_TRUE;
	vulkan12Features.samplerMirrorClampToEdge = VK_TRUE;
	vulkan12Features.descriptorIndexing       = VK_TRUE;
	vulkan12Features.samplerFilterMinmax      = VK_TRUE;
	vulkan12Features.shaderOutputViewportIndex = VK_TRUE;
	vulkan12Features.bufferDeviceAddress       = VK_TRUE;
	vulkan12Features.descriptorBindingVariableDescriptorCount = VK_TRUE;
	AddExtension(vulkan12Features);


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
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;   // structure type
	deviceCreateInfo.pQueueCreateInfos       = deviceQueueCreateInfo.data();                 // vk device queue create info
	deviceCreateInfo.queueCreateInfoCount    = static_cast<std::uint32_t>(deviceQueueCreateInfo.size());
	deviceCreateInfo.pEnabledFeatures        = &defaultFeatures;	
	deviceCreateInfo.ppEnabledExtensionNames = reviseExtensionNameList.data();
	deviceCreateInfo.enabledExtensionCount   = static_cast<UINT32>(reviseExtensionNameList.size());
	deviceCreateInfo.pNext                   = deviceCreateInfoNext;

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
std::uint32_t RHIDevice::GetMemoryTypeIndex(std::uint32_t typeBits, const VkMemoryPropertyFlags& flags)
{
	const auto vkAdapter = std::static_pointer_cast<vulkan::RHIDisplayAdapter>(_adapter);

	VkPhysicalDeviceMemoryProperties memoryProperties = {};
	vkGetPhysicalDeviceMemoryProperties(vkAdapter->GetPhysicalDevice(), &memoryProperties);
	for (size_t index = 0; index < memoryProperties.memoryTypeCount; ++index)
	{
		if ((typeBits & 1) == 1)
		{
			if ((memoryProperties.memoryTypes[index].propertyFlags & flags) == flags)
				return static_cast<uint32_t>(index);
		}

		typeBits >>= 1;
	}

	throw std::runtime_error("failed to get memory type index");
}
#pragma endregion          Set Up Function
#pragma region Property

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
