//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BLEND_STATE_HPP
#define GPU_BLEND_STATE_HPP

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
	class GPUDepthStencilState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool GetDepthWriteEnable() const noexcept { return _depthWriteEnable; }
		bool GetStencilEnable   () const noexcept { return _stencilEnable; }
		bool GetDepthEnable     () const noexcept { return _depthEnable; }
		const CompareOperator& GetDepthOperator() const noexcept { return _depthOperator; }
		const StencilOperatorInfo& GetFrontFace() const noexcept { return _frontFace; }
		const StencilOperatorInfo& GetBackFace () const noexcept { return _backFace; }
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
			const bool            depthEnable      = true,
			const bool            depthWriteEnable = true,
			const bool            stencilEnable    = false,
			const CompareOperator depthOperator    = CompareOperator::LessEqual,
			const StencilOperatorInfo& front       = StencilOperatorInfo(),
			const StencilOperatorInfo& back        = StencilOperatorInfo()
		) : GPUState(), _depthWriteEnable(depthWriteEnable), _stencilEnable(stencilEnable), _depthEnable(depthEnable), _depthOperator(depthOperator), _frontFace(front), _backFace(back) { }
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		core::CompareOperator     _depthOperator = core::CompareOperator::LessEqual;
		core::StencilOperatorInfo _frontFace     = core::StencilOperatorInfo();
		core::StencilOperatorInfo _backFace      = core::StencilOperatorInfo();
		/* @brief : depth buffer write*/
		bool _depthWriteEnable = true;
		/* @brief : use depth test*/
		bool _depthEnable      = true;
		/* @brief : use stencil*/
		bool _stencilEnable    = false;
	};
}
#endif