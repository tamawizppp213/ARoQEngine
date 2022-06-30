//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12DepthStencilState.cpp
///             @brief  DepthStencil State
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUDepthStencilState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUDepthStencilState::GPUDepthStencilState(const std::shared_ptr<rhi::core::RHIDevice>& device,
	const bool  depthEnable, const bool depthWriteEnable,
	const bool  stencilEnable,
	const core::CompareOperator depthOperator,
	const core::StencilOperatorInfo& front, const core::StencilOperatorInfo& back
) :
	core::GPUDepthStencilState(device, depthEnable, depthWriteEnable, stencilEnable, depthOperator, front, back)
{
	_depthStencilDesc.DepthEnable      = _depthEnable;
	_depthStencilDesc.DepthWriteMask   = _depthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	_depthStencilDesc.DepthFunc        = EnumConverter::Convert(_depthOperator);
	_depthStencilDesc.StencilEnable    = _stencilEnable;
	_depthStencilDesc.StencilReadMask  = D3D12_DEFAULT_STENCIL_READ_MASK;
	_depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	_depthStencilDesc.FrontFace.StencilFunc        = EnumConverter::Convert(_frontFace.CompareOperator);
	_depthStencilDesc.FrontFace.StencilDepthFailOp = EnumConverter::Convert(_frontFace.DepthFailOperator);
	_depthStencilDesc.FrontFace.StencilPassOp      = EnumConverter::Convert(_frontFace.PassOperator);
	_depthStencilDesc.FrontFace.StencilFailOp      = EnumConverter::Convert(_frontFace.FailOperator);
	_depthStencilDesc.FrontFace.StencilFunc        = EnumConverter::Convert(_backFace.CompareOperator);
	_depthStencilDesc.FrontFace.StencilDepthFailOp = EnumConverter::Convert(_backFace.DepthFailOperator);
	_depthStencilDesc.FrontFace.StencilPassOp      = EnumConverter::Convert(_backFace.PassOperator);
	_depthStencilDesc.FrontFace.StencilFailOp      = EnumConverter::Convert(_backFace.FailOperator);

}