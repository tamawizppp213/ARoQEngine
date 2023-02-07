//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleText.hpp
///             @brief  Text Sample
///             @author Toide Yutaro
///             @date   2022_12_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_TEXT_HPP
#define SAMPLE_TEXT_HPP

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
	class Font;
	class Text;
	class UIRenderer;
}
namespace sample
{
	
	/****************************************************************************
	*				  			SampleEmpty
	*************************************************************************//**
	*  @class     SampleEmpty
	*  @brief     Empty sample
	*****************************************************************************/
	class SampleText : public Scene
	{
		using UIRendererPtr = std::shared_ptr<gc::ui::UIRenderer>;
		using TextPtr       = std::shared_ptr<gc::ui::Text>;
		using FontPtr       = std::shared_ptr<gc::ui::Font>;
		using GPUResourceCachePtr = std::shared_ptr<rhi::core::GPUResourceCache>;
		using GPUResourceViewPtr = std::shared_ptr<rhi::core::GPUResourceView>;
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
		SampleText();
		~SampleText();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials  () override;
		void OnKeyboardInput() override;
		void OnMouseInput   () override;
		void OnGamePadInput () override;
		void ExecuteSceneTransition() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		UIRendererPtr _renderer = nullptr;
		TextPtr       _text     = nullptr;
		FontPtr       _font     = nullptr;
		GPUResourceCachePtr _resourceCache = nullptr;
		GPUResourceViewPtr _resourceView = nullptr;
	};
}
#endif