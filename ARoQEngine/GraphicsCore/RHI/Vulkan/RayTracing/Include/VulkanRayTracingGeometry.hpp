//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanRayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RAYTRACING_GEOMETRY_HPP
#define VULKAN_RAYTRACING_GEOMETRY_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingGeometry.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::vulkan
{

	/****************************************************************************
	*				  			TemplateStruct
	****************************************************************************/
	/* @struct     TemplateStruct
	*  @brief     temp
	*****************************************************************************/
	class RayTracingGeometry : public rhi::core::RayTracingGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		const VkAccelerationStructureGeometryKHR& GetDesc() const noexcept { return _geometryDesc; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RayTracingGeometry() = default;
		~RayTracingGeometry() = default;
		RayTracingGeometry(const gu::SharedPointer<core::RHIDevice>& device,
			const core::RayTracingGeometryFlags flags,
			const gu::SharedPointer<core::GPUBuffer>& vertexBuffer,
			const gu::SharedPointer<core::GPUBuffer>& indexBuffer);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkAccelerationStructureGeometryKHR _geometryDesc = {};
	};
}
#endif