//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12Core.hpp
//            Content:  DirectX12 Alias, Forward Declaration
//             Author:  Toide Yutaro
//             Create:  2020_11_
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_CORE_HPP
#define DIRECTX12_CORE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <wrl/client.h>
#include "GameUtility/Math/Include/GMTypeDef.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                         Forward Declaration
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Alias 
//////////////////////////////////////////////////////////////////////////////////
using Microsoft::WRL::ComPtr;
struct ID3D12Device10;
struct IDXGIFactory7;
struct IDXGIAdapter4;
struct IDXGIOutput6;
struct ID3D12GraphicsCommandList7;
struct ID3D12CommandQueue;
struct ID3D12CommandAllocator;
struct IDXGISwapChain4;
struct ID3D12Resource2;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Fence;
struct  ID3D10Blob;
struct ID3D12Debug6;
struct ID3D12Heap;
struct D3D12_GLOBAL_ROOT_SIGNATURE;
struct D3D12_LOCAL_ROOT_SIGNATURE;

using IDevice           = ID3D12Device10;
using IFactory          = IDXGIFactory7;
using IAdapter          = IDXGIAdapter4;
using IOutput           = IDXGIOutput6;
using ICommandList      = ID3D12GraphicsCommandList7;
using ICommandQueue     = ID3D12CommandQueue;
using ICommandAllocator = ID3D12CommandAllocator;
using ISwapChain        = IDXGISwapChain4;
using Resource          = ID3D12Resource2;
using PSODesc           = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
using IRootSignature    = ID3D12RootSignature;
using IPipelineState    = ID3D12PipelineState;
using IDescriptorHeap   = ID3D12DescriptorHeap;
using IFence            = ID3D12Fence;
using IBlob             = ID3D10Blob;
using IDebug            = ID3D12Debug6;
using IHeap             = ID3D12Heap;

using DeviceComPtr           = ComPtr<IDevice>;
using AdapterComPtr          = ComPtr<IAdapter>;
using FactoryComPtr          = ComPtr<IFactory>;
using SwapchainComPtr        = ComPtr<ISwapChain>;
using OutputComPtr           = ComPtr<IOutput>;
using CommandAllocatorComPtr = ComPtr<ICommandAllocator>;
using CommandQueueComPtr     = ComPtr<ICommandQueue>;
using CommandListComPtr      = ComPtr<ICommandList>;
using FenceComPtr            = ComPtr<IFence>;
using ResourceComPtr         = ComPtr<Resource>;
using DescriptorHeapComPtr   = ComPtr<IDescriptorHeap>;
using PipelineStateComPtr    = ComPtr<IPipelineState>;
using RootSignatureComPtr    = ComPtr<IRootSignature>;
using BlobComPtr             = ComPtr<IBlob>;
using DebugComPtr            = ComPtr<IDebug>;
using HeapComPtr             = ComPtr<IHeap>;

#endif DirectX12_CORE_HPP


