//////////////////////////////////////////////////////////////////////////////////
/// @file   DirectX12GPUBarrierBatcher.cpp
/// @brief  temp
/// @author toide
///　@date   2024/04/14 0:22:20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBarrierBatcher.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
void GPUBarrierBatcher::PushTransitionBarrier(ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after, const gu::uint32 subresource)
{
	Check(before != after);

	if (_barriers.Size() > 0)
	{
		// 意味のない遷移 (単純に遷移を行って戻ってというような状況)が行われているかを確認します
		// 処理が通った場合は新規に追加もせず, 最後の要素を削除します. 
		// 処理負荷の観点で最後だけ行っています. 
		const auto last = _barriers.Back();
		if(last.Type    == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION && 
			resource    == last.Transition.pResource   && 
			subresource == last.Transition.Subresource && 
			before      == last.Transition.StateAfter  && 
			after       == last.Transition.StateBefore)
		{
			_barriers.Pop();
			return;
		}
	}

	GPUBarrier barrier = {};
	barrier.Type  = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource   = resource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter  = after;
	barrier.Transition.Subresource = subresource;
	_barriers.Push(gu::type::Forward<GPUBarrier>(barrier));
}

void GPUBarrierBatcher::PushAliasingBarrier(ID3D12Resource* beforeResource, ID3D12Resource* afterResource)
{
	GPUBarrier barrier = {};
	barrier.Type  = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Aliasing.pResourceBefore = beforeResource;
	barrier.Aliasing.pResourceAfter  = afterResource;
	_barriers.Push(gu::type::Forward<GPUBarrier>(barrier));
}

void GPUBarrierBatcher::PushUAVBarrier(ID3D12Resource* resource)
{
	GPUBarrier barrier = {};
	barrier.Type          = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.Flags         = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.UAV.pResource = resource;
	_barriers.Push(gu::type::Forward<GPUBarrier>(barrier));
}

void GPUBarrierBatcher::Flush(ICommandList* commandList)
{
	if (_barriers.Size() == 0) { return; }

	commandList->ResourceBarrier(static_cast<gu::uint32>(_barriers.Size()), _barriers.Data());
	_barriers.Clear();
}
#pragma endregion Main Function