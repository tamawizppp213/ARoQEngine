//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIMultiGPUMask.cpp
///             @brief  temp
///             @author toide
///             @date   2023/12/30 2:45:18
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/RHIMultiGPUMask.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;

gu::uint32 RHIMultiGPUMask::GPU_COUNT_FOR_RENDERING = 1;
gu::uint32 RHIMultiGPUMask::VIRTUAL_MULTI_GPU = 0;
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     ToIndex
****************************************************************************/
/* @brief  IDは一つだけ持っているか. (複数のビットマスクがオンになっており, 複数のGPUが指定されていないか)
*****************************************************************************/
bool RHIMultiGPUMask::HasSingleIndex() const
{
	// 2のべき乗かを調べることで唯一のGPUインデックスを取得できていることを確認する
	return (_mask & (_mask - 1)) == 0;
}

/****************************************************************************
*                     ToIndex
****************************************************************************/
/* @brief   0から数えて, 最初に1を与える桁(Index)を取得する
*****************************************************************************/
uint32 RHIMultiGPUMask::ToIndex() const
{
	#if WITH_MULTI_GPU
		Check(HasSingleIndex());
		return CountTrailingZeros(_mask);
	#else
		return 0;
	#endif
}

/****************************************************************************
*                     CountTrailingZero
****************************************************************************/
/* @brief     最下位ビットから数えて最初の1が現れる位置を返す
*****************************************************************************/
uint32 RHIMultiGPUMask::CountTrailingZeros(uint32 value) const
{
	if (value == 0) { return 32; }

	uint32 bitIndex = 0;

	while ((value & 1) == 0)
	{
		value >>= 1;
		++bitIndex;
	}

	return bitIndex;
}
#pragma endregion Main Function