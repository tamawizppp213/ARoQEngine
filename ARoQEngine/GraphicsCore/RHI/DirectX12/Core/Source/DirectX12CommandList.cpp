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
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Query.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBarrierBatcher.hpp"
#include <stdexcept>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Memory/Include/GUAlignment.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHICommandList::~RHICommandList()
{
	if (_commandList) { _commandList.Reset(); _commandList = nullptr; }
}

RHICommandList::RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const gu::tstring& name) : 
	rhi::core::RHICommandList(device, commandAllocator)
{
	/*-------------------------------------------------------------------
	-               Get device and command allocator
	---------------------------------------------------------------------*/
	const auto dxDevice     = static_cast<RHIDevice*>(_device.Get())->GetDevice();
	const auto rhiAllocator = gu::StaticPointerCast<directX12::RHICommandAllocator>(_commandAllocator);
	const auto dxAllocator  = rhiAllocator->GetAllocator();

	/*-------------------------------------------------------------------
	-               Get command list type
	---------------------------------------------------------------------*/
	_commandListType = rhiAllocator->GetCommandListType();

	/*-------------------------------------------------------------------
	-               Prepare Closed Command List
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandList(
		_device->GetGPUMask().Value(),            // GPU index
		EnumConverter::Convert(_commandListType),
		dxAllocator.Get(),                        // Associated command allocator
		nullptr,                                  // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));

	ThrowIfFailed(_commandList->SetName(name.CString()));
	ThrowIfFailed(_commandList->Close());
	_isOpen = false;

	_barrierBatcher = gu::MakeShared<GPUBarrierBatcher>();
}

#pragma endregion Constructor and Destructor

#pragma region Call Draw Frame
/****************************************************************************
*                     BeginRecording
*************************************************************************//**
/* @brief     �R�}���h���X�g���L�^��ԂɕύX���܂�. �����Draw�֐��̍ŏ��Ɏg�p���܂� @n
*  @param[in] �`��t���[�����ɌĂ΂��ꍇ�ɃR�}���h�A���P�[�^�̒��g��Reset���邩�����肷��bool�l.@n
*             �`��t���[�����ɌĂԂ̂�, �R�}���h���X�g��؂�ւ���ۂɎg�p�����\�������邽�߂ł�.
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRecording(const bool stillMidFrame)
{
	/*-------------------------------------------------------------------
	-        �R�}���h���X�g���L�^�\��Ԃ������ꍇ, Reset���邱�Ƃ��o���Ȃ�.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-        �`��t���[���̓r���łȂ����CommandAllocator�̃o�b�t�@��擪�ɖ߂�.
	---------------------------------------------------------------------*/
	if (_commandAllocator)
	{
		if (!stillMidFrame)
		{
			_commandAllocator->CleanUp();
		} // command buffer clear
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	
	/*-------------------------------------------------------------------
	-        �R�}���h���X�g���L�^�\��ԂɕύX���܂�
	---------------------------------------------------------------------*/
	_isOpen          = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     EndRecording
*************************************************************************//**
/*  @brief     �R�}���h���X�g���L�^��Ԃ�����s�\��ԂɕύX���܂�. �����Draw�֐��̍Ō�Ɏg�p���܂�
* 
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRecording()
{
	/*-------------------------------------------------------------------
	-      �@���ɕ����Ă���Ȃ炱�̌�̏����͍s��Ȃ�
	---------------------------------------------------------------------*/
	if (IsClosed()) { _RPT0(_CRT_WARN, "Already closed. \n");  return; }
	
	/*-------------------------------------------------------------------
	-      �@�L�^��Ԃ���N���[�Y��ԂɈȍ~����
	---------------------------------------------------------------------*/
	ThrowIfFailed(_commandList->Close());

	/*-------------------------------------------------------------------
	-      �@�I������
	---------------------------------------------------------------------*/
	_isOpen = false;
	_beginRenderPass = false;
}

/****************************************************************************
*                     Reset
*************************************************************************//**
*  @fn        void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
*
*  @brief     Proceed to the record state. �R�}���h���X�g���L�^��ԂɕύX���܂�.
		      ��{�I�ɂ�, Reset�ł͂Ȃ�BeginRecording���g�p���Ă�������.
*
*  @param[in] const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator
*             �R�}���h�����߂Ă����o�b�t�@�ł�
*
*  @return    void
*****************************************************************************/
void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
{
	/*-------------------------------------------------------------------
	-        �R�}���h���X�g���L�^�\��Ԃ������ꍇ, Reset���邱�Ƃ��o���Ȃ�.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-         �V�����R�}���h�o�b�t�@�������L���[�̎�ނł��邩���m�F����
	---------------------------------------------------------------------*/
	if (commandAllocator) 
	{
		// �R�}���h���X�g�̎�ރ`�F�b�N
		if (_commandAllocator->GetCommandListType() != GetType()) 
		{
			_RPTW0(_CRT_WARN, L"Different commandList type");
			return;
		}

		// �R�}���h���X�g��o�^
		_commandAllocator = commandAllocator; 

		// Reset
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	else
	{
		return;
	}

	/*-------------------------------------------------------------------
	-        �R�}���h���X�g���J���Ă����Ԃɂ���
	---------------------------------------------------------------------*/
	_isOpen = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     BeginRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
*
*  @brief     Begin render pass and frame buffer.
*
*  @param[in] const gu::SharedPointer<core::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	for (uint64 i = 0; i < frameBuffer->GetRenderTargetSize(); ++i)
	{
		PushTransitionBarrier(frameBuffer->GetRenderTargets()[i], states[i]);
	}
	FlushResourceBarriers();

	/*-------------------------------------------------------------------
	-          Select renderpass and frame buffer action
	---------------------------------------------------------------------*/
	if (_device->IsSupportedRenderPass()) { BeginRenderPassImpl(gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }
	else                                  { OMSetFrameBuffer   (gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }

	// �Q�ƃJ�E���g�������ɑ�������o�O�Ή��Ɏg�p���܂���.
	if (_renderPass ) { _renderPass.Reset(); }
	if (_frameBuffer) { _frameBuffer.Reset(); }
	_renderPass  = renderPass;
	_frameBuffer = frameBuffer;
	_beginRenderPass = true;
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
	if (!_beginRenderPass) { return; }
	if (_device->IsSupportedRenderPass()) { _commandList->EndRenderPass(); }

	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	for (uint64 i = 0; i < _frameBuffer->GetRenderTargetSize(); ++i)
	{
		PushTransitionBarrier(_frameBuffer->GetRenderTargets()[i], states[i]);
	}
	FlushResourceBarriers();

	_beginRenderPass = false;
}

#pragma endregion Call Draw Frame

#pragma region GPU Command
/****************************************************************************
*                       SetDepthBounds
*************************************************************************//**
/* @brief     �[�x���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳���܂�.
*  
*  @param[in] const float �ŏ��̐[�x���
*  @param[in] const float �ő�̐[�x���
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetDepthBounds(const float minDepth, const float maxDepth)
{
#if PLATFORM_OS_WINDOWS
	if (_device->IsSupportedDepthBoundsTest())
	{
		_commandList->OMSetDepthBounds(minDepth, maxDepth);
	}
#endif
}

void RHICommandList::SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap)
{
	const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(heap);
	_commandList->SetDescriptorHeaps(1, dxHeap->GetHeap().GetAddressOf());
}

#pragma region Query
/****************************************************************************
*                       BeginQuery
*************************************************************************//**
*  @fn        void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU�����擾���邽�߂̃N�G�����J�n���܂�
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return �@�@void
*****************************************************************************/
void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
{
	/*-------------------------------------------------------------------
	-              Query�̎擾
	---------------------------------------------------------------------*/
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	
	Checkf(query, "query is nullptr");
	Check(query->GetDxQueryType() == D3D12_QUERY_TYPE_OCCLUSION ||
		  query->GetDxQueryType() == D3D12_QUERY_TYPE_PIPELINE_STATISTICS);
	
	/*-------------------------------------------------------------------
	-              QueryHeap�̎擾
	---------------------------------------------------------------------*/
	const auto heap = query->GetHeap();

	Checkf(heap, "heap is nullptr");

	/*-------------------------------------------------------------------
	-               Common command
	---------------------------------------------------------------------*/
	_commandList->BeginQuery(heap.Get(), query->GetDxQueryType(), location.QueryID);
}

/****************************************************************************
*                       EndQuery
*************************************************************************//**
*  @fn        void RHICommandList::EndQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU�����擾���邽�߂̃N�G�����I�����܂�
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return �@�@void
*****************************************************************************/
void RHICommandList::EndQuery(const core::QueryResultLocation& location)
{
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	Checkf(query, "query is nullptr");
	
	// �N�G���̏I��
	_commandList->EndQuery(query->GetHeap().Get(), query->GetDxQueryType(), location.QueryID);
}
#pragma endregion Query
/****************************************************************************
*                       SetPrimitiveTopology
*************************************************************************//**
/* @brief    ���_���̂Ȃ�����ݒ肵�܂�. 
* 
*  @param[in] �v���~�e�B�u�̃g�|���W�[���
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetPrimitiveTopology(const core::PrimitiveTopology topology)
{
	_commandList->IASetPrimitiveTopology(EnumConverter::Convert(topology));
}

/****************************************************************************
*                       SetViewport
*************************************************************************//**
/*  @brief     �r���[�|�[�g�ɂ���ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
*  @param[in] const core::Viewport* : �`��̈�������P��̃r���[�|�[�g
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport& viewport)
{
	const D3D12_VIEWPORT v = 
	{
		.TopLeftX = viewport.TopLeftX,
		.TopLeftY = viewport.TopLeftY,
		.Width    = viewport.Width,
		.Height   = viewport.Height,
		.MinDepth = viewport.MinDepth,
		.MaxDepth = viewport.MaxDepth
	};


	const D3D12_RECT r =
	{
		.left   = static_cast<LONG>(viewport.TopLeftX),
		.top    = static_cast<LONG>(viewport.TopLeftY),
		.right  = static_cast<LONG>(viewport.TopLeftX + viewport.Width),
		.bottom = static_cast<LONG>(viewport.TopLeftY + viewport.Height)
	};

	_commandList->RSSetViewports(1, &v);
	_commandList->RSSetScissorRects(1, &r);
}


/****************************************************************************
*                       SetViewport
*************************************************************************//**
/*  @brief     �r���[�|�[�g�̔z��(�A�h���X)�����ĕ`��̈��ݒ肵�܂�
*  @param[in] const core::Viewport* : �`��̈���L�q�����z��, �������͒P���Viewport�̃A�h���X
*  @param[in] const gu::uint32 : �r���[�|�[�g�̔z��
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport* viewport, const gu::uint32 numViewport)
{
	gu::DynamicArray<D3D12_VIEWPORT> v(numViewport);
	for (uint32 i = 0; i < numViewport; ++i)
	{
		v[i].TopLeftX = viewport[i].TopLeftX;
		v[i].TopLeftY = viewport[i].TopLeftY;
		v[i].Width    = viewport[i].Width;
		v[i].Height   = viewport[i].Height;
		v[i].MaxDepth = viewport[i].MaxDepth;
		v[i].MinDepth = viewport[i].MinDepth;
	}

	gu::DynamicArray<D3D12_RECT> r(numViewport);
	for (uint32 i = 0; i < numViewport; ++i)
	{
		r[i].left   = static_cast<LONG>(viewport[i].TopLeftX);
		r[i].top    = static_cast<LONG>(viewport[i].TopLeftY);
		r[i].right  = static_cast<LONG>(viewport[i].TopLeftX + viewport[i].Width);
		r[i].bottom = static_cast<LONG>(viewport[i].TopLeftY + viewport[i].Height);
	}
	_commandList->RSSetViewports   (numViewport, v.Data());
	_commandList->RSSetScissorRects(numViewport, r.Data());
}

/****************************************************************************
*                       SetViewport
*************************************************************************//**
/* @brief     VR�̂悤�ȗ��̎����s�����ɐݒ肷��`��̈�ł�
* 
*  @param[in] const core::Viewport& �����̎���������`��̈�
*  @param[in] const core::Viewport& �E���̎���������`��̈�
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView)
{
	D3D12_VIEWPORT viewports[2] = {};

	viewports[0].TopLeftX = leftView.TopLeftX;
	viewports[0].TopLeftY = leftView.TopLeftY;
	viewports[0].Width    = leftView.Width;
	viewports[0].Height   = leftView.Height;
	viewports[0].MinDepth = leftView.MinDepth;
	viewports[0].MaxDepth = leftView.MaxDepth;

	viewports[1].TopLeftX = leftView.TopLeftX;
	viewports[1].TopLeftY = leftView.TopLeftY;
	viewports[1].Width    = leftView.Width;
	viewports[1].Height   = leftView.Height;
	viewports[1].MinDepth = leftView.MinDepth;
	viewports[1].MaxDepth = leftView.MaxDepth;

	D3D12_RECT scissorRects[2] =
	{
		{(LONG)viewports[0].TopLeftX, (LONG)viewports[0].TopLeftY, (LONG)(viewports[0].TopLeftX + viewports[0].Width), (LONG)(viewports[0].TopLeftY + viewports[0].Height)},
		{(LONG)viewports[1].TopLeftX, (LONG)viewports[1].TopLeftY, (LONG)(viewports[1].TopLeftX + viewports[1].Width), (LONG)(viewports[1].TopLeftY + viewports[1].Height)}
	};

	_commandList->RSSetViewports   (2, viewports);
	_commandList->RSSetScissorRects(2, scissorRects);
}

/****************************************************************************
*                       SetScissorRect
*************************************************************************//**
/*  @brief     �r���[�|�[�g���Ŏ��ۂɕ`�悳���̈�𐧌����邽�߂̃V�U�[��`���蓮�Őݒ肵�܂�.
* 
*  @param[in] const core::ScissorRect* : �`��̈�𐧌����邽�߂̃V�U�[��`�̔z��
*  @param[in] const gu::uint32 : �V�U�[��`�̔z��
* 
*  @return �@�@void
*****************************************************************************/
void RHICommandList::SetScissor(const core::ScissorRect* rect, const gu::uint32 numRect)
{
	gu::DynamicArray<D3D12_RECT> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].left   = rect->Left;
		r[i].right  = rect->Right;
		r[i].bottom = rect->Bottom;
		r[i].top    = rect->Top;
	}
	_commandList->RSSetScissorRects(numRect, r.Data());
}

/****************************************************************************
*                       SetViewportAndScissor
*************************************************************************//**
/* @brief     �`��̈�������r���[�|�[�g��, ���̒��Ŏ��ۂɕ`�悳���͈͂��w�肷��V�U�[��`�����ꂼ��蓮�Őݒ肵�܂�.
* 
*  @param[in] const core::Viewport& �`��̈�������r���[�|�[�g
*  @param[in] const core::ScissorRect& ���ۂɕ`�悳���͈͂������V�U�[��`
* 
*  @return �@�@void
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

void RHICommandList::SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer)
{
#if __DEBUG
	// Is vertex buffer
	assert(buffer->GetUsage() == core::BufferCreateFlags::VertexBuffer);
#endif

	// Set up vertex buffer view
	D3D12_VERTEX_BUFFER_VIEW view = {
		gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->GetTotalByteSize()),
		static_cast<UINT>(buffer->GetElementByteSize())
	};
	// Regist command list
	_commandList->IASetVertexBuffers(0, 1, &view);
}

void RHICommandList::SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetGraphicsRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetComputeRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState)
{
	SetPrimitiveTopology(pipelineState->GetInputAssemblyState()->GetPrimitiveTopology());
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUGraphicsPipelineState>(pipelineState)->GetPipeline().Get());
}

void RHICommandList::SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipelineState)
{
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUComputePipelineState>(pipelineState)->GetPipeline().Get());
}
void RHICommandList::SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot)
{
	auto views = gu::DynamicArray<D3D12_VERTEX_BUFFER_VIEW>(buffers.Size());
	for (size_t i = 0; i < views.Size(); ++i)
	{
#if __DEBUG
		assert(buffers[i]->GetUsage() == core::BufferCreateFlags::VertexBuffer);
#endif
		views[i].BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffers[i])->GetResourcePtr()->GetGPUVirtualAddress();
		views[i].SizeInBytes    = static_cast<UINT>(buffers[i]->GetTotalByteSize());
		views[i].StrideInBytes  = static_cast<UINT>(buffers[i]->GetElementByteSize());
	}

	_commandList->IASetVertexBuffers(static_cast<UINT>(startSlot), static_cast<UINT>(views.Size()), views.Data());
}

void RHICommandList::SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType)
{
#if __DEBUG
	assert(buffer->GetUsage() == core::BufferCreateFlags::IndexBuffer);
#endif

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<UINT>(buffer->GetTotalByteSize());
	view.Format         = EnumConverter::Convert(indexType);

	_commandList->IASetIndexBuffer(&view);
}

void RHICommandList::DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation)
{
	_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
}


void RHICommandList::DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation, gu::uint32 startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RHICommandList::DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount)
{
	const auto dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(argumentBuffer);
	_commandList->ExecuteIndirect(dxDevice->GetDefaultDrawIndexedIndirectCommandSignature().Get(), drawCallCount, dxBuffer->GetResourcePtr(), 0, nullptr, 0);
}

void RHICommandList::DispatchMesh(const gu::uint32 threadGroupCountX, const gu::uint32 threadGroupCountY, const gu::uint32 threadGroupCountZ)
{
	_commandList->DispatchMesh(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

void RHICommandList::Dispatch(gu::uint32 threadGroupCountX, gu::uint32 threadGroupCountY, gu::uint32 threadGroupCountZ)
{
	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

#pragma region Transition Resource State
/*!**********************************************************************
*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�.
*  @param[in] const gu::SharedPointer<core::GPUResource> GPU���\�[�X
*  @param[in] const core::ResourceState �J�ڑO�̃��\�[�X���
*  @param[in] const core::ResourceState �J�ڌ�̃��\�[�X���
*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�) 0xfffffff�̏ꍇ�͑S�ẴC���f�b�N�X�ŗL��������܂�
*  @return    void
*************************************************************************/
void RHICommandList::PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource)
{
	if (resource->GetResourceState() == after) { return; }

	_barrierBatcher->PushTransitionBarrier(resource, after, subresource );
}

/*!**********************************************************************
*  @brief  �����������̈�Ƀ}�b�s���O���ꂽ������GPU���\�[�X�ɑ΂�, �g�p���郊�\�[�X���̂�؂�ւ���ۂɎg�p���܂�. @n
*          �����Ɏg�p���Ȃ����Ƃ��S�ۂ���Ă��郊�\�[�X�̃�������ߖ񂷂邱�Ƃ��\�ƂȂ�܂�. @n
*          ������, �{�֐����g�p����ꍇ��,CreateCommittedResource�ł͖���, CreatePlacedResource���g�p�������@�Ńq�[�v�̊m�ۂ��s���Ă�������.@n
*          (�ʁX�̃q�[�v������Ă��܂����ƂɂȂ�, �����������̈������Ȃ��Ȃ邽��.)
*  @note   https://logicalbeat.jp/blog/8185/ (AliasingBarrier�̊��p���@�ɂ��Ă̋L�q)
*  @param[in] const gu::SharedPointer<core::GPUResource> �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
*  @param[in] const gu::SharedPointer<core::GPUResource> �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
*  @return    void
*************************************************************************/
void RHICommandList::PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before, const gu::SharedPointer<core::GPUResource>& after)
{
	_barrierBatcher->PushAliasingBarrier
	(
		before->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(before)->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(before)->GetResourcePtr(),
		after ->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(after) ->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(after) ->GetResourcePtr()
	);
}
		
