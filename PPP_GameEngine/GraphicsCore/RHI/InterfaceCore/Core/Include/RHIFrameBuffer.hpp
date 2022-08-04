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
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
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
		/* @brief : Return depth stencil pointer*/
		std::shared_ptr<GPUTexture> GetDepthStencil() { return _depthStencil; };
		/* @brief : Return viewport */
		Viewport    GetFullViewport(const size_t index = 0) const noexcept;
		/* @brief : Return scissor rect*/
		ScissorRect GetFullScissorRect(const size_t index = 0) const noexcept;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIFrameBuffer() = default;
		virtual ~RHIFrameBuffer() = default;
		explicit RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::vector<std::shared_ptr<GPUTexture>>& renderTargets, const std::shared_ptr<GPUTexture>& depthStencil = nullptr);
		explicit RHIFrameBuffer(const std::shared_ptr<RHIDevice>& device, const std::shared_ptr<GPUTexture>& renderTarget, const std::shared_ptr<GPUTexture>& depthStencil = nullptr);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		std::vector<std::shared_ptr<GPUTexture>> _renderTargets;
		std::shared_ptr<GPUTexture> _depthStencil = nullptr;

	private:
		void Prepare();
	};
}
#endif