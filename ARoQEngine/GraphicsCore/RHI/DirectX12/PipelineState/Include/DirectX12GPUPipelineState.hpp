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
#define COM_NO_WINDOWS_H
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
	****************************************************************************/
	/* @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUGraphicsPipelineState : public rhi::core::GPUGraphicsPipelineState
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     GraphicsPipelineが作成できたタイミングで動作させます. Descriptorを作成します.
		*  @param[in] const gu::tstring デバッグ表示名
		*  @return    void
		*************************************************************************/
		void CompleteSetting(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     DirectX12で使用するGraphics Pipelineのポインタ
		*  @param[in] void
		*  @return    PipelineStateComPtr
		*************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _graphicsPipeline; }

		/*!**********************************************************************
		*  @brief     自身のPSOを再生成する際に使用します. PSOの一部でキャッシュを使用することで高速化を図ります.
		*  @param[in] void
		*  @return    BlobComPtr
		*************************************************************************/
		__forceinline BlobComPtr GetCachePSO() const noexcept { return _cachePSO; }

		/*!**********************************************************************
		*  @brief     デバッグ時に使用する名前
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		void SetName(const gu::tstring& name) override { _graphicsPipeline->SetName(name.CString());}

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUGraphicsPipelineState() = default;
		
		/*! @brief デストラクタ*/
		~GPUGraphicsPipelineState() = default;
		
		/*! @brief RenderPassとResourceLayoutを使って初期化*/
		explicit GPUGraphicsPipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHIRenderPass>&renderPass,
			const gu::SharedPointer<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass,layout){};

		#pragma endregion

	protected:

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief DirectX12を使用したGraphics Pipeline*/
		PipelineStateComPtr _graphicsPipeline = nullptr;

		/*! @brief パイプライン生成の際に使用するキャッシュ*/
		BlobComPtr _cachePSO = nullptr;

		#pragma endregion
	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	****************************************************************************/
	/* @brief  Compute Shaderを使用する場合のCompute pipeline state
	*****************************************************************************/
	class GPUComputePipelineState : public rhi::core::GPUComputePipelineState
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     ComputePipelineが作成できたタイミングで動作させます. Descriptorを作成します.
		*  @param[in] const gu::tstring デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12で使用するGraphics Pipelineのポインタ
		*  @param[in] void
		*  @return    PipelineStateComPtr
		*************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _computePipeline; }
		
		/*!**********************************************************************
		*  @brief     自身のPSOを再生成する際に使用します. PSOの一部でキャッシュを使用することで高速化を図ります. 
		*  @param[in] void
		*  @return    BlobComPtr
		*************************************************************************/
		__forceinline BlobComPtr GetCachePSO() const noexcept { return _cachePSO; }

		/*!**********************************************************************
		*  @brief     デバッグ時に使用する名前
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUComputePipelineState() = default;
		
		/*! @brief デストラクタ*/
		~GPUComputePipelineState() = default;
		
		/*! @brief 論理デバイスとResourceLayoutを使って初期化*/
		explicit GPUComputePipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
		const gu::SharedPointer<core::RHIResourceLayout>& layout = nullptr) : core::GPUComputePipelineState(device, layout){};
		
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		
		/* @brief DirectX12で使用するCompute Pipeline*/
		PipelineStateComPtr _computePipeline = nullptr;

		/*! @brief パイプライン生成の際に使用するキャッシュ https://logicalbeat.jp/blog/6220/ */
		BlobComPtr _cachePSO = nullptr;
		#pragma endregion

	};
}
#endif