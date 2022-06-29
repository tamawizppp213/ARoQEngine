//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12BlendState.hpp
///             @brief  DirectX12BlendState.hpp
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BLEND_STATE_HPP
#define DIRECTX12_GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUBlendState.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gpu::directX12
{

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUBlendState : public rhi::core::GPUBlendState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		D3D12_BLEND_DESC&       GetBlendState()       { return _blendState; }
		const D3D12_BLEND_DESC& GetBlendState() const { return _blendState; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBlendState() = default;
		~GPUBlendState() = default;
		explicit GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::vector<rhi::core::BlendProperty>& blendProperties);
		explicit GPUBlendState(const std::shared_ptr<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_BLEND_DESC _blendState = {};
	};
}
#endif