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
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     PushTransitionBarrier
****************************************************************************/
/* @brief     単独のGPUリソースの状態遷移を示します. @n
*             リソースの使い方が変わるタイミングで呼び出します. 
*
*  @param[in] gu::SharedPointer<GPUResource>& GPUリソース (これだけGPUResourceを代入するのは後でリソースの遷移後の状態を代入する必要があるため)
*  @param[in] const D3D12_RESOURCE_STATES 遷移後のリソース状態
*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します)
*
*  @return 　　bool Barrierが登録されたらtrue
*****************************************************************************/
bool GPUBarrierBatcher::PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource)
{
	const auto dxResource    = resource->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer> (resource)->GetResourcePtr():
			                                          gu::StaticPointerCast<directX12::GPUTexture>(resource)->GetResourcePtr();
	const auto dxBeforeState = EnumConverter::Convert(resource->GetResourceState());
	const auto dxAfterState  = EnumConverter::Convert(after);

	/*-------------------------------------------------------------------
	-         意味のない遷移が実行されているかを確認する.
	---------------------------------------------------------------------*/
	Check(dxBeforeState != dxAfterState);
	if (_barriers.Size() > 0)
	{
		// 意味のない遷移 (単純に遷移を行って戻ってというような状況)が行われているかを確認します
		// 処理が通った場合は新規に追加もせず, 最後の要素を削除します. 
		// 処理負荷の観点で最後だけ行っています. 
		
		const auto last = _barriers.Back();
		if(last.Type      == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION && 
			dxResource    == last.Transition.pResource   && 
			subresource   == last.Transition.Subresource && 
			dxBeforeState == last.Transition.StateAfter  && 
			dxAfterState  == last.Transition.StateBefore)
		{
			_barriers.Pop();
			_afterTransitions.Pop();
			return false;
		}
	}

	/*-------------------------------------------------------------------
	-         新規のバリアを作成する. 
	---------------------------------------------------------------------*/
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateTransition(dxResource,  dxBeforeState, dxAfterState, subresource)));
	_afterTransitions.Push({ after, resource });
	return true;
}

/****************************************************************************
*                     PushAliasingBarrier
****************************************************************************/
/* @brief     Unordered access view専用の状態バリア @n
*             UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
*
*  @param[in]  ID3D12Resource* Unordered access viewを持つGPUリソース
*
*  @return 　　void
*****************************************************************************/
void GPUBarrierBatcher::PushAliasingBarrier(ID3D12Resource* beforeResource, ID3D12Resource* afterResource)
{
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateAliasing(beforeResource, afterResource)));
}

/****************************************************************************
*                     PushUAVBarrier
****************************************************************************/
/* @brief     同じメモリ領域にマッピングされた複数のGPUリソースに対し, 使用するリソース自体を切り替える際に使用します. @n
*          同時に使用しないことが担保されているリソースのメモリを節約することが可能となります. @n
*          ただし, 本関数を使用する場合は,CreateCommittedResourceでは無く, CreatePlacedResourceを使用した方法でヒープの確保を行ってください.@n
*          (別々のヒープを作ってしまうことになり, 同じメモリ領域を扱わなくなるため.)
*
*  @param[in] ID3D12Resource* 切り替える前に使用していたGPUリソース
*  @param[in] ID3D12Resource* 切り替えた後に使用する予定のGPUリソース
*
*  @return 　　void
*****************************************************************************/
void GPUBarrierBatcher::PushUAVBarrier(ID3D12Resource* resource)
{
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateUAV(resource)));
}

/****************************************************************************
*                     Flush
****************************************************************************/
/* @brief     既に存在しているHeap内にまだマップまでは行わない予約済みのリソースを作成
*
*  @param[in] ICommandList* 描画命令を実行するコマンドリスト
*
*  @return 　　void
*****************************************************************************/
void GPUBarrierBatcher::Flush(ICommandList* commandList)
{
	if (_barriers.Size() == 0) { return; }

	commandList->ResourceBarrier(static_cast<gu::uint32>(_barriers.Size()), _barriers.Data());

	/*-------------------------------------------------------------------
	-      Barrierを実行しないと遷移したとみなさない. 
	      しかし, GPUとCPUのResourceStateが同期されているわけではないため, CPUでは変更完了したがGPUではまだという状況が発生する可能性があります. 
	---------------------------------------------------------------------*/
	for (gu::uint64 i = 0; i < _afterTransitions.Size(); ++i)
	{
		_afterTransitions[i].Value->SetResourceState(_afterTransitions[i].Key);
	}

	/*-------------------------------------------------------------------
	-      貯めこんだバッファのサイズを0にする. (メモリは高速化のために初期化しない)
	---------------------------------------------------------------------*/
	_barriers.Clear();
	_afterTransitions.Clear();
}
#pragma endregion Main Function