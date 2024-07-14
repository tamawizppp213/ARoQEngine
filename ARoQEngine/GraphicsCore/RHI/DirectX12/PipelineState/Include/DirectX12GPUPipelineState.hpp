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
		*  @brief     GraphicsPipeline���쐬�ł����^�C�~���O�œ��삳���܂�. Descriptor���쐬���܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		void CompleteSetting(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����Graphics Pipeline�̃|�C���^
		*  @param[in] void
		*  @return    PipelineStateComPtr
		*************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _graphicsPipeline; }

		/*!**********************************************************************
		*  @brief     ���g��PSO���Đ�������ۂɎg�p���܂�. PSO�̈ꕔ�ŃL���b�V�����g�p���邱�Ƃō�������}��܂�.
		*  @param[in] void
		*  @return    BlobComPtr
		*************************************************************************/
		__forceinline BlobComPtr GetCachePSO() const noexcept { return _cachePSO; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O���Ɏg�p���閼�O
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		void SetName(const gu::tstring& name) override { _graphicsPipeline->SetName(name.CString());}

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUGraphicsPipelineState() = default;
		
		/*! @brief �f�X�g���N�^*/
		~GPUGraphicsPipelineState() = default;
		
		/*! @brief RenderPass��ResourceLayout���g���ď�����*/
		explicit GPUGraphicsPipelineState(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHIRenderPass>&renderPass,
			const gu::SharedPointer<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass,layout){};

		#pragma endregion

	protected:

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief DirectX12���g�p����Graphics Pipeline*/
		PipelineStateComPtr _graphicsPipeline = nullptr;

		/*! @brief �p�C�v���C�������̍ۂɎg�p����L���b�V��*/
		BlobComPtr _cachePSO = nullptr;

		#pragma endregion
	};

	/****************************************************************************
	*				  			GPUComputePipelineState
	****************************************************************************/
	/* @brief  Compute Shader���g�p����ꍇ��Compute pipeline state
	*****************************************************************************/
	class GPUComputePipelineState : public rhi::core::GPUComputePipelineState
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     ComputePipeline���쐬�ł����^�C�~���O�œ��삳���܂�. Descriptor���쐬���܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����Graphics Pipeline�̃|�C���^
		*  @param[in] void
		*  @return    PipelineStateComPtr
		*************************************************************************/
		PipelineStateComPtr GetPipeline() const noexcept { return _computePipeline; }
		
		/*!**********************************************************************
		*  @brief     ���g��PSO���Đ�������ۂɎg�p���܂�. PSO�̈ꕔ�ŃL���b�V�����g�p���邱�Ƃō�������}��܂�. 
		*  @param[in] void
		*  @return    BlobComPtr
		*************************************************************************/
		__forceinline BlobComPtr GetCachePSO() const noexcept { return _cachePSO; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O���Ɏg�p���閼�O
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUComputePipelineState() = default;
		
		/*! @brief �f�X�g���N�^*/
		~GPUComputePipelineState() = default;
		
		/*! @brief �_���f�o�C�X��ResourceLayout���g���ď�����*/
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
		
		/* @brief DirectX12�Ŏg�p����Compute Pipeline*/
		PipelineStateComPtr _computePipeline = nullptr;

		/*! @brief �p�C�v���C�������̍ۂɎg�p����L���b�V�� https://logicalbeat.jp/blog/6220/ */
		BlobComPtr _cachePSO = nullptr;
		#pragma endregion

	};
}
#endif