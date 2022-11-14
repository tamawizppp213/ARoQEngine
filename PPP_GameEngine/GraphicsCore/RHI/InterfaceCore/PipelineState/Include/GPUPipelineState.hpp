//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIPipelineState.hpp
///             @brief  PipelineState
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_PIPELINE_STATE_HPP
#define RHI_PIPELINE_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "GPUBlendState.hpp"
#include "GPUShaderState.hpp"
#include "GPUDepthStencilState.hpp"
#include "GPUInputAssemblyState.hpp"
#include "GPURasterizerState.hpp"

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
	class GPUBasePipelineState : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		static void DestroyAll(){};
		

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIRenderPass>              GetRenderPass() const noexcept { return _renderPass; }
		inline std::shared_ptr<RHIResourceLayout>   GetLayout() const noexcept { return _resourceLayout; }
		inline void SetLayout(const std::shared_ptr<RHIResourceLayout>& resourceLayout) { _resourceLayout = resourceLayout; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBasePipelineState() = default;
		explicit GPUBasePipelineState(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIResourceLayout>& layout)
			: _device(device), _renderPass(renderPass) ,_resourceLayout(layout) { };
		~GPUBasePipelineState() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<rhi::core::RHIDevice>     _device        = nullptr;
		std::shared_ptr<rhi::core::RHIRenderPass> _renderPass    = nullptr;
		std::shared_ptr<rhi::core::RHIResourceLayout> _resourceLayout = nullptr;
	};

	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUGraphicsPipelineState : public GPUBasePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void CompleteSetting() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline std::shared_ptr<GPUInputAssemblyState> GetInputAssemblyState() const noexcept { return _inputAssemblyState; }
		inline std::shared_ptr<GPURasterizerState>    GetRasterizerState   () const noexcept { return _rasterizerState; }
		inline std::shared_ptr<GPUDepthStencilState>  GetDepthStencilState () const noexcept { return _depthStencilState; }
		inline std::shared_ptr<GPUBlendState>         GetBlendState        () const noexcept { return _blendState; }
		inline std::shared_ptr<GPUShaderState>        GetVertexShader      () const noexcept { return _vertexShaderState; }
		inline std::shared_ptr<GPUShaderState>        GetPixelShader       () const noexcept { return _pixelShaderState; }
		inline std::shared_ptr<GPUShaderState>        GetGeometryShader    () const noexcept { return _geometryShaderState; }
		inline std::shared_ptr<GPUShaderState>        GetHullShader        () const noexcept { return _hullShaderState; }
		inline std::shared_ptr<GPUShaderState>        GetDomainShader      () const noexcept { return _domainShaderState; }

		inline void SetInputAssemblyState(const std::shared_ptr<GPUInputAssemblyState>& inputAssembly)     { _inputAssemblyState = inputAssembly; }
		inline void SetRasterizerState   (const std::shared_ptr<GPURasterizerState>   & rasterizerState)   { _rasterizerState    = rasterizerState; }
		inline void SetDepthStencilState (const std::shared_ptr<GPUDepthStencilState> & depthStencilState) { _depthStencilState  = depthStencilState; }
		inline void SetBlendState        (const std::shared_ptr<GPUBlendState>        & blendState)        { _blendState         = blendState; }
		
		inline void SetVertexShader  (const std::shared_ptr<GPUShaderState>& vertexShader)   { _vertexShaderState   = (vertexShader  ->GetShaderType() == ShaderType::Vertex  ) ? vertexShader   : nullptr; }
		inline void SetPixelShader   (const std::shared_ptr<GPUShaderState>& pixelShader)    { _pixelShaderState    = (pixelShader   ->GetShaderType() == ShaderType::Pixel   ) ? pixelShader    : nullptr; }
		inline void SetGeometryShader(const std::shared_ptr<GPUShaderState>& geometryShader) { _geometryShaderState = (geometryShader->GetShaderType() == ShaderType::Geometry) ? geometryShader : nullptr; }
		inline void SetHullShader    (const std::shared_ptr<GPUShaderState>& hullShader)     { _hullShaderState     = (hullShader    ->GetShaderType() == ShaderType::Hull    ) ? hullShader     : nullptr; }
		inline void SetDomainShader  (const std::shared_ptr<GPUShaderState>& domainShader)   { _domainShaderState   = (domainShader  ->GetShaderType() == ShaderType::Domain  ) ? domainShader   : nullptr; }
		
		// @brief : This function is needed to call after calling completeSetting function 
		virtual void SetName(const std::wstring& name) = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		GPUGraphicsPipelineState() = default;
		explicit GPUGraphicsPipelineState(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass, const std::shared_ptr<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, renderPass, layout){};
		~GPUGraphicsPipelineState() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<GPUInputAssemblyState> _inputAssemblyState  = nullptr;
		std::shared_ptr<GPURasterizerState>    _rasterizerState     = nullptr;
		std::shared_ptr<GPUDepthStencilState>  _depthStencilState   = nullptr;
		std::shared_ptr<GPUBlendState>         _blendState          = nullptr;
		std::shared_ptr<GPUShaderState>        _vertexShaderState   = nullptr;
		std::shared_ptr<GPUShaderState>        _pixelShaderState    = nullptr;
		std::shared_ptr<GPUShaderState>        _hullShaderState     = nullptr;
		std::shared_ptr<GPUShaderState>        _domainShaderState   = nullptr;
		std::shared_ptr<GPUShaderState>        _geometryShaderState = nullptr;
	};

	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUComputePipelineState : public GPUBasePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void CompleteSetting() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		GPUComputePipelineState() = default;
		explicit GPUComputePipelineState(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<RHIRenderPass>& renderPass = nullptr, const std::shared_ptr<RHIResourceLayout>& layout = nullptr) : core::GPUBasePipelineState(device, renderPass, layout){};
		~GPUComputePipelineState() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<GPUShaderState> _computeShaderState = nullptr;
	};

}
#endif