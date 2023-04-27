//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPURasterizerState.hpp
///             @brief  DirectX12GPURasterizerState.hpp
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP
#define DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUDepthStencilState.hpp"
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
	*				  			GPUDepthStencilState
	*************************************************************************//**
	*  @class     GPUDepthStencilState
	*  @brief     DepthStencilState
	*****************************************************************************/
	class GPUDepthStencilState : public rhi::core::GPUDepthStencilState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() const noexcept { return _depthStencilDesc; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit GPUDepthStencilState(
			const std::shared_ptr<rhi::core::RHIDevice>& device,
			const core::DepthStencilProperty& depthStencilProperty
		);

		~GPUDepthStencilState() = default;
		
		GPUDepthStencilState() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_DEPTH_STENCIL_DESC _depthStencilDesc = {};
	};
}
#endif