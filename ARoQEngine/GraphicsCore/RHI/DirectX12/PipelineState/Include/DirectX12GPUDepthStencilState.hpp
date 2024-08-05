//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUDepthStencilState.hpp
///  @brief  �s�N�Z���ɑ΂��Đ[�x�e�X�g��X�e���V���e�X�g���s�����߂̐ݒ荀�ڂ��L�q����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP
#define DIRECTX12_GPU_DEPTH_STENCIL_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUDepthStencilState.hpp"
#define COM_NO_WINDOWS_H
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
	*				  			GPUDepthStencilState
	****************************************************************************/
	/* @brief  �s�N�Z���ɑ΂��Đ[�x�e�X�g��X�e���V���e�X�g���s�����߂̐ݒ荀�ڂ��L�q����N���X�ł�.
	*****************************************************************************/
	class GPUDepthStencilState : public rhi::core::GPUDepthStencilState
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12��p�̐ݒ荀�ڂ�Ԃ��܂�
		*  @param[in] void
		*  @return    const D3D12_DEPTH_STENCIL_DESC& : DirectX12��p�̐ݒ荀��
		*************************************************************************/
		const D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() const noexcept { return _depthStencilDesc.Desc0; }
		
		/*!**********************************************************************
		*  @brief     DirectX12��p�̐ݒ荀�ڂ�Ԃ��܂�
		*  @param[in] void
		*  @return    const D3D12_DEPTH_STENCIL_DESC& : DirectX12��p�̐ݒ荀��
		*************************************************************************/
		const D3D12_DEPTH_STENCIL_DESC1& GetDepthStencilState1() const noexcept { return _depthStencilDesc.Desc1; }
		
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief ���X�^���C�U�̊�{�ݒ��p���č쐬����R���X�g���N�^*/
		explicit GPUDepthStencilState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::DepthStencilProperty& depthStencilProperty
		);

		/*! @brief �f�X�g���N�^*/
		~GPUDepthStencilState() = default;
		
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUDepthStencilState() = default;
		#pragma endregion
	
	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! DirectX12��DepthStencil�ݒ�*/
		union DepthStencilDesc
		{
			D3D12_DEPTH_STENCIL_DESC  Desc0;
			D3D12_DEPTH_STENCIL_DESC1 Desc1;
		} _depthStencilDesc;
		#pragma endregion
	};
}
#endif