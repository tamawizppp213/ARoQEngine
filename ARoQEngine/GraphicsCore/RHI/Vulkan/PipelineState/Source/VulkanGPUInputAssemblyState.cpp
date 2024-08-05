//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUInputAssemblyState.cpp
///             @brief  Input assembly
///             @author Toide Yutaro
///             @date   2022_07_01
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUInputAssemblyState.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::vulkan::GPUInputAssemblyState::GPUInputAssemblyState(
	const gu::SharedPointer<core::RHIDevice>& device,
	const gu::DynamicArray<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology
) : core::GPUInputAssemblyState(device, elements, primitiveTopology)
{
	/* @brief : how each vertex stream is to be read*/
	_vertexBinding = gu::DynamicArray<VkVertexInputBindingDescription>(_slotCount);

	gu::DynamicArray<std::pair<bool, core::InputClassification>> inputTypes(
		_slotCount, 
		std::pair<bool, core::InputClassification>(false, core::InputClassification::PerVertex));
	/*-------------------------------------------------------------------
	-                      Set vertex attributes
	---------------------------------------------------------------------*/
	for (size_t index = 0; index < _elements.Size(); ++index)
	{
		const auto slot = _elements[index].Slot;

		/* Set the vertex attribute information contained in each vertex stream.*/
		_vertexAttributes.Push
		(
			VkVertexInputAttributeDescription
			(
				static_cast<std::uint32_t>(index),                // location
				static_cast<std::uint32_t>(slot),                 // binding
				EnumConverter::Convert(_elements[index].Format),  // input format
				_vertexBinding[slot].stride                       // vertex attribute offset 
			)
		);

		/*-------------------------------------------------------------------
		-                 Check the ClassificationType for the slot
		---------------------------------------------------------------------*/
		if (inputTypes[slot].first)
		{
			if (inputTypes[slot].second != _elements[index].Classification)
			{
				throw std::runtime_error("You set the different classification type. InputClassification should be unified for each slot.");
			}
		}
		else
		{
			inputTypes[slot].first  = true; // has initial value 
			inputTypes[slot].second = _elements[index].Classification;
		}

		// count offset 
		_vertexBinding[slot].stride += static_cast<std::uint32_t>(core::PixelFormatInfo::GetConst(_elements[index].Format).BlockBytes);
	}

	for (size_t index = 0; index < _vertexBinding.Size(); ++index)
	{
		_vertexBinding[index].binding   = static_cast<std::uint32_t>(index);
		_vertexBinding[index].inputRate = EnumConverter::Convert(inputTypes[index].second); // 各スロットごとにInputClassificationは統一する,.
	}
	
	/*-------------------------------------------------------------------
	-                  Set vertex input state create info
	---------------------------------------------------------------------*/
	_vertexInput.pNext = nullptr;
	_vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	_vertexInput.flags = 0;
	_vertexInput.vertexBindingDescriptionCount   = static_cast<std::uint32_t>(_vertexBinding.Size());
	_vertexInput.vertexAttributeDescriptionCount = static_cast<std::uint32_t>(_vertexAttributes.Size());
	_vertexInput.pVertexBindingDescriptions      = _vertexBinding.IsEmpty()    ? nullptr : _vertexBinding.Data();
	_vertexInput.pVertexAttributeDescriptions    = _vertexAttributes.IsEmpty() ? nullptr : _vertexAttributes.Data();

	/*-------------------------------------------------------------------
	-                  Set input assembly state create info
	---------------------------------------------------------------------*/
	_inputAssembly.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	_inputAssembly.pNext                  = nullptr;
	_inputAssembly.flags                  = 0;
	_inputAssembly.primitiveRestartEnable = false; // よくわからん
	_inputAssembly.topology               = EnumConverter::Convert(_primitiveTopology);
}