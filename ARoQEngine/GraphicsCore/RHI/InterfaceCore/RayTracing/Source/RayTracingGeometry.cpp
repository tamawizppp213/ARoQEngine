//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingGeometry.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RayTracingGeometry::RayTracingGeometry(const gu::SharedPointer<RHIDevice>& device,
	const RayTracingGeometryFlags flags,
	const gu::SharedPointer<GPUBuffer>& vertexBuffer,
	const gu::SharedPointer<GPUBuffer>& indexBuffer)
	: _device(device), _geometryFlags(flags), _vertexBuffer(vertexBuffer), _indexBuffer(indexBuffer)
{
	if (vertexBuffer->GetBufferType() != BufferType::Vertex)
	{
		throw std::runtime_error("Different buffer type (vertex buffer). Please set vertex buffer");
	}
	if (indexBuffer)
	{
		if (indexBuffer->GetBufferType() != BufferType::Index)
		{
			throw std::runtime_error("Different buffer type (index buffer). Please set index buffer");
		}
	}
}