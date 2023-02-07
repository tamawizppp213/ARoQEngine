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
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
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
		
		FrontFace   GetFrontFace  () const noexcept { return _frontFace; }

		CullingMode GetCullingMode() const noexcept { return _cullingMode; }

		FillMode    GetFillMode   () const noexcept { return _fillMode; }

		bool        UseDepthClamp () const noexcept { return _useDepthClamp; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPURasterizerState() = default;

		virtual ~GPURasterizerState() = default;

		explicit GPURasterizerState(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const FrontFace   frontFace   = FrontFace::Clockwise,
			const CullingMode cullingMode = CullingMode::None,
			const FillMode    fillMode    = FillMode::Solid,
			const bool        depthClamp  = true) 
			: GPUState(device), _frontFace(frontFace), _cullingMode(cullingMode), _fillMode(fillMode), _useDepthClamp(depthClamp) { };
	
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
		FrontFace   _frontFace   = FrontFace::Clockwise;
		CullingMode _cullingMode = CullingMode::None;
		FillMode    _fillMode    = FillMode::Solid;
		bool _useDepthClamp = true;
	};

}
#endif