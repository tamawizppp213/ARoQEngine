//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUDepthStencilState.cpp
///  @brief  ピクセルに対して深度テストやステンシルテストを行うための設定項目を記述するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_11
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUDepthStencilState.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUDepthStencilState::GPUDepthStencilState(const gu::SharedPointer<rhi::core::RHIDevice>& device,
	const core::DepthStencilProperty& depthStencilProperty
) :
	core::GPUDepthStencilState(device, depthStencilProperty)
{
	// Depth
	_depthStencilDesc.Desc0.DepthEnable      = UseDepthTest();
	_depthStencilDesc.Desc0.DepthWriteMask   = _property.DepthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO; // left: writable, right: not write
	_depthStencilDesc.Desc0.DepthFunc        = EnumConverter::Convert(_property.DepthOperator);

	// Stencil
	_depthStencilDesc.Desc0.StencilEnable    = _property.StencilWriteEnable;
	_depthStencilDesc.Desc0.StencilReadMask  = D3D12_DEFAULT_STENCIL_READ_MASK;
	_depthStencilDesc.Desc0.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	// Front surface stencil
	_depthStencilDesc.Desc0.FrontFace.StencilFunc        = EnumConverter::Convert(_property.Front.CompareOperator);
	_depthStencilDesc.Desc0.FrontFace.StencilDepthFailOp = EnumConverter::Convert(_property.Front.DepthFailOperator);
	_depthStencilDesc.Desc0.FrontFace.StencilPassOp      = EnumConverter::Convert(_property.Front.PassOperator);
	_depthStencilDesc.Desc0.FrontFace.StencilFailOp      = EnumConverter::Convert(_property.Front.FailOperator);

	// Back surface stencil
	_depthStencilDesc.Desc0.BackFace.StencilFunc         = EnumConverter::Convert(_property.Back.CompareOperator);
	_depthStencilDesc.Desc0.BackFace.StencilDepthFailOp  = EnumConverter::Convert(_property.Back.DepthFailOperator);
	_depthStencilDesc.Desc0.BackFace.StencilPassOp       = EnumConverter::Convert(_property.Back.PassOperator);
	_depthStencilDesc.Desc0.BackFace.StencilFailOp       = EnumConverter::Convert(_property.Back.FailOperator);
	
#if PLATFORM_OS_WINDOWS
	_depthStencilDesc.Desc1.DepthBoundsTestEnable = _property.UseDepthBoundsTest;
#endif

}