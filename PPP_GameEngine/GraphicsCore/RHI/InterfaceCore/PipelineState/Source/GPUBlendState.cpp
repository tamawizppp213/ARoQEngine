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
	_blendProperties.clear(); _blendProperties.shrink_to_fit();
	_device.reset();
}
rhi::core::BlendProperty GPUBlendState::GetDefaultBlendState(DefaultBlendStateType type)
{
	BlendProperty blendProperty;
	blendProperty.Source           = BlendFactor::Source_Alpha;
	blendProperty.SourceAlpha      = BlendFactor::One;
	blendProperty.Destination      = BlendFactor::Inverse_Source_Alpha;
	blendProperty.DestinationAlpha = BlendFactor::Inverse_Source_Alpha;
	blendProperty.ColorOperator    = BlendOperator::Add;
	blendProperty.AlphaOperator    = BlendOperator::Add;

	switch (type)
	{
		case DefaultBlendStateType::NoColorWrite:
		{
			blendProperty.ColorMask = ColorMask::None;
			blendProperty.Enable    = false;
			break;
		}
		case DefaultBlendStateType::OverWrite:
		{
			blendProperty.ColorMask = ColorMask::All;
			blendProperty.Enable    = false;
			break;
		}
		case DefaultBlendStateType::AlphaBlend:
		{
			blendProperty.ColorMask = ColorMask::All;
			blendProperty.Enable    = true;
			break;
		}
	}
	return blendProperty;
}