//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanGPUPipelineState.cpp
///             @brief  Pipeline State (Graphic and Compute)
///             @author Toide Yutaro
///             @date   2022_08_14
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUPipelineState.hpp"
#include "../../Core/Include/VulkanDevice.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
#include "../../Core/Include/VulkanResourceLayout.hpp"
#include "../../Core/Include/VulkanRenderPass.hpp"
#include "../Include/VulkanGPUBlendState.hpp"
#include "../Include/VulkanGPUDepthStencilState.hpp"
#include "../Include/VulkanGPUInputAssemblyState.hpp"
#include "../Include/VulkanGPURasterizerState.hpp"
#include "../Include/VulkanGPUShaderState.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Graphics Pipeline State
GPUGraphicsPipelineState::GPUGraphicsPipelineState(
	const gu::SharedPointer<core::RHIDevice>& device,
	const gu::SharedPointer<core::RHIRenderPass>& renderPass,
	const gu::SharedPointer<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass, layout)
{

}

GPUGraphicsPipelineState::~GPUGraphicsPipelineState()
{
	VkDevice vkDevice = nullptr;
	vkDevice = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();

	vkDestroyPipeline(vkDevice, _pipeline, nullptr);
	_dynamicStates.clear(); _dynamicStates.shrink_to_fit();
}

void GPUGraphicsPipelineState::CompleteSetting()
{
	/*-------------------------------------------------------------------
	-           Set vk resources 
	---------------------------------------------------------------------*/
	const auto vkDevice = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();

	const auto vkInputAssemblyState = gu::StaticPointerCast<vulkan::GPUInputAssemblyState>(_inputAssemblyState);
	const auto vkRasterizerState    = gu::StaticPointerCast<vulkan::GPURasterizerState>(_rasterizerState);
	const auto vkDepthStencilState  = gu::StaticPointerCast<vulkan::GPUDepthStencilState>(_depthStencilState);
	const auto vkBlendState         = gu::StaticPointerCast<vulkan::GPUBlendState>(_blendState);

	/*-------------------------------------------------------------------
	-           Add shader 
	---------------------------------------------------------------------*/
	std::vector<VkPipelineShaderStageCreateInfo> shaderStates = {};
	if (_vertexShaderState)   { shaderStates.push_back(gu::StaticPointerCast<vulkan::GPUShaderState>(_vertexShaderState)  ->GetStage()); }
	if (_pixelShaderState)    { shaderStates.push_back(gu::StaticPointerCast<vulkan::GPUShaderState>(_pixelShaderState)   ->GetStage()); }
	if (_geometryShaderState) { shaderStates.push_back(gu::StaticPointerCast<vulkan::GPUShaderState>(_geometryShaderState)->GetStage()); }
	if (_hullShaderState)     { shaderStates.push_back(gu::StaticPointerCast<vulkan::GPUShaderState>(_hullShaderState)    ->GetStage()); }
	if (_domainShaderState)   { shaderStates.push_back(gu::StaticPointerCast<vulkan::GPUShaderState>(_domainShaderState)  ->GetStage()); }
	
	/*-------------------------------------------------------------------
	-           Set up Viewport
	---------------------------------------------------------------------*/
	const VkPipelineViewportStateCreateInfo viewportInfo = 
	{
		.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.pNext         = nullptr,
		.flags         = 0,
		.viewportCount = 1,
		.pViewports    = nullptr,
		.scissorCount  = 1,
		.pScissors     = nullptr
	};

	/*-------------------------------------------------------------------
	-           Set up Viewport
	---------------------------------------------------------------------*/
	const VkPipelineMultisampleStateCreateInfo multiSampleInfo = 
	{
		.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.pNext                 = nullptr,
		.flags                 = 0,
		.rasterizationSamples  = EnumConverter::Convert(_renderPass->GetMaxSample()),
		.sampleShadingEnable   = VK_FALSE,
		.minSampleShading      = 0.0f,
		.pSampleMask           = nullptr,
		.alphaToCoverageEnable = _blendState->UseAlphaToCoverage(),
		.alphaToOneEnable      = VK_FALSE
	};

	_dynamicStates = 
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR,
		VK_DYNAMIC_STATE_BLEND_CONSTANTS
	};

	/*-------------------------------------------------------------------
	-           Set up dynamic state info
	---------------------------------------------------------------------*/
	const VkPipelineDynamicStateCreateInfo dynamicStateInfo = 
	{
		.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.pNext             = nullptr,
		.flags             = 0,
		.dynamicStateCount = static_cast<std::uint32_t>(_dynamicStates.size()),
		.pDynamicStates    = _dynamicStates.data()
	};

	/*-------------------------------------------------------------------
	-           Set up graphics pipeline create info
	---------------------------------------------------------------------*/
	const VkGraphicsPipelineCreateInfo pipelineInfo = 
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.stageCount          = static_cast<std::uint32_t>(shaderStates.size()),
		.pStages             = shaderStates.data(),
		.pVertexInputState   = _inputAssemblyState ? &vkInputAssemblyState->GetVertexInput()       : nullptr,
		.pInputAssemblyState = _inputAssemblyState ? &vkInputAssemblyState->GetInputAssembly()     : nullptr,
		.pTessellationState  = nullptr,
		.pViewportState      = &viewportInfo,
		.pRasterizationState = _rasterizerState    ? &vkRasterizerState->GetRasterizerState()      : nullptr,
		.pMultisampleState   = &multiSampleInfo,
		.pDepthStencilState  = _depthStencilState  ? &vkDepthStencilState ->GetDepthStencilState() : nullptr,
		.pColorBlendState    = _blendState         ? &vkBlendState        ->GetBlendState()        : nullptr,
		.pDynamicState       = &dynamicStateInfo,
		.layout              = gu::StaticPointerCast<vulkan::RHIResourceLayout>(_resourceLayout)->GetLayout(),
		.renderPass          = gu::StaticPointerCast<vulkan::RHIRenderPass>(_renderPass)->GetRenderPass(),
		.subpass             = 0,
		.basePipelineHandle  = nullptr,
		.basePipelineIndex   = 0
	};

	/*-------------------------------------------------------------------
	-           Create graphics pipelines
	---------------------------------------------------------------------*/
	if (vkCreateGraphicsPipelines(vkDevice, {}, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline (vulkan api)");
	}
}

