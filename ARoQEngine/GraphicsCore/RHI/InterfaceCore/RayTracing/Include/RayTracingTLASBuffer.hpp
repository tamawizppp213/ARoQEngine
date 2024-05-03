//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingTLASBuffer.hpp
///             @brief  Top Level Acceleration Structure Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RAYTRACING_TLAS_BUFFER_HPP
#define RAYTRACING_TLAS_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class ASInstance;
	class RHIDescriptorHeap;
	class GPUBuffer;
	class GPUResourceView;
	class RayTracingGeometry;
	/****************************************************************************
	*				  			TLASBuffer
	*************************************************************************//**
	*  @struct    TLASBuffer
	*  @brief     TLASBuffer
	*****************************************************************************/
	class TLASBuffer : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Build(const gu::SharedPointer<RHICommandList>& commandList) = 0;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		gu::SharedPointer<GPUBuffer> GetDest() const noexcept { return _destination; }
		gu::SharedPointer<GPUBuffer> GetScratch() const noexcept { return _scratch; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		TLASBuffer() = default;
		~TLASBuffer() = default;
		TLASBuffer(const gu::SharedPointer<RHIDevice>& device,
			const gu::DynamicArray<gu::SharedPointer<ASInstance>>& asInstance,
			const core::BuildAccelerationStructureFlags flags,
			const gu::SharedPointer<RHIDescriptorHeap>& customHeap = nullptr) : _device(device), _asInstance(asInstance), _flags(flags), _customHeap(customHeap)
		{
		};
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		gu::SharedPointer<GPUBuffer> _destination           = nullptr;
		gu::SharedPointer<GPUBuffer> _scratch               = nullptr;
		gu::SharedPointer<GPUBuffer> _asInstanceDescsBuffer = nullptr;
		gu::SharedPointer<GPUResourceView> _resourceView    = nullptr;
		gu::SharedPointer<RHIDescriptorHeap> _customHeap    = nullptr;
		gu::DynamicArray<gu::SharedPointer<core::ASInstance>> _asInstance;
		BuildAccelerationStructureFlags _flags  = BuildAccelerationStructureFlags::None;
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*@brief : build ray tracing acceleration structure and prepare create resource view*/
		bool _hasBuilt = false;
	};
}
#endif