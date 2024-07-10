//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUDepthStencilState.hpp
///  @brief  �s�N�Z���ɑ΂��Đ[�x�e�X�g��X�e���V���e�X�g���s�����߂̐ݒ荀�ڂ��L�q����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_DEPTH_STENCIL_STATE_HPP
#define GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/RHICommonState.hpp"
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
	*				  			GPUDepthStencilState
	****************************************************************************/
	/* @brief   �s�N�Z���ɑ΂��Đ[�x�e�X�g��X�e���V���e�X�g���s�����߂̐ݒ荀�ڂ��L�q����N���X�ł�.
	*****************************************************************************/
	class GPUDepthStencilState : public GPUState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �[�x�e�X�g���s�����ǂ�����Ԃ��܂�. �s�N�Z����`�悷�邩�����肷�邽�߂̃e�X�g�ł�.
		*  @param[in] void
		*  @return    bool : true�Ő[�x�e�X�g���s��.
		*************************************************************************/
		__forceinline bool UseDepthTest() const noexcept { return _property.DepthOperator != rhi::core::CompareOperator::Always || _property.DepthWriteEnable; }

		/*!**********************************************************************
		*  @brief     �`��}�X�N���s���X�e���V���e�X�g���s�����ǂ�����Ԃ��܂�. true�ɂ���ꍇ�͕K��Stencil�ɑΉ�����s�N�Z���t�H�[�}�b�g�ɂ��邱��
		*  @param[in] void
		*  @return    bool : true�ŃX�e���V���e�X�g���s��. 
		*************************************************************************/
		__forceinline bool UseStencilTest() const noexcept { return _property.StencilWriteEnable; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTest���s������Ԃ��܂�. �s���ꍇ�͕K��MinDepthBounds, MaxDepthBounds��ݒ肵�Ă�������.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool UseDepthBoundsTest() const noexcept { return _property.UseDepthBoundsTest; }

		/*!**********************************************************************
		*  @brief     DepthBuffer�ɏ������݂��s������Ԃ��܂�.
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		__forceinline bool GetDepthWriteEnable() const noexcept { return _property.DepthWriteEnable; }

		/*!**********************************************************************
		*  @brief     DepthTest�����s����ۂ̔�r���Z�q��Ԃ��܂�. LessEqual�Ƃ��ł���΍��񏑂����ރs�N�Z���̐[�x�l���ȑO�̒l�ȉ��ł���ꍇ�ɕ`�悵�܂�.
		*  @param[in] void
		*  @return    const CompareOperator
		*************************************************************************/
		__forceinline CompareOperator GetDepthOperator() const noexcept { return _property.DepthOperator; }
		
		/*!**********************************************************************
		*  @brief     FrontFace�̃X�e���V���e�X�g�̐ݒ��Ԃ��܂�.
		*  @param[in] void
		*  @return    const StencilOperatorInfo
		*************************************************************************/
		__forceinline const StencilOperatorInfo& GetFrontFaceStencil() const noexcept { return _property.Front; }
		
		/*!**********************************************************************
		*  @brief     BackFace�̃X�e���V���e�X�g�̐ݒ��Ԃ��܂�.
		*  @param[in] void
		*  @return    const StencilOperatorInfo
		*************************************************************************/
		__forceinline const StencilOperatorInfo& GetBackFaceStencil () const noexcept { return _property.Back; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTest�̍ŏ��f�v�X�l��Ԃ��܂�. 
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		__forceinline float GetMinDepthBounds() const noexcept { return _property.MinDepthBounds; }

		/*!**********************************************************************
		*  @brief     DepthBoundsTest�̍ő�f�v�X�l��Ԃ��܂�.
		*  @param[in] void
		*  @return    float
		*************************************************************************/
		__forceinline float GetMaxDepthBounds() const noexcept { return _property.MaxDepthBounds; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUDepthStencilState() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~GPUDepthStencilState() = default;

		/*! @brief DepthStencilProperty���g���č쐬���܂�. */
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const DepthStencilProperty& depthStencilProperty
		) : GPUState(device), _property(depthStencilProperty){ };

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief �f�v�X�X�e���V���̐ݒ荀�� */
		DepthStencilProperty _property = DepthStencilProperty();
		#pragma endregion
	};
}
#endif