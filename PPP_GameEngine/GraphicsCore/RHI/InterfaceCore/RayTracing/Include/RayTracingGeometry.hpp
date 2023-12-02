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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
		gu::SharedPointer<GPUBuffer> GetVertexBuffer () const noexcept { return _vertexBuffer; }
		gu::SharedPointer<GPUBuffer> GetIndexBuffer  () const noexcept { return _indexBuffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RayTracingGeometry() = default;
		~RayTracingGeometry() = default;
		RayTracingGeometry(const gu::SharedPointer<RHIDevice>& device,
			const RayTracingGeometryFlags flags,
			const gu::SharedPointer<GPUBuffer>& vertexBuffer,
			const gu::SharedPointer<GPUBuffer>& indexBuffer);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		RayTracingGeometryFlags    _geometryFlags = RayTracingGeometryFlags::None;
		gu::SharedPointer<GPUBuffer> _vertexBuffer  = nullptr;
		gu::SharedPointer<GPUBuffer> _indexBuffer   = nullptr;
		gu::SharedPointer<RHIDevice> _device        = nullptr;
	};
}

#endif