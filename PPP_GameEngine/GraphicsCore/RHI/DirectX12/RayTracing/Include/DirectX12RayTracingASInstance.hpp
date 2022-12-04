//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingASInstance.hpp
///             @brief  Bind Between TLAS and BLAS
///             @author Toide Yutaro
///             @date   2022_11_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RAYTRACING_AS_INSTANCE_HPP
#define DIRECTX12_RAYTRACING_AS_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingASInstance.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	class BLASBuffer;
	/****************************************************************************
	*				  			ASInstance
	*************************************************************************//**
	*  @struct    ASInstance
	*  @brief     Bind Between TLAS and BLAS
	*****************************************************************************/
	class ASInstance : public core::ASInstance
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const D3D12_RAYTRACING_INSTANCE_DESC& GetDesc() { return _instanceDesc; }
		const std::shared_ptr<directX12::BLASBuffer> GetDxBLASBuffer() const noexcept { return std::static_pointer_cast<directX12::BLASBuffer>(_blasBuffer); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ASInstance() = default;
		~ASInstance();
		ASInstance(
			const std::shared_ptr<core::RHIDevice>& device,
			const std::shared_ptr<core::BLASBuffer>& blasBuffer,
			const gm::Float3x4& blasTransform,
			const std::uint32_t instanceID,
			const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF,
			const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None
			);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_RAYTRACING_INSTANCE_DESC _instanceDesc = {};
	};
}
#endif