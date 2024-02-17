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
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
		// Note : êVÇΩÇ…VertexÇGameCoreÇ≈íËã`Ç∑ÇÈèÍçáÇÕÇ±ÇÃä÷êîÇéÊÇËèúÇ´, à⁄ìÆÇ∑ÇÈ. 
		static gu::DynamicArray<InputLayoutElement> GetDefaultVertexElement()     { return DEFAULT_VERTEX_ELEMENTS; }
		static gu::DynamicArray<InputLayoutElement> GetDefaultSkinVertexElement() { return DEFAULT_SKINVERTEX_ELEMENTS; }
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t GetSlot() const noexcept { return _slotCount; }
		
		/* @brief return input layout element*/
		const InputLayoutElement& GetElement(const size_t index) const { return _elements[index]; }
		
		/* @brief return input layout elements*/
		const gu::DynamicArray<InputLayoutElement>& GetElements() const { return _elements; }
		
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
			const gu::DynamicArray<InputLayoutElement>& elements,
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
		gu::DynamicArray<InputLayoutElement> _elements = {};

		size_t                          _slotCount = 1;

		core::PrimitiveTopology _primitiveTopology = PrimitiveTopology::TriangleList;

	private:
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_VERTEX_ELEMENTS;
		static const gu::DynamicArray<InputLayoutElement> DEFAULT_SKINVERTEX_ELEMENTS;
	};

}
#endif