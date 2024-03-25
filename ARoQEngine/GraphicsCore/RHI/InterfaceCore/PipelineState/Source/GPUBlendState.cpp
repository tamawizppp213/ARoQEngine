//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUBlendState.cpp
///             @brief  Blend State 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUBlendState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
rhi::core::GPUBlendState::~GPUBlendState()
{
	_blendProperties.Clear(); _blendProperties.ShrinkToFit();
	_device.Reset();
}