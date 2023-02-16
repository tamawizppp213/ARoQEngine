//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandList.cpp
///             @brief  CommandList 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RenderPass.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceLayout.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
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
RHICommandList::~RHICommandList()
{
	if (_commandList) { _commandList.Reset(); _commandList = nullptr; }
}

RHICommandList::RHICommandList(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator) : 
	rhi::core::RHICommandList(device, commandAllocator)
{
	/*-------------------------------------------------------------------
	-               Get device and command allocator
	---------------------------------------------------------------------*/
	const auto dxDevice     = static_cast<RHIDevice*>(_device.get())->GetDevice();
	const auto rhiAllocator = std::static_pointer_cast<directX12::RHICommandAllocator>(_commandAllocator);
	const auto dxAllocator  = rhiAllocator->GetAllocator();

	/*-------------------------------------------------------------------
	-               Get command list type
	---------------------------------------------------------------------*/
	_commandListType = rhiAllocator->GetCommandListType();

	/*-------------------------------------------------------------------
	-               Prepare Closed Command List
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandList(
		0,
		EnumConverter::Convert(_commandListType),
		dxAllocator.Get(), // Associated command allocator
		nullptr,           // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));

	ThrowIfFailed(_commandList->SetName(L"DirectX12::CommandList"));
	ThrowIfFailed(_commandList->Close());
	_isOpen = false;
}

#pragma endregion Constructor and Destructor

#pragma region Call Draw Frame
/****************************************************************************
*                     BeginRecording
*************************************************************************//**
*  @fn        void RHICommandList::BeginRecording()
*
*  @brief     Call at begin draw frame
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRecording()
{
	/*-------------------------------------------------------------------
	-          Reset command list and allocator
	---------------------------------------------------------------------*/
	_commandAllocator->CleanUp(); // command buffer clear
	ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.get())->GetAllocator().Get(), nullptr));
	_isOpen = true;
}

void RHICommandList::Reset(const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator)
{
	/*-------------------------------------------------------------------
	-          Reset command list and allocator
	---------------------------------------------------------------------*/
	if (commandAllocator) { _commandAllocator = commandAllocator; }
	ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.get())->GetAllocator().Get(), nullptr));
	_isOpen = false;
}

/****************************************************************************
*                     EndRecording
*************************************************************************//**
*  @fn        void RHICommandList::EndRecording()
*
*  @brief     Call at end draw frame. Close executed command list.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRecording()
{
	if (!_isOpen) { return; }
	ThrowIfFailed(_commandList->Close());
	_isOpen = false;
}

/****************************************************************************
*                     BeginRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPass(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIFrameBuffer>& frameBuffer)
*
*  @brief     Begin render pass and frame buffer.
*
*  @param[in] const std::shared_ptr<core::RHIRenderPass>& renderPass
*  @param[in] const std::shared_ptr<core::RHIFrameBuffer>& frameBuffer
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRenderPass(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	std::vector<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	TransitionResourceStates(static_cast<std::uint32_t>(frameBuffer->GetRenderTargetSize()), frameBuffer->GetRenderTargets().data(), states.data());

	/*-------------------------------------------------------------------
	-          Select renderpass and frame buffer action
	---------------------------------------------------------------------*/
	if (_device->IsSupportedRenderPass()) { BeginRenderPassImpl(std::static_pointer_cast<directX12::RHIRenderPass>(renderPass), std::static_pointer_cast<directX12::RHIFrameBuffer>(frameBuffer)); }
	else                                  { OMSetFrameBuffer   (std::static_pointer_cast<directX12::RHIRenderPass>(renderPass), std::static_pointer_cast<directX12::RHIFrameBuffer>(frameBuffer)); }

	_renderPass  = renderPass;
	_frameBuffer = frameBuffer;
}

