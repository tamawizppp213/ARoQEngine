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

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constants, const std::wstring& name)
	:core::RHIResourceLayout(device, elements, samplers, constants)
{
	SetUp();
	SetName(name);
}

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant, const std::wstring& name)
	: core::RHIResourceLayout(device, element, sampler, constant)
{
	SetUp();
	SetName(name);
}
#pragma region SetUp Function
/****************************************************************************
*                     SetUp
*************************************************************************//**
*  @fn        void RHIResourceLayout::SetUp(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits)
*  @brief     Set up pipeline layout
*  @param[in] const std::vector<core::ResourceLayoutElement>& elements
*  @param[in] const std::vector<core::SamplerLayoutElement>& samplers
*  @param[in] std::optional<core::Constant32Bits>& constant32Bits
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
	for (const auto& element : _elements) { maxRegisterSpace = std::max(maxRegisterSpace, element.RegisterSpace + 1); }
	for (const auto& sampler : _samplers) { maxRegisterSpace = std::max(maxRegisterSpace, sampler.RegisterSpace + 1); }

	std::vector<std::vector<VkDescriptorSetLayoutBinding>> bindings(maxRegisterSpace);
	/*-------------------------------------------------------------------
	-                 Generate the binding infomation of elements
	---------------------------------------------------------------------*/
	for (const auto& element : _elements)
	{
		const VkDescriptorSetLayoutBinding binding =
		{
			.binding            = static_cast<std::uint32_t>(element.Binding),
			.descriptorType     = EnumConverter::Convert(element.DescriptorType),
			.descriptorCount    = 1,
			.stageFlags         = VkShaderStageFlags(EnumConverter::Convert(element.Visibility)),
			.pImmutableSamplers = nullptr
		};

		bindings[element.RegisterSpace].push_back(binding);
	}

	/*-------------------------------------------------------------------
	-                 Generate the binding infomation of samplers
	---------------------------------------------------------------------*/
	for (const auto& sampler : _samplers)
	{
		const VkDescriptorSetLayoutBinding binding = 
		{
			.binding            = static_cast<std::uint32_t>(sampler.Binding),
			.descriptorType     = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER,
			.descriptorCount    = 1,
			.stageFlags         = VkShaderStageFlags(EnumConverter::Convert(sampler.Visibility)),
			.pImmutableSamplers = &gu::StaticPointerCast<vulkan::GPUSampler>(sampler.Sampler)->GetSampler()
		};
		

		bindings[sampler.RegisterSpace].push_back(binding);
	}

	/*-------------------------------------------------------------------
	-                Create Descriptor set layouts
	---------------------------------------------------------------------*/
	_descriptorSetLayouts = std::vector<VkDescriptorSetLayout>(maxRegisterSpace);
	for (int i = 0; i < _descriptorSetLayouts.size(); ++i)
	{
		const VkDescriptorSetLayoutCreateInfo createInfo = 
		{
			.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext        = nullptr,
			.flags        = 0,
			.bindingCount = static_cast<std::uint32_t>(bindings[i].size()),
			.pBindings    = bindings[i].data()
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
	if (_constant32Bits.has_value())
	{
		range.offset     = 0;
		range.size       = static_cast<std::uint32_t>(_constant32Bits->Count * sizeof(std::uint32_t));
		range.stageFlags = EnumConverter::Convert(_constant32Bits->Visibility);
	}

	/*-------------------------------------------------------------------
	-                Create pipeline layout infomation
	---------------------------------------------------------------------*/
	VkPipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.flags = 0;
	layoutInfo.setLayoutCount         = static_cast<std::uint32_t>(_descriptorSetLayouts.size());
	layoutInfo.pushConstantRangeCount = _constant32Bits.has_value() ? 1 : 0;
	layoutInfo.pSetLayouts            = _descriptorSetLayouts.data();
	layoutInfo.pPushConstantRanges    = &range;

	if (vkCreatePipelineLayout(vkDevice, &layoutInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout (vulkan api)");
	}
}

void RHIResourceLayout::SetName(const std::wstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_PIPELINE_LAYOUT, reinterpret_cast<std::uint64_t>(_pipelineLayout));
}
#pragma endregion SetUp Function