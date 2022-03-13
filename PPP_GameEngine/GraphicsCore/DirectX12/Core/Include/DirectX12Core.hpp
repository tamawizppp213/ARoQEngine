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
#include <wrl.h>

//////////////////////////////////////////////////////////////////////////////////
//                         Forward Declaration
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Alias 
//////////////////////////////////////////////////////////////////////////////////
using Microsoft::WRL::ComPtr;
struct ID3D12Device9;
struct IDXGIFactory7;
struct IDXGIAdapter4;
struct IDXGIOutput6;
struct ID3D12GraphicsCommandList6;
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
struct ID3D12Debug5;

using Device           = ID3D12Device9;
using Factory          = IDXGIFactory7;
using Adapter          = IDXGIAdapter4;
using Output           = IDXGIOutput6;
using CommandList      = ID3D12GraphicsCommandList6;
using CommandQueue     = ID3D12CommandQueue;
using CommandAllocator = ID3D12CommandAllocator;
using SwapChain        = IDXGISwapChain4;
using Resource         = ID3D12Resource2;
using PSODesc          = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
using RootSignature    = ID3D12RootSignature;
using PipelineState    = ID3D12PipelineState;
using DescriptorHeap   = ID3D12DescriptorHeap;
using Fence            = ID3D12Fence;
using Blob             = ID3D10Blob;
using IDebug           = ID3D12Debug5;

using DeviceComPtr           = ComPtr<Device>;
using AdapterComPtr          = ComPtr<Adapter>;
using FactoryComPtr          = ComPtr<Factory>;
using SwapchainComPtr        = ComPtr<SwapChain>;
using OutputComPtr           = ComPtr<Output>;
using CommandAllocatorComPtr = ComPtr<CommandAllocator>;
using CommandQueueComPtr     = ComPtr<CommandQueue>;
using CommandListComPtr      = ComPtr<CommandList>;
using FenceComPtr            = ComPtr<Fence>;
using ResourceComPtr         = ComPtr<Resource>;
using DescriptorHeapComPtr   = ComPtr<DescriptorHeap>;
using PipelineStateComPtr    = ComPtr<PipelineState>;
using RootSignatureComPtr    = ComPtr<RootSignature>;
using BlobComPtr             = ComPtr<Blob>;
using DebugComPtr            = ComPtr<IDebug>;

#endif DirectX12_CORE_HPP


