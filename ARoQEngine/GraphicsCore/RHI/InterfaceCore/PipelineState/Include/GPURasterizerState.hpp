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

		/*----------------------------------------------------------------------
		*  @brief : Return polygon front face. 
		*           Clockwise       -> for  left hand coordinate, 
		*　　　　　　　 CounterClockwise-> for right hand coordinate
		/*----------------------------------------------------------------------*/
		__forceinline FrontFace GetFrontFace  () const noexcept { return _property.FaceType; }

		/*----------------------------------------------------------------------
		*  @brief : Drawing culling mode. default use none.
		*           None  -> all   face render
		*           Front -> front face culling
		*           Back  -> back  face culling
		/*----------------------------------------------------------------------*/
		__forceinline CullingMode GetCullingMode() const noexcept { return _property.CullingType; }

		/*----------------------------------------------------------------------
		*  @brief : Polygon fillmode. You can select the wireframe, solid, (only vulkan) point filling modes.
		/*----------------------------------------------------------------------*/
		__forceinline FillMode GetFillMode() const noexcept { return _property.FillType; }

		/*----------------------------------------------------------------------
		*  @brief :  Return use depth clamp
		/*----------------------------------------------------------------------*/
		__forceinline bool UseDepthClamp () const noexcept { return _property.UseDepthClamp; }

		/*----------------------------------------------------------------------
		*  @brief :  Return use multi sampling
		/*----------------------------------------------------------------------*/
		__forceinline bool UseMultiSample() const { return _property.UseMultiSample; }

		/*----------------------------------------------------------------------
		*  @brief :  Return use anti alias line
		/*----------------------------------------------------------------------*/
		__forceinline bool UseAntiAliasLine() const { return _property.UseAntiAliasLine; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPURasterizerState() = default;

		virtual ~GPURasterizerState() = default;

		explicit GPURasterizerState(
			const gu::SharedPointer<RHIDevice>& device,
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