//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBlendState.hpp
///  @brief  �`�掞��, �ǂ̂悤�ɏ������ޑO�̃����_�[�^�[�Q�b�g�Ə������ސ�̃����_�[�^�[�Q�b�g�̐F���������邩���`����State�ł�@n
///          �}���`�����_�[�^�[�Q�b�g�ɂ��Ή��\�ł� (�ő吔��8)
///  @author Toide Yutaro
///  @date   2024_05_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BLEND_STATE_HPP
#define GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	*				  			GPUBlendState
	*************************************************************************//**
	/* @brief  �`�掞��, �ǂ̂悤�ɏ������ޑO�̃����_�[�^�[�Q�b�g�Ə������ސ�̃����_�[�^�[�Q�b�g�̐F���������邩���`����State�ł�@n
	*          �}���`�����_�[�^�[�Q�b�g�ɂ��Ή��\�ł� (�ő吔��8)
	*****************************************************************************/
	class GPUBlendState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion
		
		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief  �@�@�`�掞�̊�{�ݒ��Ԃ��܂�.
		*  @param[in] const gu::uint32 �z��̃C���f�b�N�X (��{��0��, �ő��8�ł��肢���܂�.)
		*************************************************************************/
		const core::BlendProperty& GetProperty(const gu::uint32 index = 0) { return _blendProperties[index]; }

		/*!**********************************************************************
		*  @brief  �@�@BlendProperty�̗v�f�����擾���܂�
		*************************************************************************/
		const gu::uint64 Size() const { return _blendProperties.Size(); }

		/*!**********************************************************************
		*  @brief  �@�@�u�����f�B���O��L�������Ă���ꍇtrue�ɂȂ�܂�
		*  @return    �ʐݒ�̏ꍇ��index�̒l��, ���ʐݒ�̏ꍇ��0�̒l��BlendProperty�̃u�����f�B���O�L�����ݒ肪�m�F�ł��܂�.
		*************************************************************************/
		__forceinline bool EnableBlend(const gu::uint32 index) const 
		{
			Check(index < _blendProperties.Size());
			return _isIndependentBlendEnable ? _blendProperties[index].EnableBlend :_blendProperties[0].EnableBlend;
		}

		/*!**********************************************************************
		*  @brief  �@�@�u�����f�B���O��L�������Ă���ꍇtrue�ɂȂ�܂�
		*  @return    �ʐݒ�̏ꍇ��index�̒l��, ���ʐݒ�̏ꍇ��0�̒l��BlendProperty�̃u�����f�B���O�L�����ݒ肪�m�F�ł��܂�.
		*************************************************************************/
		__forceinline ColorMask GetColorMask(const gu::uint32 index) const
		{
			Check(index < _blendProperties.Size());
			return _isIndependentBlendEnable ? _blendProperties[index].ColorMask : _blendProperties[0].ColorMask;
		}

		/*!**********************************************************************
		*  @brief  �@�@�s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\���L��������Ă��邩
		*  @return    true�ŗL�����ς�
		*************************************************************************/
		__forceinline bool UseAlphaToCoverage() const { return _alphaToConverageEnable; }

		/*!**********************************************************************
		*  @brief  �@�@0�`7�܂ł̃����_�[�^�[�Q�b�g�ɑ΂��ČʂɃu�����h�ݒ���s����
		*  @return    true�ŌʂɃu�����h�ݒ���s��, false��property[0]�̐ݒ�����ʂ̂��̂Ƃ��Ē�`���܂�.
		*************************************************************************/
		__forceinline bool IsIndependentBlendEnable() const { return _isIndependentBlendEnable; }

		#pragma endregion

		#pragma	region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBlendState() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~GPUBlendState();

		/*!**********************************************************************
		*  @brief     �ʂɐݒ肷������̃u�����h�X�e�[�g
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& �_���f�o�C�X
		*  @param[in] const gu::DynamicArray<rhi::core::BlendProperty>& �ʂ̃u�����h�ݒ�
		*  @param[in] const bool �s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\��L����
		*************************************************************************/
		explicit GPUBlendState(
			const gu::SharedPointer<RHIDevice>& device, 
			const gu::DynamicArray<BlendProperty>& properties,
			const bool alphaToCoverageEnable) 
			: GPUState(device), _blendProperties(properties), _isIndependentBlendEnable(true), _alphaToConverageEnable(alphaToCoverageEnable) 
		{

		};
		
		/*!**********************************************************************
		*  @brief     �S�Ă�BlendState�ŋ��ʂ̃v���p�e�B���g�p�������
		*  @param[in] const gu::SharedPointer<rhi::core::RHIDevice>& �_���f�o�C�X
		*  @param[in] const rhi::core::BlendProperty ���ʂ̃u�����h�ݒ�
		*  @param[in] const bool �s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\��L����
		*************************************************************************/
		explicit GPUBlendState(const gu::SharedPointer<RHIDevice>& device, const BlendProperty& blendProperty, const bool alphaToConverageEnable) : GPUState(device),
			_isIndependentBlendEnable(false),
			_alphaToConverageEnable(alphaToConverageEnable)
		{
			_blendProperties.Push(blendProperty);
		}

		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables
		/* @brief : �`�掞�̐ݒ�ł�. Source : ���ꂩ�烌���_�����O����F (�s�N�Z���V�F�[�_�[), Destination : �����_�����O�� (�����_�[�^�[�Q�b�g)*/
		gu::DynamicArray<core::BlendProperty> _blendProperties;
		
		/*! @brief �s�N�Z���V�F�[�_�[����o�͂��ꂽ���������擾��, �}���`�T���v�����O�A���`�G�C���A�X������K�p����@�\��L����*/
		bool _alphaToConverageEnable = false;

		/*! @brief 0�`7�܂ł̃����_�[�^�[�Q�b�g�ɑ΂��ČʂɃu�����h�ݒ���s����*/
		bool _isIndependentBlendEnable = false;
		#pragma endregion
	};

}
#endif