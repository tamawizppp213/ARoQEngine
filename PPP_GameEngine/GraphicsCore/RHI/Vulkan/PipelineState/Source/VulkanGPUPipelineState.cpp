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
	const std::shared_ptr<core::RHIDevice>& device,
	const std::shared_ptr<core::RHIRenderPass>& renderPass,
	const std::shared_ptr<core::RHIResourceLayout>& layout) : core::GPUGraphicsPipelineState(device, renderPass, layout)
{

}

GPUGraphicsPipelineState::~GPUGraphicsPipelineState()
{
	VkDevice vkDevice = nullptr;
	vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	vkDestroyPipeline(vkDevice, _pipeline, nullptr);
	_dynamicStates.clear(); _dynamicStates.shrink_to_fit();
}

void GPUGraphicsPipelineState::CompleteSetting()
{
	VkDevice vkDevice = std::static_pointer_cast<RHIDevice>(_device)->GetDevice();

	const auto vkInputAssemblyState = std::static_pointer_cast<vulkan::GPUInputAssemblyState>(_inputAssemblyState);
	const auto vkRasterizerState    = std::static_pointer_cast<vulkan::GPURasterizerState>(_rasterizerState);
	const auto vkDepthStencilState  = std::static_pointer_cast<vulkan::GPUDepthStencilState>(_depthStencilState);
	const auto vkBlendState         = std::static_pointer_cast<vulkan::GPUBlendState>(_blendState);


	/*-------------------------------------------------------------------
	-           Add shader 
	---------------------------------------------------------------------*/
	std::vector<VkPipelineShaderStageCreateInfo> shaderStates = {};
	if (_vertexShaderState)   { shaderStates.push_back(std::static_pointer_cast<vulkan::GPUShaderState>(_vertexShaderState)->GetStage()); }
	if (_pixelShaderState)    { shaderStates.push_back(std::static_pointer_cast<vulkan::GPUShaderState>(_pixelShaderState) ->GetStage()); }
	if (_geometryShaderState) { shaderStates.push_back(std::static_pointer_cast<vulkan::GPUShaderState>(_geometryShaderState)->GetStage()); }
	if (_hullShaderState)     { shaderStates.push_back(std::static_pointer_cast<vulkan::GPUShaderState>(_hullShaderState)->GetStage()); }
	if (_domainShaderState)   { shaderStates.push_back(std::static_pointer_cast<vulkan::GPUShaderState>(_domainShaderState)->GetStage()); }
	
	/*-------------------------------------------------------------------
	-           Set up Viewport
	---------------------------------------------------------------------*/
	VkPipelineViewportStateCreateInfo viewportInfo = {};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.pNext = nullptr;
	viewportInfo.flags = 0;
	viewportInfo.viewportCount = 1;
	viewportInfo.scissorCount  = 1;
	viewportInfo.pViewports    = nullptr;
	viewportInfo.pScissors     = nullptr;

	/*-------------------------------------------------------------------
	-           Set up Viewport
	---------------------------------------------------------------------*/
	VkPipelineMultisampleStateCreateInfo multiSampleInfo = {};
	multiSampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multiSampleInfo.pNext = nullptr;
	multiSampleInfo.flags = 0;
	multiSampleInfo.pSampleMask = nullptr;
	multiSampleInfo.rasterizationSamples  = EnumConverter::Convert(_renderPass->GetMaxSample());
	multiSampleInfo.sampleShadingEnable   = VK_FALSE;
	multiSampleInfo.alphaToCoverageEnable = VK_FALSE;
	multiSampleInfo.alphaToOneEnable      = VK_FALSE;
	multiSampleInfo.minSampleShading      = 0.0f;

	_dynamicStates = std::vector<VkDynamicState>(3);
	_dynamicStates[0] = VK_DYNAMIC_STATE_VIEWPORT;
	_dynamicStates[1] = VK_DYNAMIC_STATE_SCISSOR;
	_dynamicStates[2] = VK_DYNAMIC_STATE_BLEND_CONSTANTS;

	/*-------------------------------------------------------------------
	-           Set up dynamic state info
	---------------------------------------------------------------------*/
	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.pNext = nullptr;
	dynamicStateInfo.flags = 0;
	dynamicStateInfo.dynamicStateCount = static_cast<std::uint32_t>(_dynamicStates.size());
	dynamicStateInfo.pDynamicStates    = _dynamicStates.data();
	/*-------------------------------------------------------------------
	-           Set up graphics pipeline create info
	---------------------------------------------------------------------*/
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.flags = 0;
	pipelineInfo.pNext = nullptr;
	pipelineInfo.layout = std::static_pointer_cast<vulkan::RHIResourceLayout>(_resourceLayout)->GetLayout();
	pipelineInfo.renderPass = std::static_pointer_cast<vulkan::RHIRenderPass>(_renderPass)->GetRenderPass();
	pipelineInfo.basePipelineHandle  = nullptr;
	pipelineInfo.basePipelineIndex   = 0;
	pipelineInfo.pVertexInputState   = _inputAssemblyState ? &vkInputAssemblyState->GetVertexInput()       : nullptr;
	pipelineInfo.pInputAssemblyState = _inputAssemblyState ? &vkInputAssemblyState->GetInputAssembly()     : nullptr;
	pipelineInfo.pRasterizationState = _rasterizerState    ? &vkRasterizerState   ->GetRasterizerState()   : nullptr;
	pipelineInfo.pDepthStencilState  = _depthStencilState  ? &vkDepthStencilState ->GetDepthStencilState() : nullptr;
	pipelineInfo.pColorBlendState    = _blendState         ? &vkBlendState        ->GetBlendState()        : nullptr;
	pipelineInfo.pTessellationState  = nullptr; // ‚à‚µ‚©‚µ‚½‚çŽg‚¤‚©‚à
	pipelineInfo.pStages             = shaderStates.data();
	pipelineInfo.stageCount          = static_cast<std::uint32_t>(shaderStates.size());
	pipelineInfo.subpass             = 0;
	pipelineInfo.pViewportState      = &viewportInfo;
	pipelineInfo.pMultisampleState   = &multiSampleInfo;
	pipelineInfo.pDynamicState       = &dynamicStateInfo;;
	/*-------------------------------------------------------------------
	-           Create graphics pipelines
	---------------------------------------------------------------------*/
	if (vkCreateGraphicsPipelines(vkDevice, {}, 1, &pipelineInfo, nullptr, &_pipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline (vulkan api)");
	}
}
#pragma endregion Graphcis Pipeline State