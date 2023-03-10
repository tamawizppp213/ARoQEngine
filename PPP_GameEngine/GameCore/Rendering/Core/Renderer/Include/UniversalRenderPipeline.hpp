//////////////////////////////////////////////////////////////////////////////////
///             @file   RenderingEngine.hpp
///             @brief  RenderingEngine
///             @author Toide Yutaro
///             @date   2022_06_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UNIVERSAL_RENDER_PIPELINE_HPP
#define UNIVERSAL_RENDER_PIPELINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RenderPipeline.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	namespace ui
	{
		class UIRenderer;
	}

	namespace basepass
	{
		class ZPrepass;
		class LightCulling;
		class GBuffer;
	}
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	enum URPDrawType
	{
		Differed = 0x01,
		Forward  = 0x02,
		Both     = Differed | Forward
	};

	/****************************************************************************
	*				  			URP
	*************************************************************************//**
	*  @class     URP
	*  @brief     Universal Rendering Pipeline
	*****************************************************************************/
	class URP : public IRenderPipeline
	{
		using UIRendererPtr   = std::shared_ptr<ui::UIRenderer>;
		using ZPrepassPtr     = std::shared_ptr<basepass::ZPrepass>;
		using LightCullingPtr = std::shared_ptr<basepass::LightCulling>;
		using GBufferPtr      = std::shared_ptr<basepass::GBuffer>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Draw(const ResourceViewPtr& scene) override;

		void Add(const URPDrawType type, const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		UIRendererPtr GetUIRenderer() const noexcept { return _uiRenderer; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		URP(const LowLevelGraphicsEnginePtr& engine);

		~URP();

	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void PrepareModelPipeline();

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		UIRendererPtr _uiRenderer = nullptr;

		ZPrepassPtr _zPrepass = nullptr;

		LightCullingPtr _lightCulling = nullptr;

		GBufferPtr _gBuffer = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		PipelineStatePtr _pipeline = nullptr;

		std::vector<GameModelPtr> _forwardModels  = {};

		static constexpr std::uint32_t MAX_UI_COUNT = 1024;
	};
}
#endif