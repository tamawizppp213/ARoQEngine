//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingShaderTable.hpp
///             @brief  RayTracing shader table
///             @author Toide Yutaro
///             @date   2023_01_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RAYTRACING_SHADER_TABLE_HPP
#define RAYTRACING_SHADER_TABLE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUBuffer;
	class GPUShaderState;
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class RayTracingShaderTable : public NonCopyable
	{
	protected:
		using DevicePtr = gu::SharedPointer<rhi::core::RHIDevice>;
		using ShaderPtr = gu::SharedPointer<rhi::core::GPUShaderState>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		gu::SharedPointer<GPUBuffer> GetBuffer() const noexcept { return _buffer; }

		std::int32_t GetRayGenerationShaderCount() const { return _rayGenerationShaderCount; }

		std::int32_t GetMissShaderCount() const { return _missShaderCount; }

		std::int32_t GetHitGroupCount() const { return _hitShaderCount; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RayTracingShaderTable() = default;

		virtual ~RayTracingShaderTable();

		RayTracingShaderTable(const DevicePtr& device) :_device(device) {};

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		std::uint32_t RoundUp(const std::uint64_t size, const std::uint32_t alignment);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		gu::SharedPointer<GPUBuffer> _buffer = nullptr;

		std::vector<ShaderPtr> _shaderList = {};

		// each shader count
		std::int32_t _rayGenerationShaderCount = 0;
		std::int32_t _missShaderCount = 0;
		std::int32_t _hitShaderCount  = 0;

	};
}

#endif