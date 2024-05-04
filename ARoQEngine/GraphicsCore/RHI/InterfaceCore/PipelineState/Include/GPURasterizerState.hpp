//////////////////////////////////////////////////////////////////////////////////
///  @file   GPURasterizerState.hpp
///  @brief  �s�N�Z����`�悷�郉�X�^���C�U�X�e�[�g�ɂ�����ݒ荀�ڂ��L�q����N���X�ł�. 
///  @author Toide Yutaro
///  @date   2024_05_03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RASTERIZER_STATE_HPP
#define GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
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
	*				  			GPURasterizerState
	*************************************************************************//**
	/*  @brief  �s�N�Z����`�悷�郉�X�^���C�U�X�e�[�g�ɂ�����ݒ荀�ڂ��L�q����N���X�ł�. 
	*****************************************************************************/
	class GPURasterizerState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief   ���X�^���C�U�̐ݒ荀�ڂ�S�ĕԂ��܂�.
		*************************************************************************/
		const RasterizerProperty& GetProperty() const { return _property; }

		/*!**********************************************************************
		*  @brief   �O�ʂ������v��肩���v��肩�����肵�܂�.
		*************************************************************************/
		__forceinline FrontFace GetFrontFace  () const noexcept { return _property.FaceType; }

		/*!**********************************************************************
		*  @brief   �w�肳�ꂽ��(�\��, ����)�ɑ΂���O�p�`�|���S���̕`��ۂ�Ԃ��܂�. 
		*  @return  None : �\�������`��, Front : ���̂ݕ`��, Back : �\�̂ݕ`��
		*************************************************************************/
		__forceinline CullingMode GetCullingMode() const noexcept { return _property.CullingType; }

		/*!**********************************************************************
		*  @brief   �|���S���̎O�p�`�����̕`��ݒ���@��Ԃ��܂�.
		*  @return  Solid : �O�p�`������`��, WireFrame : ���̂ݕ`��, Point : ���_�����`�� [Vulkan�̂�]
		*************************************************************************/
		__forceinline FillMode GetFillMode() const noexcept { return _property.FillType; }

		/*!**********************************************************************
		*  @brief   �[�x�̑傫���ɉ�����Clamp����(�s�N�Z���̕`����s��Ȃ�)�����s���Ă��邩���m�F���܂�.
		*************************************************************************/
		__forceinline bool UseDepthClamp() const noexcept { return _property.UseDepthClamp; }

		/*!**********************************************************************
		*  @brief   ���X�^���C�Y�ɂ�����, �}���`�T���v�����O���L���ɂȂ��Ă��邩���m�F���܂�.
		*************************************************************************/
		__forceinline bool UseMultiSample() const { return _property.UseMultiSample; }

		/*!**********************************************************************
		*  @brief   DirectX12�݂̂Őݒ�\�ł�. ���̃A���`�G�C���A�X���L��������Ă��邩���m�F���܂�.
		*  @return  true�ŗL�����ݒ肪on�ƂȂ��Ă��܂�. (������, DirectX12�݂̂��������l�������܂�.)
		*************************************************************************/
		__forceinline bool UseAntiAliasLine() const { return _property.UseAntiAliasLine; }

		/*!**********************************************************************
		*  @brief   �����ł��s�N�Z���̕`��͈͂ɓ������烉�X�^���C�Y�����s����@�\���L��������Ă��邩���m�F���܂�
		*************************************************************************/
		__forceinline bool UseConservativeRaster() const { return _property.UseConservativeRaster; }

		/*!**********************************************************************
		*  @brief   �^����ꂽ�s�N�Z���ɉ�������Depth�l��Ԃ��܂�. 
		*  @note    �v�Z���@ : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetDepthBias() const { return _property.DepthBias; }

		/*!**********************************************************************
		*  @brief    MaxDepthSlope�ɑ΂���␳���̒l��Ԃ��܂�.
		*  @note     �v�Z���@ : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetSlopeScaleDepthBias() const { return _property.SlopeScaleDepthBias; }

		/*!**********************************************************************
		*  @brief   �ŏI�I��Bias�ɑ΂���Clamp�����Bias�l��Ԃ��܂�.
		*  @note    �v�Z���@ : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		*************************************************************************/
		__forceinline float GetClampMaxDepthBias() const { return _property.ClampMaxDepthBias; }
		#pragma endregion 

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPURasterizerState() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~GPURasterizerState() = default;

		/*! @brief RasterizerProperty���g���č쐬���܂�. */
		explicit GPURasterizerState(
			const gu::SharedPointer<RHIDevice>& device,
			const RasterizerProperty& rasterizerProperty) 
			: GPUState(device), _property(rasterizerProperty) { };
	
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �V�F�[�_�p�C�v���C���ɂ�����, ���X�^���C�U�X�e�[�g�Ɋւ���ݒ荀�ڂł�.*/
		RasterizerProperty _property = RasterizerProperty();

		#pragma endregion

	};

}
#endif