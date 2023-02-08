//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandList.hpp
///             @brief  CommandList
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_COMMANDLIST_HPP
#define VULKAN_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4100)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void BeginRecording() override; // first call at draw frame 
		void EndRecording () override;  // end call   at draw frame
		void BeginRenderPass(const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<core::RHIFrameBuffer>& frameBuffer) override;
		void EndRenderPass() override;

		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetDescriptorHeap(const std::shared_ptr<core::RHIDescriptorHeap>& heap) override {};
		void SetResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& layout) override;
		void SetGraphicsPipeline(const std::shared_ptr<core::GPUGraphicsPipelineState>& pipeline);
		void SetPrimitiveTopology(core::PrimitiveTopology topology) override;
		void SetViewport          (const core::Viewport* viewport, std::uint32_t numViewport = 1)override;
		void SetScissor           (const core::ScissorRect* rect , std::uint32_t numRect = 1) override;
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void SetComputePipeline(const std::shared_ptr<core::GPUComputePipelineState>& pipeline) override {};
		void SetVertexBuffer (const std::shared_ptr<core::GPUBuffer>& buffer) override;
		void SetVertexBuffers(const std::vector<std::shared_ptr<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		void SetIndexBuffer  (const std::shared_ptr<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0)override;
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0)override;
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const std::shared_ptr<core::RHIResourceLayout>& resourceLayout) override{};
		void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1)override;
		/*-------------------------------------------------------------------
		-                Transition Resource State
		---------------------------------------------------------------------*/
		void TransitionResourceState(const std::shared_ptr<core::GPUTexture>& texture, core::ResourceState after) override;
		void TransitionResourceStates(const std::uint32_t numStates, const std::shared_ptr<core::GPUTexture>* textures, core::ResourceState* afters) override;
		void CopyResource(const std::shared_ptr<core::GPUTexture>& dest, const std::shared_ptr<core::GPUTexture>& source) override {};;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkCommandBuffer GetCommandList() { return _commandBuffer; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;
		~RHICommandList();
		explicit RHICommandList(
			const std::shared_ptr<core::RHIDevice>& device,
			const std::shared_ptr<core::RHICommandAllocator>& allocator);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkCommandBuffer _commandBuffer = nullptr;
		std::shared_ptr<RHIResourceLayout> _resourceLayout = nullptr;
	
		bool _isFirstFrame = true;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		VkAccessFlags SelectVkAccessFlag(const VkImageLayout imageLayout);
	};
	inline void rhi::vulkan::RHICommandList::DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}
	inline void rhi::vulkan::RHICommandList::DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, std::uint32_t startInstanceLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	inline void rhi::vulkan::RHICommandList::Dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
	{
		vkCmdDispatch(_commandBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}
}
#endif