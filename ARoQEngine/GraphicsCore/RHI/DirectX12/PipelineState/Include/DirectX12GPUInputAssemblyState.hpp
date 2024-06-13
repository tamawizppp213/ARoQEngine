//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUInputAssemblyState.hpp
///  @brief  頂点シェーダの入力がどのようなデータ構成であるかを設定します. 
///  @author Toide Yutaro
///  @date   2024_04_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP
#define DIRECTX12_GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUInputAssemblyState.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUInputAssemblyState
	****************************************************************************/
	/* @brief  頂点シェーダの入力がどのようなデータ構成であるかを設定します. 
	*****************************************************************************/
	class GPUInputAssemblyState : public rhi::core::GPUInputAssemblyState
	{
	public:
		#pragma region Public Function
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     入力レイアウト要素が個数と実データの参照先を指定するディスクリプタ
		*  @return    const D3D12_INPUT_LAYOUT_DESC : 入力レイアウト要素の参照情報
		*************************************************************************/
		const D3D12_INPUT_LAYOUT_DESC& GetDxLayoutDesc() const noexcept { return _inputLayout; }
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		GPUInputAssemblyState() = default;
		
		/*! @brief デフォルトデストラクタ*/
		~GPUInputAssemblyState() = default;
		
		/*! @brief 論理デバイスと各入力レイアウト情報とPrimitiveTopologyを使って作成します. */
		explicit GPUInputAssemblyState(
			const gu::SharedPointer<rhi::core::RHIDevice>& device, 
			const gu::DynamicArray<core::InputLayoutElement>& elements, 
			const core::PrimitiveTopology primitiveTopology = core::PrimitiveTopology::TriangleList);

		#pragma endregion

	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Function

		/*! @brief D3D12_INPUT_LAYOUT_DESCの実データ. */
		gu::DynamicArray<D3D12_INPUT_ELEMENT_DESC> _inputLayoutElements = {};

		/*! @brief 入力レイアウト要素が個数と実データの参照先を指定するディスクリプタ*/
		D3D12_INPUT_LAYOUT_DESC _inputLayout = {nullptr, 0};
		
#pragma endregion
	};
}
#endif