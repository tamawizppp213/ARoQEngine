//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanRayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RAYTRACING_AS_INSTANCE_HPP
#define VULKAN_RAYTRACING_AS_INSTANCE_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingASInstance.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::vulkan
{
	class BLASBuffer;
	/****************************************************************************
	*				  			TemplateStruct
	*************************************************************************//**
	*  @struct     TemplateStruct
	*  @brief     temp
	*****************************************************************************/
	class ASInstance : public rhi::core::ASInstance
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkAccelerationStructureInstanceKHR& GetDesc() const { return _instanceDesc; }
		gu::SharedPointer<vulkan::BLASBuffer> GetVkBLASBuffer() const noexcept { return gu::StaticPointerCast<vulkan::BLASBuffer>(_blasBuffer); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ASInstance() = default;
		~ASInstance();
		ASInstance(const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::BLASBuffer>& blasBuffer,
			const gm::Float3x4& blasTransform,
			const std::uint32_t instanceID,
			const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF,
			const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkAccelerationStructureInstanceKHR _instanceDesc = {};
	};
}
#endif