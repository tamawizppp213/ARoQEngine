//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingTLASBuffer.hpp
///             @brief  TLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RAYTRACING_TLAS_BUFFER_HPP
#define DIRECTX12_RAYTRACING_TLAS_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingTLASBuffer.hpp"
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
	*				  			TLASBuffer
	*************************************************************************//**
	*  @struct    TLASBuffer
	*  @brief     Bottom Level Acceleration Structure Buffer
	*****************************************************************************/
	class TLASBuffer : public core::TLASBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Build(const gu::SharedPointer<core::RHICommandList>& commandList) override;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		TLASBuffer() = default;
		~TLASBuffer();
		TLASBuffer(const gu::SharedPointer<core::RHIDevice>& device, 
			const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& asInstance,
			const core::BuildAccelerationStructureFlags flags,
			const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC _rayTracingASDesc = {};
	};
}
#endif