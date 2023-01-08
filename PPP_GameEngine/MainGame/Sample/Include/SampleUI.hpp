//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleUI.hpp
///             @brief  UI Sample
///             @author Toide Yutaro
///             @date   2023_01_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_UI_HPP
#define SAMPLE_UI_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUResourceCache;
	class GPUResourceView;
}
namespace gc::ui
{
	class Slider;
	class Button;
	class Image;
	class UIRenderer;
}
namespace sample
{

	/****************************************************************************
	*				  			SampleUI
	*************************************************************************//**
	*  @class     SampleUI
	*  @brief     UI sample
	*****************************************************************************/
	class SampleUI : public Scene
	{
		using UIRendererPtr       = std::shared_ptr<gc::ui::UIRenderer>;
		using ImagePtr            = std::shared_ptr<gc::ui::Image>;
		using ButtonPtr           = std::shared_ptr<gc::ui::Button>;
		using SliderPtr           = std::shared_ptr<gc::ui::Slider>;
		using GPUResourceCachePtr = std::shared_ptr<rhi::core::GPUResourceCache>;
		using GPUResourceViewPtr  = std::shared_ptr<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleUI();
		~SampleUI();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		void ExecuteSceneTransition() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		UIRendererPtr       _renderer      = nullptr;
		GPUResourceCachePtr _resourceCache = nullptr;
		GPUResourceViewPtr  _resourceView  = nullptr;

		// UI
		ButtonPtr _button = nullptr;
		SliderPtr _slider = nullptr;

	};
}
#endif