//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanASInstance.hpp
///             @brief  Bind Between BLAS and TLAS
///             @author Toide Yutaro
///             @date   2022_11_25
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingGeometry.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
BLASBuffer::BLASBuffer(const std::shared_ptr<core::RHIDevice>& device,
	//const std::shared_ptr<core::GPUBuffer>& source,
	const std::vector<std::shared_ptr<core::RayTracingGeometry>>& geometryDesc,
	const core::BuildAccelerationStructureFlags flags)
	: core::BLASBuffer(device, geometryDesc, flags)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);

	/*-------------------------------------------------------------------
	-         Push backs vulkan GeometryDesc
	---------------------------------------------------------------------*/
	std::vector<VkAccelerationStructureGeometryKHR> vkGeometryDesc = {};
	for (const auto& desc : _geometryDescs)
	{
		vkGeometryDesc.emplace_back(std::static_pointer_cast<vulkan::RayTracingGeometry>(desc)->GetDesc());
	}

	/*-------------------------------------------------------------------
	-         Set up acceleration structure inputs
	---------------------------------------------------------------------*/
	VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
	buildInfo.sType         = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
	buildInfo.type          = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
	buildInfo.flags         = EnumConverter::Convert(flags);
	buildInfo.geometryCount = static_cast<std::uint32_t>(vkGeometryDesc.size());
	buildInfo.pGeometries   = vkGeometryDesc.data();
	buildInfo.pNext         = nullptr;
	// –¢ŽÀ‘•

}
#pragma endregion Constructor and Destructor