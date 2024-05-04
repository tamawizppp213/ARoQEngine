//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12BlendState.hpp
///  @brief  �`�掞��, �ǂ̂悤�ɏ������ޑO�̃����_�[�^�[�Q�b�g�Ə������ސ�̃����_�[�^�[�Q�b�g�̐F���������邩���`����State�ł�@n
///          �}���`�����_�[�^�[�Q�b�g�ɂ��Ή��\�ł�(�ő吔��8)
///  @author Toide Yutaro
///  @date   2024_05_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BLEND_STATE_HPP
#define DIRECTX12_GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUBlendState.hpp"
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
	/*  @brief  �`�掞��, �ǂ̂悤�ɏ������ޑO�̃����_�[�^�[�Q�b�g�Ə������ސ�̃����_�[�^�[�Q�b�g�̐F���������邩���`����State�ł�@n
	*           �}���`�����_�[�^�[�Q�b�g�ɂ��Ή��\�ł� (�ő吔��8)
	*****************************************************************************/
	class GPUBlendState : public rhi::core::GPUBlendState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief  �@�@DirectX12�̃u�����h�X�e�[�g�ݒ�
		*************************************************************************/
		const D3D12_BLEND_DESC& GetDxBlendState() const { return _blendState; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBlendState() = default;
		
		/*! @brief �f�X�g���N�^*/
		~GPUBlendState() = default;
		
		/*!**********************************************************************
		*  @brief     �ʂɐݒ肷������̃u�����h�X�e�[�g
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& �_���f�o�C�X
		*  @param[in] const gu::DynamicArray<rhi::core::BlendProperty>& �ʂ̃u�����h�ݒ�
		*  @param[in] const bool �s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\��L����
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::DynamicArray<rhi::core::BlendProperty>& blendProperties, const bool alphaToCoverageEnable);
		
		/*!**********************************************************************
		*  @brief     �S�Ă�BlendState�ŋ��ʂ̃v���p�e�B���g�p�������
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& �_���f�o�C�X
		*  @param[in] const rhi::core::BlendProperty ���ʂ̃u�����h�ݒ�
		*  @param[in] const bool �s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\��L����
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<rhi::core::RHIDevice>& device, const rhi::core::BlendProperty& blendProperty, const bool alphaToCoverageEnable);
		#pragma endregion
	
	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief DirectX12�p�̃u�����h�X�e�[�g*/
		D3D12_BLEND_DESC _blendState = {};

		#pragma endregion
	};
}
#endif