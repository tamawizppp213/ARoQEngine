//////////////////////////////////////////////////////////////////////////////////
///             @file   BasePassZPrepass.hpp
///             @brief  ZPrepass
///             @author Toide Yutaro
///             @date   2022_05_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_PASS_ZPREPASS_HPP
#define BASE_PASS_ZPREPASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <cstdint>
#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class RHIResourceLayout;
	class RHIFrameBuffer;
	class RHIRenderPass;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUTexture;
}

namespace gc::core
{
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::basepass
{

	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class ZPrepass : public NonCopyable
	{
		using PipelineStatePtr   = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr  = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using GPUResourceViewPtr = std::shared_ptr<rhi::core::GPUResourceView>;
		using FrameBufferPtr     = std::shared_ptr<rhi::core::RHIFrameBuffer>;
		using TexturePtr         = std::shared_ptr<rhi::core::GPUTexture>;
		using RenderPassPtr      = std::shared_ptr<rhi::core::RHIRenderPass>;
		using GameModelPtr       = std::shared_ptr<gc::core::GameModel>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t width, const std::uint32_t height);

		void Draw(const GPUResourceViewPtr& scene);

		void Add(const GameModelPtr& actor);

		void Clear();

		void Clear(const GameModelPtr& actor);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		TexturePtr GetRenderedTexture() const noexcept;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ZPrepass() = default;

		ZPrepass(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName = L"");

		~ZPrepass();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PreparePipelineState(const std::wstring& name);
		
		void PrepareFrameBuffers();
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameModelPtr> _gameModels = {};

		std::uint32_t _width = 0;
		std::uint32_t _height = 0;
		
		/*-------------------------------------------------------------------
		-          Render Resource
		---------------------------------------------------------------------*/
		PipelineStatePtr  _pipeline = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		RenderPassPtr _renderPass = nullptr;

		std::vector<FrameBufferPtr> _frameBuffers = {};
	};
}
#endif