void GPUGraphicsPipelineState::SetName(const gu::wstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<std::uint64_t>(_pipeline));
}
#pragma endregion Graphcis Pipeline State

#pragma region Compute Pipeline State
void GPUComputePipelineState::CompleteSetting()
{
	if (!_computeShaderState) { OutputDebugStringA("compute shader state is set nullptr\n");  return; }

	/*-------------------------------------------------------------------
	-           Add shader
	---------------------------------------------------------------------*/
	const auto vkDevice    = gu::StaticPointerCast<RHIDevice>(_device)->GetDevice();
	const auto& shaderState = gu::StaticPointerCast<vulkan::GPUShaderState>(_computeShaderState)->GetStage();

	/*-------------------------------------------------------------------
	-           Create compute pipeline 
	---------------------------------------------------------------------*/
	const VkComputePipelineCreateInfo pipelineInfo =
	{
		.sType              = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
		.pNext              = nullptr,
		.flags              = 0,
		.stage              = shaderState,
		.layout             = gu::StaticPointerCast<vulkan::RHIResourceLayout>(_resourceLayout)->GetLayout(),
		.basePipelineHandle = nullptr,
		.basePipelineIndex  = 0
	};

	if (vkCreateComputePipelines(vkDevice, {}, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create compute pipeline (vulkan api)");
	}
}

void GPUComputePipelineState::SetName(const gu::wstring& name)
{
	const auto device = gu::StaticPointerCast<vulkan::RHIDevice>(_device);
	device->SetVkResourceName(name, VK_OBJECT_TYPE_PIPELINE, reinterpret_cast<std::uint64_t>(_pipeline));
}
#pragma endregion Compute Pipeline State