//////////////////////////////////////////////////////////////////////////////////
///             @file   RHITypeCore.hpp
///             @brief  Forward decrare list 
///             @author Toide Yutaro
///             @date   2023_04_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_TYPE_CORE_HPP
#define RHI_TYPE_CORE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDisplayAdapter;
	class RHICommandAllocator;
	class RHICommandList;
	class RHICommandQueue;
	class RHIDescriptorHeap;
	class RHIDevice;
	class RHIFence;
	class RHIFrameBuffer;
	class RHIInstance;
	class RHIRenderPass;
	class RHIResourceLayout;
	class RHISwapchain;

	class GPUBlendState;
	class GPUDepthStencilState;
	class GPUInputAssemblyState;
	class GPUPipelineFactory;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPURasterizerState;
	class GPUShaderState;
	
	class GPUBuffer;
	class GPUResourceCache;
	class GPUResourceView;
	class GPUSampler;
	class GPUTexture;

	struct GPUBufferMetaData;
	struct GPUTextureMetaData;
}

using DisplayAdapterPtr     = gu::SharedPointer<rhi::core::RHIDisplayAdapter>;
using CommandAllocatorPtr   = gu::SharedPointer<rhi::core::RHICommandAllocator>;
using CommandListPtr        = gu::SharedPointer<rhi::core::RHICommandList>;
using CommandQueuePtr       = gu::SharedPointer<rhi::core::RHICommandQueue>;
using DescriptorHeapPtr     = gu::SharedPointer<rhi::core::RHIDescriptorHeap>;
using DevicePtr             = gu::SharedPointer<rhi::core::RHIDevice>;
using FencePtr              = gu::SharedPointer<rhi::core::RHIFence>;
using FrameBufferPtr        = gu::SharedPointer<rhi::core::RHIFrameBuffer>;
using InstancePtr           = gu::SharedPointer<rhi::core::RHIInstance>;
using RenderPassPtr         = gu::SharedPointer<rhi::core::RHIRenderPass>;
using ResourceLayoutPtr     = gu::SharedPointer<rhi::core::RHIResourceLayout>;
using SwapchainPtr          = gu::SharedPointer<rhi::core::RHISwapchain>;

using BlendStatePtr         = gu::SharedPointer<rhi::core::GPUBlendState>;
using DepthStecilStatePtr   = gu::SharedPointer<rhi::core::GPUDepthStencilState>;
using InputAssemblyStatePtr = gu::SharedPointer<rhi::core::GPUInputAssemblyState>;
using PipelineFactoryPtr    = gu::SharedPointer<rhi::core::GPUPipelineFactory>;
using GraphicsPipelinePtr   = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
using ComputePipelinePtr    = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
using RasterizerStatePtr    = gu::SharedPointer<rhi::core::GPURasterizerState>;
using ShaderStatePtr        = gu::SharedPointer<rhi::core::GPUShaderState>;
using BufferPtr             = gu::SharedPointer<rhi::core::GPUBuffer>;
using ResourceCachePtr      = gu::SharedPointer<rhi::core::GPUResourceCache>;
using ResourceViewPtr       = gu::SharedPointer<rhi::core::GPUResourceView>;
using SamplerPtr            = gu::SharedPointer<rhi::core::GPUSampler>;
using TexturePtr            = gu::SharedPointer<rhi::core::GPUTexture>;
#endif