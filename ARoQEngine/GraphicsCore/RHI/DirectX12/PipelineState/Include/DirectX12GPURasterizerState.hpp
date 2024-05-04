//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPURasterizerState.hpp
///  @brief  �s�N�Z����`�悷�郉�X�^���C�U�X�e�[�g�ɂ�����ݒ荀�ڂ��L�q����N���X�ł�. 
///  @author Toide Yutaro
///  @date   2024_05_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RASTERIZER_STATE_HPP
#define DIRECTX12_GPU_RASTERIZER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPURasterizerState.hpp"
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
	*				  			GPURasterizerState
	*************************************************************************//**
	/*  @brief     �s�N�Z����`�悷�郉�X�^���C�U�X�e�[�g�ɂ�����ݒ荀�ڂ��L�q����N���X�ł�. 
	*****************************************************************************/
	class GPURasterizerState : public rhi::core::GPURasterizerState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief   DirectX12��p�̐ݒ荀�ڂ�Ԃ��܂�
		*************************************************************************/
		const D3D12_RASTERIZER_DESC& GetDxRasterizerDesc() const noexcept { return _rasterizerState; }
		
		#pragma endregion 
		
		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPURasterizerState() = default;
		
		/*! @brief �f�X�g���N�^*/
		~GPURasterizerState() = default;
		
		/*! @brief ���X�^���C�U�̊�{�ݒ��p���č쐬����R���X�g���N�^*/
		explicit GPURasterizerState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const rhi::core::RasterizerProperty& rasterizerProperty);

		#pragma endregion 

	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief DirectX12��p�̐ݒ荀�� */
		D3D12_RASTERIZER_DESC _rasterizerState = {};

		#pragma endregion
	};
}
#endif