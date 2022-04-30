//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12BlendState.cpp
///             @brief  BlendState
///             @author Toide Yutaro
///             @date   2022_04_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12BlendState.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
D3D12_BLEND_DESC GetBlendState(BlendStateType type)
{
	D3D12_BLEND_DESC blendDesc;
	blendDesc.IndependentBlendEnable      = FALSE; // all blend states is the same setting.
	blendDesc.AlphaToCoverageEnable       = TRUE;

	blendDesc.RenderTarget[0].LogicOp               = D3D12_LOGIC_OP_NOOP;
	blendDesc.RenderTarget[0].LogicOpEnable         = FALSE;
	blendDesc.RenderTarget[0].SrcBlend              = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend             = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp               = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha         = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha        = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha          = D3D12_BLEND_OP_ADD;

	switch (type)
	{
		case BlendStateType::NoColorWrite:
		{
			blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
			blendDesc.RenderTarget[0].BlendEnable = false;
			break;
		}
		case BlendStateType::OverWrite:
		{
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = false;
			break;
		}
		case BlendStateType::AlphaBlend:
		{
			blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
			blendDesc.RenderTarget[0].BlendEnable = true;
			break;
		}
	}

	for (UINT i = 1; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
	{
		blendDesc.RenderTarget[i] = blendDesc.RenderTarget[0];
	}
	return blendDesc;	
}