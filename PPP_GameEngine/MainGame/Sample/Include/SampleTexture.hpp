//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleTexture.hpp
///             @brief  Texture Sample
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_TEXTURE_HPP
#define SAMPLE_TEXTURE_HPP

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
	class Image;
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
	class SampleTexture : public Scene
	{
		using UIRendererPtr = gu::SharedPointer<gc::ui::UIRenderer>;
		using ImagePtr      = gu::SharedPointer<gc::ui::Image>;
		using GPUResourceCachePtr = gu::SharedPointer<rhi::core::GPUResourceCache>;
		using GPUResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleTexture();
		~SampleTexture();
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
		UIRendererPtr _renderer = nullptr;
		ImagePtr      _image    = nullptr;
		ImagePtr      _miniImage = nullptr;
		GPUResourceCachePtr _resourceCache = nullptr;
		GPUResourceViewPtr _resourceView = nullptr;
	};
}
#endif