//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanBLASBuffer.hpp
///             @brief  RayTracing Bottom Level Acceleration Structure
///             @author Toide Yutaro
///             @date   2022_11_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RAYTRACING_BLAS_BUFFER_HPP
#define VULKAN_RAYTRACING_BLAS_BUFFER_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingBLASBuffer.hpp"
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
	*************************************************************************//**
	*  @struct     TemplateStruct
	*  @brief     temp
	*****************************************************************************/
	class BLASBuffer : public rhi::core::BLASBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Build([[maybe_unused]]const gu::SharedPointer<core::RHICommandList>& commandList) override {};
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		BLASBuffer() = default;
		~BLASBuffer() = default;
		BLASBuffer(const gu::SharedPointer<core::RHIDevice>& device,
			//const gu::SharedPointer<core::GPUBuffer>& source,
			const std::vector<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc,
			const core::BuildAccelerationStructureFlags flags);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/

	};
}
#endif