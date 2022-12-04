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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
#include <vector>
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
	class TLASBuffer : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Build(const std::shared_ptr<RHICommandList>& commandList) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<GPUBuffer> GetDest() const noexcept { return _destination; }
		std::shared_ptr<GPUBuffer> GetScratch() const noexcept { return _scratch; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		TLASBuffer() = default;
		~TLASBuffer() = default;
		TLASBuffer(const std::shared_ptr<RHIDevice>& device,
			const std::vector<std::shared_ptr<ASInstance>>& asInstance,
			const core::BuildAccelerationStructureFlags flags,
			const std::shared_ptr<RHIDescriptorHeap>& customHeap = nullptr) : _device(device), _asInstance(asInstance), _flags(flags), _customHeap(customHeap)
		{
		};
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<GPUBuffer> _destination           = nullptr;
		std::shared_ptr<GPUBuffer> _scratch               = nullptr;
		std::shared_ptr<GPUBuffer> _asInstanceDescsBuffer = nullptr;
		std::shared_ptr<GPUResourceView> _resourceView    = nullptr;
		std::shared_ptr<RHIDescriptorHeap> _customHeap    = nullptr;
		std::vector<std::shared_ptr<core::ASInstance>> _asInstance;
		BuildAccelerationStructureFlags _flags  = BuildAccelerationStructureFlags::None;
		std::shared_ptr<RHIDevice> _device = nullptr;

		/*@brief : build ray tracing acceleration structure and prepare create resource view*/
		bool _hasBuilt = false;
	};
}
#endif