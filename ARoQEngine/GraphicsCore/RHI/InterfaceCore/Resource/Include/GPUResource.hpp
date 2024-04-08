//////////////////////////////////////////////////////////////////////////////////
/// @file   GPUResource.hpp
/// @brief  GPU�Ń��������m�ۂ��郊�\�[�X���. �e�N�X�`���ƃo�b�t�@���ꂼ�ꂪ���̃N���X��h�����܂�. 
/// @author Toide Yutaro
/// @date   2024_04_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
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
	*				  			GPUResource
	*************************************************************************//**
	/* @class     GPUResource 
	*  @brief     GPU�Ń��������m�ۂ��郊�\�[�X���. �e�N�X�`���ƃo�b�t�@���ꂼ�ꂪ���̃N���X��h�����܂�. 
	*****************************************************************************/
	class GPUResource : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     GPU�̃��\�[�X��Ԃ�ύX���܂�. 
		*  @attention �����ł̓��\�[�X�̃����o�ϐ���ύX���邾���̂��߂Ɏg�p���邽��, Barrier�Ȃǂƕ��p���Ďg�p���܂�.
		*  @param[in] const core::ResourceState GPU����������������̃��\�[�X���
		*  @return    void
		*************************************************************************/
		virtual void TransitionResourceState(const core::ResourceState after) = 0;

		#pragma endregion

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     �f�o�b�O����ݒ肵�܂�
		*  @param[in] const gu::tstring& �f�o�b�O��
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		/*!**********************************************************************
		*  @brief     �����_��GPUResource�̈����� (IndexBuffer�Ƃ��Ďg�p����Ȃ�...)���擾���܂�
		*  @param[in] void
		*  @return    core::ResourceState ���\�[�X�̈�����
		*************************************************************************/
		virtual core::ResourceState GetResourceState() const noexcept = 0;
		
		/*!**********************************************************************
		*  @brief     �e�N�X�`���Ƃ��Ďg�p���Ă���ꍇ��true��Ԃ��܂�
		*************************************************************************/
		bool IsTexture() const { return _isTexture; }

		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ƃ��Ďg�p���Ă���ꍇ��true��Ԃ��܂�
		*************************************************************************/
		bool IsBuffer() const { return !_isTexture; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion 
		
	protected:
		#pragma region Protected Constructor and Destructor

		/*@brief : �f�t�H���g�R���X�g���N�^*/
		GPUResource() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~GPUResource()
		{
			if (_device) { _device.Reset(); }
		}

		/*! @brief �_���f�o�C�X���g���ď���������R���X�g���N�^*/
		explicit GPUResource(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		#pragma endregion 
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		// @brief �e�N�X�`�����o�b�t�@��
		bool _isTexture = true;
		#pragma endregion
	}; 
}


#endif