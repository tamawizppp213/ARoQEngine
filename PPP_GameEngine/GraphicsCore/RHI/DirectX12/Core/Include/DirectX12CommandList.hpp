//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMANDLIST_HPP
#define DIRECTX12_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include "DirectX12Core.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	class RHIDevice;
	class RHICommandAllocator;
	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice
	*  @brief     Device interface
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void BeginRecording() override ;
		void EndRecording  () override;

		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetPrimitiveTopology(core::PrimitiveTopology topology) override;
		void SetViewport(const core::Viewport* viewport, std::uint32_t numViewport = 1) override;
		void SetScissor(const core::ScissorRect* rect, std::uint32_t numRect = 1) override;
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) override;
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) override;
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandListComPtr GetCommandList() { return _commandList; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;
		~RHICommandList();
		explicit RHICommandList(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandAllocator>& commandAllocator);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandListComPtr _commandList = nullptr;
	};
}
#endif