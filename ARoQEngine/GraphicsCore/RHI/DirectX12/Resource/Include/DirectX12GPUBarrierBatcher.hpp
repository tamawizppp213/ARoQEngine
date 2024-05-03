//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUBarrierBatcher.hpp
///  @brief  GPUのリソース状態を安全に使用するためのクラスです. @n
///          GPUリソースの使い方が変わった場合にはPushTransitionBarrier, @n
///          同じGPUヒープ領域に異なるリソースを排他的に使用するならPushAliasingBarrier @n
///          UnorderedAccessViewの読み書きを実行が完了するまでほかの影響を与えないならPushUAVBarrierを実行すること
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
	/*  @brief     GPUのリソースを排他的に処理を行うクラスです. 
	*****************************************************************************/
	struct GPUBarrier : public D3D12_RESOURCE_BARRIER
	{
		/*!**********************************************************************
		*  @brief  単独のGPUリソースの状態遷移を示します. @n
		*          リソースの使い方が変わるタイミングで呼び出します. 
		*  @param[in] ID3D12Resource* GPUリソース (これだけGPUResourceを代入するのは後でリソースの遷移後の状態を代入する必要があるため)
		*  @param[in] const D3D12_RESOURCE_STATES 遷移後のリソース状態
		*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します)
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
		*  @brief  同じメモリ領域にマッピングされた複数のGPUリソースに対し, 使用するリソース自体を切り替える際に使用します. @n
		*  @param[in] ID3D12Resource* 切り替える前に使用していたGPUリソース
		*  @param[in] ID3D12Resource* 切り替えた後に使用する予定のGPUリソース
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
		*  @brief  Unordered access view専用の状態バリア @n
		*          UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
		*  @param[in] ID3D12Resource* Unordered access viewを持つGPUリソース
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
	*   @brief     GPUのリソース状態を安全に使用するためのクラスです. @n
	*              GPUリソースの使い方が変わった場合にはPushTransitionBarrier, @n
	*              同じGPUヒープ領域に異なるリソースを排他的に使用するならPushAliasingBarrier @n
	*              UnorderedAccessViewの読み書きを実行が完了するまでほかの影響を与えないならPushUAVBarrierを実行すること
	*****************************************************************************/
	class GPUBarrierBatcher : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  単独のGPUリソースの状態遷移を示します. @n
		*          リソースの使い方が変わるタイミングで呼び出します. 
		*  @param[in] gu::SharedPointer<GPUResource>& GPUリソース (これだけGPUResourceを代入するのは後でリソースの遷移後の状態を代入する必要があるため)
		*  @param[in] const D3D12_RESOURCE_STATES 遷移後のリソース状態
		*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します)
		*  @return    bool Barrierが登録されたらtrue
		*************************************************************************/
		bool PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

		/*!**********************************************************************
		*  @brief  同じメモリ領域にマッピングされた複数のGPUリソースに対し, 使用するリソース自体を切り替える際に使用します. @n
		*          同時に使用しないことが担保されているリソースのメモリを節約することが可能となります. @n
		*          ただし, 本関数を使用する場合は,CreateCommittedResourceでは無く, CreatePlacedResourceを使用した方法でヒープの確保を行ってください.@n
		*          (別々のヒープを作ってしまうことになり, 同じメモリ領域を扱わなくなるため.)
		*  @note   https://logicalbeat.jp/blog/8185/ (AliasingBarrierの活用方法についての記述)
		*  @param[in] ID3D12Resource* 切り替える前に使用していたGPUリソース
		*  @param[in] ID3D12Resource* 切り替えた後に使用する予定のGPUリソース
		*  @return    void
		*************************************************************************/
		void PushAliasingBarrier(ID3D12Resource* beforeResource, ID3D12Resource* afterResource);

		/*!**********************************************************************
		*  @brief  Unordered access view専用の状態バリア @n
		*          UAVの読み書き中にほかのUAVが対象リソースを読み書きする描画コマンドの実行を防ぐことを目的とします @n
		*  @param[in] ID3D12Resource* Unordered access viewを持つGPUリソース
		*  @return    void
		*************************************************************************/
		void PushUAVBarrier(ID3D12Resource* resource);

		/*!**********************************************************************
		*  @brief  コマンドリストを使ってResourceBarrierをまとめて呼び出します. 
		*  @param[in] ICommandList* 描画命令を実行するコマンドリスト
		*  @return    void
		*************************************************************************/
		void Flush(ICommandList* commandList);

		/*!**********************************************************************
		*  @brief  現在たまっているバリアの個数
		*************************************************************************/
		__forceinline gu::uint64 Count() const { return _barriers.Size(); }

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUBarrierBatcher() : _barriers(), _afterTransitions()
		{
			// メモリ領域を事前に確保することで高速化
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
		/*! @brief GPUのバリアを貯めておく場所*/
		gu::DynamicArray<GPUBarrier> _barriers = {};

		gu::DynamicArray<gu::Pair<core::ResourceState, gu::SharedPointer<core::GPUResource>>> _afterTransitions = {};
		#pragma endregion 

	};

}
#endif