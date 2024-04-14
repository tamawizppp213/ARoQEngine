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
		*  @brief  単独のGPUリソースの状態遷移を示します. @n
		*          リソースの使い方が変わるタイミングで呼び出します. 
		*  @param[in] ID3D12Resource* GPUリソース
		*  @param[in] const D3D12_RESOURCE_STATES 遷移前のリソース状態
		*  @param[in] const D3D12_RESOURCE_STATES 遷移後のリソース状態
		*  @param[in] const gu::uint32 サブリソースを示すインデックス (デフォルトは全てのサブリソースを示します)
		*  @return    void
		*************************************************************************/
		void PushTransitionBarrier(ID3D12Resource* resource, const D3D12_RESOURCE_STATES before, const D3D12_RESOURCE_STATES after, const gu::uint32 subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);

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

		#pragma region Public Member Variables

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
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
		/*! @brief GPUのバリアを貯めておく場所*/
		gu::DynamicArray<GPUBarrier> _barriers = {};
		#pragma endregion 

	};

}
#endif