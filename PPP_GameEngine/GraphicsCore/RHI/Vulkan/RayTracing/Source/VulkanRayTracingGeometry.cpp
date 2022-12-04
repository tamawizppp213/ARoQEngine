//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingGeometry.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RayTracingGeometry::RayTracingGeometry(const std::shared_ptr<core::RHIDevice>& device,
	const core::RayTracingGeometryFlags flags,
	const std::shared_ptr<core::GPUBuffer>& vertexBuffer,
	const std::shared_ptr<core::GPUBuffer>& indexBuffer) :
	rhi::core::RayTracingGeometry(device, flags, vertexBuffer, indexBuffer)
{
	const auto vkDevice       = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	const auto vkVertexBuffer = std::static_pointer_cast<vulkan::GPUBuffer>(_vertexBuffer);
	const auto vkIndexBuffer  = std::static_pointer_cast<vulkan::GPUBuffer>(_indexBuffer);

	/*-------------------------------------------------------------------
	-         Set up geometry desc
	---------------------------------------------------------------------*/
	_geometryDesc.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
	_geometryDesc.pNext = nullptr;
	_geometryDesc.flags = EnumConverter::Convert(flags);
	_geometryDesc.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR; // Œã‚Å•ÏX‚·‚é

	// set vkAccelerationStructureGeometry
	auto& triangles = _geometryDesc.geometry.triangles;
	triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
	triangles.pNext = nullptr;
	triangles.vertexStride = vkVertexBuffer->GetElementByteSize();
	triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
	triangles.maxVertex    = static_cast<std::uint32_t>(vkVertexBuffer->GetElementCount());
	triangles.indexType    = VK_INDEX_TYPE_UINT32;
	triangles.vertexData.deviceAddress = vkDevice->GetDeviceAddress(vkVertexBuffer->GetBuffer());
	triangles.indexData .deviceAddress = vkDevice->GetDeviceAddress(vkIndexBuffer->GetBuffer());
	
}