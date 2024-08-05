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
	class RHIQuery;

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

using RHIDisplayAdapterPtr     = gu::SharedPointer<rhi::core::RHIDisplayAdapter>;
using RHICommandAllocatorPtr   = gu::SharedPointer<rhi::core::RHICommandAllocator>;
using RHICommandListPtr        = gu::SharedPointer<rhi::core::RHICommandList>;
using RHICommandQueuePtr       = gu::SharedPointer<rhi::core::RHICommandQueue>;
using RHIDescriptorHeapPtr     = gu::SharedPointer<rhi::core::RHIDescriptorHeap>;
using RHIDevicePtr             = gu::SharedPointer<rhi::core::RHIDevice>;
using RHIFencePtr              = gu::SharedPointer<rhi::core::RHIFence>;
using RHIFrameBufferPtr        = gu::SharedPointer<rhi::core::RHIFrameBuffer>;
using RHIInstancePtr           = gu::SharedPointer<rhi::core::RHIInstance>;
using RHIRenderPassPtr         = gu::SharedPointer<rhi::core::RHIRenderPass>;
using RHIResourceLayoutPtr     = gu::SharedPointer<rhi::core::RHIResourceLayout>;
using RHISwapchainPtr          = gu::SharedPointer<rhi::core::RHISwapchain>;
using RHIQueryPtr              = gu::SharedPointer<rhi::core::RHIQuery>;

using GPUBlendStatePtr         = gu::SharedPointer<rhi::core::GPUBlendState>;
using GPUDepthStecilStatePtr   = gu::SharedPointer<rhi::core::GPUDepthStencilState>;
using GPUInputAssemblyStatePtr = gu::SharedPointer<rhi::core::GPUInputAssemblyState>;
using GPUPipelineFactoryPtr    = gu::SharedPointer<rhi::core::GPUPipelineFactory>;
using GPUGraphicsPipelinePtr   = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
using GPUComputePipelinePtr    = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
using GPURasterizerStatePtr    = gu::SharedPointer<rhi::core::GPURasterizerState>;
using GPUShaderStatePtr        = gu::SharedPointer<rhi::core::GPUShaderState>;
using GPUBufferPtr             = gu::SharedPointer<rhi::core::GPUBuffer>;
using GPUResourceCachePtr      = gu::SharedPointer<rhi::core::GPUResourceCache>;
using GPUResourceViewPtr       = gu::SharedPointer<rhi::core::GPUResourceView>;
using GPUSamplerPtr            = gu::SharedPointer<rhi::core::GPUSampler>;
using GPUTexturePtr            = gu::SharedPointer<rhi::core::GPUTexture>;
#endif