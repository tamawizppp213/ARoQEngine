//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12Adapter.hpp
///  @brief  �_���f�o�C�X�ɓn�������f�o�C�X(Apdapter)�̐ݒ�, GPU�����擾
///  @author Toide Yutaro
///  @date   2024_03_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_ADAPTER_HPP
#define DIRECTX12_RHI_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	/* @class     RHIDisplayAdapter
	*  @brief     �_���f�o�C�X�ɓn�������f�o�C�X(Apdapter)�̐ݒ�, GPU�����擾
	*****************************************************************************/
	class RHIDisplayAdapter : public core::RHIDisplayAdapter, public gu::EnableSharedFromThis<RHIDisplayAdapter>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief ���g�̕����f�o�C�X�Ɋ�Â��Ę_���f�o�C�X�𐶐���, ����SharedPointer��n���܂�.
		*  @param[in] void
		*  @return gu::SharedPointer<core::RHIDevice> �_���f�o�C�X�̃|�C���^
		*************************************************************************/
		gu::SharedPointer<core::RHIDevice> CreateDevice() override;

		/*!**********************************************************************
		*  @brief �����f�o�C�X�̖��O�ƃX�y�b�N���o�͂ɕ\�����܂�@n
		*         ��{�I�Ɏ��s���̃��O�Ƃ��Ďg�p������̂ɂȂ�܂�. @n
		*         �t�@�C���╶����ɏo�͍͂s��Ȃ��ł�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void PrintInfo() const override;

		#pragma endregion Public Function

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �A�_�v�^�ɒ��ڐڑ�����Ă���o�͂̐� (���j�^�[�Ȃ�)��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::uint64 �A�_�v�^�ɒ��ڂ���Ă���o�͂̐�
		*************************************************************************/
		virtual gu::uint64 GetOutputCount() const override;

		/*!**********************************************************************
		*  @brief  DirectX12�Ŏg�p����IAdapter��Comptr��Ԃ��܂�.
		*  @param[in] void
		*  @return    �����f�o�C�X��Com�|�C���^
		*************************************************************************/
		AdapterComPtr GetAdapter() { return _adapter; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief Default Constructor*/
		RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const AdapterComPtr& adapter);

		/*! @brief Destructor */
		~RHIDisplayAdapter();

		#pragma endregion 

	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/* @brief : DirectX12�Ŏg�p����Adapter�̃|�C���^*/
		AdapterComPtr _adapter = nullptr;		
		#pragma endregion Protected Property
	};
}

#endif