//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanASInstance.hpp
///             @brief  Bind Between BLAS and TLAS
///             @author Toide Yutaro
///             @date   2022_11_25
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingASInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/RayTracing/Include/VulkanRayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
ASInstance::~ASInstance()
{

}
ASInstance::ASInstance(
	const gu::SharedPointer<core::RHIDevice>& device,
	const gu::SharedPointer<core::BLASBuffer>& blasBuffer,
	const gm::Float3x4& blasTransform,
	const std::uint32_t instanceID,
	const std::uint32_t instanceContributionToHitGroupIndex,
	const std::uint32_t instanceMask,
	const core::RayTracingInstanceFlags flags)
	: core::ASInstance(device, blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags)
{
	const auto vkBLASBuffer = gu::StaticPointerCast<vulkan::BLASBuffer>(_blasBuffer);

	std::memcpy(&_instanceDesc.transform, &blasTransform, sizeof(_instanceDesc.transform));
	_instanceDesc.instanceCustomIndex = instanceID;
	_instanceDesc.mask                = instanceMask;
	_instanceDesc.flags               = EnumConverter::Convert(flags);
	_instanceDesc.accelerationStructureReference = NULL;
	_instanceDesc.instanceShaderBindingTableRecordOffset = instanceContributionToHitGroupIndex;

}