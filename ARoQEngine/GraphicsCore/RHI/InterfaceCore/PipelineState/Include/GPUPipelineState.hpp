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
		*  @brief     ResourceLayout��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<RHIResourceLayout> : ResourceLayout
		*************************************************************************/
		__forceinline gu::SharedPointer<RHIResourceLayout> GetLayout() const noexcept { return _resourceLayout; }
		
		/*!**********************************************************************
		*  @brief     ResourceLayout��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<RHIResourceLayout>& resourceLayout : ResourceLayout
		*  @return    void
		*************************************************************************/
		__forceinline void SetLayout(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) { _resourceLayout = resourceLayout; }
		
		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBasePipelineState() = default;

		/*! @brief �_���f�o�C�X���g���ď��������܂�. */
		explicit GPUBasePipelineState(const gu::SharedPointer<RHIDevice>& device,  const gu::SharedPointer<RHIResourceLayout>& layout)
			: _device(device) ,_resourceLayout(layout) { };
		
		/*! @brief �f�t�H���g�f�X�g���N�^*/
		virtual ~GPUBasePipelineState() = default;

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<rhi::core::RHIDevice> _device = nullptr;

		/*! @brief ResourceLayout*/
		gu::SharedPointer<rhi::core::RHIResourceLayout> _resourceLayout = nullptr;

		#pragma endregion
	};

	/****************************************************************************
	*				  			RHIPipelineState
	****************************************************************************/
	/* @brief  �O���t�B�N�X�p�C�v���C���̊�{�N���X (VS, PS, GS, HS, DS���g�p�ł��܂�.)
	*****************************************************************************/
	class GPUGraphicsPipelineState : public GPUBasePipelineState
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ComputePipeline���쐬�ł����^�C�~���O�œ��삳���܂�. Descriptor���쐬���܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name = SP("GraphicsPipeline")) = 0;
		
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     RenderPass��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::SharedPointer<RHIRenderPass> : �����_�[�p�X
		*************************************************************************/
		__forceinline gu::SharedPointer<RHIRenderPass> GetRenderPass() const noexcept { return _renderPass; }
		
		/*!**********************************************************************
		*  @brief     InputAssemblyState��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUInputAssemblyState> : ���̓A�Z���u���X�e�[�g
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUInputAssemblyState> GetInputAssemblyState() const noexcept { return _inputAssemblyState; }
		
		/*!**********************************************************************
		*  @brief     RasterizerState��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::SharedPointer<GPURasterizerState> : ���X�^���C�U�X�e�[�g
		*************************************************************************/
		__forceinline gu::SharedPointer<GPURasterizerState> GetRasterizerState() const noexcept { return _rasterizerState; }
		
		/*!**********************************************************************
		*  @brief     DepthStencilState��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUDepthStencilState> : �f�v�X�X�e���V���X�e�[�g
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUDepthStencilState> GetDepthStencilState() const noexcept { return _depthStencilState; }
		
		/*!**********************************************************************
		*  @brief     BlendState��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUBlendState> : �u�����h�X�e�[�g
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUBlendState> GetBlendState() const noexcept { return _blendState; }
		
		/*!**********************************************************************
		*  @brief     ���_�V�F�[�_�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : ���_�V�F�[�_
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetVertexShader() const noexcept { return _vertexShaderState; }
		
		/*!**********************************************************************
		*  @brief     �s�N�Z���V�F�[�_�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : �s�N�Z���V�F�[�_
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetPixelShader() const noexcept { return _pixelShaderState; }
		
		/*!**********************************************************************
		*  @brief     �W�I���g���V�F�[�_�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : �W�I���g���V�F�[�_
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetGeometryShader() const noexcept { return _geometryShaderState; }
		
		/*!**********************************************************************
		*  @brief     Hull�V�F�[�_�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : �n���V�F�[�_
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetHullShader() const noexcept { return _hullShaderState; }
		
		/*!**********************************************************************
		*  @brief     �h���C���V�F�[�_�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::SharedPointer<GPUShaderState> : �h���C���V�F�[�_
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUShaderState> GetDomainShader() const noexcept { return _domainShaderState; }

		/*!**********************************************************************
		*  @brief     VertexShader�����݂��邩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasVertexShader() const { return _vertexShaderState; }

		/*!**********************************************************************
		*  @brief     PixelShader�����݂��邩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasPixelShader() const { return _pixelShaderState; }

		/*!**********************************************************************
		*  @brief     GeometryShader�����݂��邩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasGeometryShader() const { return _geometryShaderState; }

		/*!**********************************************************************
		*  @brief     HullShader�����݂��邩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool HasHullShader() const { return _hullShaderState; }

		/*!**********************************************************************
		*  @brief     DomainShader�����݂��邩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		__forceinline bool HasDomainShader() const { return _domainShaderState; }

		/*!**********************************************************************
		*  @brief     InputAssemblyState�X�e�[�g��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUInputAssemblyState>& inputAssembly
		*  @return    void
		*************************************************************************/
		__forceinline void SetInputAssemblyState(const gu::SharedPointer<GPUInputAssemblyState>& inputAssembly){ _inputAssemblyState = inputAssembly; }
		
		/*!**********************************************************************
		*  @brief     Rasterizer�X�e�[�g��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPURasterizerState>& rasterizerState
		*  @return    void
		*************************************************************************/
		__forceinline void SetRasterizerState(const gu::SharedPointer<GPURasterizerState>& rasterizerState){ _rasterizerState = rasterizerState; }
		
		/*!**********************************************************************
		*  @brief     DepthStencil�X�e�[�g��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUDepthStencilState>& depthStencilState
		*  @return    void
		*************************************************************************/
		__forceinline void SetDepthStencilState (const gu::SharedPointer<GPUDepthStencilState> & depthStencilState) { _depthStencilState  = depthStencilState; }
		
		/*!**********************************************************************
		*  @brief     �u�����h�X�e�[�g��ݒ肵�܂�
		*  @param[in] const gu::ShadredPointer<GPUBlendState>& blendState
		*  @return    void
		*************************************************************************/
		__forceinline void SetBlendState(const gu::SharedPointer<GPUBlendState>& blendState) { _blendState = blendState; }
		
		/*!**********************************************************************
		*  @brief     ���_�V�F�[�_��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& ���_�V�F�[�_
		*  @return    void
		*************************************************************************/
		__forceinline void SetVertexShader(const gu::SharedPointer<GPUShaderState>& vertexShader) 
		{
			Check(vertexShader->GetShaderType() == core::ShaderType::Vertex); 
			_vertexShaderState = vertexShader; 
		}
		
		/*!**********************************************************************
		*  @brief     �s�N�Z���V�F�[�_��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& �s�N�Z���V�F�[�_
		*  @return    void
		*************************************************************************/
		__forceinline void SetPixelShader(const gu::SharedPointer<GPUShaderState>& pixelShader) 
		{
			Check(pixelShader->GetShaderType() == core::ShaderType::Pixel);
			_pixelShaderState = pixelShader; 
		}
		
		/*!**********************************************************************
		*  @brief     �W�I���g���V�F�[�_��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& �W�I���g���V�F�[�_
		*  @return    void
		*************************************************************************/
		__forceinline void SetGeometryShader(const gu::SharedPointer<GPUShaderState>& geometryShader)
		{
			Check(geometryShader->GetShaderType() == core::ShaderType::Geometry);
			_geometryShaderState = (geometryShader->GetShaderType() == ShaderType::Geometry) ? geometryShader : nullptr; 
		}
		
		/*!**********************************************************************
		*  @brief     Hull�V�F�[�_��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& �W�I���g���V�F�[�_
		*  @return    void
		*************************************************************************/
		__forceinline void SetHullShader(const gu::SharedPointer<GPUShaderState>& hullShader)
		{
			Check(hullShader->GetShaderType() == core::ShaderType::Hull);
			_hullShaderState = hullShader;
		}
		
		/*!**********************************************************************
		*  @brief     �h���C���V�F�[�_��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& �h���C���V�F�[�_
		*  @return    void
		*************************************************************************/
		__forceinline void SetDomainShader(const gu::SharedPointer<GPUShaderState>& domainShader)
		{
			Check(domainShader->GetShaderType() == core::ShaderType::Domain);
			_domainShaderState = domainShader;
		}
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O���Ɏg�p���閼�O
		*  @param[in] const gu::tstring& name
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUGraphicsPipelineState() = default;
		
		/*! @brief �_���f�o�C�X�ŏ�����*/
		explicit GPUGraphicsPipelineState(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIResourceLayout>& layout) : core::GPUBasePipelineState(device, layout), _renderPass(renderPass) {};
		
		/*! @brief �f�X�g���N�^*/
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
		*  @brief     ComputePipeline���쐬�ł����^�C�~���O�œ��삳���܂�. Descriptor���쐬���܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting(const gu::tstring& name = SP("ComputePipeline")) = 0;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ComputeShader��ݒ肵�܂�
		*  @param[in] const gu::SharedPointer<GPUShaderState>& shaderState
		*  @return    void
		*************************************************************************/
		__forceinline void SetComputeShader(const gu::SharedPointer<GPUShaderState>& shaderState) 
		{
			Check(shaderState->GetShaderType() == core::ShaderType::Compute);
			_computeShaderState = shaderState; 
		};
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O���Ɏg�p���閼�O
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
		*  @brief     RayTracingPipeline���쐬�ł����^�C�~���O�œ��삳���܂�. Descriptor���쐬���܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void CompleteSetting() = 0;

		#pragma endregion

		#pragma region Public Property
		void SetShader(const gu::SharedPointer<GPUShaderState>& shaderState) { _rayTracingShaderState = shaderState; };
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O���Ɏg�p���閼�O
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