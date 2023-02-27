//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUStencilState.hpp
///             @brief  Stencil State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_DEPTH_STENCIL_STATE_HPP
#define GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			GPUDepthStencilState
	*************************************************************************//**
	*  @class     GPUDepthStencilState
	*  @brief     DepthStencilState
	*****************************************************************************/
	class GPUDepthStencilState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool GetDepthWriteEnable() const noexcept { return _property.DepthWriteEnable; }

		bool GetStencilEnable   () const noexcept { return _property.StenciWriteEnable; }

		bool GetDepthEnable     () const noexcept { return _property.UseDepthTest; }

		const CompareOperator& GetDepthOperator() const noexcept { return _property.DepthOperator; }
		
		const StencilOperatorInfo& GetFrontFace() const noexcept { return _property.Front; }
		
		const StencilOperatorInfo& GetBackFace () const noexcept { return _property.Back; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUDepthStencilState() = default;

		virtual ~GPUDepthStencilState() = default;

		explicit GPUDepthStencilState(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const DepthStencilProperty& depthStencilProperty
		) : GPUState(device), _property(depthStencilProperty){ };
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		DepthStencilProperty _property = DepthStencilProperty();
	};
}
#endif