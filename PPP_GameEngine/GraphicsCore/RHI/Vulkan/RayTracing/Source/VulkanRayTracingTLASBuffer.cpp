//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingBLASBuffer.hpp
///             @brief  BLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingTLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingASInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingGeometry.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandList.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
TLASBuffer::~TLASBuffer()
{

}
TLASBuffer::TLASBuffer(const gu::SharedPointer<core::RHIDevice>& device,
	const std::vector<gu::SharedPointer<core::ASInstance>>& asInstance,
	const core::BuildAccelerationStructureFlags flags)
	: core::TLASBuffer(device, asInstance, flags)
{
}