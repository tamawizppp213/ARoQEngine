//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUInputAssemblyState.hpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP
#define DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUInputAssemblyState.hpp"
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
	class GPUInputAssemblyState : public rhi::core::GPUInputAssemblyState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const D3D12_INPUT_LAYOUT_DESC& GetLayout() const noexcept { return _inputLayout; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUInputAssemblyState() = default;
		~GPUInputAssemblyState() = default;
		explicit GPUInputAssemblyState(
			const std::shared_ptr<rhi::core::RHIDevice>& device, 
			const std::vector<core::InputLayoutElement>& elements, 
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList);

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_INPUT_LAYOUT_DESC _inputLayout = {nullptr, 0};
		std::vector<D3D12_INPUT_ELEMENT_DESC> _inputLayoutElements = {};
	};
}
#endif