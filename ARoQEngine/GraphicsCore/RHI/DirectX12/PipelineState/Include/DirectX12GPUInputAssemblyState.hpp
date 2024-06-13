//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUInputAssemblyState.hpp
///  @brief  ���_�V�F�[�_�̓��͂��ǂ̂悤�ȃf�[�^�\���ł��邩��ݒ肵�܂�. 
///  @author Toide Yutaro
///  @date   2024_04_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP
#define DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUInputAssemblyState.hpp"
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
	*				  			GPUInputAssemblyState
	****************************************************************************/
	/* @brief  ���_�V�F�[�_�̓��͂��ǂ̂悤�ȃf�[�^�\���ł��邩��ݒ肵�܂�. 
	*****************************************************************************/
	class GPUInputAssemblyState : public rhi::core::GPUInputAssemblyState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ���̓��C�A�E�g�v�f�����Ǝ��f�[�^�̎Q�Ɛ���w�肷��f�B�X�N���v�^
		*  @return    const D3D12_INPUT_LAYOUT_DESC : ���̓��C�A�E�g�v�f�̎Q�Ə��
		*************************************************************************/
		const D3D12_INPUT_LAYOUT_DESC& GetDxLayoutDesc() const noexcept { return _inputLayout; }
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUInputAssemblyState() = default;
		
		/*! @brief �f�t�H���g�f�X�g���N�^*/
		~GPUInputAssemblyState() = default;
		
		/*! @brief �_���f�o�C�X�Ɗe���̓��C�A�E�g����PrimitiveTopology���g���č쐬���܂�. */
		explicit GPUInputAssemblyState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device, 
			const gu::DynamicArray<core::InputLayoutElement>& elements, 
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList);

		#pragma endregion

	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Function

		/*! @brief D3D12_INPUT_LAYOUT_DESC�̎��f�[�^. */
		gu::DynamicArray<D3D12_INPUT_ELEMENT_DESC> _inputLayoutElements = {};

		/*! @brief ���̓��C�A�E�g�v�f�����Ǝ��f�[�^�̎Q�Ɛ���w�肷��f�B�X�N���v�^*/
		D3D12_INPUT_LAYOUT_DESC _inputLayout = {nullptr, 0};
		
#pragma endregion
	};
}
#endif