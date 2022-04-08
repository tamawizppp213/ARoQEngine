//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12CommandContext.hpp
///             @brief  DirectX12CommandContext
///             @author Toide Yutaro
///             @date   2022_03_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMAND_CONTEXT_HPP
#define DIRECTX12_COMMAND_CONTEXT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC;
struct D3D12_DISPATCH_RAYS_DESC;
class  GraphicsDeviceDirectX12;
class  GPUResource;
//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			CommandContext
*************************************************************************//**
*  @class     CommandContext
*  @brief     CommandContext
*****************************************************************************/
class CommandContext
{
public:
	/****************************************************************************
	**                Public Function 
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                Set up
	---------------------------------------------------------------------*/
	void Initialize(GraphicsDeviceDirectX12* device);
	void Close();
	void Reset(ICommandAllocator* allocator, IPipelineState* pipelineState);
	/*-------------------------------------------------------------------
	-               Graphic Pipeline command
	---------------------------------------------------------------------*/
	void SetVertexBuffer(const D3D12_VERTEX_BUFFER_VIEW* view);
	void SetIndexBuffer (const D3D12_INDEX_BUFFER_VIEW * view);
	void SetPrimitiveTopology   (D3D12_PRIMITIVE_TOPOLOGY topology);
	void SetViewport            (const D3D12_VIEWPORT& viewport, UINT numViewport = 1);
	void SetScissor             (const D3D12_RECT    & rect    , UINT numRect = 1);
	void SetViewportAndScissor  (const D3D12_VIEWPORT& viewPort, const D3D12_RECT& rect);
	void SetDescriptorHeap      (IDescriptorHeap* heap);
	void SetDescriptorHeaps     (int heapsArraySize, IDescriptorHeap* heaps);
	void SetRootSignature       (IRootSignature* rootSignature);
	void SetPipelineState       (IPipelineState* pipelineState);
	void SetGraphicsRootDescriptorTable(UINT rootIndex, D3D12_GPU_DESCRIPTOR_HANDLE address);
	void SetGraphicsConstantBufferView  (UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetGraphicsShaderResourceView  (UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetGraphicsUnOrderedAccessView (UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	/*-------------------------------------------------------------------
	-               Compute Pipeline command
	---------------------------------------------------------------------*/
	void SetComputeRootSignature(IRootSignature* rootSignature);
	void SetComputeRootDescriptorTable(UINT rootIndex, D3D12_GPU_DESCRIPTOR_HANDLE address);
	void SetComputeConstantBufferView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetComputeShaderResourceView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);
	void SetComputeUnOrderedAccessView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address);

	/*-------------------------------------------------------------------
	-                Graphics Command
	---------------------------------------------------------------------*/
	void DrawIndexed         (UINT indexCount, UINT startIndexLocation = 0, INT baseVertexLocation = 0);
	void DrawIndexedInstanced(UINT indexCountPerInstance, UINT numInstance, UINT startIndexLocation = 0, INT baseVertexLocation = 0, UINT startInstanceLocation = 0);
	/*-------------------------------------------------------------------
	-                Compute Command
	---------------------------------------------------------------------*/
	void Dispatch(UINT threadGroupCountX = 1, UINT threadGroupCountY = 1, UINT threadGroupCountZ = 1);
	/*-------------------------------------------------------------------
	-                Mesh shader Command
	---------------------------------------------------------------------*/
	void DispatchMesh(UINT threadGroupCountX = 1, UINT threadGroupCountY = 1, UINT threadGroupCountZ = 1);
	/*-------------------------------------------------------------------
	-                RayTracing Command
	---------------------------------------------------------------------*/
	void BuildRayTracingAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc);
	void DispatchRays(const D3D12_DISPATCH_RAYS_DESC& desc);
	/*-------------------------------------------------------------------
	-                Copy resource
	---------------------------------------------------------------------*/
	void CopyBuffer       (GPUResource* dest, GPUResource* source);
	void CopyBufferRegion (GPUResource* dest, size_t destOffset, GPUResource* source, size_t sourceOffset, size_t numBytes);
	void CopyTextureRegion(GPUResource* dest, GPUResource* source, D3D12_BOX& rect, UINT x = 0, UINT y = 0, UINT z = 0);
	/*-------------------------------------------------------------------
	-                Clear command
	---------------------------------------------------------------------*/
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor);
	void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue);
	/*-------------------------------------------------------------------
	-                Transition command
	---------------------------------------------------------------------*/
	void ResourceBarrier(D3D12_RESOURCE_BARRIER& barrier);
	void ResourceBarriers(UINT numBarrier, D3D12_RESOURCE_BARRIER* barriers);
	void TransitionResourceState(Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
	void WaitUntilToPossibleSetRenderTarget(Resource* renderTarget);
	void WaitUntilFinishDrawingToRenderTarget(Resource* renderTarget);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetCommandList(ICommandList* commandList) { _commandList = commandList; }
	      ICommandList* GetCommandList()       { return _commandList; }
	const ICommandList* GetCommandList() const { return _commandList; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	CommandContext () = default;
	~CommandContext() = default;
	CommandContext(const CommandContext&)            = delete;
	CommandContext& operator=(const CommandContext&) = delete;
	CommandContext(CommandContext&&)                 = default;
	CommandContext& operator=(CommandContext&&)      = delete;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	enum {MAX_DESCRIPTOR_HEAP = 4};
	void PrepareCopyBuffer(GPUResource* dest, GPUResource* source);
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	GraphicsDeviceDirectX12* _device;
	ICommandList*    _commandList;
	IDescriptorHeap* _descriptorHeap[MAX_DESCRIPTOR_HEAP];
};
#pragma region Inline

inline void CommandContext::Close()
{
	_commandList->Close();
}
inline void CommandContext::Reset(ICommandAllocator* allocator, IPipelineState* pipelineState)
{
	_commandList->Reset(allocator, pipelineState);
}
inline void CommandContext::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
}
inline void CommandContext::DrawIndexedInstanced(UINT indexCountPerInstance, UINT numInstance, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, numInstance, startIndexLocation, baseVertexLocation, startInstanceLocation);
}
inline void CommandContext::Dispatch(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
{
	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}
inline void CommandContext::DispatchMesh(UINT threadGroupCountX, UINT threadGroupCountY, UINT threadGroupCountZ)
{
	_commandList->DispatchMesh(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}
inline void CommandContext::BuildRayTracingAccelerationStructure(const D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC& desc)
{
	_commandList->BuildRaytracingAccelerationStructure(&desc, 0, nullptr);
}
inline void CommandContext::DispatchRays   (const D3D12_DISPATCH_RAYS_DESC& desc)
{
	_commandList->DispatchRays(&desc);
}
inline void CommandContext::SetVertexBuffer(const D3D12_VERTEX_BUFFER_VIEW* view)
{
	_commandList->IASetVertexBuffers(0, 1, view);
}
inline void CommandContext::SetIndexBuffer (const D3D12_INDEX_BUFFER_VIEW* view)
{
	_commandList->IASetIndexBuffer(view);
}
inline void CommandContext::SetViewport    (const D3D12_VIEWPORT& viewport, UINT numViewport)
{
	_commandList->RSSetViewports(numViewport, &viewport);
}
inline void CommandContext::SetScissor     (const D3D12_RECT& rect, UINT numRect)
{
	_commandList->RSSetScissorRects(numRect, &rect);
}
inline void CommandContext::SetViewportAndScissor(const D3D12_VIEWPORT& viewPort, const D3D12_RECT& rect)
{
	_commandList->RSSetViewports(1, &viewPort);
	_commandList->RSSetScissorRects(1, &rect);
}
inline void CommandContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	_commandList->IASetPrimitiveTopology(topology);
}
inline void CommandContext::SetDescriptorHeap(IDescriptorHeap* descriptorHeap)
{
	_descriptorHeap[0] = descriptorHeap;
	_commandList->SetDescriptorHeaps(1, &descriptorHeap);
}
inline void CommandContext::SetDescriptorHeaps(int heapsArraySize, IDescriptorHeap* heaps)
{
	if (heapsArraySize >= MAX_DESCRIPTOR_HEAP) { return; }
	for (int i = 0; i < heapsArraySize; ++i) { _descriptorHeap[i] = &heaps[i]; }
	_commandList->SetDescriptorHeaps(heapsArraySize, _descriptorHeap);
}
inline void CommandContext::SetRootSignature(IRootSignature* rootSignature)
{
	_commandList->SetGraphicsRootSignature(rootSignature);
}
inline void CommandContext::SetComputeRootSignature(IRootSignature* rootSignature)
{
	_commandList->SetComputeRootSignature(rootSignature);
}
inline void CommandContext::SetPipelineState(IPipelineState* pipelineState)
{
	_commandList->SetPipelineState(pipelineState);
}
inline void CommandContext::ResourceBarrier(D3D12_RESOURCE_BARRIER& barrier)
{
	_commandList->ResourceBarrier(1, &barrier);
}
inline void CommandContext::ResourceBarriers(UINT numBarrier, D3D12_RESOURCE_BARRIER* barriers)
{
	_commandList->ResourceBarrier(numBarrier, barriers);
}
inline void CommandContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
{
	_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}
inline void CommandContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
{
	_commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, clearValue, 0, 0, nullptr);
}
inline void CommandContext::SetGraphicsConstantBufferView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetGraphicsRootConstantBufferView(rootIndex, address);
}
inline void CommandContext::SetGraphicsRootDescriptorTable(UINT rootIndex, D3D12_GPU_DESCRIPTOR_HANDLE address)
{
	_commandList->SetGraphicsRootDescriptorTable(rootIndex, address);
}
inline void CommandContext::SetGraphicsShaderResourceView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetGraphicsRootShaderResourceView(rootIndex, address);
}
inline void CommandContext::SetGraphicsUnOrderedAccessView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetGraphicsRootUnorderedAccessView(rootIndex, address);
}
inline void CommandContext::SetComputeRootDescriptorTable(UINT rootIndex, D3D12_GPU_DESCRIPTOR_HANDLE address)
{
	_commandList->SetComputeRootDescriptorTable(rootIndex, address);
}
inline void CommandContext::SetComputeConstantBufferView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetComputeRootConstantBufferView(rootIndex, address);
}
inline void CommandContext::SetComputeShaderResourceView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetComputeRootShaderResourceView(rootIndex, address);
}
inline void CommandContext::SetComputeUnOrderedAccessView(UINT rootIndex, D3D12_GPU_VIRTUAL_ADDRESS address)
{
	_commandList->SetComputeRootUnorderedAccessView(rootIndex, address);
}
#pragma endregion Inline
#pragma region 
#endif