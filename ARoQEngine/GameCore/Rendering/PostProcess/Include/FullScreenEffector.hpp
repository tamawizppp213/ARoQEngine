//////////////////////////////////////////////////////////////////////////////////
///  @file   FullScreenEffector.hpp
///  @brief  ���̃N���X�͎�Ƀ|�X�g�E�G�t�F�N�g�̂��߂Ɏg���� @n
///          �t���X�N���[���̃|���S����`�悷�邽�߂̃C���^�[�t�F�[�X.
///  @author Toide Yutaro
///  @date   2024_07_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FULL_SCREEN_EFFECTOR_HPP
#define FULL_SCREEN_EFFECTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIResourceLayout;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPUBuffer;
	class GPUTexture;
	class RHIRenderPass;
	class GPUShaderState;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  			IFullScreenEffector
	****************************************************************************/
	/* @brief  �t���X�N���[���̃|���S����`�悷�邽�߂̃C���^�[�t�F�[�X. 
	*****************************************************************************/
	class IFullScreenEffector : public gu::NonCopyable
	{
	protected:
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ComputePipelineStatePtr   = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;

	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     ��ʂ̃T�C�Y��ύX���܂�. 
		*  @param[in] const gu::uint32 �V������
		*  @param[in] const gu::uint32 �V��������
		*  @return    void
		*************************************************************************/
		virtual void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight) = 0;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɕ`����e��o�^���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void Draw() = 0;

		#pragma endregion

	    #pragma region Public Property

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor 

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		IFullScreenEffector();

		/*! @brief �f�X�g���N�^*/
		virtual ~IFullScreenEffector();

		/*! @brief �O���t�B�N�X�G���W�����g���ď�����*/
		IFullScreenEffector(const LowLevelGraphicsEnginePtr& engine);

		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Resize�\���𔻒肵�܂�
		*  @param[in] const gu::uint32 ��
		*  @param[in] const gu::uint32 ����
		*  @return    bool
		*************************************************************************/
		__forceinline bool EnableResize(const gu::uint32 newWidth, const gu::uint32 newHeight) const 
		{
			return newWidth != _width || newHeight != _height;
		}

		/*!**********************************************************************
		*  @brief     FullScreenState�̊�{�̃O���t�B�N�X�p�C�v���C���̐ݒ���s���܂�.
		*  @param[in] const gu::SharedPointer<rhi::core::RHIRenderPass>& �����_�[�p�X
		*  @param[in] const gu::SharedPointer<rhi::core::RHIResourceLayout>& ���\�[�X���C�A�E�g
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& ���_�V�F�[�_
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& �s�N�Z���V�F�[�_
		*  @return    void
		*************************************************************************/
		PipelineStatePtr CreateDefaultFullScreenGraphicsPipelineState(
			const gu::SharedPointer<rhi::core::RHIRenderPass>& renderPass, 
			const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout,
			const gu::SharedPointer<rhi::core::GPUShaderState>& vs,
			const gu::SharedPointer<rhi::core::GPUShaderState>& ps) const;

		/*!**********************************************************************
		*  @brief     FullScreenState�̊�{��Compute�p�C�v���C���̐ݒ���s���܂�.
		*  @param[in] const gu::SharedPointer<rhi::core::RHIResourceLayout>& ���\�[�X���C�A�E�g
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& �R���s���[�g�V�F�[�_�[
		*  @return    void
		*************************************************************************/
		ComputePipelineStatePtr CreateDefaultFullScreenComputePipelineState(
			const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout,
			const gu::SharedPointer<rhi::core::GPUShaderState>& cs);

		/*!**********************************************************************
		*  @brief     �p�C�v���C���X�e�[�g���������܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) = 0;

		/*!**********************************************************************
		*  @brief     ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���������܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O�\�������������܂�. ������, �f�t�H���g�l��FSEffector::���ǉ�����܂�.
		*  @param[in] const gu::tstring ��������������
		*  @param[in] const gu::tstring ���݂̕�����
		*  @return    gu::tstring
		*************************************************************************/
		gu::tstring CombineDebugName(const gu::tstring& addName, const gu::tstring& original) const;

		#pragma endregion

		#pragma region Protected Property

		/* @brief : �����`�̒��_�o�b�t�@*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		
		/*! @brief �����`�̃C���f�b�N�X�o�b�t�@*/
		IndexBufferPtr  _indexBuffer = nullptr;
		
		/*! @brief �}�N���̎g�p (�J���}��؂��)*/
		gu::DynamicArray<gu::tstring> _useMacros = {};

		/*! @brief �O���t�B�b�N�X�p�C�v���C��*/
		PipelineStatePtr  _pipeline       = nullptr;

		/*! @brief �R���s���[�g�p�C�v���C��*/
		ComputePipelineStatePtr _computePipeline = nullptr;

		/*! @brief GPU�̃��\�[�X�̃o�C���h���@��ݒ肷�郊�\�[�X���C�A�E�g*/
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		/*! @brief GPU�̃��\�[�X�r���[*/
		gu::DynamicArray<ResourceViewPtr> _resourceViews = {};

		/* @brief : ��*/
		gu::uint32 _width  = 0;

		/* @brief : ����*/
		gu::uint32 _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		#pragma endregion
	};
}
#endif