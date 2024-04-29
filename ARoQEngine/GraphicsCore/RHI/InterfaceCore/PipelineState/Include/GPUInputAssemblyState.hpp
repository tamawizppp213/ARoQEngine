//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUInputAssemblyState.hpp
///  @brief  InputAssembly State
///  @author Toide Yutaro
///  @date   2024_04_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_INPUT_ASSEMBLY_STATE_HPP
#define GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GPUState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			RHIInputAssemblyState
	*************************************************************************//**
	/*  @brief     頂点シェーダの入力がどのようなデータ構成であるかを設定します. 
	*****************************************************************************/
	class GPUInputAssemblyState : public GPUState
	{
	public:
		#pragma region Static Function
		/*!**********************************************************************
		*  @brief     単純な3Dポリゴン用の頂点データのレイアウト
		*  @note      新たにVertexをGameCoreで定義する場合はこの関数を取り除き, 移動する. 
		*  @return    gu::DynamicArray<InputLayoutElement>
		*************************************************************************/
		static gu::DynamicArray<InputLayoutElement> GetDefaultVertexElement()     { return DEFAULT_VERTEX_ELEMENTS; }

		/*!**********************************************************************
		*  @brief     単純なスキンメッシュ用の頂点データのレイアウト
		*  @note      新たにVertexをGameCoreで定義する場合はこの関数を取り除き, 移動する.
		*  @return    gu::DynamicArray<InputLayoutElement>
		*************************************************************************/
		static gu::DynamicArray<InputLayoutElement> GetDefaultSkinVertexElement() { return DEFAULT_SKINVERTEX_ELEMENTS; }
		#pragma endregion 
		
		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     入力レイアウトにおいて, 異なる値を持つスロットが何個あるかを示します
		*  @return    gu::uint8 入力レイアウトの個数です (0～15までしかないため, gu::uint8です)
		*************************************************************************/
		gu::uint8 GetSlotCount() const noexcept { return _slotCount; }
		
		/*!**********************************************************************
		*  @brief     入力レイアウトの各要素にアクセスします. 
		*  @param[in] 入力レイアウトの要素インデックス (範囲外チェックは行っておりません.)
		*  @return    const InputLayoutElement& 入力レイアウト, 外からの変更は行いません
		*************************************************************************/
		const InputLayoutElement& GetElement(const gu::uint32 index) const { return _elements[index]; }
		
		/*!**********************************************************************
		*  @brief     入力レイアウトの配列データにアクセスします
		*  @return    const gu::DynamicArray<InputLayoutElement 入力レイアウトの配列
		*************************************************************************/
		const gu::DynamicArray<InputLayoutElement>& GetElements() const { return _elements; }
		
		/*!**********************************************************************
		*  @brief     隣接する各頂点同士の接続方法を指定するEnumを返します. 
		*  @return    PrimitiveTopology 隣接頂点の接続方法.
		*************************************************************************/
		PrimitiveTopology GetPrimitiveTopology() const { return _primitiveTopology; }
		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUInputAssemblyState() = default;

		/*! @brief デストラクタ*/
		~GPUInputAssemblyState() = default;

		/*! @brief 基本的なInputAssembly Stageで作成するコンストラクタ*/
		explicit GPUInputAssemblyState(
			const gu::SharedPointer<RHIDevice>& device,
			const gu::DynamicArray<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList
		) : GPUState(device), _elements(elements), _primitiveTopology(primitiveTopology)
		{
			for (const auto& element : _elements)
			{
				const gu::uint8 nextSlot = element.Slot + 1;
				_slotCount = nextSlot >= _slotCount ? nextSlot : _slotCount;
			}
		}

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Member Variables

		/*! @brief 入力レイアウトの設定データです.*/
		gu::DynamicArray<InputLayoutElement> _elements = {};

		/*! @brief 入力レイアウトにおいて, 異なる値を持つスロットが何個あるかを示します. @n
		           gu::DynamicArrayのSizeで設定しないのは, 同じスロットを使用する可能性があるためです.*/
		gu::uint8 _slotCount = 1;

		/*! @brief 隣接する各頂点同士の接続方法を指定します. デフォルトはTriangleListです.*/
		core::PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

		#pragma endregion 

	private:
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_VERTEX_ELEMENTS;
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_SKINVERTEX_ELEMENTS;
	};

}
#endif