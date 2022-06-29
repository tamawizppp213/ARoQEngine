//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPURasterizerState.hpp
///             @brief  DirectX12GPURasterizerState.hpp
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RASTERIZER_STATE_HPP
#define DIRECTX12_GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPURasterizerState.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPURasterizerState : public rhi::core::GPURasterizerState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const D3D12_RASTERIZER_DESC& GetRasterizerState() const noexcept { return _rasterizerState; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPURasterizerState() = default;
		~GPURasterizerState() = default;
		explicit GPURasterizerState(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const core::FrontFace   frontFace   = core::FrontFace::Clockwise,
			const core::CullingMode cullingMode = core::CullingMode::None,
			const core::FillMode    fillMode    = core::FillMode::Solid,
			const bool useDepthClamp = true);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_RASTERIZER_DESC _rasterizerState = {};
	};
}
#endif