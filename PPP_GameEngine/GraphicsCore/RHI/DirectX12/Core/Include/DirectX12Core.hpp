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
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                         Forward Declaration
//////////////////////////////////////////////////////////////////////////////////
#define D3D12_CORE_ENABLED              (1)
#define D3D12_MAX_DEVICE_INTERFACE      (10)
#define D3D12_MAX_COMMANDLIST_INTERFACE (7)
#define D3D12_MAX_DEBUG_INTERFACE       (6)
#define D3D12_MAX_RESOURCE_INTERFACE    (2)
#define D3D12_MAX_INFO_QUEUE_INTERFACE  (1)
#define DXGI_MAX_FACTORY_INTERFACE      (7)
#define DXGI_MAX_SWAPCHAIN_INTERFACE    (4)
#define DXGI_MAX_OUTPUT_INTERFACE       (6)
#define DXGI_MAX_ADAPTER_INTERFACE      (4)
#define D3D12_MAX_GPU_COUNT             (1)

//////////////////////////////////////////////////////////////////////////////////
//                         Alias 
//////////////////////////////////////////////////////////////////////////////////
using Microsoft::WRL::ComPtr;

// device
#if   D3D12_MAX_DEVICE_INTERFACE >= 12
struct ID3D12Device12;
using  IDevice = ID3D12Device12;
#elif D3D12_MAX_DEVICE_INTERFACE >= 11
struct ID3D12Device11;
using  IDevice = ID3D12Device11;
#elif D3D12_MAX_DEVICE_INTERFACE >= 10
struct ID3D12Device10;
using  IDevice = ID3D12Device10;
#elif D3D12_MAX_DEVICE_INTERFACE >= 9
struct ID3D12Device9;
using  IDevice = ID3D12Device9;
#elif D3D12_MAX_DEVICE_INTERFACE >= 8
struct ID3D12Device8;
using  IDevice = ID3D12Device8;
#elif D3D12_MAX_DEVICE_INTERFACE >= 7
struct ID3D12Device7;
using  IDevice = ID3D12Device7;
#elif D3D12_MAX_DEVICE_INTERFACE >= 6
struct ID3D12Device6;
using  IDevice = ID3D12Device6;
#elif D3D12_MAX_DEVICE_INTERFACE >= 5
struct ID3D12Device5;
using  IDevice = ID3D12Device5;
#elif D3D12_MAX_DEVICE_INTERFACE >= 4
struct ID3D12Device4;
using  IDevice = ID3D12Device4;
#elif D3D12_MAX_DEVICE_INTERFACE >= 3
struct ID3D12Device3;
using  IDevice = ID3D12Device3;
#elif D3D12_MAX_DEVICE_INTERFACE >= 2
struct ID3D12Device2;
using  IDevice = ID3D12Device2;
#elif D3D12_MAX_DEVICE_INTERFACE >= 1
struct ID3D12Device;
using  IDevice = ID3D12Device;
#endif

// factory
#if   DXGI_MAX_FACTORY_INTERFACE >= 7
struct IDXGIFactory7;
using IFactory = IDXGIFactory7;
#elif DXGI_MAX_FACTORY_INTERFACE >= 6
struct IDXGIFactory6;
using IFactory = IDXGIFactory6;
#elif DXGI_MAX_FACTORY_INTERFACE >= 5
struct IDXGIFactory5;
using IFactory = IDXGIFactory5;
#elif DXGI_MAX_FACTORY_INTERFACE >= 4
struct IDXGIFactory4;
using IFactory = IDXGIFactory4;
#elif DXGI_MAX_FACTORY_INTERFACE >= 3
struct IDXGIFactory3;
using IFactory = IDXGIFactory3;
#elif DXGI_MAX_FACTORY_INTERFACE >= 2
struct IDXGIFactory2;
using IFactory = IDXGIFactory2;
#elif DXGI_MAX_FACTORY_INTERFACE >= 1
struct IDXGIFactory;
using IFactory = IDXGIFactory;
#endif

// Adapter
#if  D3D12_MAX_RESOURCE_INTERFACE >= 2
struct ID3D12Resource2;
using Resource = ID3D12Resource2;
#elif D3D12_MAX_RESOURCE_INTERFACE >= 1
struct ID3D12Resource1;
using Resource = ID3D12Resource1;
#elif D3D12_MAX_RESOURCE_INTERFACE >= 0
struct ID3D12Resource;
using Resource = ID3D12Resource;
#endif

// Debug
#if   D3D12_MAX_DEBUG_INTERFACE >= 6
struct ID3D12Debug6;
using IDebug = ID3D12Debug6;
#elif D3D12_MAX_DEBUG_INTERFACE >= 5
struct ID3D12Debug5;
using IDebug = ID3D12Debug5;
#elif D3D12_MAX_DEBUG_INTERFACE >= 4
struct ID3D12Debug4;
using IDebug = ID3D12Debug4;
#elif D3D12_MAX_DEBUG_INTERFACE >= 3
struct ID3D12Debug3;
using IDebug = ID3D12Debug3;
#elif D3D12_MAX_DEBUG_INTERFACE >= 2
struct ID3D12Debug2;
using IDebug = ID3D12Debug2;
#elif D3D12_MAX_DEBUG_INTERFACE >= 1
struct ID3D12Debug;
using IDebug = ID3D12Debug;
#endif

