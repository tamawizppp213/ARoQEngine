//////////////////////////////////////////////////////////////////////////////////
///             @file   BasePassGBuffer.hpp
///             @brief  BasePassGBuffer (ue4 like render pipeline) reference HLSL シェーダの魔導書
///             @author Toide Yutaro
///             @date   2022_05_04 
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GBUFFER_HPP
#define GBUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GBufferDesc.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
#include <string>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class RHIRenderPass;
	class RHIFrameBuffer;
	class GPUResourceView;
	class RHIResourceLayout;
	class GPUTexture;
	class GPUGraphicsPipelineState;
}

namespace gc::core
{
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	/****************************************************************************
	*				  			GBuffer
	*************************************************************************//**
	*  @class     GBuffer
	*  @brief     GBuffer (普段はDefaultTypeを使用するが, カスタマイズも可能にしている.)
	*****************************************************************************/
	class GBuffer : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using GameModelPtr              = std::shared_ptr<gc::core::GameModel>;
		using TexturePtr                = std::shared_ptr<rhi::core::GPUTexture>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using FrameBufferPtr            = std::shared_ptr<rhi::core::RHIFrameBuffer>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using RenderPassPtr             = std::shared_ptr<rhi::core::RHIRenderPass>;
		using GPUResourceViewPtr        = std::shared_ptr<rhi::core::GPUResourceView>;
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void OnResize(const std::uint32_t width, const std::uint32_t height) = 0;

		virtual void Draw(const GPUResourceViewPtr& scene) = 0;

		virtual void Add(const GameModelPtr& model);

		virtual void Clear();

		virtual void Clear(const GameModelPtr& model);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		TexturePtr GetRenderedTexture(const std::uint32_t index) const noexcept;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GBuffer() = default;

		GBuffer(const LowLevelGraphicsEnginePtr& engine,
			const GBufferDesc& desc = GBufferDesc(),
			const std::wstring& addName = L"");

		virtual ~GBuffer();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		virtual void PreparePipelineState(const std::wstring& name) = 0;

		virtual void PrepareFrameBuffers() = 0;

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameModelPtr> _gameModels = {};

		GBufferDesc _desc = {};

		/*-------------------------------------------------------------------
		-          Render Resource
		---------------------------------------------------------------------*/
		PipelineStatePtr _pipeline = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		RenderPassPtr _renderPass = nullptr;

		std::vector<FrameBufferPtr> _frameBuffers = {};
	};
}
#endif