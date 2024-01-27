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
		//static void DestroyAll(){};

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		inline gu::SharedPointer<RHIResourceLayout>   GetLayout() const noexcept { return _resourceLayout; }
		
		inline void SetLayout(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) { _resourceLayout = resourceLayout; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBasePipelineState() = default;

		explicit GPUBasePipelineState(const gu::SharedPointer<RHIDevice>& device,  const gu::SharedPointer<RHIResourceLayout>& layout)
			: _device(device) ,_resourceLayout(layout) { };
		
		virtual ~GPUBasePipelineState() = default;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<rhi::core::RHIDevice> _device = nullptr;

		gu::SharedPointer<rhi::core::RHIResourceLayout> _resourceLayout = nullptr;
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
		gu::SharedPointer<RHIRenderPass>                GetRenderPass() const noexcept { return _renderPass; }
		
		inline gu::SharedPointer<GPUInputAssemblyState> GetInputAssemblyState() const noexcept { return _inputAssemblyState; }
		
		inline gu::SharedPointer<GPURasterizerState>    GetRasterizerState   () const noexcept { return _rasterizerState; }
		
		inline gu::SharedPointer<GPUDepthStencilState>  GetDepthStencilState () const noexcept { return _depthStencilState; }
		
		inline gu::SharedPointer<GPUBlendState>         GetBlendState        () const noexcept { return _blendState; }
		
		inline gu::SharedPointer<GPUShaderState>        GetVertexShader      () const noexcept { return _vertexShaderState; }
		
		inline gu::SharedPointer<GPUShaderState>        GetPixelShader       () const noexcept { return _pixelShaderState; }
		
		inline gu::SharedPointer<GPUShaderState>        GetGeometryShader    () const noexcept { return _geometryShaderState; }
		
		inline gu::SharedPointer<GPUShaderState>        GetHullShader        () const noexcept { return _hullShaderState; }
		
		inline gu::SharedPointer<GPUShaderState>        GetDomainShader      () const noexcept { return _domainShaderState; }

		inline bool HasVertexShader() const { return _vertexShaderState; }

		inline bool HasPixelShader() const { return _pixelShaderState; }

		inline bool HasGeometryShader() const { return _geometryShaderState; }

		inline bool HasHullShader() const { return _hullShaderState; }

		inline bool HasDomainShader() const { return _domainShaderState; }

		inline void SetInputAssemblyState(const gu::SharedPointer<GPUInputAssemblyState>& inputAssembly)     { _inputAssemblyState = inputAssembly; }
		
		inline void SetRasterizerState   (const gu::SharedPointer<GPURasterizerState>   & rasterizerState)   { _rasterizerState    = rasterizerState; }
		
		inline void SetDepthStencilState (const gu::SharedPointer<GPUDepthStencilState> & depthStencilState) { _depthStencilState  = depthStencilState; }
		
		inline void SetBlendState        (const gu::SharedPointer<GPUBlendState>        & blendState)        { _blendState         = blendState; }
		
		inline void SetVertexShader  (const gu::SharedPointer<GPUShaderState>& vertexShader)   { _vertexShaderState   = (vertexShader  ->GetShaderType() == ShaderType::Vertex  ) ? vertexShader   : nullptr; }
		
		inline void SetPixelShader   (const gu::SharedPointer<GPUShaderState>& pixelShader)    { _pixelShaderState    = (pixelShader   ->GetShaderType() == ShaderType::Pixel   ) ? pixelShader    : nullptr; }
		
		inline void SetGeometryShader(const gu::SharedPointer<GPUShaderState>& geometryShader) { _geometryShaderState = (geometryShader->GetShaderType() == ShaderType::Geometry) ? geometryShader : nullptr; }
		
		inline void SetHullShader    (const gu::SharedPointer<GPUShaderState>& hullShader)     { _hullShaderState     = (hullShader    ->GetShaderType() == ShaderType::Hull    ) ? hullShader     : nullptr; }
		
		inline void SetDomainShader  (const gu::SharedPointer<GPUShaderState>& domainShader)   { _domainShaderState   = (domainShader  ->GetShaderType() == ShaderType::Domain  ) ? domainShader   : nullptr; }
		
		// @brief : This function is needed to call after calling completeSetting function 
		virtual void SetName(const gu::wstring& name) = 0;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		GPUGraphicsPipelineState() = default;
		
		explicit GPUGraphicsPipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, layout), _renderPass(renderPass) {};
		
		~GPUGraphicsPipelineState() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<GPUInputAssemblyState> _inputAssemblyState  = nullptr;
		gu::SharedPointer<GPURasterizerState>    _rasterizerState     = nullptr;
		gu::SharedPointer<GPUDepthStencilState>  _depthStencilState   = nullptr;
		gu::SharedPointer<GPUBlendState>         _blendState          = nullptr;
		gu::SharedPointer<GPUShaderState>        _vertexShaderState   = nullptr;
		gu::SharedPointer<GPUShaderState>        _pixelShaderState    = nullptr;
		gu::SharedPointer<GPUShaderState>        _hullShaderState     = nullptr;
		gu::SharedPointer<GPUShaderState>        _domainShaderState   = nullptr;
		gu::SharedPointer<GPUShaderState>        _geometryShaderState = nullptr;
		gu::SharedPointer<RHIRenderPass>         _renderPass          = nullptr;
	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	*************************************************************************//**
	*  @class     GPUComputePipelineState
	*  @brief     Compute PipelineState
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
		void SetComputeShader(const gu::SharedPointer<GPUShaderState>& shaderState) { _computeShaderState = shaderState; };
		
		virtual void SetName(const gu::wstring& name) = 0;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		
		GPUComputePipelineState() = default;
		
		explicit GPUComputePipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, layout){};
		
		~GPUComputePipelineState() = default;
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<GPUShaderState> _computeShaderState = nullptr;
	};

	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPURayTracingPipelineState : public GPUBasePipelineState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void CompleteSetting() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetShader(const gu::SharedPointer<GPUShaderState>& shaderState) { _rayTracingShaderState = shaderState; };
		
		virtual void SetName(const gu::wstring& name) const = 0;
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		GPURayTracingPipelineState() = default;
		
		explicit GPURayTracingPipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIResourceLayout>& layout = nullptr) : core::GPUBasePipelineState(device, layout) {};
		
		~GPURayTracingPipelineState() = default;
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<GPUShaderState> _rayTracingShaderState = nullptr;
	};
}
#endif