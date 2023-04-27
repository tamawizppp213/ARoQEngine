//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RASTERIZER_STATE_HPP
#define GPU_RASTERIZER_STATE_HPP

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
	*				  			RHIRasterizerState
	*************************************************************************//**
	*  @class     RHIRasterizerState
	*  @brief     RasterizerState
	*****************************************************************************/
	class GPURasterizerState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const RasterizerProperty& GetProperty() const { return _property; }

		/* @brief: Return polygon front face. Clockwise -> for left hand coordinate, CounterClockwise-> for right hand coordinate*/
		FrontFace   GetFrontFace  () const noexcept { return _property.FaceType; }

		/* @brief : Drawing culling mode. default use none.*/
		CullingMode GetCullingMode() const noexcept { return _property.CullingType; }

		/* @brief : Polygon fillmode. You can select the wireframe, solid, (only vulkan) point filling modes.*/
		FillMode    GetFillMode   () const noexcept { return _property.FillType; }

		/* @brief : Return use depth clamp.*/
		bool        UseDepthClamp () const noexcept { return _property.UseDepthClamp; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPURasterizerState() = default;

		virtual ~GPURasterizerState() = default;

		explicit GPURasterizerState(
			const std::shared_ptr<RHIDevice>& device,
			const RasterizerProperty& rasterizerProperty) 
			: GPUState(device), _property(rasterizerProperty) { };
	
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		RasterizerProperty _property = RasterizerProperty();

	};

}
#endif