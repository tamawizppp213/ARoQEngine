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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	protected:
		using TexturePtr      = gu::SharedPointer<GPUTexture>;
		using ResourceViewPtr = gu::SharedPointer<GPUResourceView>;
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
		TexturePtr GetRenderTarget(const size_t index = 0) { return _renderTargets[index]; };

		/* @brief : Return render target pointer list*/
		std::vector<TexturePtr>& GetRenderTargets() { return _renderTargets; }

		/* @brief : Return depth stencil pointer*/
		TexturePtr GetDepthStencil() { return _depthStencil; };

		/* @brief : Return viewport */
		Viewport    GetFullViewport(const size_t index = 0) const noexcept;

		/* @brief : Return scissor rect*/
		ScissorRect GetFullScissorRect(const size_t index = 0) const noexcept;

		/* @brief : Return render target view pointer list*/
		const std::vector<ResourceViewPtr>& GetRenderTargetViews() const { return _renderTargetViews; }

		/* @brief : Return render target view pointer*/
		ResourceViewPtr GetRenderTargetView(const size_t index = 0) const { return _renderTargetViews[index]; }

		/* @brief : Return render target shader resource view*/
		ResourceViewPtr GetRenderTargetSRV(const size_t index = 0) const noexcept { return _renderTargetSRVs[index]; }

		ResourceViewPtr GetRenderTargetUAV(const size_t index = 0) const noexcept { return _renderTargetUAVs[index]; }

		/* @brief : Return depth stencil view pointer (if not used : return nullptr)*/
		ResourceViewPtr GetDepthStencilView() const noexcept { return _depthStencilView; }

		ResourceViewPtr GetDepthStencilSRV() const noexcept { return _depthStencilSRV; }

		void SetRenderTargets(const std::vector<TexturePtr>& textures);

		void SetRenderTarget(const TexturePtr& texture, const size_t index = 0);

		void SetDepthStencil(const TexturePtr& texture);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/


	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIFrameBuffer() = default;

		virtual ~RHIFrameBuffer();

		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);

		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice>     _device     = nullptr;
		gu::SharedPointer<RHIRenderPass> _renderPass = nullptr;

		std::vector<TexturePtr>      _renderTargets     = {nullptr};
		std::vector<ResourceViewPtr> _renderTargetViews = {nullptr};
		std::vector<ResourceViewPtr> _renderTargetSRVs = { nullptr };
		std::vector<ResourceViewPtr> _renderTargetUAVs = { nullptr };

		TexturePtr      _depthStencil     = nullptr;
		ResourceViewPtr _depthStencilView = nullptr;
		ResourceViewPtr _depthStencilSRV  = nullptr;

	private:
		void CheckResourceFormat();
	};
}
#endif