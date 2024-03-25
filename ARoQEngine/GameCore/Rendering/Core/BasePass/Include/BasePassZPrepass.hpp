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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include <cstdint>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
	class ZPrepass : public gu::NonCopyable
	{
	protected:
		using PipelineStatePtr   = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr  = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using GPUResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
		using FrameBufferPtr     = gu::SharedPointer<rhi::core::RHIFrameBuffer>;
		using TexturePtr         = gu::SharedPointer<rhi::core::GPUTexture>;
		using RenderPassPtr      = gu::SharedPointer<rhi::core::RHIRenderPass>;
		using GameModelPtr       = gu::SharedPointer<gc::core::GameModel>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
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

		GPUResourceViewPtr GetRenderedTextureView() const noexcept;

 		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ZPrepass() = default;

		ZPrepass(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const gu::tstring& addName = SP(""));

		~ZPrepass();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PreparePipelineState(const gu::tstring& name);
		
		void PrepareFrameBuffers(const gu::tstring& name);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		gu::DynamicArray<GameModelPtr> _gameModels = {};

		std::uint32_t _width = 0;
		std::uint32_t _height = 0;
		
		/*-------------------------------------------------------------------
		-          Render Resource
		---------------------------------------------------------------------*/
		PipelineStatePtr  _pipeline = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		RenderPassPtr _renderPass = nullptr;

		gu::DynamicArray<FrameBufferPtr> _frameBuffers = {};
	};
}
#endif