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
	class GPUBuffer;
	class RayTracingGeometry;
	/****************************************************************************
	*				  			BLASBuffer
	*************************************************************************//**
	*  @struct    BLASBuffer
	*  @brief     BLASBuffer
	*****************************************************************************/
	class BLASBuffer : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Build(const std::shared_ptr<RHICommandList>& commandList) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		//std::shared_ptr<GPUBuffer> GetSource () const noexcept { return _source; }
		std::shared_ptr<GPUBuffer> GetDest   () const noexcept { return _destination; }
		std::shared_ptr<GPUBuffer> GetScratch() const noexcept { return _scratch; }
		std::shared_ptr<RayTracingGeometry> GetGeometryDesc(const std::uint64_t index = 0) const noexcept { return _geometryDescs[index]; }
		std::uint64_t GetGeometryDescSize() const noexcept { return _geometryDescs.size(); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		BLASBuffer() = default;
		~BLASBuffer() = default;
		BLASBuffer(const std::shared_ptr<RHIDevice>& device, 
			const std::vector<std::shared_ptr<RayTracingGeometry>>& geometryDesc,
			const core::BuildAccelerationStructureFlags flags) : _device(device),  _geometryDescs(geometryDesc), _flags(flags) { };
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		//std::shared_ptr<GPUBuffer> _source        = nullptr;Å@å„Ç≈ïKóvÇ…Ç»ÇÈÇ©Ç‡
		std::shared_ptr<GPUBuffer> _destination   = nullptr;
		std::shared_ptr<GPUBuffer> _scratch       = nullptr;
		BuildAccelerationStructureFlags _flags    = BuildAccelerationStructureFlags::None;
		std::vector<std::shared_ptr<RayTracingGeometry>> _geometryDescs = {};
		std::shared_ptr<RHIDevice> _device        = nullptr;

		bool _hasBuilt = false;
	};
}
#endif