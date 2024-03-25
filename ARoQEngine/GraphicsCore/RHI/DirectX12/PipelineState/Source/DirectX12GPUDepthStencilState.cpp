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
	_depthStencilDesc.DepthEnable      = _property.UseDepthTest || _property.DepthOperator != rhi::core::CompareOperator::Always;
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
	
	// Ç»Ç∫åpè≥Ç∂Ç·Ç»Ç¢ÇÒÇæ... d3d12
	_depthStencilDesc1.DepthEnable      = _depthStencilDesc.DepthEnable;
	_depthStencilDesc1.DepthWriteMask   = _depthStencilDesc.DepthWriteMask;
	_depthStencilDesc1.DepthFunc        = _depthStencilDesc.DepthFunc;
	_depthStencilDesc1.StencilEnable    = _depthStencilDesc.StencilEnable;
	_depthStencilDesc1.StencilReadMask  = _depthStencilDesc.StencilReadMask;
	_depthStencilDesc1.StencilWriteMask = _depthStencilDesc.StencilWriteMask;
	_depthStencilDesc1.FrontFace        = _depthStencilDesc.FrontFace;
	_depthStencilDesc1.BackFace         = _depthStencilDesc.BackFace;
#if PLATFORM_OS_WINDOWS
	_depthStencilDesc1.DepthBoundsTestEnable = _property.UseDepthBoundsTest;
#endif

}