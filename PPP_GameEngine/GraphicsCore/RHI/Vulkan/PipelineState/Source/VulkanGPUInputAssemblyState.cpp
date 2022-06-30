//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUInputAssemblyState.cpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_07_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUInputAssemblyState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::vulkan::GPUInputAssemblyState::GPUInputAssemblyState(
	const std::shared_ptr<core::RHIDevice>& device,
	const std::vector<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology
) : core::GPUInputAssemblyState(device, elements, primitiveTopology)
{
	_vertexBinding = std::vector<VkVertexInputBindingDescription>(_slotCount);

	for (size_t index = 0; index < _elements.size(); ++index)
	{
		const auto slot = _elements[index].Slot;
		_vertexAttributes.push_back(
			VkVertexInputAttributeDescription(
				static_cast<std::uint32_t>(index),
				static_cast<std::uint32_t>(slot),
				EnumConverter::Convert(_elements[index].Format),
				_vertexBinding[slot].stride
			)
		);

		_vertexBinding[slot].stride += core::PixelFormatSizeOf::Get(_elements[index].Format);
	}

	for (size_t index = 0; index < _vertexBinding.size(); ++index)
	{
		_vertexBinding[index].binding = static_cast<std::uint32_t>(index);
	}

	_vertexInput.pNext = nullptr;
	_vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vertexInput.flags = 0;
	_vertexInput.vertexBindingDescriptionCount   = static_cast<std::uint32_t>(_vertexBinding.size());
	_vertexInput.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(_vertexAttributes.size());
	_vertexInput.pVertexBindingDescriptions      = _vertexBinding.empty()    ? nullptr : _vertexBinding.data();
	_vertexInput.pVertexAttributeDescriptions    = _vertexAttributes.empty() ? nullptr : _vertexAttributes.data();

	_inputAssembly.pNext = nullptr;
	_inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	_inputAssembly.flags = 0;
	_inputAssembly.primitiveRestartEnable = false;
	_inputAssembly.topology = EnumConverter::Convert(_primitiveTopology);
}