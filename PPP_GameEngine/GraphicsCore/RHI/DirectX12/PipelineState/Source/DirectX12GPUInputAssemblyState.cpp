//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUInputAssemblyState.cpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_07_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUInputAssemblyState.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::directX12::GPUInputAssemblyState::GPUInputAssemblyState(
	const gu::SharedPointer<core::RHIDevice>& device, 
	const gu::DynamicArray<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology
) : core::GPUInputAssemblyState(device, elements, primitiveTopology)
{
	auto offsetInBytes = gu::DynamicArray<std::uint32_t>(_slotCount); // slot‚²‚Æ‚ÌOffset

	gu::DynamicArray<std::pair<bool, core::InputClassification>> inputTypes(
		_slotCount,
		std::pair<bool, core::InputClassification>(false, core::InputClassification::PerVertex));

	for (auto& element : _elements)
	{
		const auto slot = element.Slot;

		_inputLayoutElements.Push(
			{
				element.SemanticName.CString(),                // SemanticName
				0,                                           // SemanticIndex
				EnumConverter::Convert(element.Format),      // Format
				static_cast<std::uint32_t>(element.Slot),    // InputSlot
				offsetInBytes[element.Slot],                 // AlignedByteOffset
				EnumConverter::Convert(element.Classification), // InputSlotClass
				element.Classification == core::InputClassification::PerInstance  // InstanceDataStepRate
			}
		);

		/*-------------------------------------------------------------------
		-                 Check the ClassificationType for the slot
		---------------------------------------------------------------------*/
		if (inputTypes[slot].first)
		{
			if (inputTypes[slot].second != element.Classification)
			{
				throw std::runtime_error("You set the different classification type. InputClassification should be unified for each slot.");
			}
		}
		else
		{
			inputTypes[slot].first = true; // has initial value 
			inputTypes[slot].second = element.Classification;
		}

		offsetInBytes[element.Slot] += static_cast<std::uint32_t>(core::InputFormatSizeOf::Get(element.Format));
	}

	_inputLayout.NumElements        = static_cast<std::uint32_t>(_inputLayoutElements.Size());
	_inputLayout.pInputElementDescs = _inputLayoutElements.Data();
}