//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanResourceLayout.cpp
///             @brief  RenderPass
///             @author Toide Yutaro
///             @date   2022_08_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanResourceLayout.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUSampler.hpp"
#include <stdexcept> 

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
#undef max
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIResourceLayout::~RHIResourceLayout()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	vkDestroyPipelineLayout(vkDevice, _pipelineLayout, nullptr);
	for (auto& setLayout : _descriptorSetLayouts)
	{
		vkDestroyDescriptorSetLayout(vkDevice, setLayout, nullptr);
	}
}

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const gu::Optional<core::Constant32Bits>& constants, const gu::tstring& name)
	:core::RHIResourceLayout(device, elements, samplers, constants)
{
	SetUp();
	SetName(name);
}

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const gu::Optional<core::Constant32Bits>& constant, const gu::tstring& name)
	: core::RHIResourceLayout(device, element, sampler, constant)
{
	SetUp();
	SetName(name);
}
#pragma region SetUp Function
/****************************************************************************
*                     SetUp
****************************************************************************/
/* @fn        void RHIResourceLayout::SetUp(const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const gu::Optional<core::Constant32Bits>& constant32Bits)
*  @brief     Set up pipeline layout
*  @param[in] const gu::DynamicArray<core::ResourceLayoutElement>& elements
*  @param[in] const gu::DynamicArray<core::SamplerLayoutElement>& samplers
*  @param[in] gu::Optional<core::Constant32Bits>& constant32Bits
*  @return Å@Å@void
*****************************************************************************/
void RHIResourceLayout::SetUp()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-                 Find the max register space
	---------------------------------------------------------------------*/
	size_t maxRegisterSpace = 0;
	for (const auto& element : _desc.Elements) { maxRegisterSpace = std::max(maxRegisterSpace, (gu::uint64)element.RegisterSpace + 1); }
	for (const auto& sampler : _desc.Samplers) { maxRegisterSpace = std::max(maxRegisterSpace, (gu::uint64)sampler.RegisterSpace + 1); }

	gu::DynamicArray<gu::DynamicArray<VkDescriptorSetLayoutBinding>> bindings(maxRegisterSpace);
	/*-------------------------------------------------------------------
	-                 Generate the binding infomation of elements
	---------------------------------------------------------------------*/
	for (const auto& element : _desc.Elements)
	{
		const VkDescriptorSetLayoutBinding binding =
		{
			.binding            = static_cast<std::uint32_t>(element.Binding),
			.descriptorType     = EnumConverter::Convert(element.DescriptorType),
			.descriptorCount    = 1,
			.stageFlags         = VkShaderStageFlags(EnumConverter::Convert(element.Visibility)),
			.pImmutableSamplers = nullptr
		};

		bindings[element.RegisterSpace].Push(binding);
	}

	/*-------------------------------------------------------------------
	-                 Generate the binding infomation of samplers
	---------------------------------------------------------------------*/
	for (const auto& sampler : _desc.Samplers)
	{
		const VkDescriptorSetLayoutBinding binding = 
		{
			.binding            = static_cast<std::uint32_t>(sampler.Binding),
			.descriptorType     = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER,
			.descriptorCount    = 1,
			.stageFlags         = VkShaderStageFlags(EnumConverter::Convert(sampler.Visibility)),
			.pImmutableSamplers = &gu::StaticPointerCast<vulkan::GPUSampler>(sampler.Sampler)->GetSampler()
		};
		

		bindings[sampler.RegisterSpace].Push(binding);
	}

	/*-------------------------------------------------------------------
	-                Create Descriptor set layouts
	---------------------------------------------------------------------*/
	_descriptorSetLayouts = gu::DynamicArray<VkDescriptorSetLayout>(maxRegisterSpace);
	for (int i = 0; i < _descriptorSetLayouts.Size(); ++i)
	{
		const VkDescriptorSetLayoutCreateInfo createInfo = 
		{
			.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext        = nullptr,
			.flags        = 0,
			.bindingCount = static_cast<std::uint32_t>(bindings[i].Size()),
			.pBindings    = bindings[i].Data()
		};


		if (vkCreateDescriptorSetLayout(vkDevice, &createInfo, nullptr, &_descriptorSetLayouts[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout (vulkan api)");
		}
	}

	/*-------------------------------------------------------------------
	-                Create constant range
	---------------------------------------------------------------------*/
	VkPushConstantRange range = {};
	if (_desc.Constant32Bits.HasValue())
	{
		range.offset     = 0;
		range.size       = static_cast<std::uint32_t>(_desc.Constant32Bits->Count * sizeof(std::uint32_t));
		range.stageFlags = EnumConverter::Convert(_desc.Constant32Bits->Visibility);
	}

	/*-------------------------------------------------------------------
	-                Create pipeline layout infomation
	---------------------------------------------------------------------*/
	VkPipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.flags = 0;
	layoutInfo.setLayoutCount         = static_cast<std::uint32_t>(_descriptorSetLayouts.Size());
	layoutInfo.pushConstantRangeCount = _desc.Constant32Bits.HasValue() ? 1 : 0;
	layoutInfo.pSetLayouts            = _descriptorSetLayouts.Data();
	layoutInfo.pPushConstantRanges    = &range;

	if (vkCreatePipelineLayout(vkDevice, &layoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout (vulkan api)");
	}
}

void RHIResourceLayout::SetName(const gu::tstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<std::uint64_t>(_pipelineLayout));
}
#pragma endregion SetUp Function