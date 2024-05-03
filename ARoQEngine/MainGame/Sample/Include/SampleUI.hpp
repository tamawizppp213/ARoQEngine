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
	class Font;
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
		using UIRendererPtr       = gu::SharedPointer<gc::ui::UIRenderer>;
		using ImagePtr            = gu::SharedPointer<gc::ui::Image>;
		using ButtonPtr           = gu::SharedPointer<gc::ui::Button>;
		using SliderPtr           = gu::SharedPointer<gc::ui::Slider>;
		using FontPtr             = gu::SharedPointer<gc::ui::Font>;
		using GPUResourceCachePtr = gu::SharedPointer<rhi::core::GPUResourceCache>;
		using GPUResourceViewPtr  = gu::SharedPointer<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Property
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
		void DebugSliderValue(const float value);
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		UIRendererPtr       _renderer      = nullptr;
		GPUResourceCachePtr _resourceCache = nullptr;
		GPUResourceViewPtr  _resourceView  = nullptr;

		// UI
		ButtonPtr _button = nullptr;
		SliderPtr _slider = nullptr;
		FontPtr  _font = nullptr;

	};
}
#endif