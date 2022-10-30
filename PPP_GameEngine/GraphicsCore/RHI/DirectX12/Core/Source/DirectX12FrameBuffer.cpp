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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
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
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTargets, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderPass, renderTarget, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::~RHIFrameBuffer()
{
	_renderTargetViews.clear(); _renderTargetViews.shrink_to_fit();
	_renderTargets    .clear(); _renderTargets    .shrink_to_fit();
	if (_renderTargetHeap) { _renderTargetHeap.reset(); }
	if (_depthStencilHeap) { _depthStencilHeap.reset(); }
}
#pragma region Prepare
void RHIFrameBuffer::Prepare()
{
	const auto rhiDevice = std::static_pointer_cast<directX12::RHIDevice>(_device);
	const auto dxDevice  = rhiDevice->GetDevice();

	/*-------------------------------------------------------------------
	-                Create Render Target Heap
	---------------------------------------------------------------------*/
	if (!_renderTargetHeap) // device default heap
	{
		_renderTargetHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(rhiDevice->GetDefaultHeap(core::DescriptorHeapType::RTV));
		_renderTargetHeap->GetHeap()->SetName(L"DirectX12::RenderTargetHeap");
		_rtvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	/*-------------------------------------------------------------------
	-                Create Depth Stencil Heap
	---------------------------------------------------------------------*/
	if (!_depthStencilHeap) // device default heap
	{
		_depthStencilHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(rhiDevice->GetDefaultHeap(core::DescriptorHeapType::DSV));
		_depthStencilHeap->GetHeap()->SetName(L"DirectX12::DepthStencilHeap");
		_dsvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}

	/*-------------------------------------------------------------------
	-                Create Render Target View
	---------------------------------------------------------------------*/
	_renderTargetViews.resize(_renderTargets.size());
	for (size_t i = 0; i < _renderTargets.size(); ++i)
	{
		_renderTargetViews[i] = rhiDevice->CreateResourceView(core::ResourceViewType::RenderTarget, _renderTargets[i], nullptr);
	}

	/*-------------------------------------------------------------------
	-				 Set Depth / Stencil Descriptor
	---------------------------------------------------------------------*/
	if (_depthStencil != nullptr)
	{
		_depthStencilView = rhiDevice->CreateResourceView(core::ResourceViewType::DepthStencil, _depthStencil, nullptr);
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