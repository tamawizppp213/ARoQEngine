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
#include <memory>
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

using DisplayAdapterPtr     = std::shared_ptr<rhi::core::RHIDisplayAdapter>;
using CommandAllocatorPtr   = std::shared_ptr<rhi::core::RHICommandAllocator>;
using CommandListPtr        = std::shared_ptr<rhi::core::RHICommandList>;
using CommandQueuePtr       = std::shared_ptr<rhi::core::RHICommandQueue>;
using DescriptorHeapPtr     = std::shared_ptr<rhi::core::RHIDescriptorHeap>;
using DevicePtr             = std::shared_ptr<rhi::core::RHIDevice>;
using FencePtr              = std::shared_ptr<rhi::core::RHIFence>;
using FrameBufferPtr        = std::shared_ptr<rhi::core::RHIFrameBuffer>;
using InstancePtr           = std::shared_ptr<rhi::core::RHIInstance>;
using RenderPassPtr         = std::shared_ptr<rhi::core::RHIRenderPass>;
using ResourceLayoutPtr     = std::shared_ptr<rhi::core::RHIResourceLayout>;
using SwapchainPtr          = std::shared_ptr<rhi::core::RHISwapchain>;

using BlendStatePtr         = std::shared_ptr<rhi::core::GPUBlendState>;
using DepthStecilStatePtr   = std::shared_ptr<rhi::core::GPUDepthStencilState>;
using InputAssemblyStatePtr = std::shared_ptr<rhi::core::GPUInputAssemblyState>;
using PipelineFactoryPtr    = std::shared_ptr<rhi::core::GPUPipelineFactory>;
using GraphicsPipelinePtr   = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
using ComputePipelinePtr    = std::shared_ptr<rhi::core::GPUComputePipelineState>;
using RasterizerStatePtr    = std::shared_ptr<rhi::core::GPURasterizerState>;
using ShaderStatePtr        = std::shared_ptr<rhi::core::GPUShaderState>;
using BufferPtr             = std::shared_ptr<rhi::core::GPUBuffer>;
using ResourceCachePtr      = std::shared_ptr<rhi::core::GPUResourceCache>;
using ResourceViewPtr       = std::shared_ptr<rhi::core::GPUResourceView>;
using SamplerPtr            = std::shared_ptr<rhi::core::GPUSampler>;
using TexturePtr            = std::shared_ptr<rhi::core::GPUTexture>;
#endif