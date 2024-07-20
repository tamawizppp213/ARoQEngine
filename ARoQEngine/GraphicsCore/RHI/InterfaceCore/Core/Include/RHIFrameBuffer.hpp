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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	****************************************************************************/
	/* @class     RHIFrameBuffer
	*  @brief     Render and Depth Stencil Buffer
	*****************************************************************************/
	class RHIFrameBuffer : public gu::NonCopyable
	{
	protected:
		using TexturePtr      = gu::SharedPointer<GPUTexture>;
		using ResourceViewPtr = gu::SharedPointer<GPUResourceView>;
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		/* @brief : Render Target Size (basically one) */
		gu::uint32 GetRenderTargetSize() { return static_cast<gu::uint32>(_renderTargets.Size()); };

		/* @brief : Return render target pointer*/
		TexturePtr GetRenderTarget(const gu::uint32 index = 0) { return _renderTargets[index]; };

		/* @brief : Return render target pointer list*/
		gu::DynamicArray<TexturePtr>& GetRenderTargets() { return _renderTargets; }

		/* @brief : Return depth stencil pointer*/
		TexturePtr GetDepthStencil() { return _depthStencil; };

		/* @brief : Return viewport */
		Viewport    GetFullViewport(const gu::uint32 index = 0) const noexcept;

		/* @brief : Return scissor rect*/
		ScissorRect GetFullScissorRect(const gu::uint32 index = 0) const noexcept;

		/* @brief : Return render target view pointer list*/
		const gu::DynamicArray<ResourceViewPtr>& GetRenderTargetViews() const { return _renderTargetViews; }

		/* @brief : Return render target view pointer*/
		ResourceViewPtr GetRenderTargetView(const gu::uint32 index = 0) const { return _renderTargetViews[index]; }

		/* @brief : Return render target shader resource view*/
		ResourceViewPtr GetRenderTargetSRV(const gu::uint32 index = 0) const noexcept { return _renderTargetSRVs[index]; }

		ResourceViewPtr GetRenderTargetUAV(const gu::uint32 index = 0) const noexcept { return _renderTargetUAVs[index]; }

		/* @brief : Return depth stencil view pointer (if not used : return nullptr)*/
		ResourceViewPtr GetDepthStencilView() const noexcept { return _depthStencilView; }

		ResourceViewPtr GetDepthStencilSRV() const noexcept { return _depthStencilSRV; }

		void SetRenderTargets(const gu::DynamicArray<TexturePtr>& textures);

		void SetRenderTarget(const TexturePtr& texture, const gu::uint32 index = 0);

		void SetDepthStencil(const TexturePtr& texture);

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion


	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIFrameBuffer() = default;

		/*! @brief デストラクタ*/
		virtual ~RHIFrameBuffer();

		/*! @brief 論理デバイス, レンダーパス, 複数のレンダーターゲット, デプスステンシルを使って初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::DynamicArray<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);

		/*! @brief 論理デバイス, レンダーパス, レンダーターゲット, デプスステンシルを使って初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);
		#pragma endregion 

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		gu::SharedPointer<RHIDevice>     _device     = nullptr;
		gu::SharedPointer<RHIRenderPass> _renderPass = nullptr;

		gu::DynamicArray<TexturePtr>      _renderTargets     = {nullptr};
		gu::DynamicArray<ResourceViewPtr> _renderTargetViews = {nullptr};
		gu::DynamicArray<ResourceViewPtr> _renderTargetSRVs = { nullptr };
		gu::DynamicArray<ResourceViewPtr> _renderTargetUAVs = { nullptr };

		TexturePtr      _depthStencil     = nullptr;
		ResourceViewPtr _depthStencilView = nullptr;
		ResourceViewPtr _depthStencilSRV  = nullptr;
		#pragma endregion

	private:
		#pragma region Private Function
		void CheckResourceFormat();
		#pragma endregion
	};
}
#endif