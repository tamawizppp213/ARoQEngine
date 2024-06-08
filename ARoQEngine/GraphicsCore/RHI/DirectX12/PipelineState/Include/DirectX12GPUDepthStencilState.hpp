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
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		const D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() const noexcept { return _depthStencilDesc; }
		
		const D3D12_DEPTH_STENCIL_DESC1& GetDepthStencilState1() const noexcept { return _depthStencilDesc1; }
		
		#pragma endregion

		#pragma region Public Constructor and Destructor
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::DepthStencilProperty& depthStencilProperty
		);

		~GPUDepthStencilState() = default;
		
		GPUDepthStencilState() = default;
		#pragma endregion
	
	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		D3D12_DEPTH_STENCIL_DESC  _depthStencilDesc  = {};
		D3D12_DEPTH_STENCIL_DESC1 _depthStencilDesc1 = {};
		#pragma endregion
	};
}
#endif