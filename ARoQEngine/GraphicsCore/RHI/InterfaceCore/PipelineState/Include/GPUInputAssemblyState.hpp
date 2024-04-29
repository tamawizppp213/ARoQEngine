//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUInputAssemblyState.hpp
///  @brief  InputAssembly State
///  @author Toide Yutaro
///  @date   2024_04_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_INPUT_ASSEMBLY_STATE_HPP
#define GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GPUState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

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
	*				  			RHIInputAssemblyState
	*************************************************************************//**
	/*  @brief     ���_�V�F�[�_�̓��͂��ǂ̂悤�ȃf�[�^�\���ł��邩��ݒ肵�܂�. 
	*****************************************************************************/
	class GPUInputAssemblyState : public GPUState
	{
	public:
		#pragma region Static Function
		/*!**********************************************************************
		*  @brief     �P����3D�|���S���p�̒��_�f�[�^�̃��C�A�E�g
		*  @note      �V����Vertex��GameCore�Œ�`����ꍇ�͂��̊֐�����菜��, �ړ�����. 
		*  @return    gu::DynamicArray<InputLayoutElement>
		*************************************************************************/
		static gu::DynamicArray<InputLayoutElement> GetDefaultVertexElement()     { return DEFAULT_VERTEX_ELEMENTS; }

		/*!**********************************************************************
		*  @brief     �P���ȃX�L�����b�V���p�̒��_�f�[�^�̃��C�A�E�g
		*  @note      �V����Vertex��GameCore�Œ�`����ꍇ�͂��̊֐�����菜��, �ړ�����.
		*  @return    gu::DynamicArray<InputLayoutElement>
		*************************************************************************/
		static gu::DynamicArray<InputLayoutElement> GetDefaultSkinVertexElement() { return DEFAULT_SKINVERTEX_ELEMENTS; }
		#pragma endregion 
		
		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     ���̓��C�A�E�g�ɂ�����, �قȂ�l�����X���b�g�������邩�������܂�
		*  @return    gu::uint8 ���̓��C�A�E�g�̌��ł� (0�`15�܂ł����Ȃ�����, gu::uint8�ł�)
		*************************************************************************/
		gu::uint8 GetSlotCount() const noexcept { return _slotCount; }
		
		/*!**********************************************************************
		*  @brief     ���̓��C�A�E�g�̊e�v�f�ɃA�N�Z�X���܂�. 
		*  @param[in] ���̓��C�A�E�g�̗v�f�C���f�b�N�X (�͈͊O�`�F�b�N�͍s���Ă���܂���.)
		*  @return    const InputLayoutElement& ���̓��C�A�E�g, �O����̕ύX�͍s���܂���
		*************************************************************************/
		const InputLayoutElement& GetElement(const gu::uint32 index) const { return _elements[index]; }
		
		/*!**********************************************************************
		*  @brief     ���̓��C�A�E�g�̔z��f�[�^�ɃA�N�Z�X���܂�
		*  @return    const gu::DynamicArray<InputLayoutElement ���̓��C�A�E�g�̔z��
		*************************************************************************/
		const gu::DynamicArray<InputLayoutElement>& GetElements() const { return _elements; }
		
		/*!**********************************************************************
		*  @brief     �אڂ���e���_���m�̐ڑ����@���w�肷��Enum��Ԃ��܂�. 
		*  @return    PrimitiveTopology �אڒ��_�̐ڑ����@.
		*************************************************************************/
		PrimitiveTopology GetPrimitiveTopology() const { return _primitiveTopology; }
		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUInputAssemblyState() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUInputAssemblyState() = default;

		/*! @brief ��{�I��InputAssembly Stage�ō쐬����R���X�g���N�^*/
		explicit GPUInputAssemblyState(
			const gu::SharedPointer<RHIDevice>& device,
			const gu::DynamicArray<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList
		) : GPUState(device), _elements(elements), _primitiveTopology(primitiveTopology)
		{
			for (const auto& element : _elements)
			{
				const gu::uint8 nextSlot = element.Slot + 1;
				_slotCount = nextSlot >= _slotCount ? nextSlot : _slotCount;
			}
		}

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables

		/*! @brief ���̓��C�A�E�g�̐ݒ�f�[�^�ł�.*/
		gu::DynamicArray<InputLayoutElement> _elements = {};

		/*! @brief ���̓��C�A�E�g�ɂ�����, �قȂ�l�����X���b�g�������邩�������܂�. @n
		           gu::DynamicArray��Size�Őݒ肵�Ȃ��̂�, �����X���b�g���g�p����\�������邽�߂ł�.*/
		gu::uint8 _slotCount = 1;

		/*! @brief �אڂ���e���_���m�̐ڑ����@���w�肵�܂�. �f�t�H���g��TriangleList�ł�.*/
		core::PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

		#pragma endregion 

	private:
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_VERTEX_ELEMENTS;
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_SKINVERTEX_ELEMENTS;
	};

}
#endif