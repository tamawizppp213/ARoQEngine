//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingBLASBuffer.hpp
///             @brief  Bottom Level Acceleration Structure Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RAYTRACING_BLAS_BUFFER_HPP
#define RAYTRACING_BLAS_BUFFER_HPP

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
	class GPUBuffer;
	class RayTracingGeometry;
	/****************************************************************************
	*				  			BLASBuffer
	*************************************************************************//**
	*  @struct    BLASBuffer
	*  @brief     BLASBuffer
	*****************************************************************************/
	class BLASBuffer : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Build(const gu::SharedPointer<RHICommandList>& commandList) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		//gu::SharedPointer<GPUBuffer> GetSource () const noexcept { return _source; }
		gu::SharedPointer<GPUBuffer> GetDest   () const noexcept { return _destination; }
		gu::SharedPointer<GPUBuffer> GetScratch() const noexcept { return _scratch; }
		gu::SharedPointer<RayTracingGeometry> GetGeometryDesc(const std::uint64_t index = 0) const noexcept { return _geometryDescs[index]; }
		std::uint64_t GetGeometryDescSize() const noexcept { return _geometryDescs.Size(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		BLASBuffer() = default;
		~BLASBuffer() = default;
		BLASBuffer(const gu::SharedPointer<RHIDevice>& device, 
			const gu::DynamicArray<gu::SharedPointer<RayTracingGeometry>>& geometryDesc,
			const core::BuildAccelerationStructureFlags flags) : _device(device),  _geometryDescs(geometryDesc), _flags(flags) { };
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		//gu::SharedPointer<GPUBuffer> _source        = nullptr;Å@å„Ç≈ïKóvÇ…Ç»ÇÈÇ©Ç‡
		gu::SharedPointer<GPUBuffer> _destination   = nullptr;
		gu::SharedPointer<GPUBuffer> _scratch       = nullptr;
		BuildAccelerationStructureFlags _flags    = BuildAccelerationStructureFlags::None;
		gu::DynamicArray<gu::SharedPointer<RayTracingGeometry>> _geometryDescs = {};
		gu::SharedPointer<RHIDevice> _device        = nullptr;

		bool _hasBuilt = false;
	};
}
#endif