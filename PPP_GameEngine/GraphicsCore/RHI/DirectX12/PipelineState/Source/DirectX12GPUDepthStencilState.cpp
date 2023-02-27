//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12DepthStencilState.cpp
///             @brief  DepthStencil State
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUDepthStencilState.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUDepthStencilState::GPUDepthStencilState(const std::shared_ptr<rhi::core::RHIDevice>& device,
	const core::DepthStencilProperty& depthStencilProperty
) :
	core::GPUDepthStencilState(device, depthStencilProperty)
{
	_depthStencilDesc.DepthEnable      = _property.UseDepthTest;
	_depthStencilDesc.DepthWriteMask   = _property.DepthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO; // left: writable, right: not write
	_depthStencilDesc.DepthFunc        = EnumConverter::Convert(_property.DepthOperator);
	_depthStencilDesc.StencilEnable    = _property.StenciWriteEnable;
	_depthStencilDesc.StencilReadMask  = D3D12_DEFAULT_STENCIL_READ_MASK;
	_depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	_depthStencilDesc.FrontFace.StencilFunc        = EnumConverter::Convert(_property.Front.CompareOperator);
	_depthStencilDesc.FrontFace.StencilDepthFailOp = EnumConverter::Convert(_property.Front.DepthFailOperator);
	_depthStencilDesc.FrontFace.StencilPassOp      = EnumConverter::Convert(_property.Front.PassOperator);
	_depthStencilDesc.FrontFace.StencilFailOp      = EnumConverter::Convert(_property.Front.FailOperator);
	_depthStencilDesc.BackFace.StencilFunc         = EnumConverter::Convert(_property.Back.CompareOperator);
	_depthStencilDesc.BackFace.StencilDepthFailOp  = EnumConverter::Convert(_property.Back.DepthFailOperator);
	_depthStencilDesc.BackFace.StencilPassOp       = EnumConverter::Convert(_property.Back.PassOperator);
	_depthStencilDesc.BackFace.StencilFailOp       = EnumConverter::Convert(_property.Back.FailOperator);

}