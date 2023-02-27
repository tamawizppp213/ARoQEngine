//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanRasterizerState.cpp
///             @brief  Rasterizer State
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPURasterizerState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPURasterizerState::GPURasterizerState(const std::shared_ptr<core::RHIDevice>& device, const rhi::core::RasterizerProperty& rasterizerProperty)
	: rhi::core::GPURasterizerState(device, rasterizerProperty )
{
	_rasterizerState.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	_rasterizerState.pNext                   = nullptr;
	_rasterizerState.flags                   = 0;
	_rasterizerState.polygonMode             = EnumConverter::Convert(_property.FillType);
	_rasterizerState.frontFace               = EnumConverter::Convert(_property.FaceType);
	_rasterizerState.depthClampEnable        = _property.UseDepthClamp;                      // Specifies whether to enable clipping based on distance.   
	_rasterizerState.rasterizerDiscardEnable = false;                                        // Controls whether primitives are discarded just before the rasterization stage (Pixelシェーダまで持ってこない時とか)
	_rasterizerState.depthBiasEnable         = false;                                        // Bias = (float)DepthBias *  + SlopeScaledDepthBias * MaxDepthSlope;
	_rasterizerState.depthBiasConstantFactor = 0;                                            // Basically don't use the depth bias.
	_rasterizerState.depthBiasClamp          = 0;
	_rasterizerState.depthBiasSlopeFactor    = 0;
	_rasterizerState.lineWidth               = 1.0f;                                         // Rasterize line width

}