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
	****************************************************************************/
	/* @brief     PipelineState
	*****************************************************************************/
	class GPUBasePipelineState : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief     ResourceLayoutを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<RHIResourceLayout> : ResourceLayout
		*************************************************************************/
		__forceinline gu::SharedPointer<RHIResourceLayout> GetLayout() const noexcept { return _resourceLayout; }
		
		/*!**********************************************************************
		*  @brief     ResourceLayoutを設定します
		*  @param[in] const gu::SharedPointer<RHIResourceLayout>& resourceLayout : ResourceLayout
		*  @return    void
		*************************************************************************/
		__forceinline void SetLayout(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) { _resourceLayout = resourceLayout; }
		
		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUBasePipelineState() = default;

		/*! @brief 論理デバイスを使って初期化します. */
		explicit GPUBasePipelineState(const gu::SharedPointer<RHIDevice>& device,  const gu::SharedPointer<RHIResourceLayout>& layout)
			: _device(device) ,_resourceLayout(layout) { };
		
		/*! @brief デフォルトデストラクタ*/
		virtual ~GPUBasePipelineState() = default;

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief 論理デバイス*/
		gu::SharedPointer<rhi::core::RHIDevice> _device = nullptr;

		/*! @brief ResourceLayout*/
		gu::SharedPointer<rhi::core::RHIResourceLayout> _resourceLayout = nullptr;

		#pragma endregion
	};

	/****************************************************************************
	*				  			RHIPipelineState
	****************************************************************************/
	/* @brief  グラフィクスパイプラインの基本クラス (VS, PS, GS, HS, DSが使用できます.)
	*****************************************************************************/
	class GPUGraphicsPipelineState : public GPUBasePipelineState
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ComputePipelineが作成できたタイミングで動作させます. Descriptorを作成します.
		*  @param[in] const gu::tstring デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name = SP("GraphicsPipeline")) = 0;
		
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     RenderPassを返します
		*  @param[in] void
		*  @return    gu::SharedPointer<RHIRenderPass> : レンダーパス
		*************************************************************************/
		__forceinline gu::SharedPointer<RHIRenderPass> GetRenderPass() const noexcept { return _renderPass; }
		
		/*!**********************************************************************
		*  @brief     InputAssemblyStateを返します
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUInputAssemblyState> : 入力アセンブリステート
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUInputAssemblyState> GetInputAssemblyState() const noexcept { return _inputAssemblyState; }
		
		/*!**********************************************************************
		*  @brief     RasterizerStateを返します
		*  @param[in] void
		*  @return    gu::SharedPointer<GPURasterizerState> : ラスタライザステート
		*************************************************************************/
		__forceinline gu::SharedPointer<GPURasterizerState> GetRasterizerState() const noexcept { return _rasterizerState; }
		
		/*!**********************************************************************
		*  @brief     DepthStencilStateを返します
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUDepthStencilState> : デプスステンシルステート
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUDepthStencilState> GetDepthStencilState() const noexcept { return _depthStencilState; }
		
		/*!**********************************************************************
		*  @brief     BlendStateを返します
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUBlendState> : ブレンドステート
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUBlendState> GetBlendState() const noexcept { return _blendState; }
		
		/*!**********************************************************************
		*  @brief     頂点シェーダのポインタを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : 頂点シェーダ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetVertexShader() const noexcept { return _vertexShaderState; }
		
		/*!**********************************************************************
		*  @brief     ピクセルシェーダのポインタを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : ピクセルシェーダ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetPixelShader() const noexcept { return _pixelShaderState; }
		
		/*!**********************************************************************
		*  @brief     ジオメトリシェーダのポインタを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : ジオメトリシェーダ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetGeometryShader() const noexcept { return _geometryShaderState; }
		
		/*!**********************************************************************
		*  @brief     Hullシェーダのポインタを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : ハルシェーダ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetHullShader() const noexcept { return _hullShaderState; }
		
		/*!**********************************************************************
		*  @brief     ドメインシェーダのポインタを返します.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : ドメインシェーダ
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetDomainShader() const noexcept { return _domainShaderState; }

		/*!**********************************************************************
		*  @brief     VertexShaderが存在するかどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasVertexShader() const { return _vertexShaderState; }

		/*!**********************************************************************
		*  @brief     PixelShaderが存在するかどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasPixelShader() const { return _pixelShaderState; }

		/*!**********************************************************************
		*  @brief     GeometryShaderが存在するかどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasGeometryShader() const { return _geometryShaderState; }

		/*!**********************************************************************
		*  @brief     HullShaderが存在するかどうかを返します
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasHullShader() const { return _hullShaderState; }

		/*!**********************************************************************
		*  @brief     DomainShaderが存在するかどうかを返します
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		__forceinline bool HasDomainShader() const { return _domainShaderState; }

		/*!**********************************************************************
		*  @brief     InputAssemblyStateステートを設定します
		*  @param[in] const gu::SharedPointer<GPUInputAssemblyState>& inputAssembly
		*  @return    void
		*************************************************************************/
		__forceinline void SetInputAssemblyState(const gu::SharedPointer<GPUInputAssemblyState>& inputAssembly){ _inputAssemblyState = inputAssembly; }
		
		/*!**********************************************************************
		*  @brief     Rasterizerステートを設定します
		*  @param[in] const gu::SharedPointer<GPURasterizerState>& rasterizerState
		*  @return    void
		*************************************************************************/
		__forceinline void SetRasterizerState(const gu::SharedPointer<GPURasterizerState>& rasterizerState){ _rasterizerState = rasterizerState; }
		
		/*!**********************************************************************
		*  @brief     DepthStencilステートを設定します
		*  @param[in] const gu::SharedPointer<GPUDepthStencilState>& depthStencilState
		*  @return    void
		*************************************************************************/
		__forceinline void SetDepthStencilState (const gu::SharedPointer<GPUDepthStencilState> & depthStencilState) { _depthStencilState  = depthStencilState; }
		
		/*!**********************************************************************
		*  @brief     ブレンドステートを設定します
		*  @param[in] const gu::ShadredPointer<GPUBlendState>& blendState
		*  @return    void
		*************************************************************************/
		__forceinline void SetBlendState(const gu::SharedPointer<GPUBlendState>& blendState) { _blendState = blendState; }
		
		/*!**********************************************************************
		*  @brief     頂点シェーダを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& 頂点シェーダ
		*  @return    void
		*************************************************************************/
		__forceinline void SetVertexShader(const gu::SharedPointer<GPUShaderState>& vertexShader) 
		{
			Check(vertexShader->GetShaderType() == core::ShaderType::Vertex); 
			_vertexShaderState = vertexShader; 
		}
		
		/*!**********************************************************************
		*  @brief     ピクセルシェーダを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& ピクセルシェーダ
		*  @return    void
		*************************************************************************/
		__forceinline void SetPixelShader(const gu::SharedPointer<GPUShaderState>& pixelShader) 
		{
			Check(pixelShader->GetShaderType() == core::ShaderType::Pixel);
			_pixelShaderState = pixelShader; 
		}
		
		/*!**********************************************************************
		*  @brief     ジオメトリシェーダを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& ジオメトリシェーダ
		*  @return    void
		*************************************************************************/
		__forceinline void SetGeometryShader(const gu::SharedPointer<GPUShaderState>& geometryShader)
		{
			Check(geometryShader->GetShaderType() == core::ShaderType::Geometry);
			_geometryShaderState = (geometryShader->GetShaderType() == ShaderType::Geometry) ? geometryShader : nullptr; 
		}
		
		/*!**********************************************************************
		*  @brief     Hullシェーダを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& ジオメトリシェーダ
		*  @return    void
		*************************************************************************/
		__forceinline void SetHullShader(const gu::SharedPointer<GPUShaderState>& hullShader)
		{
			Check(hullShader->GetShaderType() == core::ShaderType::Hull);
			_hullShaderState = hullShader;
		}
		
		/*!**********************************************************************
		*  @brief     ドメインシェーダを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& ドメインシェーダ
		*  @return    void
		*************************************************************************/
		__forceinline void SetDomainShader(const gu::SharedPointer<GPUShaderState>& domainShader)
		{
			Check(domainShader->GetShaderType() == core::ShaderType::Domain);
			_domainShaderState = domainShader;
		}
		
		/*!**********************************************************************
		*  @brief     デバッグ時に使用する名前
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUGraphicsPipelineState() = default;
		
		/*! @brief 論理デバイスで初期化*/
		explicit GPUGraphicsPipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, layout), _renderPass(renderPass) {};
		
		/*! @brief デストラクタ*/
		~GPUGraphicsPipelineState() = default;
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
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
		#pragma endregion
	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	****************************************************************************/
	/* @class     GPUComputePipelineState
	*  @brief     Compute PipelineState
	*****************************************************************************/
	class GPUComputePipelineState : public GPUBasePipelineState
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     ComputePipelineが作成できたタイミングで動作させます. Descriptorを作成します.
		*  @param[in] const gu::tstring デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name = SP("ComputePipeline")) = 0;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ComputeShaderを設定します
		*  @param[in] const gu::SharedPointer<GPUShaderState>& shaderState
		*  @return    void
		*************************************************************************/
		__forceinline void SetComputeShader(const gu::SharedPointer<GPUShaderState>& shaderState) 
		{
			Check(shaderState->GetShaderType() == core::ShaderType::Compute);
			_computeShaderState = shaderState; 
		};
		
		/*!**********************************************************************
		*  @brief     デバッグ時に使用する名前
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		GPUComputePipelineState() = default;
		
		explicit GPUComputePipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, layout){};
		
		~GPUComputePipelineState() = default;

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		gu::SharedPointer<GPUShaderState> _computeShaderState = nullptr;
		#pragma endregion
	};

	/****************************************************************************
	*				  			RHIPipelineState
	****************************************************************************/
	/* @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPURayTracingPipelineState : public GPUBasePipelineState
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     RayTracingPipelineが作成できたタイミングで動作させます. Descriptorを作成します.
		*  @param[in] const gu::tstring デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting() = 0;

		#pragma endregion

		#pragma region Public Property
		void SetShader(const gu::SharedPointer<GPUShaderState>& shaderState) { _rayTracingShaderState = shaderState; };
		
		/*!**********************************************************************
		*  @brief     デバッグ時に使用する名前
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) const = 0;
		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		GPURayTracingPipelineState() = default;
		
		explicit GPURayTracingPipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIResourceLayout>& layout = nullptr) : core::GPUBasePipelineState(device, layout) {};
		
		~GPURayTracingPipelineState() = default;
		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		gu::SharedPointer<GPUShaderState> _rayTracingShaderState = nullptr;
		#pragma endregion
	};
}
#endif