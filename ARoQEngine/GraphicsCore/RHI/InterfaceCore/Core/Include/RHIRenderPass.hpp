//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIRenderPass.hpp
///  @brief  Render Pass (for use vulkan)
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RENDER_PASS_HPP
#define RHI_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIFrameBuffer;
	/****************************************************************************
	*				  			RHIRenderPass
	****************************************************************************/
	/* @brief �@�����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V����FrameBuffer���������݊J�n��, �I�����̐ݒ荀��
	*****************************************************************************/
	class RHIRenderPass : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����ꂼ��̃N���A�J���[��ݒ肵�܂�. 
		*  @param[in] const gu::Optional<ClearValue>& color �����_�[�^�[�Q�b�g�̃N���A�J���[
		*  @param[in] const gu::Optional<ClearValue>& depth �f�v�X�X�e���V���̃N���A�J���[
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    void
		*************************************************************************/
		void SetClearValue(const gu::Optional<ClearValue>& color, const gu::Optional<ClearValue>& depth = {}, const gu::uint32 index = 0)
		{
			if (color.HasValue())
			{
				_colorClearValues[index] = color.Value();
			}
			_depthClearValue = depth; // optional
		}

		/*!**********************************************************************
		*  @brief     �����̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����ꂼ��̃N���A�J���[��ݒ肵�܂�.
		*  @param[in] const gu::DynamicArray<ClearValue>& colors �����_�[�^�[�Q�b�g�̃N���A�J���[
		*  @param[in] const gu::Optional<ClearValue>&     depth �f�v�X�X�e���V���̃N���A�J���[
		*  @return    void
		*************************************************************************/
		void SetClearValue(const gu::DynamicArray<ClearValue>& colors, const gu::Optional<ClearValue>& depth = {})
		{
			_colorClearValues = colors;
			_depthClearValue  = depth;
		}

		/*!**********************************************************************
		*  @brief     �F�A�^�b�`�����g�̐��́A�����_�[�^�[�Q�b�g�̐��ȏ�łȂ���΂Ȃ�Ȃ�. �[�x�X�e���V�������݂���ꍇ�A�[�x�A�^�b�`�����g��NULL�ɂ��邱�Ƃ͂ł��Ȃ��B
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Compatible(const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) const;
	
		/*!**********************************************************************
		*  @brief     �}���`�T���v�������擾���܂�.
		*  @param[in] void
		*  @return    MultiSample �}���`�T���v����
		*************************************************************************/
		__forceinline MultiSample GetMaxSample() const noexcept { return _maxSample; }
		
		/*!**********************************************************************
		*  @brief    �@�����_�[�^�[�Q�b�g���̃N���A�J���[���擾���܂�.
		*  @param[in] void
		*  @return    gu::DynamicArray<ClearValue> �����_�[�^�[�Q�b�g�̃N���A�J���[
		*************************************************************************/
		__forceinline const gu::DynamicArray<ClearValue>& GetClearColor() const noexcept { return _colorClearValues; }
		
		/*!**********************************************************************
		*  @brief    �@�f�v�X�X�e���V�����̃N���A�J���[���擾���܂�.
		*  @param[in] void
		*  @return    gu::Optional<ClearColor> �f�v�X�X�e���V���̃N���A�J���[
		*************************************************************************/
		__forceinline gu::Optional<ClearValue> GetDepthClear() const noexcept { return _depthClearValue; }

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���̃A�^�b�`�����g���擾���܂�.
		*  @param[in] void
		*  @return    gu::Optional<Attachment> �����_�[�^�[�Q�b�g�̃A�^�b�`�����g
		*************************************************************************/
		__forceinline gu::Optional<Attachment> GetColorAttachment(const gu::uint32 index = 0) const { return _colorAttachments[index]; }
	
		/*!**********************************************************************
		*  @brief     �f�v�X�X�e���V�����̃A�^�b�`�����g���擾���܂�.
		*  @param[in] void
		*  @return    gu::Optional<Attachment> �f�v�X�X�e���V���̃A�^�b�`�����g
		*************************************************************************/
		__forceinline gu::Optional<Attachment> GetDepthAttachment() const noexcept { return _depthAttachment; }
		
		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���̃A�^�b�`�����g�̐����擾���܂�.
		*  @param[in] void
		*  @return    gu::uint32 �����_�[�^�[�Q�b�g�̃A�^�b�`�����g�̐�
		*************************************************************************/
		__forceinline gu::uint32 GetColorAttachmentSize() const noexcept { return static_cast<gu::uint32>(_colorAttachments.Size()); }

		/*!**********************************************************************
		*  @brief     �����̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����ꂼ��̃N���A�J���[��ݒ肵�܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion

		#pragma region Public Constructor and Destructor

		#pragma endregion
		
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIRenderPass() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHIRenderPass()
		{
			_depthClearValue.Reset();
			_depthAttachment.Reset();
			_colorClearValues.Clear(); _colorClearValues.ShrinkToFit();
			_colorAttachments.Clear(); _colorAttachments.ShrinkToFit();

			if (_device) { _device.Reset(); }
		}

		/*! @brief �e�����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̏�����*/
		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const gu::DynamicArray<Attachment>& colors, const gu::Optional<Attachment>& depth = {});
		
		/*! @brief ��̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����g����������*/
		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const Attachment& color, const gu::Optional<Attachment>& depth = {});

		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/* @brief �����_�[�^�[�Q�b�g�̏������ݐݒ荀��*/
		gu::DynamicArray<Attachment> _colorAttachments = {};

		/*! @brief �����_�[�^�[�Q�b�g�̃N���A���ɏ����������F*/
		gu::DynamicArray<ClearValue> _colorClearValues = {};

		/*! @brief �f�v�X�X�e���V���̏������ݎ��ݒ荀��*/
		gu::Optional<Attachment> _depthAttachment;

		/*! @brief �f�v�X�X�e���V���̃N���A���ɏ����������F*/
		gu::Optional<ClearValue> _depthClearValue;

		/*! @brief �}���`�T���v����*/
		MultiSample _maxSample = MultiSample::Count1;
		#pragma endregion 
	};
}
#endif