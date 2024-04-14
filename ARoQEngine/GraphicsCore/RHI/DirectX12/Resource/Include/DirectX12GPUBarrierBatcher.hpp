//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUBarrierBatcher.hpp
///  @brief  temp
///  @author toide
///  @date   2024/04/13 23:32:27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BARRIER_BATCHER_HPP
#define  DIRECTX12_GPU_BARRIER_BATCHER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	class RHICommandList;
	
	/****************************************************************************
	*				  		GPUBarrierBatcher
	*************************************************************************//**
	/*  @class     GPUBarrierBatcher
	*   @brief     temp
	*****************************************************************************/
	class GPUBarrierBatcher : public gu::NonCopyable
	{
	private :
		struct GPUBarrier : public D3D12_RESOURCE_BARRIER
		{
			operator const D3D12_RESOURCE_BARRIER& () const { return *this; }
		};

	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
		*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�. 
		*  @param[in] ID3D12Resource* GPU���\�[�X
		*  @param[in] const D3D12_RESOURCE_STATES �J�ڑO�̃��\�[�X���
		*  @param[in] const D3D12_RESOURCE_STATES �J�ڌ�̃��\�[�X���
		*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�)
		*  @return    void
		*************************************************************************/
		void PushTransitionBarrier(ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after, const gu::uint32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

		/*!**********************************************************************
		*  @brief  �����������̈�Ƀ}�b�s���O���ꂽ������GPU���\�[�X�ɑ΂�, �g�p���郊�\�[�X���̂�؂�ւ���ۂɎg�p���܂�. @n
		*          �����Ɏg�p���Ȃ����Ƃ��S�ۂ���Ă��郊�\�[�X�̃�������ߖ񂷂邱�Ƃ��\�ƂȂ�܂�. @n
		*          ������, �{�֐����g�p����ꍇ��,CreateCommittedResource�ł͖���, CreatePlacedResource���g�p�������@�Ńq�[�v�̊m�ۂ��s���Ă�������.@n
		*          (�ʁX�̃q�[�v������Ă��܂����ƂɂȂ�, �����������̈������Ȃ��Ȃ邽��.)
		*  @note   https://logicalbeat.jp/blog/8185/ (AliasingBarrier�̊��p���@�ɂ��Ă̋L�q)
		*  @param[in] ID3D12Resource* �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
		*  @param[in] ID3D12Resource* �؂�ւ�����Ɏg�p����\���GPU���\�[�X
		*  @return    void
		*************************************************************************/
		void PushAliasingBarrier(ID3D12Resource* beforeResource, ID3D12Resource* afterResource);

		/*!**********************************************************************
		*  @brief  Unordered access view��p�̏�ԃo���A @n
		*          UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
		*  @param[in] ID3D12Resource* Unordered access view������GPU���\�[�X
		*  @return    void
		*************************************************************************/
		void PushUAVBarrier(ID3D12Resource* resource);

		/*!**********************************************************************
		*  @brief  �R�}���h���X�g���g����ResourceBarrier���܂Ƃ߂ČĂяo���܂�. 
		*  @param[in] ICommandList* �`�施�߂����s����R�}���h���X�g
		*  @return    void
		*************************************************************************/
		void Flush(ICommandList* commandList);

		/*!**********************************************************************
		*  @brief  ���݂��܂��Ă���o���A�̌�
		*************************************************************************/
		__forceinline gu::uint64 Count() const { return _barriers.Size(); }

		#pragma endregion 

		#pragma region Public Member Variables

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBarrierBatcher() = default;

		~GPUBarrierBatcher()
		{
			_barriers.Clear();
			_barriers.ShrinkToFit();
		}
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables
		/*! @brief GPU�̃o���A�𒙂߂Ă����ꏊ*/
		gu::DynamicArray<GPUBarrier> _barriers = {};
		#pragma endregion 

	};

}
#endif