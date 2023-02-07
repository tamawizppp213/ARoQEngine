//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIFrameBuffer.hpp
///             @brief  Frame Buffer (Render Target and )
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_FRAME_BUFFER_HPP
#define RHI_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUTexture;
	class RHIRenderPass;
	class GPUResourceView;
	/****************************************************************************
	*				  			RHIFrameBuffer
	*************************************************************************//**
	*  @class     RHIFrameBuffer
	*  @brief     Render and Depth Stencil Buffer
	*****************************************************************************/
	class RHIFrameBuffer : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Render Target Size (basically one) */
		size_t      GetRenderTargetSize() { return _renderTargets.size(); };

		/* @brief : Return render target pointer*/
		std::shared_ptr<GPUTexture> GetRenderTarget(const size_t index = 0) { return _renderTargets[index]; };

		/* @brief : Return render target pointer list*/
		std::vector<std::shared_ptr<GPUTexture>>& GetRenderTargets() { return _renderTargets; }

		/* @brief : Return depth stencil pointer*/
		std::shared_ptr<GPUTexture> GetDepthStencil() { return _depthStencil; };

		/* @brief : Return viewport */
		Viewport    GetFullViewport(const size_t index = 0) const noexcept;

		/* @brief : Return scissor rect*/
		ScissorRect GetFullScissorRect(const size_t index = 0) const noexcept;

		/* @brief : Return render target view pointer list*/
		const std::vector<std::shared_ptr<GPUResourceView>>& GetRenderTargetViews() const { return _renderTargetViews; }

		/* @brief : Return render target view pointer*/
		std::shared_ptr<GPUResourceView> GetRenderTargetView(const size_t index = 0) const { return _renderTargetViews[index]; }

		/* @brief : Return depth stencil view pointer (if not used : return nullptr)*/
		std::shared_ptr<GPUResourceView> GetDepthStencilView() const noexcept { return _depthStencilView; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIFrameBuffer() = default;

		virtual ~RHIFrameBuffer();

		explicit RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil = nullptr);

		explicit RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<core::RHIRenderPass>& renderPass, const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil = nullptr);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>     _device     = nullptr;
		std::shared_ptr<RHIRenderPass> _renderPass = nullptr;

		std::vector<std::shared_ptr<GPUTexture>>      _renderTargets     = {nullptr};
		std::vector<std::shared_ptr<GPUResourceView>> _renderTargetViews = {nullptr};

		std::shared_ptr<GPUTexture>      _depthStencil     = nullptr;
		std::shared_ptr<GPUResourceView> _depthStencilView = nullptr;

	private:
		void Prepare();
	};
}
#endif