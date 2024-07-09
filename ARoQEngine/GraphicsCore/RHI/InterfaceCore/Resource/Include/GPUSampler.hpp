//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUSampler.hpp
///  @brief  �e�N�X�`���T���v�����O�̕��@�̐ݒ荀�ڂ��L�q����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SAMPLER_HPP
#define GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	*				  			GPUSampler
	****************************************************************************/
	/* @brief   �e�N�X�`���T���v�����O�̕��@�̐ݒ荀�ڂ��L�q����N���X�ł�.
	*****************************************************************************/
	class GPUSampler : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �e�N�X�`���T���v�����O�̐ݒ荀��
		*  @param[in] void
		*  @return    const SamplerInfo& �T���v���[���
		*************************************************************************/
		const SamplerInfo& GetSamplerInfo  () const noexcept { return _samplerInfo; }
		
		/*!**********************************************************************
		*  @brief     �t�B���^�����O�̎��
		*  @param[in] void
		*  @return    FilterOption �t�B���^�����O�̎��
		*************************************************************************/
		FilterOption  GetFilterOption () const noexcept { return _samplerInfo.Filter; }
		
		/*!**********************************************************************
		*  @brief     U�����̃e�N�X�`���A�h���b�V���O���[�h��Ԃ��܂�.
		*  @param[in] void
		*  @return    SamplerAddressMode U�����̃e�N�X�`���A�h���b�V���O���[�h
		*************************************************************************/
		SamplerAddressMode GetAddressModeU () const noexcept { return _samplerInfo.AddressModeU; }
		
		/*!**********************************************************************
		*  @brief     V�����̃e�N�X�`���A�h���b�V���O���[�h��Ԃ��܂�.
		*  @param[in] void
		*  @return    SamplerAddressMode V�����̃e�N�X�`���A�h���b�V���O���[�h
		*************************************************************************/
		SamplerAddressMode GetAddressModeV () const noexcept { return _samplerInfo.AddressModeV; }
		
		/*!**********************************************************************
		*  @brief     W�����̃e�N�X�`���A�h���b�V���O���[�h��Ԃ��܂�.	
		*  @param[in] void
		*  @return    SamplerAddressMode W�����̃e�N�X�`���A�h���b�V���O���[�h
		*************************************************************************/
		SamplerAddressMode GetAddressModeW () const noexcept { return _samplerInfo.AddressModeW; }
		
		/*!**********************************************************************
		*  @brief     1�`16�܂ł̒l�Ŏg�p�����N�����v�l
		*  @param[in] void
		*  @return    gu::uint8 �N�����v�l
		*************************************************************************/
		gu::uint8   GetMaxAnisotropy() const noexcept { return _samplerInfo.MaxAnisotropy; }
		
		/*!**********************************************************************
		*  @brief     �e�N�X�`���̋��E�F��Ԃ��܂�.
		*  @param[in] void
		*  @return    BorderColor �{�[�_�[�J���[
		*************************************************************************/
		BorderColor GetBorderColor() const noexcept { return _samplerInfo.Border; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUSampler() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~GPUSampler() = default;

		/*! @brief �_���f�o�C�X�ƃT���v���[���ŏ�����*/
		explicit GPUSampler(const gu::SharedPointer<RHIDevice>& device, const SamplerInfo& samplerInfo) : _device(device), _samplerInfo(samplerInfo) {};

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief �T���v���[���*/
		SamplerInfo _samplerInfo = {};
		#pragma endregion
	};
}


#endif