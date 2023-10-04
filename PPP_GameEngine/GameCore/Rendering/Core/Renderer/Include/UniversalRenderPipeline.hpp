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
#include "GameCore/Rendering/Light/Include/SceneLightBuffer.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class GameTimer;
namespace gc
{
	class SSAO;

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

	namespace rendering
	{
		class CascadeShadow;
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
		using SSAOPtr         = std::shared_ptr<SSAO>;
		using ShadowMapPtr    = std::shared_ptr<rendering::CascadeShadow>;
		using DirectionalLightPtr = std::shared_ptr<gc::rendering::SceneLightBuffer<gc::rendering::DirectionalLightData>>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Draw() override;

		void Add(const URPDrawType type, const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		UIRendererPtr GetUIRenderer() const noexcept { return _uiRenderer; }

		ResourceViewPtr GetSceneView() const noexcept override { return _scene; }
		void SetSceneView(const ResourceViewPtr& scene) override { _scene = scene; }

		template<class TLight> requires std::is_base_of_v<gc::rendering::LightData, TLight> 
		void SetLight(const gc::rendering::LightType type, const std::uint32_t index, const TLight& light);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		URP(const LowLevelGraphicsEnginePtr& engine, const std::shared_ptr<GameTimer>& gameTimer);

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

		DirectionalLightPtr _directionalLights = nullptr;

		GBufferPtr _gBuffer = nullptr;

		SSAOPtr _ssao = nullptr;

		ShadowMapPtr _cascadeShadowMap = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		PipelineStatePtr _pipeline = nullptr;

		std::shared_ptr<GameTimer> _gameTimer = nullptr;

		std::vector<GameModelPtr> _forwardModels  = {};

		ResourceViewPtr _scene = nullptr;

		static constexpr std::uint32_t MAX_UI_COUNT = 1024;
	};

	template<class TLight> requires std::is_base_of_v<gc::rendering::LightData, TLight>
	void URP::SetLight(const gc::rendering::LightType type, const std::uint32_t index, const TLight& light)
	{
		using namespace gc::rendering;

		switch (type)
		{
			case LightType::Directional: 
			{
#ifdef _DEBUG
				assert(typeid(light) == typeid(DirectionalLightData));
#endif
				_directionalLights->SetLight(index, light);
				break;
			}
		}
	}
}
#endif