/****************************************************************************
*                     EndRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::EndRenderPass()
*
*  @brief     End render pass
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRenderPass()
{
	if (_device->IsSupportedRenderPass()) { _commandList->EndRenderPass(); }

	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	std::vector<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	TransitionResourceStates(static_cast<std::uint32_t>(_frameBuffer->GetRenderTargetSize()), _frameBuffer->GetRenderTargets().data(), states.data());
}

#pragma endregion Call Draw Frame

#pragma region GPU Command
void RHICommandList::SetDescriptorHeap(const std::shared_ptr<core::RHIDescriptorHeap>& heap)
{
	const auto dxHeap = std::static_pointer_cast<directX12::RHIDescriptorHeap>(heap);
	_commandList->SetDescriptorHeaps(1, dxHeap->GetHeap().GetAddressOf());
}
/****************************************************************************
*                       SetPrimitiveTopology
*************************************************************************//**
*  @fn        void RHICommandList::SetPrimitiveTopology(core::PrimitiveTopology topology)
*  @brief     Regist Primitive topology to command list
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetPrimitiveTopology(core::PrimitiveTopology topology)
{
	_commandList->IASetPrimitiveTopology(EnumConverter::Convert(topology));
}
/****************************************************************************
*                       SetViewport
*************************************************************************//**
*  @fn        void rhi::directX12::RHICommandList::SetViewport(const core::Viewport* viewport, std::uint32_t numViewport)
*  @brief     Regist multi viewport to command list
*  @param[in] const core::Viewport view port
*  @param[in] std::uint32_t viewport count
*  @return 　　void
*****************************************************************************/
void rhi::directX12::RHICommandList::SetViewport(const core::Viewport* viewport, std::uint32_t numViewport)
{
	std::vector<D3D12_VIEWPORT> v(numViewport);
	for (UINT i = 0; i < numViewport; ++i)
	{
		v[i].TopLeftX = viewport->TopLeftX;
		v[i].TopLeftY = viewport->TopLeftY;
		v[i].Width    = viewport->Width;
		v[i].Height   = viewport->Height;
		v[i].MaxDepth = viewport->MaxDepth;
		v[i].MinDepth = viewport->MinDepth;
	}
	_commandList->RSSetViewports(numViewport, v.data());
}
/****************************************************************************
*                       SetScissorRect
*************************************************************************//**
*  @fn        void rhi::directX12::RHICommandList::SetScissor(const core::ScissorRect* rect, std::uint32_t numRect)
*  @brief     Regist multi scissorRect to command list
*  @param[in] const core::ScissorRect*
*  @param[in] std::uint32_t numRect
*  @return 　　void
*****************************************************************************/
void rhi::directX12::RHICommandList::SetScissor(const core::ScissorRect* rect, std::uint32_t numRect)
{
	std::vector<D3D12_RECT> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].left = rect->Left;
		r[i].right = rect->Right;
		r[i].bottom = rect->Bottom;
		r[i].top = rect->Top;
	}
	_commandList->RSSetScissorRects(numRect, r.data());
}
/****************************************************************************
*                       SetViewportAndScissor
*************************************************************************//**
*  @fn        void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
*  @brief     Regist viewport and scissorRect to command list
*  @param[in] const core::Viewport& viewport
*  @param[in] const core::ScissorRect& rect
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
{
	D3D12_VIEWPORT v = {};
	v.TopLeftX = viewport.TopLeftX;
	v.TopLeftY = viewport.TopLeftY;
	v.Width    = viewport.Width;
	v.Height   = viewport.Height;
	v.MaxDepth = viewport.MaxDepth;
	v.MinDepth = viewport.MinDepth;

	D3D12_RECT r = {};
	r.left   = rect.Left;
	r.right  = rect.Right;
	r.bottom = rect.Bottom;
	r.top    = rect.Top;

	_commandList->RSSetViewports(1, &v);
	_commandList->RSSetScissorRects(1, &r);
}

void RHICommandList::SetVertexBuffer(const std::shared_ptr<core::GPUBuffer>& buffer)
{
#if __DEBUG
	// Is vertex buffer
	assert(buffer->GetUsage() == core::ResourceUsage::VertexBuffer);
#endif

	// Set up vertex buffer view
	D3D12_VERTEX_BUFFER_VIEW view = {
		std::static_pointer_cast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->GetTotalByteSize()),
		static_cast<UINT>(buffer->GetElementByteSize())
	};
	// Regist command list
	_commandList->IASetVertexBuffers(0, 1, &view);
}

void RHICommandList::SetResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetGraphicsRootSignature(std::static_pointer_cast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetComputeResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetComputeRootSignature(std::static_pointer_cast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetGraphicsPipeline(const std::shared_ptr<core::GPUGraphicsPipelineState>& pipelineState)
{
	SetPrimitiveTopology(pipelineState->GetInputAssemblyState()->GetPrimitiveTopology());
	_commandList->SetPipelineState(std::static_pointer_cast<directX12::GPUGraphicsPipelineState>(pipelineState)->GetPipeline().Get());
}

void RHICommandList::SetComputePipeline(const std::shared_ptr<core::GPUComputePipelineState>& pipelineState)
{
	_commandList->SetPipelineState(std::static_pointer_cast<directX12::GPUComputePipelineState>(pipelineState)->GetPipeline().Get());
}
void RHICommandList::SetVertexBuffers(const std::vector<std::shared_ptr<core::GPUBuffer>>& buffers, const size_t startSlot)
{
	auto views = std::vector<D3D12_VERTEX_BUFFER_VIEW>(buffers.size());
	for (size_t i = 0; i < views.size(); ++i)
	{
#if __DEBUG
		assert(buffers[i]->GetUsage() == core::ResourceUsage::VertexBuffer);
#endif
		views[i].BufferLocation = std::static_pointer_cast<directX12::GPUBuffer>(buffers[i])->GetResourcePtr()->GetGPUVirtualAddress();
		views[i].SizeInBytes    = static_cast<UINT>(buffers[i]->GetTotalByteSize());
		views[i].StrideInBytes  = static_cast<UINT>(buffers[i]->GetElementByteSize());
	}

	_commandList->IASetVertexBuffers(static_cast<UINT>(startSlot), static_cast<UINT>(views.size()), views.data());
}

void RHICommandList::SetIndexBuffer(const std::shared_ptr<core::GPUBuffer>& buffer, const core::IndexType indexType)
{
#if __DEBUG
	assert(buffer->GetUsage() == core::ResourceUsage::IndexBuffer);
#endif

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = std::static_pointer_cast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<UINT>(buffer->GetTotalByteSize());
	view.Format         = EnumConverter::Convert(indexType);

	_commandList->IASetIndexBuffer(&view);
}

void RHICommandList::DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation)
{
	_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
}
void RHICommandList::DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, std::uint32_t startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RHICommandList::Dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
{
	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

#pragma region Transition Resource State
void RHICommandList::TransitionResourceState(const std::shared_ptr<core::GPUTexture>& texture, core::ResourceState after)
{
	BARRIER barrier = BARRIER::Transition(std::static_pointer_cast<directX12::GPUTexture>(texture)->GetResource().Get(),
		EnumConverter::Convert(texture->GetResourceState()), EnumConverter::Convert(after));
	_commandList->ResourceBarrier(1, &barrier);
	texture->TransitionResourceState(after);
}

void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const std::shared_ptr<core::GPUTexture>* textures, core::ResourceState* afters)
{
	std::vector<BARRIER> barriers = {};
	for (std::uint32_t i = 0; i < numStates; ++i)
	{
		BARRIER barrier = BARRIER::Transition(std::static_pointer_cast<directX12::GPUTexture>(textures[i])->GetResource().Get(),
			EnumConverter::Convert(textures[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
		textures[i]->TransitionResourceState(afters[i]);
		barriers.emplace_back(barrier);
	}
	_commandList->ResourceBarrier(numStates, barriers.data());
}

void RHICommandList::TransitionResourceStates(const std::vector<std::shared_ptr<core::GPUResource>>& resources, core::ResourceState* afters)
{
	std::vector<BARRIER> barriers(resources.size());
	for (std::uint32_t i = 0; i < resources.size(); ++i)
	{
		if (resources[i]->IsTexture())
		{
			barriers[i] = BARRIER::Transition(std::static_pointer_cast<directX12::GPUTexture>(resources[i])->GetResource().Get(),
				EnumConverter::Convert(resources[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
			resources[i]->TransitionResourceState(afters[i]);
		}
		else
		{
			barriers[i] = BARRIER::Transition(std::static_pointer_cast<directX12::GPUBuffer>(resources[i])->GetResource().Get(),
				EnumConverter::Convert(resources[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
			resources[i]->TransitionResourceState(afters[i]);
		}
	}
	_commandList->ResourceBarrier(barriers.size(), barriers.data());
}

#pragma endregion Transition Resource State
#pragma region Copy 
void RHICommandList::CopyResource(const std::shared_ptr<core::GPUTexture>& dest, const std::shared_ptr<core::GPUTexture>& source)
{
	CopyResource(std::static_pointer_cast<core::GPUResource>(dest), std::static_pointer_cast<core::GPUResource>(source));
}

void RHICommandList::CopyResource(const std::shared_ptr<core::GPUResource>& dest, const std::shared_ptr<core::GPUResource>& source)
{
	std::shared_ptr<core::GPUResource> resources[] = { dest, source };
	rhi::core::ResourceState befores[] = { dest->GetResourceState()            , source->GetResourceState() };
	rhi::core::ResourceState afters[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	TransitionResourceStates({ dest, source }, afters);

	if (dest->IsTexture() && source->IsTexture())
	{
		_commandList->CopyResource(
			std::static_pointer_cast<directX12::GPUTexture>(dest)->GetResource().Get(),
			std::static_pointer_cast<directX12::GPUTexture>(source)->GetResource().Get());
	}
	else if (dest->IsTexture() && !source->IsTexture())
	{
		_commandList->CopyResource(
			std::static_pointer_cast<directX12::GPUTexture>(dest)->GetResource().Get(),
			std::static_pointer_cast<directX12::GPUBuffer>(source)->GetResource().Get());
	}
	else
	{
		_commandList->CopyResource(
			std::static_pointer_cast<directX12::GPUBuffer>(dest)->GetResource().Get(),
			std::static_pointer_cast<directX12::GPUBuffer>(source)->GetResource().Get());
	}

	TransitionResourceStates({ dest, source }, befores);
}
#pragma endregion Copy
#pragma endregion GPU Command
#pragma region Private Function
/****************************************************************************
*                     BeginRenderPassImpl
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPassImpl(const std::shared_ptr<directX12::RHIRenderPass>& renderPass, const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer)<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If you used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const std::shared_ptr<directX12::RHIRenderPass>& renderPass
*  @param[in] const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::BeginRenderPassImpl(const std::shared_ptr<directX12::RHIRenderPass>& renderPass, const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	auto renderTargets = frameBuffer->GetRenderTargets();
	auto depthStencil  = frameBuffer->GetDepthStencil();
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil() != nullptr;
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	// render target 
	std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rtvDescs(renderTargets.size());
	for (size_t i = 0; i < rtvDescs.size(); ++i)
	{
		// get rtv handle
		const auto view = std::static_pointer_cast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));
#ifdef _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif

		const auto rtvHandle = view->GetCPUHandler();
		if (!rtvHandle.ptr) { continue; }

		// set render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS begin = {};
		begin.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->LoadOp);
		begin.Clear.ClearValue.Color[0] = renderPass->GetClearColor()[i].Color[core::ClearValue::Red];
		begin.Clear.ClearValue.Color[1] = renderPass->GetClearColor()[i].Color[core::ClearValue::Green];
		begin.Clear.ClearValue.Color[2] = renderPass->GetClearColor()[i].Color[core::ClearValue::Blue];
		begin.Clear.ClearValue.Color[3] = renderPass->GetClearColor()[i].Color[core::ClearValue::Alpha];
		// set render pass store op
		D3D12_RENDER_PASS_ENDING_ACCESS end = {};
		end.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->StoreOp);
		
		// set render pass descriptor
		rtvDescs[i].BeginningAccess = begin;
		rtvDescs[i].cpuDescriptor   = rtvHandle;
		rtvDescs[i].EndingAccess    = end;
	}
	// depth stencil
	D3D12_RENDER_PASS_DEPTH_STENCIL_DESC dsvDesc = {};
	if (hasDSV)
	{
		// get dsv handle
		const auto view = std::static_pointer_cast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());

#if __DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif

		const auto dsvHandle = view->GetCPUHandler();

		// set depth render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS depthBegin   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->LoadOp), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    depthEnd     = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StoreOp), {} };
		D3D12_RENDER_PASS_BEGINNING_ACCESS stencilBegin = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilLoad), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    stencilEnd   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilStore), {} };
		depthBegin.Clear.ClearValue.DepthStencil.Depth     = renderPass->GetDepthClear()->Depth;
		stencilBegin.Clear.ClearValue.DepthStencil.Stencil = renderPass->GetDepthClear()->Stencil;
		
		// set depth stencil descriptor
		dsvDesc.DepthBeginningAccess   = depthBegin;
		dsvDesc.DepthEndingAccess      = depthEnd;
		dsvDesc.StencilBeginningAccess = stencilBegin;
		dsvDesc.StencilEndingAccess    = stencilEnd;
		dsvDesc.cpuDescriptor          = dsvHandle;
	}

	_commandList->BeginRenderPass(static_cast<std::uint32_t>(rtvDescs.size()), hasRTV ? rtvDescs.data() : nullptr, hasDSV ? &dsvDesc : nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

/****************************************************************************
*                     OMSetFrameBuffer
*************************************************************************//**
*  @fn        void RHICommandList::OMSetFrameBuffer(const std::shared_ptr<directX12::RHIRenderPass>& renderPass, const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If not used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const std::shared_ptr<directX12::RHIRenderPass>& renderPass
*  @param[in] const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::OMSetFrameBuffer(const std::shared_ptr<directX12::RHIRenderPass>& renderPass, const std::shared_ptr<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	auto renderTargets = frameBuffer->GetRenderTargets();
	auto depthStencil  = frameBuffer->GetDepthStencil();
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil() != nullptr;
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandle(renderTargets.size());
	for (size_t i = 0; i < rtvHandle.size(); ++i)
	{
		const auto view = std::static_pointer_cast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));

#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif 

		rtvHandle[i] = view->GetCPUHandler();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	if (hasDSV)
	{
		const auto view = std::static_pointer_cast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());
#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif
		dsvHandle = view->GetCPUHandler();
	}
	/*-------------------------------------------------------------------
	-          Clear frame buffer resources
	---------------------------------------------------------------------*/
	// render target
	for (int i = 0; i < rtvHandle.size(); ++i)
	{
		const auto colorAttachment = renderPass->GetColorAttachment(i);
		if (colorAttachment->LoadOp != core::AttachmentLoad::Clear) { continue; }
		_commandList->ClearRenderTargetView(rtvHandle[i], renderPass->GetClearColor()[i].Color, 0, nullptr);
	}
	// depth stencil 
	if (hasDSV)
	{
		const auto depthAttachment = renderPass->GetDepthAttachment();

		// set clear flag 
		D3D12_CLEAR_FLAGS clearFlags = {};
		if (depthAttachment->LoadOp      == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_DEPTH; }
		if (depthAttachment->StencilLoad == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_STENCIL; }
		// clear depth stencil
		if (clearFlags)
		{
			_commandList->ClearDepthStencilView(dsvHandle, clearFlags, renderPass->GetDepthClear()->Depth, renderPass->GetDepthClear()->Stencil, 0, nullptr);
		}
	}
	/*-------------------------------------------------------------------
	-          Set render target and depth stencil
	---------------------------------------------------------------------*/
	_commandList->OMSetRenderTargets(static_cast<std::uint32_t>(rtvHandle.size()), hasRTV ? rtvHandle.data() : nullptr, FALSE, hasDSV ? &dsvHandle : nullptr);
}

#pragma endregion Private Function