//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUPipelineState.hpp
///             @brief  PipelineState.hpp
///             @author Toide Yutaro
///             @date   2022_08_14
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_PIPELINE_STATE_HPP
#define DIRECTX12_GPU_PIPELINE_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
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
	class GPUGraphicsPipelineState : public rhi::core::GPUGraphicsPipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _graphicsPipeline; }
		
		// @brief : This function is needed to call after calling completeSetting function 
		void SetName(const gu::tstring& name) override { _graphicsPipeline->SetName(name.CString());}
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUGraphicsPipelineState() = default;
		
		~GPUGraphicsPipelineState() = default;
		
		explicit GPUGraphicsPipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHIRenderPass>&renderPass,
			const gu::SharedPointer<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass,layout){};
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		PipelineStateComPtr _graphicsPipeline = nullptr;

	};

	/****************************************************************************
	*				  			GPUBlendState
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUComputePipelineState : public rhi::core::GPUComputePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CompleteSetting() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _computePipeline; }
		
		void SetName(const gu::tstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUComputePipelineState() = default;
		
		~GPUComputePipelineState() = default;
		
		explicit GPUComputePipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
		const gu::SharedPointer<core::RHIResourceLayout>& layout = nullptr) : core::GPUComputePipelineState(device, layout){};
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		PipelineStateComPtr _computePipeline = nullptr;

	};
}
#endif