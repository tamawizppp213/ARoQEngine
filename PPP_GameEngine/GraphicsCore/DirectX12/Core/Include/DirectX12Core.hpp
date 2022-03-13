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
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

//////////////////////////////////////////////////////////////////////////////////
//                         Forward Declaration
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Alias 
//////////////////////////////////////////////////////////////////////////////////
using Microsoft::WRL::ComPtr;
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
using Blob             = ID3DBlob;
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


