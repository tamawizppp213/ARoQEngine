//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanDepthStencilState.cpp
///             @brief  DepthStencil State
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanGPUDepthStencilState.hpp"
#include "../../Core/Include/VulkanEnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUDepthStencilState::GPUDepthStencilState( const gu::SharedPointer<rhi::core::RHIDevice>& device,
	const core::DepthStencilProperty& depthStencilProperty
):
	core::GPUDepthStencilState(device, depthStencilProperty)
{
	_depthStencilDesc.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	_depthStencilDesc.pNext                 = nullptr;
	_depthStencilDesc.flags                 = 0;
	_depthStencilDesc.depthTestEnable       = _property.UseDepthTest;                           // Use depth test
	_depthStencilDesc.depthWriteEnable      = _property.DepthWriteEnable;                       // Enable to write depth buffer
	_depthStencilDesc.depthCompareOp        = EnumConverter::Convert(_property.DepthOperator);  // Depth compare operator : Depth compare test is used.
	_depthStencilDesc.depthBoundsTestEnable = _property.UseDepthBoundsTest;                     // Use depth bounds test: situations (https://shikihuiku.wordpress.com/2012/06/27/depth-bounds-test1/
	_depthStencilDesc.minDepthBounds        = 0.0f;                                             // Min depth bounds test region 
	_depthStencilDesc.maxDepthBounds        = 0.0f;                                             // Max depth bounds test region
	_depthStencilDesc.stencilTestEnable     = _property.StenciWriteEnable;                      // Use stencil test
	_depthStencilDesc.front = VkStencilOpState(
		EnumConverter::Convert(_property.Front.FailOperator),                                   // front stencil failed operator
		EnumConverter::Convert(_property.Front.PassOperator),                                   // front stencil pass operator
		EnumConverter::Convert(_property.Front.DepthFailOperator),                              // front stencil pass and depth test failed operator
		EnumConverter::Convert(_property.Front.CompareOperator),                                // stencil compare operator
		0,
		0,
		_property.Front.Reference
	);
	_depthStencilDesc.back = VkStencilOpState(
		EnumConverter::Convert(_property.Back.FailOperator),
		EnumConverter::Convert(_property.Back.PassOperator),
		EnumConverter::Convert(_property.Back.DepthFailOperator),
		EnumConverter::Convert(_property.Back.CompareOperator),
		0,
		0,
		_property.Front.Reference
	);
}