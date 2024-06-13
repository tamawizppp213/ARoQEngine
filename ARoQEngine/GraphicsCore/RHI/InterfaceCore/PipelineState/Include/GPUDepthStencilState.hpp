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
#include "../../Core/Include/RHICommonState.hpp"
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
	****************************************************************************/
	/* @class     GPUDepthStencilState
	*  @brief     DepthStencilState
	*****************************************************************************/
	class GPUDepthStencilState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		bool UseStencilTest() const noexcept { return _property.StenciWriteEnable; }

		bool UseDepthTest() const noexcept { return _property.UseDepthTest; }

		bool UseDepthBoundsTest() const noexcept { return _property.UseDepthBoundsTest; }

		bool GetDepthWriteEnable() const noexcept { return _property.DepthWriteEnable; }

		const CompareOperator& GetDepthOperator() const noexcept { return _property.DepthOperator; }
		
		const StencilOperatorInfo& GetFrontFace() const noexcept { return _property.Front; }
		
		const StencilOperatorInfo& GetBackFace () const noexcept { return _property.Back; }

		float GetMinDepthBounds() const noexcept { return _property.MinDepthBounds; }

		float GetMaxDepthBounds() const noexcept { return _property.MaxDepthBounds; }

		
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
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const DepthStencilProperty& depthStencilProperty
		) : GPUState(device), _property(depthStencilProperty){ };
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		DepthStencilProperty _property = DepthStencilProperty();
	};
}
#endif