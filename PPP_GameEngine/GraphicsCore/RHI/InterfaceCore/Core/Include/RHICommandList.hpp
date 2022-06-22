//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMANDLIST_HPP
#define RHI_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice
	*  @brief     Device interface
	*****************************************************************************/
	class RHICommandList
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		virtual void SetPrimitiveTopology(PrimitiveTopology topology) = 0;
		virtual void SetViewport          (const Viewport* viewport, std::uint32_t numViewport = 1) = 0;
		virtual void SetScissor           (const ScissorRect* rect , std::uint32_t numRect = 1)     = 0;
		virtual void SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)       = 0;
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		virtual void DrawIndexed         (std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) = 0;
		virtual void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) = 0;
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void Dispatch(std::uint32_t threadGroupCountX  = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) = 0;
		/*-------------------------------------------------------------------
		-                Copy Resource
		---------------------------------------------------------------------*/
		virtual void CopyBuffer(){ };
		virtual void CopyBufferRegion(){ };
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetDevice(std::shared_ptr<RHIDevice> device) { _device = device; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList()          = default;
		virtual ~RHICommandList() = default;
		explicit RHICommandList(std::shared_ptr<RHIDevice> device) { _device = device; }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
	};
}
#endif