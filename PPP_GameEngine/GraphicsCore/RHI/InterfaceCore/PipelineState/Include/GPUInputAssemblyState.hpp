//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUInputAssemblyState.hpp
///             @brief  InputAssembly State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_INPUT_ASSEMBLY_STATE_HPP
#define GPU_INPUT_ASSEMBLY_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/RHICommonState.hpp"
#include "../../Core/Include/RHIResourceLayoutElement.hpp"
#include "GPUState.hpp"
#include <vector>
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#ifdef max
#undef max
#endif
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			RHIInputAssemblyState
	*************************************************************************//**
	*  @class     RHIInputAssemblyState
	*  @brief     Set vertex information
	*****************************************************************************/
	class GPUInputAssemblyState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// Note : 新たにVertexをGameCoreで定義する場合はこの関数を取り除き, 移動する. 
		static std::vector<InputLayoutElement> GetDefaultVertexElement()     { return DEFAULT_VERTEX_ELEMENTS; }
		static std::vector<InputLayoutElement> GetDefaultSkinVertexElement() { return DEFAULT_SKINVERTEX_ELEMENTS; }
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t GetSlot() const noexcept { return _slotCount; }
		
		/* @brief return input layout element*/
		const InputLayoutElement& GetElement(const size_t index) const { return _elements[index]; }
		
		/* @brief return input layout elements*/
		const std::vector<InputLayoutElement>& GetElements() const { return _elements; }
		
		/* @brief return primitive topology*/
		PrimitiveTopology GetPrimitiveTopology() const { return _primitiveTopology; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUInputAssemblyState() = default;

		~GPUInputAssemblyState() = default;

		explicit GPUInputAssemblyState(
			const gu::SharedPointer<RHIDevice>& device,
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList
		) : GPUState(device), _elements(elements), _primitiveTopology(primitiveTopology)
		{
			for (const auto& element : _elements)
			{
				_slotCount = std::max(_slotCount, element.Slot + 1);
			}
		}

		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<InputLayoutElement> _elements = {};

		size_t                          _slotCount = 1;

		core::PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

	private:
		static const std::vector<InputLayoutElement> DEFAULT_VERTEX_ELEMENTS;
		static const std::vector<InputLayoutElement> DEFAULT_SKINVERTEX_ELEMENTS;
	};

}
#endif