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
#include "GameUtility/Base/Include/GUMemory.hpp"
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
		gu::SharedPointer<BLASBuffer> GetBLASBuffer() const noexcept { return _blasBuffer; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		ASInstance() = default;
		virtual ~ASInstance() = default;
		ASInstance(const gu::SharedPointer<RHIDevice>& device,
			const gu::SharedPointer<BLASBuffer>& blasBuffer,
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
		gu::SharedPointer<BLASBuffer> _blasBuffer = nullptr;
		gu::SharedPointer<RHIDevice> _device      = nullptr;
	};
}
#endif