//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12FrameBuffer.cpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_07_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12FrameBuffer.hpp"
#include "../Include/DirectX12CommandQueue.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12DescriptorHeap.hpp"
#include "../Include/DirectX12Debug.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTargets, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTarget, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	if (_renderTargetHeap) { _renderTargetHeap.Reset(); }
	if (_depthStencilHeap) { _depthStencilHeap.Reset(); }
}

#pragma endregion Constructor and Destructor

#pragma region Prepare
/****************************************************************************
*                      Prepare
*************************************************************************//**
*  @fn        void RHIFrameBuffer::Prepare()
*
*  @brief     Prepare heap and resource view (render target and depth stencil)
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIFrameBuffer::Prepare()
{
	const auto rhiDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto dxDevice  = rhiDevice->GetDevice();

	/*-------------------------------------------------------------------
	-                Create Render Target Heap
	---------------------------------------------------------------------*/
	if (!_renderTargetHeap) // device default heap
	{
		_renderTargetHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(rhiDevice->GetDefaultHeap(core::DescriptorHeapType::RTV));
		_renderTargetHeap->GetHeap()->SetName(L"DirectX12::RenderTargetHeap");
		_rtvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	/*-------------------------------------------------------------------
	-                Create Depth Stencil Heap
	---------------------------------------------------------------------*/
	if (!_depthStencilHeap) // device default heap
	{
		_depthStencilHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(rhiDevice->GetDefaultHeap(core::DescriptorHeapType::DSV));
		_depthStencilHeap->GetHeap()->SetName(L"DirectX12::DepthStencilHeap");
		_dsvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	/*-------------------------------------------------------------------
	-                Create Render Target View
	---------------------------------------------------------------------*/
	_renderTargetViews.resize(_renderTargets.size());
	_renderTargetSRVs.resize(_renderTargets.size());
	_renderTargetUAVs.resize(_renderTargets.size());

	for (size_t i = 0; i < _renderTargets.size(); ++i)
	{
		_renderTargetViews[i] = rhiDevice->CreateResourceView(core::ResourceViewType::RenderTarget, _renderTargets[i],0,0, nullptr);
		_renderTargetSRVs[i]  = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     , _renderTargets[i],0,0, nullptr);
		_renderTargetUAVs[i]  = rhiDevice->CreateResourceView(core::ResourceViewType::RWTexture   , _renderTargets[i],0,0, nullptr);
	}

	/*-------------------------------------------------------------------
	-				 Set Depth / Stencil Descriptor
	---------------------------------------------------------------------*/
	if (_depthStencil)
	{
		_depthStencilView = rhiDevice->CreateResourceView(core::ResourceViewType::DepthStencil, _depthStencil,0,0, nullptr);
		_depthStencilSRV  = rhiDevice->CreateResourceView(core::ResourceViewType::Texture     , _depthStencil,0,0,  nullptr);
	}
}
#pragma endregion Prepare
#pragma region Property
DescriptorHeapComPtr RHIFrameBuffer::GetRTVHeap() const noexcept
{
	return _renderTargetHeap->GetHeap();
}

DescriptorHeapComPtr RHIFrameBuffer::GetDSVHeap() const noexcept
{
	return _depthStencilHeap->GetHeap();
}

#pragma endregion Property