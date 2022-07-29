//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12FrameBuffer.cpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_07_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
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
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device)
	: core::RHIFrameBuffer(device)
{
	const auto dxDevice = static_pointer_cast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-                Create Render Target Heap
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_renderTargetHeap.GetAddressOf())));
	_renderTargetHeap->SetName(L"DirectX12::RenderTargetHeap");
	_rtvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	/*-------------------------------------------------------------------
	-                Create Depth Stencil Heap
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_depthStencilHeap.GetAddressOf())));
	_depthStencilHeap->SetName(L"DirectX12::DepthStencilHeap");
	_dsvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	/*-------------------------------------------------------------------
	-                Create Render Target View
	---------------------------------------------------------------------*/


	/*-------------------------------------------------------------------
	-				 Set Depth / Stencil Descriptor
	---------------------------------------------------------------------*/
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	// depthStencilView.Format = 
	depthStencilViewDesc.Flags = D3D12_DSV_FLAGS::D3D12_DSV_FLAG_NONE;
	//dxDevice->CreateDepthStencilView(_depthStencilBuffer.Get(), &depthStencilViewDesc, _depthStencilHeap->GetCPUDescriptorHandleForHeapStart());

}