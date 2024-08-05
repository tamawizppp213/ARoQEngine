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
namespace engine
{
	class SSAO;

	class UIRenderer;
	class ZPrepass;
	class LightCulling;
	class GBuffer;
	class CascadeShadow;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	enum URPDrawType
	{
		Differed = 0x01,
		Forward  = 0x02,
		Both     = Differed | Forward
	};

	/****************************************************************************
	*				  			URP
	****************************************************************************/
	/* @class     URP
	*  @brief     Universal Rendering Pipeline
	*****************************************************************************/
	class URP : public IRenderPipeline
	{
		using UIRendererPtr   = gu::SharedPointer<UIRenderer>;
		using ZPrepassPtr     = gu::SharedPointer<ZPrepass>;
		using LightCullingPtr = gu::SharedPointer<LightCulling>;
		using GBufferPtr      = gu::SharedPointer<GBuffer>; 
		using SSAOPtr         = gu::SharedPointer<SSAO>;
		using ShadowMapPtr    = gu::SharedPointer<CascadeShadow>;
		using DirectionalLightPtr = gu::SharedPointer<SceneLightBuffer<DirectionalLightData>>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Draw() override;

		void Add(const URPDrawType type, const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		UIRendererPtr GetUIRenderer() const noexcept { return _uiRenderer; }

		ResourceViewPtr GetSceneView() const noexcept override { return _scene; }
		void SetSceneView(const ResourceViewPtr& scene) override { _scene = scene; }

		template<class TLight> requires std::is_base_of_v<LightData, TLight> 
		void SetLight(const LightType type, const std::uint32_t index, const TLight& light);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		URP(const LowLevelGraphicsEnginePtr& engine, const gu::SharedPointer<GameTimer>& gameTimer);

		~URP();

	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void PrepareModelPipeline();

		/****************************************************************************
		**                Private Property
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

		gu::SharedPointer<GameTimer> _gameTimer = nullptr;

		std::vector<GameModelPtr> _forwardModels  = {};

		ResourceViewPtr _scene = nullptr;

		static constexpr std::uint32_t MAX_UI_COUNT = 1024;
	};

	template<class TLight> requires std::is_base_of_v<LightData, TLight>
	void URP::SetLight(const LightType type, const std::uint32_t index, const TLight& light)
	{
		using namespace engine;

		switch (type)
		{
			case LightType::Directional: 
			{
#ifdef _DEBUG
				Check(typeid(light) == typeid(DirectionalLightData));
#endif
				_directionalLights->SetLight(index, light);
				break;
			}
		}
	}
}
#endif