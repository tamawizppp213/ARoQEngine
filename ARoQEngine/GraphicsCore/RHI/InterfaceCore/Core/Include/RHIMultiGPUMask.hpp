//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIMultiGPUMask.hpp
///  @brief  GPU Mask : 1��Loginal device�ŕ�����GPU�������Ƃ��Ɏg�p���܂�. @n
///          DirectX12��NodeMask���g���Ƃ��Ɏg�p���܂�
///  @author toide
///  @date   2023/12/29 23:57:52
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_MULTI_GPU_MASK_HPP
#define RHI_MULTI_GPU_MASK_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_DESKTOP
	#define WITH_MULTI_GPU 1
#else
	#define WITH_MULTI_GPU 0
#endif
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	/****************************************************************************
	*				  			   RHIMultiGPU
	*************************************************************************//**
	*  @struct    RHIMultiGPU
	*  @brief     �e�r�b�g��GPU�C���f�b�N�X�ł���}�X�N�ł�. @n
	              SLI�łȂ��v���b�g�t�H�[�������1�ɂȂ�悤�ɍœK���ł���悤�ɁA��ɂ͂ł��܂���
	*****************************************************************************/
	struct RHIMultiGPUMask
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �P���GPU���g�p���܂� (node mask = 1)
		*************************************************************************/
		static const RHIMultiGPUMask SingleGPU() { return RHIMultiGPUMask(1); }

		/*!**********************************************************************
		*  @brief  �S�Ă�GPU���g�p���܂�. ����1�ŌŒ肵�Ă��܂���, �K�v�ɉ����ĕύX���܂�.
		*************************************************************************/
		static const RHIMultiGPUMask AllGPU() { return RHIMultiGPUMask((1 << GPU_COUNT_FOR_RENDERING) - 1); }

		/*!**********************************************************************
		*  @brief   �P���GPUIndex���g����RHIMultiGPUMask���쐬���܂� (�����ڂɂǂ�GPU��^��������)
		*************************************************************************/
		static RHIMultiGPUMask CreateFromIndex(const gu::uint32 index) { return RHIMultiGPUMask(1 << index); }

		/*!**********************************************************************
		*  @brief   �w�肳�ꂽGPU�C���f�b�N�X���O�̑S�Ă�GPU���t�B���^�����O��, �}���`GPU���œ���͈̔͂�GPU��ΏۂƂ���������s���ۂɎg�p����
		*************************************************************************/
		static const RHIMultiGPUMask FilterGPUsBeforeIndex(const gu::uint32 gpuIndex) { return RHIMultiGPUMask(~((1u << gpuIndex) - 1)) & AllGPU(); }


		#pragma endregion

		#pragma region Public Member Variables
	#if PLATFORM_DESKTOP 
		/*! @brief : �g�p�\�ȍő��GPU���ł�. (����͕`��p�Ȃǂ��l�����Ă��܂���.)*/
		static constexpr gu::uint32 MAX_GPU_COUNT = 8;

		/*! @brief : �`��Ɏg�p�����GPU���ł�.*/
		static           gu::uint32 GPU_COUNT_FOR_RENDERING;

		/*! @brief : ���z�}���`GPU���g�p���邩*/
		static           gu::uint32 VIRTUAL_MULTI_GPU;
	#else
		static constexpr gu::int32 MAX_GPU_COUNT = 1;
		static           gu::uint32 GPU_COUNT_FOR_RENDERING;
		static           gu::uint32 VIRTUAL_MULTI_GPU;
	#endif

		/*!**********************************************************************
		*  @brief  �r�b�g�}�X�N�̒l�𒼐ڎ擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 Value() const { return VIRTUAL_MULTI_GPU ? 1 : _mask; }

		/*!**********************************************************************
		*  @brief   ID�͈���������Ă��邩. (�����̃r�b�g�}�X�N���I���ɂȂ��Ă���, ������GPU���w�肳��Ă��Ȃ���)
		*************************************************************************/
		bool HasSingleIndex() const;

		/*!**********************************************************************
		*  @brief   0���琔����, �ŏ���1��^���錅(Index)���擾����
		*************************************************************************/
		__forceinline gu::uint32 ToIndex() const;

		/*!**********************************************************************
		*  @brief   �w�肵��GPU��ID���}�X�N���Ɋ܂܂�Ă��邩
		*************************************************************************/
		__forceinline bool Contain(const gu::uint32 index) const { return (_mask & (1 << index)) != 0; }

		/*!**********************************************************************
		*  @brief   �w�肵��GPUMask���S�Ċ܂܂�Ă��邩
		*************************************************************************/
		__forceinline bool ContainAll(const RHIMultiGPUMask& mask) const { return (_mask & mask._mask) == mask._mask; }

		/*!**********************************************************************
		*  @brief   �����œn���ꂽGPUMask�ɂ����ċ��ʕ����������Ă��邩
		*************************************************************************/
		__forceinline bool Intersect(const RHIMultiGPUMask& mask) const { return (_mask & mask._mask) != 0; }

		#pragma endregion

		#pragma region Operator
		__forceinline bool operator ==(const RHIMultiGPUMask& right) const { return _mask == right._mask; }
		__forceinline bool operator !=(const RHIMultiGPUMask& right) const { return _mask != right._mask; }
		__forceinline void operator |=(const RHIMultiGPUMask& right) { _mask |= right._mask; }
		__forceinline void operator &=(const RHIMultiGPUMask& right) { _mask &= right._mask; }
		RHIMultiGPUMask operator &(const RHIMultiGPUMask& right) const
		{
			return RHIMultiGPUMask(_mask & right._mask);
		}
		RHIMultiGPUMask operator |(const RHIMultiGPUMask& right) const
		{
			return RHIMultiGPUMask(_mask | right._mask);
		}
		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief Default Constructor */
		RHIMultiGPUMask() : RHIMultiGPUMask(RHIMultiGPUMask::SingleGPU()) {};

		/*! @brief GPUMask��Index�𒼐ڑ������R���X�g���N�^*/
		explicit RHIMultiGPUMask(const gu::uint32 gpuMask) : _mask(gpuMask) {};

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief   �ŉ��ʃr�b�g���琔���čŏ���1�������ʒu��Ԃ�
		*************************************************************************/
		gu::uint32 CountTrailingZeros(gu::uint32 value) const ;

		#pragma endregion
		
		#pragma region Protected Member Variables
		/*! @brief GPU�}�X�N*/
		gu::uint32 _mask = 0;
		#pragma endregion
	};

}
#endif