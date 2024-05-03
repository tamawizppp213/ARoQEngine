//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUBarrierBatcher.hpp
///  @brief  GPU�̃��\�[�X��Ԃ����S�Ɏg�p���邽�߂̃N���X�ł�. @n
///          GPU���\�[�X�̎g�������ς�����ꍇ�ɂ�PushTransitionBarrier, @n
///          ����GPU�q�[�v�̈�ɈقȂ郊�\�[�X��r���I�Ɏg�p����Ȃ�PushAliasingBarrier @n
///          UnorderedAccessView�̓ǂݏ��������s����������܂łق��̉e����^���Ȃ��Ȃ�PushUAVBarrier�����s���邱��
///  @author toide
///  @date   2024/04/13 23:32:27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BARRIER_BATCHER_HPP
#define DIRECTX12_GPU_BARRIER_BATCHER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUPair.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUResource;
}

namespace rhi::directX12
{
	class RHICommandList;

	/****************************************************************************
	*				  		GPUBarrier
	*************************************************************************//**
	/*  @brief     GPU�̃��\�[�X��r���I�ɏ������s���N���X�ł�. 
	*****************************************************************************/
	struct GPUBarrier : public D3D12_RESOURCE_BARRIER
	{
		/*!**********************************************************************
		*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
		*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�. 
		*  @param[in] ID3D12Resource* GPU���\�[�X (���ꂾ��GPUResource��������̂͌�Ń��\�[�X�̑J�ڌ�̏�Ԃ�������K�v�����邽��)
		*  @param[in] const D3D12_RESOURCE_STATES �J�ڌ�̃��\�[�X���
		*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�)
		*************************************************************************/
		static inline GPUBarrier CreateTransition(ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after, const gu::uint32 subresource = 0xffffffff)
		{
			return 
			{
				D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
				D3D12_RESOURCE_BARRIER_FLAG_NONE,
				{ resource, subresource, before, after }
			};
		}

		/*!**********************************************************************
		*  @brief  �����������̈�Ƀ}�b�s���O���ꂽ������GPU���\�[�X�ɑ΂�, �g�p���郊�\�[�X���̂�؂�ւ���ۂɎg�p���܂�. @n
		*  @param[in] ID3D12Resource* �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
		*  @param[in] ID3D12Resource* �؂�ւ�����Ɏg�p����\���GPU���\�[�X
		*  @return    void
		*************************************************************************/
		static inline GPUBarrier CreateAliasing(ID3D12Resource* before, ID3D12Resource* after)
		{
			GPUBarrier barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Aliasing.pResourceBefore = before;
			barrier.Aliasing.pResourceAfter = after;
			return barrier;
		}

		/*!**********************************************************************
		*  @brief  Unordered access view��p�̏�ԃo���A @n
		*          UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
		*  @param[in] ID3D12Resource* Unordered access view������GPU���\�[�X
		*  @return    void
		*************************************************************************/
		static inline GPUBarrier CreateUAV(ID3D12Resource* resource)
		{
			GPUBarrier barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.UAV.pResource = resource;
			return barrier;
		}

		operator const D3D12_RESOURCE_BARRIER& () const { return *this; }
	};

	/****************************************************************************
	*				  		GPUBarrierBatcher
	*************************************************************************//**
	/*  @class     GPUBarrierBatcher
	*   @brief     GPU�̃��\�[�X��Ԃ����S�Ɏg�p���邽�߂̃N���X�ł�. @n
	*              GPU���\�[�X�̎g�������ς�����ꍇ�ɂ�PushTransitionBarrier, @n
	*              ����GPU�q�[�v�̈�ɈقȂ郊�\�[�X��r���I�Ɏg�p����Ȃ�PushAliasingBarrier @n
	*              UnorderedAccessView�̓ǂݏ��������s����������܂łق��̉e����^���Ȃ��Ȃ�PushUAVBarrier�����s���邱��
	*****************************************************************************/
	class GPUBarrierBatcher : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
		*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�. 
		*  @param[in] gu::SharedPointer<GPUResource>& GPU���\�[�X (���ꂾ��GPUResource��������̂͌�Ń��\�[�X�̑J�ڌ�̏�Ԃ�������K�v�����邽��)
		*  @param[in] const D3D12_RESOURCE_STATES �J�ڌ�̃��\�[�X���
		*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�)
		*  @return    bool Barrier���o�^���ꂽ��true
		*************************************************************************/
		bool PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

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

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBarrierBatcher() : _barriers(), _afterTransitions()
		{
			// �������̈�����O�Ɋm�ۂ��邱�Ƃō�����
			_barriers.Reserve(8);
			_afterTransitions.Reserve(8);
		}

		~GPUBarrierBatcher()
		{
			_barriers.Clear();
			_barriers.ShrinkToFit();
			_afterTransitions.Clear();
			_afterTransitions.ShrinkToFit();
		}
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		/*! @brief GPU�̃o���A�𒙂߂Ă����ꏊ*/
		gu::DynamicArray<GPUBarrier> _barriers = {};

		gu::DynamicArray<gu::Pair<core::ResourceState, gu::SharedPointer<core::GPUResource>>> _afterTransitions = {};
		#pragma endregion 

	};

}
#endif