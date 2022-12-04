//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RAYTRACING_GEOMETRY_HPP
#define DIRECTX12_RAYTRACING_GEOMETRY_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingGeometry.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{

	/****************************************************************************
	*				  			TemplateStruct
	*************************************************************************//**
	*  @struct     TemplateStruct
	*  @brief     temp
	*****************************************************************************/
	class RayTracingGeometry : public rhi::core::RayTracingGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const D3D12_RAYTRACING_GEOMETRY_DESC& GetDesc() const { return _geometryDesc; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RayTracingGeometry() = default;
		~RayTracingGeometry() = default;
		RayTracingGeometry(const std::shared_ptr<core::RHIDevice>& device,
			const core::RayTracingGeometryFlags flags,
			const std::shared_ptr<core::GPUBuffer>& vertexBuffer,
			const std::shared_ptr<core::GPUBuffer>& indexBuffer);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_RAYTRACING_GEOMETRY_DESC _geometryDesc = {};
	};
}
#endif