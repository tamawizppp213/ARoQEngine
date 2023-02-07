//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingASInstance.hpp
///             @brief  Bind Between TLAS and BLAS
///             @author Toide Yutaro
///             @date   2022_11_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RAYTRACING_AS_INSTANCE_HPP
#define RAYTRACING_AS_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning (disable: 4100)
//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class BLASBuffer;
	/****************************************************************************
	*				  			ASInstance
	*************************************************************************//**
	*  @struct    ASInstance
	*  @brief     Bind Between TLAS and BLAS
	*****************************************************************************/
	class ASInstance : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<BLASBuffer> GetBLASBuffer() const noexcept { return _blasBuffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		ASInstance() = default;
		~ASInstance() = default;
		ASInstance(const std::shared_ptr<RHIDevice>& device,
			const std::shared_ptr<BLASBuffer>& blasBuffer,
			const gm::Float3x4& blasTransform,
			const std::uint32_t instanceID, 
			const std::uint32_t instanceContributionToHitGroupIndex,
			const std::uint32_t instanceMask = 0xFF,
			const core::RayTracingInstanceFlags flags = core::RayTracingInstanceFlags::None) : _device(device), _blasBuffer(blasBuffer)
		{
		};
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<BLASBuffer> _blasBuffer = nullptr;
		std::shared_ptr<RHIDevice> _device      = nullptr;
	};
}
#endif