/*!**********************************************************************
*  @brief     Unordered access view��p�̏�ԃo���A @n
*             UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
*  @param[in] const gu::SharedPointer<core::GPUResource> Unordered access view������GPU�o�b�t�@
*  @return    void
*************************************************************************/
void RHICommandList::PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& resource)
{
	_barrierBatcher->PushUAVBarrier
	(
		resource->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer>(resource)->GetResourcePtr() : gu::StaticPointerCast<directX12::GPUTexture>(resource)->GetResourcePtr()
	);
}

/*!**********************************************************************
*  @brief  �R�}���h���X�g���g����ResourceBarrier���܂Ƃ߂ČĂяo���܂�.
*  @param[in] void
*  @return    void
*************************************************************************/
void RHICommandList::FlushResourceBarriers()
{
	_barrierBatcher->Flush(_commandList.Get());
}

#pragma endregion Transition Resource State
#pragma region Copy 
/****************************************************************************
*                     CopyResource
*************************************************************************//**
/* @brief   ���郊�\�[�X�̗̈���܂Ƃ߂ĕʂ̃��\�[�X�ɃR�s�[����.
*           �g�ݍ��킹�ɉ����Ď����Ńo�b�t�@���e�N�X�`�����𔻒肵�܂�
*
*  @param[in] const gu::SharedPointer<core::GPUTexture>& �R�s�[��̃o�b�t�@
*  @param[in] const gu::SharedPointer<core::GPUTexture>& �R�s�[���̃o�b�t�@

*  @return �@�@void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source)
{
	CopyResource(gu::StaticPointerCast<core::GPUResource>(dest), gu::StaticPointerCast<core::GPUResource>(source));
}

/****************************************************************************
*                     CopyResource
*************************************************************************//**
/* @brief     ���郊�\�[�X�̗̈���܂Ƃ߂ĕʂ̃��\�[�X�ɃR�s�[����. 
*           �g�ݍ��킹�ɉ����Ď����Ńo�b�t�@���e�N�X�`�����𔻒肵�܂�
*
*  @param[in] gu::SharedPointer<core::GPUBuffer> �R�s�[��̃o�b�t�@
*  @param[in] gu::SharedPointer<core::GPUBuffer>& �R�s�[���̃o�b�t�@

*  @return �@�@void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source)
{
	gu::SharedPointer<core::GPUResource> resources[] = { dest, source };
	rhi::core::ResourceState befores[] = { dest->GetResourceState()            , source->GetResourceState() };
	rhi::core::ResourceState afters[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	_barrierBatcher->PushTransitionBarrier(dest, afters[0]);
	_barrierBatcher->PushTransitionBarrier(source, afters[1]);
	_barrierBatcher->Flush(_commandList.Get());

	if (dest->IsTexture() && source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUTexture>(source)->GetResource().Get());
	}
	else if (dest->IsTexture() && !source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}
	else
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUBuffer>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}

	_barrierBatcher->PushTransitionBarrier(dest  , befores[0]);
	_barrierBatcher->PushTransitionBarrier(source, befores[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

/****************************************************************************
*                     CopyBufferRegion
*************************************************************************//**
/*  @brief     GPU�o�b�t�@�̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
*
*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[��̃o�b�t�@
*  @param[in] const gu::uint64 : �R�s�[��̏����������|�C���^�����炷offset byte
*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[���̃o�b�t�@
*  @param[in] const gu::uint64 : �R�s�[���̏����ǂݎ��|�C���^�����炷offset byte
*  @param[in] const gu::uint64 copyByteSize �R�s�[�������o�C�g��

*  @return �@�@void
*****************************************************************************/
void RHICommandList::CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& destination, const gu::uint64 destinationOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize)
{
	using enum core::ResourceState;

	const auto rhiDestBuffer       = static_cast<directX12::GPUBuffer*>(destination.Get());
	const auto rhiSourceBuffer     = static_cast<directX12::GPUBuffer*>(source.Get());
	const auto destinationResource = rhiDestBuffer  ->GetResourcePtr();
	const auto sourceResource      = rhiSourceBuffer->GetResourcePtr();

	/*-------------------------------------------------------------------
	-       �R�s�[�\���̃`�F�b�N
	---------------------------------------------------------------------*/
	//"CopyBufferRegion�𓯂����\�[�X�Ŏg�p���邱�Ƃ͂ł��܂���. 
	// ����́A�R�s�[���ƃR�s�[��̗������������\�[�X����T�u�A���P�[�g����Ă���ꍇ�ɔ�������\��������܂�")�G
	Checkf(sourceResource != destinationResource, "CopyBufferRegion cannot be used on the same resource. This can happen when both the source and the dest are suballocated from the same resource.");
	
	// �o�b�t�@�͈͓̔��Ɏ��܂��Ă��邩
	Check(destinationOffset + copyByteSize <= rhiDestBuffer  ->GetTotalByteSize());
	Check(sourceOffset      + copyByteSize <= rhiSourceBuffer->GetTotalByteSize());

	/*-------------------------------------------------------------------
	-         �R�s�[�̏�����ԂƏI����Ԃ̒�`
	---------------------------------------------------------------------*/
	core::ResourceState barrierAfterStates[]   = { destination->GetResourceState(), source->GetResourceState() };
	core::ResourceState barrierBeforeStates[]  = { CopyDestination, CopySource };

	// �o���A�ɂ��, ���\�[�X�̓ǂݕ���`����
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierAfterStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source     ), barrierAfterStates[1]);
	_barrierBatcher->Flush(_commandList.Get());

	/*-------------------------------------------------------------------
	-           GPU�̈�̃R�s�[
	---------------------------------------------------------------------*/
	_commandList->CopyBufferRegion(destinationResource, destinationOffset, sourceResource, sourceOffset, copyByteSize);

	/*-------------------------------------------------------------------
	-          ���̏�Ԃɖ߂�
	---------------------------------------------------------------------*/
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierBeforeStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source)     , barrierBeforeStates[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

/****************************************************************************
*                     CopyTextureRegion
*************************************************************************//**
/*  @brief     GPU�e�N�X�`���̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
*
*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[��̃e�N�X�`��
*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[���̃e�N�X�`��
*  @param[in] const core::GPUTextureCopyInfo : �R�s�[����ۂ̏��

*  @return �@�@void
*****************************************************************************/
void RHICommandList::CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo)
{
	const auto  dxDestinationTexture  = static_cast<directX12::GPUTexture*>(destination.Get());
	const auto  dxSourceTexture       = static_cast<directX12::GPUTexture*>(source.Get());
	const auto& dxDestinationDesc     = dxDestinationTexture->GetMetaData();
	const auto& dxSourceDesc          = dxSourceTexture->GetMetaData();

	/*-------------------------------------------------------------------
	-         �R�s�[�̎d�����m�F����
	---------------------------------------------------------------------*/
	// �S�Ẵs�N�Z�����R�s�[���邩�ǂ���
	const bool isAllPixel       = destination->GetByteSize() == source->GetByteSize() &&
		                          (copyInfo.TexelSize.IsZero() || copyInfo.TexelSize == source->GetTexelSize());

	// �S�ẴT�u���\�[�XID���R�s�[���邩�ǂ���
	const bool isAllSubresource = (dxDestinationDesc.MipMapLevels == dxSourceDesc.MipMapLevels) && 
		                          (dxSourceDesc.MipMapLevels      == copyInfo.MipMapCount) && 
		                          (destination->GetArrayLength()  == source->GetArrayLength()) && 
		                          (source->GetArrayLength()       == copyInfo.ArraySliceCount);

	// GPU����CPU��Readback���������邩�ǂ���
	const bool useReadback = gu::HasAnyFlags(destination->GetUsage(), core::TextureCreateFlags::CPUReadback);

	// �͈̓`�F�b�N
	Checkf(copyInfo.ArraySliceCount + copyInfo.DestinationInitArraySlice <= destination->GetArrayLength()   , "This function exceeds the array slice count in the destination texture\n");
	Checkf(copyInfo.ArraySliceCount + copyInfo.SourceInitArraySlice      <= source     ->GetArrayLength()   , "This function exceeds the array slice count in the source texture\n");
	Checkf(copyInfo.MipMapCount     + copyInfo.DestinationInitMipMap     <= destination->GetMaxMipMapLevel(), "This function exceeds the mipmap count in the destination texture\n ");
	Checkf(copyInfo.MipMapCount     + copyInfo.SourceInitMipMap          <= source     ->GetMaxMipMapLevel(), "This function exceeds the mipmap count in the source texture \n");


	/*-------------------------------------------------------------------
	-         �R�s�[�̏�����ԂƏI����Ԃ̒�`
	---------------------------------------------------------------------*/
	core::ResourceState barrierAfterStates[]  = { destination->GetResourceState(), source->GetResourceState() };
	core::ResourceState barrierBeforeStates[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	// �o���A�ɂ��, ���\�[�X�̓ǂݕ���`����
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierAfterStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source)     , barrierAfterStates[1]);

	/*-------------------------------------------------------------------
	-       �ꕔ���̂݃R�s�[����ꍇ
	---------------------------------------------------------------------*/
	if (!isAllPixel || !isAllSubresource || useReadback)
	{
		const auto copyTexelSize = copyInfo.TexelSize.IsZero() ? source->GetTexelSize() >> copyInfo.SourceInitMipMap : copyInfo.TexelSize;

		// �e�N�X�`���z��̎�ނɉ����ăR�s�[
		for (gu::uint16 sliceIndex = 0; sliceIndex < copyInfo.ArraySliceCount; ++sliceIndex)
		{
			const auto sourceSliceIndex      = copyInfo.SourceInitArraySlice      + sliceIndex;
			const auto destinationSliceIndex = copyInfo.DestinationInitArraySlice + sliceIndex;

			// mipmap�̃��x���ɉ����ăR�s�[
			for (gu::uint32 mipmapIndex = 0; mipmapIndex < copyInfo.MipMapCount; ++mipmapIndex)
			{
				const auto sourceMipmapIndex      = copyInfo.SourceInitMipMap      + mipmapIndex;
				const auto destinationMipmapIndex = copyInfo.DestinationInitMipMap + mipmapIndex;
				const auto endCopyPosition        = copyInfo.SourceInitTexelPosition + copyTexelSize;

				// Todo : Box (�܂��A���C�������g�̎w�肪�����Ă��܂���)
				const D3D12_BOX sourceBox =
				{
					.left   = copyInfo.SourceInitTexelPosition.x >> mipmapIndex,
					.top    = copyInfo.SourceInitTexelPosition.y >> mipmapIndex,
					.front  = copyInfo.SourceInitTexelPosition.z >> mipmapIndex, 
					.right  = 0, //gu::Alignment::AlignUpArbitary(endCopyPosition.x > 1u << mipmapIndex ? 1u : endCopyPosition.x >> mipmapIndex),
					.bottom = 0,
					.back   = 0
				};

				// �R�s�[����CopyLocation
				D3D12_TEXTURE_COPY_LOCATION sourceCopyLocation{};
				sourceCopyLocation.pResource = dxSourceTexture->GetResourcePtr();
				sourceCopyLocation.Type      = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				sourceCopyLocation.SubresourceIndex = dxSourceTexture->GetSubresourceIndex(sourceMipmapIndex, sourceSliceIndex, 0);

				// �R�s�[���CopyLocation
				D3D12_TEXTURE_COPY_LOCATION destinationCopyLocation{};
				destinationCopyLocation.pResource = dxDestinationTexture->GetResourcePtr();
				destinationCopyLocation.Type      = useReadback ? D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT : D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
				destinationCopyLocation.SubresourceIndex = dxDestinationTexture->GetSubresourceIndex(destinationMipmapIndex, destinationSliceIndex, 0);

				// readback���w�肳���ꍇ, footprint���s��
				if (useReadback)
				{
					// Todo
				}

				// �R�s�[�̎��s
				_commandList->CopyTextureRegion(&destinationCopyLocation, 0, 0, 0, &sourceCopyLocation, &sourceBox);
			}
		}
	}
	/*-------------------------------------------------------------------
	-      �@�S�Ă̗̈���R�s�[����ꍇ
	---------------------------------------------------------------------*/
	else
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(destination)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUTexture>(source)     ->GetResource().Get());
	}

	/*-------------------------------------------------------------------
	-       ���̏�Ԃɖ߂�
	---------------------------------------------------------------------*/
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(destination), barrierBeforeStates[0]);
	_barrierBatcher->PushTransitionBarrier(gu::StaticPointerCast<core::GPUResource>(source), barrierBeforeStates[1]);
	_barrierBatcher->Flush(_commandList.Get());
}

