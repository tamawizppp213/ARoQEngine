//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIMultiGPUMask.hpp
///  @brief  GPU Mask : 1つのLoginal deviceで複数のGPUを扱うときに使用します. @n
///          DirectX12のNodeMaskを使うときに使用します
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
	*  @brief     各ビットがGPUインデックスであるマスクです. @n
	              SLIでないプラットフォームが常に1になるように最適化できるように、空にはできません
	*****************************************************************************/
	struct RHIMultiGPUMask
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  単一のGPUを使用します (node mask = 1)
		*************************************************************************/
		static const RHIMultiGPUMask SingleGPU() { return RHIMultiGPUMask(1); }

		/*!**********************************************************************
		*  @brief  全てのGPUを使用します. 今は1で固定していますが, 必要に応じて変更します.
		*************************************************************************/
		static const RHIMultiGPUMask AllGPU() { return RHIMultiGPUMask((1 << GPU_COUNT_FOR_RENDERING) - 1); }

		/*!**********************************************************************
		*  @brief   単一のGPUIndexを使ってRHIMultiGPUMaskを作成します (何桁目にどのGPUを与えたいか)
		*************************************************************************/
		static RHIMultiGPUMask CreateFromIndex(const gu::uint32 index) { return RHIMultiGPUMask(1 << index); }

		/*!**********************************************************************
		*  @brief   指定されたGPUインデックスより前の全てのGPUをフィルタリングし, マルチGPU環境で特定の範囲のGPUを対象とした操作を行う際に使用する
		*************************************************************************/
		static const RHIMultiGPUMask FilterGPUsBeforeIndex(const gu::uint32 gpuIndex) { return RHIMultiGPUMask(~((1u << gpuIndex) - 1)) & AllGPU(); }


		#pragma endregion

		#pragma region Public Member Variables
	#if PLATFORM_DESKTOP 
		/*! @brief : 使用可能な最大のGPU数です. (これは描画用などを考慮していません.)*/
		static constexpr gu::uint32 MAX_GPU_COUNT = 8;

		/*! @brief : 描画に使用されるGPU数です.*/
		static           gu::uint32 GPU_COUNT_FOR_RENDERING;

		/*! @brief : 仮想マルチGPUを使用するか*/
		static           gu::uint32 VIRTUAL_MULTI_GPU;
	#else
		static constexpr gu::int32 MAX_GPU_COUNT = 1;
		static           gu::uint32 GPU_COUNT_FOR_RENDERING;
		static           gu::uint32 VIRTUAL_MULTI_GPU;
	#endif

		/*!**********************************************************************
		*  @brief  ビットマスクの値を直接取得します
		*************************************************************************/
		__forceinline gu::uint32 Value() const { return VIRTUAL_MULTI_GPU ? 1 : _mask; }

		/*!**********************************************************************
		*  @brief   IDは一つだけ持っているか. (複数のビットマスクがオンになっており, 複数のGPUが指定されていないか)
		*************************************************************************/
		bool HasSingleIndex() const;

		/*!**********************************************************************
		*  @brief   0から数えて, 最初に1を与える桁(Index)を取得する
		*************************************************************************/
		__forceinline gu::uint32 ToIndex() const;

		/*!**********************************************************************
		*  @brief   指定したGPUのIDがマスク中に含まれているか
		*************************************************************************/
		__forceinline bool Contain(const gu::uint32 index) const { return (_mask & (1 << index)) != 0; }

		/*!**********************************************************************
		*  @brief   指定したGPUMaskが全て含まれているか
		*************************************************************************/
		__forceinline bool ContainAll(const RHIMultiGPUMask& mask) const { return (_mask & mask._mask) == mask._mask; }

		/*!**********************************************************************
		*  @brief   引数で渡されたGPUMaskにおいて共通部分を持っているか
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

		/*! @brief GPUMaskのIndexを直接代入するコンストラクタ*/
		explicit RHIMultiGPUMask(const gu::uint32 gpuMask) : _mask(gpuMask) {};

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief   最下位ビットから数えて最初の1が現れる位置を返す
		*************************************************************************/
		gu::uint32 CountTrailingZeros(gu::uint32 value) const ;

		#pragma endregion
		
		#pragma region Protected Member Variables
		/*! @brief GPUマスク*/
		gu::uint32 _mask = 0;
		#pragma endregion
	};

}
#endif