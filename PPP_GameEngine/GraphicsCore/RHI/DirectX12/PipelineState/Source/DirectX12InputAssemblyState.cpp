//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUInputAssemblyState.cpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_07_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUInputAssemblyState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::directX12::GPUInputAssemblyState::GPUInputAssemblyState(
	const std::shared_ptr<core::RHIDevice>& device, 
	const std::vector<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology
) : core::GPUInputAssemblyState(device, elements, primitiveTopology)
{
	auto offsetInBytes = std::vector<std::uint32_t>(_slotCount);

	for (auto& element : _elements)
	{
		_inputLayoutElements.push_back(
			{
				element.SemanticName.c_str(),
				0,
				EnumConverter::Convert(element.Format),
				static_cast<std::uint32_t>(element.Slot),
				offsetInBytes[element.Slot],
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
				0
			}
		);

		offsetInBytes[element.Slot] += static_cast<std::uint32_t>(core::InputFormatSizeOf::Get(element.Format));
	}

	_inputLayout.NumElements        = static_cast<std::uint32_t>(_inputLayoutElements.size());
	_inputLayout.pInputElementDescs = _inputLayoutElements.data();
}