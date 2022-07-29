//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUSampler.cpp
///             @brief  Sampler State
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Resource/Include/VulkanGPUSampler.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUSampler::GPUSampler(const std::shared_ptr<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo)
	: core::GPUSampler(device, samplerInfo)
{
	VkSamplerCreateInfo createInfo = {};
	createInfo.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO; 
	createInfo.flags                   = 0;
	createInfo.magFilter               = EnumConverter::Convert(samplerInfo.Filter, core::FilterMask::Mag);
	createInfo.minFilter               = EnumConverter::Convert(samplerInfo.Filter, core::FilterMask::Min);
	createInfo.mipmapMode              = EnumConverter::Convert(samplerInfo.Filter);
	createInfo.addressModeU            = EnumConverter::Convert(samplerInfo.AddressModeU);
	createInfo.addressModeV            = EnumConverter::Convert(samplerInfo.AddressModeV);
	createInfo.addressModeW            = EnumConverter::Convert(samplerInfo.AddressModeW);
	createInfo.mipLodBias              = samplerInfo.MipLODBias;
	createInfo.anisotropyEnable        = (samplerInfo.Filter == core::FilterOption::Anisotropy);
	createInfo.maxAnisotropy           = static_cast<float>(samplerInfo.MaxAnisotropy);
	createInfo.compareEnable           = false;
	createInfo.compareOp               = VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
	createInfo.minLod                  = samplerInfo.MinLOD;
	createInfo.maxLod                  = samplerInfo.MaxLOD;
	createInfo.borderColor             = EnumConverter::Convert(_samplerInfo.Border);
	createInfo.unnormalizedCoordinates = false;
	createInfo.pNext                   = nullptr;


	const auto vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	if (vkCreateSampler(vkDevice, &createInfo, nullptr, &_sampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create sampler state");
	}
}

GPUSampler::~GPUSampler()
{
	const auto vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();
	vkDestroySampler(vkDevice, _sampler, nullptr);
}