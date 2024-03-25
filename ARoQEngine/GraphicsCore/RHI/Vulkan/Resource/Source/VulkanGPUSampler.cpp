//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUSampler.cpp
///             @brief  Sampler State
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUSampler.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include "../../Core/Include/VulkanDevice.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUSampler::GPUSampler(const gu::SharedPointer<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo)
	: core::GPUSampler(device, samplerInfo)
{
	const VkSamplerCreateInfo createInfo = 
	{
		.sType                   = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.pNext                   = nullptr,
		.flags                   = 0,
		.magFilter               = EnumConverter::Convert(samplerInfo.Filter, core::FilterMask::Mag), // Sampling mode at magnification of texture image.
		.minFilter               = EnumConverter::Convert(samplerInfo.Filter, core::FilterMask::Min), // Sampling mode at shrinking texture image.
		.mipmapMode              = EnumConverter::Convert(samplerInfo.Filter),                        // How to use the mip map (linear or nearest)
		.addressModeU            = EnumConverter::Convert(samplerInfo.AddressModeU),                  // Texture addressing mode
		.addressModeV            = EnumConverter::Convert(samplerInfo.AddressModeV),
		.addressModeW            = EnumConverter::Convert(samplerInfo.AddressModeW),
		.mipLodBias              = samplerInfo.MipLODBias,                                            // LOD bias (本来出したいLODよりもシャープに, ボケて見せれる)
		.anisotropyEnable        = (samplerInfo.Filter == core::FilterOption::Anisotropy),            // Use anisotropy filtering 
		.maxAnisotropy           = static_cast<float>(samplerInfo.MaxAnisotropy),                    
		.compareEnable           = false,                                                             // PCFを使用する場合はtrueにする必要があるが, 現状はfalse
		.compareOp               = VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL,  
		.minLod                  = samplerInfo.MinLOD,
		.maxLod                  = samplerInfo.MaxLOD,
		.borderColor             = EnumConverter::Convert(_samplerInfo.Border),
		.unnormalizedCoordinates = false                                                              // Always normalize (0.0f ～ 1.0f)
	};

	const auto vkDevice = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();
	if (vkCreateSampler(vkDevice, &createInfo, nullptr, &_sampler) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create sampler state");
	}
}

GPUSampler::~GPUSampler()
{
	const auto vkDevice = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();
	vkDestroySampler(vkDevice, _sampler, nullptr);
}