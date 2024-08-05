//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RenderPass.hpp
///  @brief  �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���ɂ�����FrameBuffer�̏������݊J�n��, �I�����̐ݒ荀��
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RENDER_PASS_HPP
#define DIRECTX12_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "DirectX12Core.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			RHIRenderPass
	****************************************************************************/
	/* @brief     �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���ɂ�����FrameBuffer�̏������݊J�n��, �I�����̐ݒ荀��
	*****************************************************************************/
	class RHIRenderPass : public  rhi::core::RHIRenderPass
	{
	public:
		#pragma region Public Function

		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief     �����̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����ꂼ��̃N���A�J���[��ݒ肵�܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override { _name = name; };

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIRenderPass() = default;

		/*! @brief �f�X�g���N�^*/
		~RHIRenderPass() = default;

		/*! @brief �e�����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̏�����*/
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth = {}) : 
			core::RHIRenderPass(device, colors, depth){}

		/*! @brief ��̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����g����������*/
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const core::Attachment& color, const gu::Optional<core::Attachment>& depth = {}) :
			core::RHIRenderPass(device, color, depth){};

		#pragma endregion

	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		gu::tstring _name = L"";
		#pragma endregion
	};
}
#endif