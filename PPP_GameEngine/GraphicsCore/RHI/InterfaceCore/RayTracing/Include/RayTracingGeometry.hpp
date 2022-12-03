//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RAYTRACING_GEOMETRY_HPP
#define RAYTRACING_GEOMETRY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUBuffer; // vertex buffer and index buffer
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class RayTracingGeometry : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		RayTracingGeometryFlags    GetGeometryFlags() const noexcept { return _geometryFlags; }
		std::shared_ptr<GPUBuffer> GetVertexBuffer () const noexcept { return _vertexBuffer; }
		std::shared_ptr<GPUBuffer> GetIndexBuffer  () const noexcept { return _indexBuffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RayTracingGeometry() = default;
		~RayTracingGeometry() = default;
		RayTracingGeometry(const std::shared_ptr<RHIDevice>& device,
			const RayTracingGeometryFlags flags,
			const std::shared_ptr<GPUBuffer>& vertexBuffer,
			const std::shared_ptr<GPUBuffer>& indexBuffer);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		RayTracingGeometryFlags    _geometryFlags = RayTracingGeometryFlags::None;
		std::shared_ptr<GPUBuffer> _vertexBuffer  = nullptr;
		std::shared_ptr<GPUBuffer> _indexBuffer   = nullptr;
		std::shared_ptr<RHIDevice> _device        = nullptr;
	};
}

#endif