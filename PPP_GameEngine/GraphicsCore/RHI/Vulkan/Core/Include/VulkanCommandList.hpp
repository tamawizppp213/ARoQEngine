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

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
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

		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetPrimitiveTopology(core::PrimitiveTopology topology) override{};
		void SetViewport          (const core::Viewport* viewport, std::uint32_t numViewport = 1)override {};
		void SetScissor           (const core::ScissorRect* rect , std::uint32_t numRect = 1) override{};
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override{};
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0)override;
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0)override{};
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1)override;

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
	};
	inline void rhi::vulkan::RHICommandList::DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}
	inline void rhi::vulkan::RHICommandList::Dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
	{
		vkCmdDispatch(_commandBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}
}
#endif