#pragma endregion Copy
#pragma endregion GPU Command
#pragma region Property
void RHICommandList::SetName(const gu::tstring& name)
{
	_commandList->SetName(name.CString());
}

#pragma endregion Property
#pragma region Private Function
/****************************************************************************
*                     BeginRenderPassImpl
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If you used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return �@�@void
*****************************************************************************/
void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	// render target 
	gu::DynamicArray<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rtvDescs(frameBuffer->GetRenderTargetSize());
	for (size_t i = 0; i < rtvDescs.Size(); ++i)
	{
		// get rtv handle
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));
#ifdef _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif

		const auto rtvHandle = view->GetCPUHandler();
		if (!rtvHandle.ptr) { continue; }

		// set render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS begin = {};
		begin.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->LoadOp);
		begin.Clear.ClearValue.Color[0] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Red];
		begin.Clear.ClearValue.Color[1] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Green];
		begin.Clear.ClearValue.Color[2] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Blue];
		begin.Clear.ClearValue.Color[3] = renderPass->GetClearColor()[i].Type.Color[core::ClearValue::Alpha];
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
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());

#if __DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif

		const auto dsvHandle = view->GetCPUHandler();

		// set depth render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS depthBegin   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->LoadOp), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    depthEnd     = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StoreOp), {} };
		D3D12_RENDER_PASS_BEGINNING_ACCESS stencilBegin = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilLoad), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    stencilEnd   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilStore), {} };
		depthBegin.Clear.ClearValue.DepthStencil.Depth     = renderPass->GetDepthClear()->Type.DSV.Depth;
		stencilBegin.Clear.ClearValue.DepthStencil.Stencil = renderPass->GetDepthClear()->Type.DSV.Stencil;
		
		// set depth stencil descriptor
		dsvDesc.DepthBeginningAccess   = depthBegin;
		dsvDesc.DepthEndingAccess      = depthEnd;
		dsvDesc.StencilBeginningAccess = stencilBegin;
		dsvDesc.StencilEndingAccess    = stencilEnd;
		dsvDesc.cpuDescriptor          = dsvHandle;
	}

	_commandList->BeginRenderPass(static_cast<gu::uint32>(rtvDescs.Size()), hasRTV ? rtvDescs.Data() : nullptr, hasDSV ? &dsvDesc : nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

/****************************************************************************
*                     OMSetFrameBuffer
*************************************************************************//**
*  @fn        void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If not used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return �@�@void
*****************************************************************************/
void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	auto renderTargets = frameBuffer->GetRenderTargets();
	auto depthStencil  = frameBuffer->GetDepthStencil();
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	gu::DynamicArray<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandle(renderTargets.Size());
	for (size_t i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));

#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif 

		rtvHandle[i] = view->GetCPUHandler();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	if (hasDSV)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());
#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif
		dsvHandle = view->GetCPUHandler();
	}
	/*-------------------------------------------------------------------
	-          Clear frame buffer resources
	---------------------------------------------------------------------*/
	// render target
	for (int i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto colorAttachment = renderPass->GetColorAttachment(i);
		if (colorAttachment->LoadOp != core::AttachmentLoad::Clear) { continue; }
		_commandList->ClearRenderTargetView(rtvHandle[i], renderPass->GetClearColor()[i].Type.Color, 0, nullptr);
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
			_commandList->ClearDepthStencilView(dsvHandle, clearFlags, renderPass->GetDepthClear()->Type.DSV.Depth, renderPass->GetDepthClear()->Type.DSV.Stencil, 0, nullptr);
		}
	}
	/*-------------------------------------------------------------------
	-          Set render target and depth stencil
	---------------------------------------------------------------------*/
	_commandList->OMSetRenderTargets(static_cast<gu::uint32>(rtvHandle.Size()), hasRTV ? rtvHandle.Data() : nullptr, FALSE, hasDSV ? &dsvHandle : nullptr);
}

#pragma endregion Private Function