// CommandList
#if   D3D12_MAX_COMMANDLIST_INTERFACE >= 7
struct ID3D12GraphicsCommandList7;
using  ICommandList = ID3D12GraphicsCommandList7;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 6
struct ID3D12GraphicsCommandList6;
using  ICommandList = ID3D12GraphicsCommandList6;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 5
struct ID3D12GraphicsCommandList5;
using  ICommandList = ID3D12GraphicsCommandList5;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 4
struct ID3D12GraphicsCommandList4;
using  ICommandList = ID3D12GraphicsCommandList4;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 3
struct ID3D12GraphicsCommandList3;
using  ICommandList = ID3D12GraphicsCommandList3;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 2
struct ID3D12GraphicsCommandList2;
using  ICommandList = ID3D12GraphicsCommandList2;
#elif D3D12_MAX_COMMANDLIST_INTERFACE >= 1
struct ID3D12GraphicsCommandList;
using  ICommandList = ID3D12GraphicsCommandList;
#endif

// swapchain
#if DXGI_MAX_SWAPCHAIN_INTERFACE >= 4
struct IDXGISwapChain4;
using ISwapChain = IDXGISwapChain4;
#elif DXGI_MAX_SWAPCHAIN_INTERFACE >= 3
struct IDXGISwapChain3;
using ISwapChain = IDXGISwapChain3;
#elif DXGI_MAX_SWAPCHAIN_INTERFACE >= 2
struct IDXGISwapChain2;
using ISwapChain = IDXGISwapChain2;
#elif DXGI_MAX_SWAPCHAIN_INTERFACE >= 1
struct IDXGISwapChain;
using ISwapChain = IDXGISwapChain;
#endif

// Output
#if  DXGI_MAX_OUTPUT_INTERFACE >= 6
struct IDXGIOutput6;
using IOutput = IDXGIOutput6;
#elif  DXGI_MAX_OUTPUT_INTERFACE >= 5
struct IDXGIOutput5;
using IOutput = IDXGIOutput5;
#elif  DXGI_MAX_OUTPUT_INTERFACE >= 4
struct IDXGIOutput4;
using IOutput = IDXGIOutput4;
#elif  DXGI_MAX_OUTPUT_INTERFACE >= 3
struct IDXGIOutput3;
using IOutput = IDXGIOutput3;
#elif  DXGI_MAX_OUTPUT_INTERFACE >= 2
struct IDXGIOutput2;
using IOutput = IDXGIOutput2;
#elif  DXGI_MAX_OUTPUT_INTERFACE >= 1
struct IDXGIOutput;
using IOutput = IDXGIOutput;
#endif

// Adapter
#if DXGI_MAX_ADAPTER_INTERFACE >= 4
struct IDXGIAdapter4;
using IAdapter = IDXGIAdapter4;
#elif DXGI_MAX_ADAPTER_INTERFACE >= 3
struct IDXGIAdapter3;
using IAdapter = IDXGIAdapter3;
#elif DXGI_MAX_ADAPTER_INTERFACE >= 2
struct IDXGIAdapter2;
using IAdapter = IDXGIAdapter2;
#elif DXGI_MAX_ADAPTER_INTERFACE >= 1
struct IDXGIAdapter;
using IAdapter = IDXGIAdapter;
#endif 

// InfoQueue
#if D3D12_MAX_INFO_QUEUE_INTERFACE >= 1
struct ID3D12InfoQueue1;
using  IInfoQueue = ID3D12InfoQueue1;
#elif  D3D12_MAX_INFO_QUEUE_INTERFACE >= 0
struct ID3D12InfoQueue;
using  IInfoQueue = ID3D12InfoQueue;
#endif

struct ID3D12CommandQueue;
struct ID3D12CommandAllocator;
struct D3D12_GRAPHICS_PIPELINE_STATE_DESC;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12DescriptorHeap;
struct ID3D12Fence;
struct  ID3D10Blob;
struct ID3D12Heap;
struct D3D12_GLOBAL_ROOT_SIGNATURE;
struct D3D12_LOCAL_ROOT_SIGNATURE;
struct ID3D12CommandSignature;

using ICommandQueue     = ID3D12CommandQueue;
using ICommandAllocator = ID3D12CommandAllocator;
using PSODesc           = D3D12_GRAPHICS_PIPELINE_STATE_DESC;
using IRootSignature    = ID3D12RootSignature;
using IPipelineState    = ID3D12PipelineState;
using IDescriptorHeap   = ID3D12DescriptorHeap;
using IFence            = ID3D12Fence;
using IBlob             = ID3D10Blob;
using IHeap             = ID3D12Heap;
using ICommandSignature = ID3D12CommandSignature;

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
using InfoQueuePtr           = ComPtr<IInfoQueue>;
using CommandSignaturePtr    = ComPtr<ICommandSignature>;

#endif DirectX12_CORE_HPP


